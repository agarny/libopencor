/*
Copyright libOpenCOR contributors.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "cellmlfileruntime_p.h"

#include "solvernla_p.h"

#include "compiler.h"

namespace libOpenCOR {

CellmlFileRuntime::Impl::Impl(const CellmlFilePtr &pCellmlFile, const SolverNlaPtr &pNlaSolver, bool pCompiled)
{
    auto cellmlFileAnalyser = pCellmlFile->analyser();

    if (cellmlFileAnalyser->errorCount() != 0) {
        addIssues(cellmlFileAnalyser);
    } else {
        // Get an NLA solver, if needed.

        if (pNlaSolver != nullptr) {
            mNlaSolverAddress = nlaSolverAddress(pNlaSolver.get());
        }

        // Get either a compiled or an interpreted version of the runtime.

#ifndef __EMSCRIPTEN__
        if (pCompiled) {
            // Determine the type of the model.

            auto cellmlFileType = pCellmlFile->type();
            auto differentialModel = (cellmlFileType == libcellml::AnalyserModel::Type::ODE)
                                     || (cellmlFileType == libcellml::AnalyserModel::Type::DAE);

            // Generate some code for the given CellML file.

            auto generator = libcellml::Generator::create();
            auto generatorProfile = libcellml::GeneratorProfile::create();

            generatorProfile->setOriginCommentString("");
            generatorProfile->setImplementationHeaderString("");
            generatorProfile->setImplementationVersionString("");
            generatorProfile->setImplementationStateCountString("");
            generatorProfile->setImplementationVariableCountString("");
            generatorProfile->setImplementationLibcellmlVersionString("");
            generatorProfile->setImplementationVoiInfoString("");
            generatorProfile->setImplementationStateInfoString("");
            generatorProfile->setImplementationVariableInfoString("");
            generatorProfile->setImplementationCreateStatesArrayMethodString("");
            generatorProfile->setImplementationCreateVariablesArrayMethodString("");
            generatorProfile->setImplementationDeleteArrayMethodString("");

            if (pNlaSolver != nullptr) {
                generatorProfile->setExternNlaSolveMethodString("typedef unsigned long long size_t;\n"
                                                                "\n"
                                                                "extern void nlaSolve(const char *, void (*objectiveFunction)(double *, double *, void *),\n"
                                                                "                     double *u, size_t n, void *data);\n");
                generatorProfile->setNlaSolveCallString(differentialModel,
                                                        std::string("nlaSolve(\"") + mNlaSolverAddress + "\", objectiveFunction[INDEX], u, [SIZE], &rfi);\n");
            }

            generator->setModel(pCellmlFile->analyserModel());
            generator->setProfile(generatorProfile);

            // Compile the generated code.

            mCompiler = Compiler::create();

#    ifdef CODE_COVERAGE_ENABLED
            mCompiler->compile(generator->implementationCode());
#    else
            if (!mCompiler->compile(generator->implementationCode())) {
                // The compilation failed, so add the issues it generated.

                addIssues(mCompiler);

                return;
            }
#    endif

            // Make sure that our compiler knows about nlaSolve(), if needed.

            if ((cellmlFileType == libcellml::AnalyserModel::Type::NLA)
                || (cellmlFileType == libcellml::AnalyserModel::Type::DAE)) {
#    ifndef CODE_COVERAGE_ENABLED
                const bool functionAdded =
#    endif
                    mCompiler->addFunction("nlaSolve", reinterpret_cast<void *>(nlaSolve));

#    ifndef CODE_COVERAGE_ENABLED
                if (!functionAdded) {
                    addIssues(mCompiler);

                    return;
                }
#    endif
            }

            // Retrieve our algebraic/differential functions and make sure that we managed to retrieve them.

            if (differentialModel) {
                mInitialiseCompiledVariablesForDifferentialModel = reinterpret_cast<InitialiseCompiledVariablesForDifferentialModel>(mCompiler->function("initialiseVariables"));
                mComputeCompiledComputedConstants = reinterpret_cast<ComputeCompiledComputedConstants>(mCompiler->function("computeComputedConstants"));
                mComputeCompiledRates = reinterpret_cast<ComputeCompiledRates>(mCompiler->function("computeRates"));
                mComputeCompiledVariablesForDifferentialModel = reinterpret_cast<ComputeCompiledVariablesForDifferentialModel>(mCompiler->function("computeVariables"));

#    ifndef CODE_COVERAGE_ENABLED
                if ((mInitialiseCompiledVariablesForDifferentialModel == nullptr)
                    || (mComputeCompiledComputedConstants == nullptr)
                    || (mComputeCompiledRates == nullptr)
                    || (mComputeCompiledVariablesForDifferentialModel == nullptr)) {
                    if (cellmlFileType == libcellml::AnalyserModel::Type::ODE) {
                        addError("The functions needed to compute the ODE model could not be retrieved.");
                    } else {
                        addError("The functions needed to compute the DAE model could not be retrieved.");
                    }
                }
#    endif
            } else {
                mInitialiseCompiledVariablesForAlgebraicModel = reinterpret_cast<InitialiseCompiledVariablesForAlgebraicModel>(mCompiler->function("initialiseVariables"));
                mComputeCompiledComputedConstants = reinterpret_cast<ComputeCompiledComputedConstants>(mCompiler->function("computeComputedConstants"));
                mComputeCompiledVariablesForAlgebraicModel = reinterpret_cast<ComputeCompiledVariablesForAlgebraicModel>(mCompiler->function("computeVariables"));

#    ifndef CODE_COVERAGE_ENABLED
                if ((mInitialiseCompiledVariablesForAlgebraicModel == nullptr)
                    || (mComputeCompiledComputedConstants == nullptr)
                    || (mComputeCompiledVariablesForAlgebraicModel == nullptr)) {
                    if (cellmlFileType == libcellml::AnalyserModel::Type::ALGEBRAIC) {
                        addError("The functions needed to compute the algebraic model could not be retrieved.");
                    } else {
                        addError("The functions needed to compute the NLA model could not be retrieved.");
                    }
                }
#    endif
            }
        } else {
#endif
            auto interpreter = libcellml::Interpreter::create();

            interpreter->setModel(pCellmlFile->analyserModel());

            mInitialiseInterpretedVariablesForAlgebraicModel = std::function<void(double *pVariables)>([interpreter](double *pVariables) {
                interpreter->initialiseVariablesForAlgebraicModel(pVariables);
            });
            mInitialiseInterpretedVariablesForDifferentialModel = std::function<void(double *pStates, double *pRates, double *pVariables)>([interpreter](double *pStates, double *pRates, double *pVariables) {
                interpreter->initialiseVariablesForDifferentialModel(pStates, pRates, pVariables);
            });
            mComputeInterpretedComputedConstants = std::function<void(double *pVariables)>([interpreter](double *pVariables) {
                interpreter->computeComputedConstants(pVariables);
            });
            mComputeInterpretedRates = std::function<void(double pVoi, double *pStates, double *pRates, double *pVariables)>([interpreter](double pVoi, double *pStates, double *pRates, double *pVariables) {
                interpreter->computeRates(pVoi, pStates, pRates, pVariables);
            });
            mComputeInterpretedVariablesForAlgebraicModel = std::function<void(double *pVariables)>([interpreter](double *pVariables) {
                interpreter->computeVariablesForAlgebraicModel(pVariables);
            });
            mComputeInterpretedVariablesForDifferentialModel = std::function<void(double pVoi, double *pStates, double *pRates, double *pVariables)>([interpreter](double pVoi, double *pStates, double *pRates, double *pVariables) {
                interpreter->computeVariablesForDifferentialModel(pVoi, pStates, pRates, pVariables);
            });
#ifndef __EMSCRIPTEN__
        }
#endif
    }
}

CellmlFileRuntime::Impl::~Impl()
{
    delete[] mNlaSolverAddress;
}

CellmlFileRuntime::CellmlFileRuntime(const CellmlFilePtr &pCellmlFile, const SolverNlaPtr &pNlaSolver, bool pCompiled)
    : Logger(new Impl {pCellmlFile, pNlaSolver, pCompiled})
{
}

CellmlFileRuntime::~CellmlFileRuntime()
{
    delete pimpl();
}

CellmlFileRuntime::Impl *CellmlFileRuntime::pimpl()
{
    return static_cast<Impl *>(Logger::pimpl());
}

const CellmlFileRuntime::Impl *CellmlFileRuntime::pimpl() const
{
    return static_cast<const Impl *>(Logger::pimpl());
}

CellmlFileRuntimePtr CellmlFileRuntime::create(const CellmlFilePtr &pCellmlFile, const SolverNlaPtr &pNlaSolver,
                                               bool pCompiled)
{
    return CellmlFileRuntimePtr {new CellmlFileRuntime {pCellmlFile, pNlaSolver, pCompiled}};
}

CellmlFileRuntime::InitialiseCompiledVariablesForAlgebraicModel CellmlFileRuntime::initialiseCompiledVariablesForAlgebraicModel() const
{
    return pimpl()->mInitialiseCompiledVariablesForAlgebraicModel;
}

CellmlFileRuntime::InitialiseCompiledVariablesForDifferentialModel CellmlFileRuntime::initialiseCompiledVariablesForDifferentialModel() const
{
    return pimpl()->mInitialiseCompiledVariablesForDifferentialModel;
}

CellmlFileRuntime::ComputeCompiledComputedConstants CellmlFileRuntime::computeCompiledComputedConstants() const
{
    return pimpl()->mComputeCompiledComputedConstants;
}

CellmlFileRuntime::ComputeCompiledRates CellmlFileRuntime::computeCompiledRates() const
{
    return pimpl()->mComputeCompiledRates;
}

CellmlFileRuntime::ComputeCompiledVariablesForAlgebraicModel CellmlFileRuntime::computeCompiledVariablesForAlgebraicModel() const
{
    return pimpl()->mComputeCompiledVariablesForAlgebraicModel;
}

CellmlFileRuntime::ComputeCompiledVariablesForDifferentialModel CellmlFileRuntime::computeCompiledVariablesForDifferentialModel() const
{
    return pimpl()->mComputeCompiledVariablesForDifferentialModel;
}

CellmlFileRuntime::InitialiseInterpretedVariablesForAlgebraicModel CellmlFileRuntime::initialiseInterpretedVariablesForAlgebraicModel() const
{
    return pimpl()->mInitialiseInterpretedVariablesForAlgebraicModel;
}

CellmlFileRuntime::InitialiseInterpretedVariablesForDifferentialModel CellmlFileRuntime::initialiseInterpretedVariablesForDifferentialModel() const
{
    return pimpl()->mInitialiseInterpretedVariablesForDifferentialModel;
}

CellmlFileRuntime::ComputeInterpretedComputedConstants CellmlFileRuntime::computeInterpretedComputedConstants() const
{
    return pimpl()->mComputeInterpretedComputedConstants;
}

CellmlFileRuntime::ComputeInterpretedRates CellmlFileRuntime::computeInterpretedRates() const
{
    return pimpl()->mComputeInterpretedRates;
}

CellmlFileRuntime::ComputeInterpretedVariablesForAlgebraicModel CellmlFileRuntime::computeInterpretedVariablesForAlgebraicModel() const
{
    return pimpl()->mComputeInterpretedVariablesForAlgebraicModel;
}

CellmlFileRuntime::ComputeInterpretedVariablesForDifferentialModel CellmlFileRuntime::computeInterpretedVariablesForDifferentialModel() const
{
    return pimpl()->mComputeInterpretedVariablesForDifferentialModel;
}

} // namespace libOpenCOR
