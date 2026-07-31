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

#include "cellmlfile.h"

#include <format>
#include <unordered_set>

namespace libOpenCOR {

CellmlFileRuntime::Impl::Impl(const CellmlFilePtr &pCellmlFile, const SolverNlaPtr &pNlaSolver)
{
#ifndef __EMSCRIPTEN__
    (void)pNlaSolver;
#endif

    auto cellmlFileAnalyser {pCellmlFile->analyser()};

    if (cellmlFileAnalyser->errorCount() != 0) {
        addIssues(cellmlFileAnalyser, "Analyser");
    } else {
        // Determine the type of the model.

        auto cellmlFileType {pCellmlFile->type()};
        auto differentialModel {(cellmlFileType == libcellml::AnalyserModel::Type::ODE)
                                || (cellmlFileType == libcellml::AnalyserModel::Type::DAE)};

        // Generate some code for the given CellML file.

        auto generator {libcellml::Generator::create()};
        auto generatorProfile {libcellml::GeneratorProfile::create()};

        generatorProfile->setOriginCommentString("");
        generatorProfile->setImplementationHeaderString("");
        generatorProfile->setImplementationVersionString("");
        generatorProfile->setImplementationStateCountString("");
        generatorProfile->setImplementationConstantCountString("");
        generatorProfile->setImplementationComputedConstantCountString("");
        generatorProfile->setImplementationAlgebraicVariableCountString("");
        generatorProfile->setImplementationExternalVariableCountString("");
        generatorProfile->setImplementationLibcellmlVersionString("");
        generatorProfile->setImplementationVoiInfoString("");
        generatorProfile->setImplementationStateInfoString("");
        generatorProfile->setImplementationConstantInfoString("");
        generatorProfile->setImplementationComputedConstantInfoString("");
        generatorProfile->setImplementationAlgebraicVariableInfoString("");
        generatorProfile->setImplementationExternalVariableInfoString("");
        generatorProfile->setImplementationCreateStatesArrayMethodString("");
        generatorProfile->setImplementationCreateConstantsArrayMethodString("");
        generatorProfile->setImplementationCreateComputedConstantsArrayMethodString("");
        generatorProfile->setImplementationCreateAlgebraicVariablesArrayMethodString("");
        generatorProfile->setImplementationCreateExternalVariablesArrayMethodString("");
        generatorProfile->setImplementationDeleteArrayMethodString("");

        static constexpr auto WITH_EXTERNAL_VARIABLES {false};

#ifdef __EMSCRIPTEN__
        // Allocate the memory needed by our objective functions using thread-local static buffers.

        if (pNlaSolver != nullptr) {
            if (differentialModel) {
                generatorProfile->setFindRootMethodString(differentialModel, WITH_EXTERNAL_VARIABLES,
                                                          R"(void findRoot[INDEX](double voi, double *states, double *rates, double *constants, double *computedConstants, double *algebraicVariables)
{
    static RootFindingInfo rfiStorage;
    static double u[[SIZE]];

    RootFindingInfo *rfi = &rfiStorage;

    rfi->voi = voi;
    rfi->states = states;
    rfi->rates = rates;
    rfi->constants = constants;
    rfi->computedConstants = computedConstants;
    rfi->algebraicVariables = algebraicVariables;

[CODE]
}
)");
            } else {
                generatorProfile->setFindRootMethodString(differentialModel, WITH_EXTERNAL_VARIABLES,
                                                          R"(void findRoot[INDEX](double *constants, double *computedConstants, double *algebraicVariables)
{
    static RootFindingInfo rfiStorage;
    static double u[[SIZE]];

    RootFindingInfo *rfi = &rfiStorage;

    rfi->constants = constants;
    rfi->computedConstants = computedConstants;
    rfi->algebraicVariables = algebraicVariables;

[CODE]
}
)");
            }
        }

        // Export our various methods.

        auto exportJavaScriptName = [](const std::string &pName) -> std::string {
            std::string exportName;

            exportName.reserve(pName.size() + 31); // NOLINT

            exportName += "__attribute__((export_name(\"";
            exportName += pName;
            exportName += "\")))\n";

            return exportName;
        };

        auto prependExportName = [&exportJavaScriptName](const std::string &pName, const std::string &pCode) {
            auto exportName {exportJavaScriptName(pName)};
            std::string res;

            res.reserve(exportName.size() + pCode.size());

            res += exportName;
            res += pCode;

            return res;
        };

        generatorProfile->setImplementationInitialiseArraysMethodString(differentialModel,
                                                                        prependExportName("initialiseArrays", generatorProfile->implementationInitialiseArraysMethodString(differentialModel)));
        generatorProfile->setImplementationComputeComputedConstantsMethodString(differentialModel,
                                                                                prependExportName("computeComputedConstants", generatorProfile->implementationComputeComputedConstantsMethodString(differentialModel)));
        generatorProfile->setImplementationComputeRatesMethodString(WITH_EXTERNAL_VARIABLES,
                                                                    prependExportName("computeRates", generatorProfile->implementationComputeRatesMethodString(WITH_EXTERNAL_VARIABLES)));
        generatorProfile->setImplementationComputeVariablesMethodString(differentialModel, WITH_EXTERNAL_VARIABLES,
                                                                        prependExportName("computeVariables", generatorProfile->implementationComputeVariablesMethodString(differentialModel, WITH_EXTERNAL_VARIABLES)));
#endif

        if (pNlaSolver != nullptr) {
            // Note: both uintptr_t and size_t are defined as follows:
            //        - Emscripten (wasm32): unsigned int (which is the same as unsigned long on 32 bits and is what we
            //          need to use here since malloc() expects an unsigned long);
            //        - Windows (64 bits): unsigned long long; and
            //        - Linux/macOS (64 bits): unsigned long.

#ifdef __EMSCRIPTEN__
            generatorProfile->setExternNlaSolveMethodString(R"(typedef unsigned long uintptr_t;
typedef unsigned long size_t;

extern void *malloc(size_t size);
extern void free(void *ptr);

extern uintptr_t nlaSolverAddress();
extern void nlaSolve(uintptr_t nlaSolverAddress, size_t computeObjectiveFunctionIndex, uintptr_t u, size_t n, uintptr_t data);
)");
            generatorProfile->setNlaSolveCallString(differentialModel, WITH_EXTERNAL_VARIABLES,
                                                    "nlaSolve(nlaSolverAddress(), [INDEX], (uintptr_t) u, [SIZE], (uintptr_t) rfi);\n");
#else
#    ifdef BUILDING_USING_MSVC
            generatorProfile->setExternNlaSolveMethodString(R"(typedef unsigned long long uintptr_t;
typedef unsigned long long size_t;

extern uintptr_t nlaSolverAddress();
extern void nlaSolve(uintptr_t nlaSolverAddress, void (*objectiveFunction)(double *, double *, void *),
                     double *u, size_t n, void *data);
)");
#    else
            generatorProfile->setExternNlaSolveMethodString(R"(typedef unsigned long uintptr_t;
typedef unsigned long size_t;

extern uintptr_t nlaSolverAddress();
extern void nlaSolve(uintptr_t nlaSolverAddress, void (*objectiveFunction)(double *, double *, void *),
                     double *u, size_t n, void *data);
)");
#    endif
            generatorProfile->setNlaSolveCallString(differentialModel, WITH_EXTERNAL_VARIABLES,
                                                    "nlaSolve(nlaSolverAddress(), objectiveFunction[INDEX], u, [SIZE], &rfi);\n");
#endif
        }

#ifdef __EMSCRIPTEN__
        // Export our various objective functions.

        auto implementationCode {generator->implementationCode(pCellmlFile->analyserModel(), generatorProfile)};

        if (pNlaSolver != nullptr) {
            std::unordered_set<size_t> handledNlaSystemIndices;
            const auto &analyserEquations = pCellmlFile->analyserModel()->analyserEquations();

            handledNlaSystemIndices.reserve(analyserEquations.size());

            for (const auto &analyserEquation : analyserEquations) {
                if (analyserEquation->type() == libcellml::AnalyserEquation::Type::NLA) {
                    auto nlaSystemIndex {analyserEquation->nlaSystemIndex()};

                    if (!handledNlaSystemIndices.contains(nlaSystemIndex)) {
                        auto objectiveFunctionName {"objectiveFunction" + std::format("{}", nlaSystemIndex)};

                        implementationCode.insert(implementationCode.find("void " + objectiveFunctionName),
                                                  exportJavaScriptName(objectiveFunctionName));

                        handledNlaSystemIndices.insert(nlaSystemIndex);
                    }
                }
            }
        }
#endif

        // Compile the generated code.

        mCompiler = Compiler::create();

#ifdef __EMSCRIPTEN__
        if (!mCompiler->compile(implementationCode, mWasmModule)) {
            // The compilation failed, so add the issues it generated.

            addIssues(mCompiler, "Compiler");

            return;
        }
#else
#    ifdef CODE_COVERAGE_ENABLED
        mCompiler->compile(generator->implementationCode(pCellmlFile->analyserModel(), generatorProfile));
#    else
        if (!mCompiler->compile(generator->implementationCode(pCellmlFile->analyserModel(), generatorProfile))) {
            // The compilation failed, so add the issues it generated.

            addIssues(mCompiler, "Compiler");

            return;
        }
#    endif

        // Make sure that our compiler knows about nlaSolve(), if needed.

        if ((cellmlFileType == libcellml::AnalyserModel::Type::NLA)
            || (cellmlFileType == libcellml::AnalyserModel::Type::DAE)) {
#    ifndef CODE_COVERAGE_ENABLED
            auto functionAdded =
#    endif
                mCompiler->addFunction("nlaSolverAddress", reinterpret_cast<void *>(nlaSolverAddress));

#    ifndef CODE_COVERAGE_ENABLED
            if (!functionAdded) {
                addIssues(mCompiler, "Compiler");

                return;
            }

            functionAdded =
#    endif
                mCompiler->addFunction("nlaSolve", reinterpret_cast<void *>(nlaSolve));

#    ifndef CODE_COVERAGE_ENABLED
            if (!functionAdded) {
                addIssues(mCompiler, "Compiler");

                return;
            }
#    endif
        }

        // Retrieve our algebraic/differential functions and make sure that we managed to retrieve them.

        if (differentialModel) {
            mInitialiseArraysForDifferentialModel = reinterpret_cast<InitialiseArraysForDifferentialModel>(mCompiler->function("initialiseArrays"));
            mComputeComputedConstantsForDifferentialModel = reinterpret_cast<ComputeComputedConstantsForDifferentialModel>(mCompiler->function("computeComputedConstants"));
            mComputeRates = reinterpret_cast<ComputeRates>(mCompiler->function("computeRates"));
            mComputeVariablesForDifferentialModel = reinterpret_cast<ComputeVariablesForDifferentialModel>(mCompiler->function("computeVariables"));

#    ifndef CODE_COVERAGE_ENABLED
            if ((mInitialiseArraysForDifferentialModel == nullptr)
                || (mComputeComputedConstantsForDifferentialModel == nullptr)
                || (mComputeRates == nullptr)
                || (mComputeVariablesForDifferentialModel == nullptr)) {
                addError(std::string("The functions needed to compute the ")
                         + ((cellmlFileType == libcellml::AnalyserModel::Type::ODE) ? "ODE" : "DAE")
                         + " model could not be retrieved.");
            }
#    endif
        } else {
            mInitialiseArraysForAlgebraicModel = reinterpret_cast<InitialiseArraysForAlgebraicModel>(mCompiler->function("initialiseArrays"));
            mComputeComputedConstantsForAlgebraicModel = reinterpret_cast<ComputeComputedConstantsForAlgebraicModel>(mCompiler->function("computeComputedConstants"));
            mComputeVariablesForAlgebraicModel = reinterpret_cast<ComputeVariablesForAlgebraicModel>(mCompiler->function("computeVariables"));

#    ifndef CODE_COVERAGE_ENABLED
            if ((mInitialiseArraysForAlgebraicModel == nullptr)
                || (mComputeComputedConstantsForAlgebraicModel == nullptr)
                || (mComputeVariablesForAlgebraicModel == nullptr)) {
                addError(std::string("The functions needed to compute the ")
                         + ((cellmlFileType == libcellml::AnalyserModel::Type::ALGEBRAIC) ? "algebraic" : "NLA")
                         + " model could not be retrieved.");
            }
#    endif
        }
#endif
    }
}

#ifdef __EMSCRIPTEN__
CellmlFileRuntime::Impl::~Impl()
{
    cleanupWorkerWasm();
}

// Lazily create a WebAssembly.Module + Instance in the current thread's private JavaScript scope and install its
// exported functions into the current thread's WebAssembly table, so C++ can call them directly through function
// pointers without any JavaScript round trip. If the current thread has already been initialised (see
// wasmFunctionBase), then reuse its existing table slots rather than grow the table again, which would otherwise
// both grow the table unboundedly and keep previously created WebAssembly instances alive forever.

// clang-format off
EM_JS(int, initialiseWorkerWasmJS, (const void* wasmBytesPtr, size_t wasmBytesSize, int wasmFunctionBase), {
    // Create a WebAssembly.Module + Instance in the current thread's private JavaScript scope, so C++ can call its
    // exported functions directly through the current thread's WebAssembly table.

    const wasmBytes = new Uint8Array(HEAPU8.buffer, wasmBytesPtr, wasmBytesSize);
    const wasmModule = new WebAssembly.Module(wasmBytes);
    const wasmInstance = new WebAssembly.Instance(wasmModule, {
        env: {
            __linear_memory: wasmMemory,
            __indirect_function_table: wasmTable,

            // Some standard C library functions.

            free: _free,
            malloc: _malloc,
            memset: _memset,

            // NLA solve function.

            nlaSolverAddress: function() {
                return globalThis.runtime.nlaSolverAddress;
            },
            nlaSolve: function(nlaSolverAddress, objectiveFunctionIndex, u, n, data) {
                Module.nlaSolve(nlaSolverAddress, objectiveFunctionIndex, u, n, data);
            },

            // Arithmetic operators.

            pow: _pow,
            sqrt: _sqrt,
            fabs: _fabs,
            exp: _exp,
            log: _log,
            log10: _log10,
            ceil: _ceil,
            floor: _floor,
            fmin: _fmin,
            fmax: _fmax,
            fmod: _fmod,

            // Trigonometric operators.

            sin: _sin,
            cos: _cos,
            tan: _tan,
            sinh: _sinh,
            cosh: _cosh,
            tanh: _tanh,
            asin: _asin,
            acos: _acos,
            atan: _atan,
            asinh: _asinh,
            acosh: _acosh,
            atanh: _atanh
        }
    });
    const exports = wasmInstance.exports;

    // Install the exported functions into the shared WebAssembly table (i.e. the main module's table, so C++ can call
    // them), in an order known to our getters:
    //   - Slot 0: initialiseArrays();
    //   - Slot 1: computeComputedConstants();
    //   - Slot 2: computeRates() (only exported by differential models, so the slot is left empty otherwise); and
    //   - Slot 3: computeVariables().

    const functionNames = ["initialiseArrays", "computeComputedConstants", "computeRates", "computeVariables"];
    const objectiveFunctionNames = [];

    for (const key in exports) {
        if (key.indexOf("objectiveFunction") === 0) {
            objectiveFunctionNames.push(key);
        }
    }

    // Reuse our existing table slots if the current thread has already been initialised, otherwise grow the table (and
    // only grow it further should more slots be needed than were previously allocated).

    const functionCount = functionNames.length + objectiveFunctionNames.length;
    let base = wasmFunctionBase;

    if (base === 0) {
        base = wasmTable.grow(functionCount);
    } else if (base + functionCount > wasmTable.length) {
        wasmTable.grow(base + functionCount - wasmTable.length);
    }

    for (let i = 0; i < functionNames.length; ++i) {
        const func = exports[functionNames[i]];

        if (func !== undefined) {
            wasmTable.set(base + i, func);
        }
    }

    // Do the same for our various objective functions, recording their table slots by NLA system index, so that C++
    // can resolve them without any JavaScript round trip.

    const computeObjectiveFunctionSlots = {};
    let slot = base + functionNames.length;

    for (const key of objectiveFunctionNames) {
        wasmTable.set(slot, exports[key]);

        computeObjectiveFunctionSlots[parseInt(key.substring(17), 10)] = slot;

        ++slot;
    }

    // Keep a small per-thread runtime record so that the generated code can resolve the NLA solver address and C++
    // can resolve objective-function table slots.

    globalThis.runtime = {
        nlaSolverAddress: 0,
        computeObjectiveFunctionSlots
    };

    return base;
}); // clang-format on

// The base index, in the current thread's WebAssembly table, of the current thread's runtime functions (each thread
// lazily creates its own WebAssembly instance, see initialiseWorkerWasmJS()). This is thread-local since the table
// slots are only valid on the thread on which they were installed, and it persists across re-initialisations on that
// thread so that its table slots can be reused (see the wasmFunctionBase parameter of initialiseWorkerWasmJS()).
// Note: the function pointers returned by computeRates(), initialiseArraysForDifferentialModel(), etc. are computed
//       from this thread-local base. They must only be called from the current thread (calling them from a different
//       thread, which has its own WebAssembly instance with different table slots, will silently execute the wrong
//       code). Solvers that cache these function pointers (see SolverOde::mComputeRates and
//       SolverCvodeUserData::computeRates) inherit this thread-affinity constraint.

namespace {
thread_local int sWasmFunctionBase = 0; // NOLINT
} // namespace

void CellmlFileRuntime::Impl::initialiseWorkerWasm() const
{
    sWasmFunctionBase = initialiseWorkerWasmJS(mWasmModule.data(), mWasmModule.size(), sWasmFunctionBase);
}

void CellmlFileRuntime::Impl::cleanupWorkerWasm() const
{
    // clang-format off
    EM_ASM({
        delete globalThis.runtime;
    }); // clang-format on
}

void CellmlFileRuntime::Impl::setNlaSolverAddress(uintptr_t pAddress) const
{
    // clang-format off
    EM_ASM({
        globalThis.runtime.nlaSolverAddress = $0;
    }, pAddress); // clang-format on
}

// The table slot offsets of the functions of our WebAssembly instances (see initialiseWorkerWasmJS()):
//   - Slot 0: initialiseArrays();
//   - Slot 1: computeComputedConstants();
//   - Slot 2: computeRates() (differential models only); and
//   - Slot 3: computeVariables().

static constexpr intptr_t INITIALISE_ARRAYS_TABLE_OFFSET = 0;
static constexpr intptr_t COMPUTE_COMPUTED_CONSTANTS_TABLE_OFFSET = 1;
static constexpr intptr_t COMPUTE_RATES_TABLE_OFFSET = 2;
static constexpr intptr_t COMPUTE_VARIABLES_TABLE_OFFSET = 3;

CellmlFileRuntime::InitialiseArraysForAlgebraicModel CellmlFileRuntime::Impl::initialiseArraysForAlgebraicModel() const
{
    return reinterpret_cast<InitialiseArraysForAlgebraicModel>(sWasmFunctionBase + INITIALISE_ARRAYS_TABLE_OFFSET);
}

CellmlFileRuntime::InitialiseArraysForDifferentialModel CellmlFileRuntime::Impl::initialiseArraysForDifferentialModel() const
{
    return reinterpret_cast<InitialiseArraysForDifferentialModel>(sWasmFunctionBase + INITIALISE_ARRAYS_TABLE_OFFSET);
}

CellmlFileRuntime::ComputeComputedConstantsForAlgebraicModel CellmlFileRuntime::Impl::computeComputedConstantsForAlgebraicModel() const
{
    return reinterpret_cast<ComputeComputedConstantsForAlgebraicModel>(sWasmFunctionBase + COMPUTE_COMPUTED_CONSTANTS_TABLE_OFFSET);
}

CellmlFileRuntime::ComputeComputedConstantsForDifferentialModel CellmlFileRuntime::Impl::computeComputedConstantsForDifferentialModel() const
{
    return reinterpret_cast<ComputeComputedConstantsForDifferentialModel>(sWasmFunctionBase + COMPUTE_COMPUTED_CONSTANTS_TABLE_OFFSET);
}

CellmlFileRuntime::ComputeRates CellmlFileRuntime::Impl::computeRates() const
{
    return reinterpret_cast<ComputeRates>(sWasmFunctionBase + COMPUTE_RATES_TABLE_OFFSET);
}

CellmlFileRuntime::ComputeVariablesForAlgebraicModel CellmlFileRuntime::Impl::computeVariablesForAlgebraicModel() const
{
    return reinterpret_cast<ComputeVariablesForAlgebraicModel>(sWasmFunctionBase + COMPUTE_VARIABLES_TABLE_OFFSET);
}

CellmlFileRuntime::ComputeVariablesForDifferentialModel CellmlFileRuntime::Impl::computeVariablesForDifferentialModel() const
{
    return reinterpret_cast<ComputeVariablesForDifferentialModel>(sWasmFunctionBase + COMPUTE_VARIABLES_TABLE_OFFSET);
}
#else
CellmlFileRuntime::InitialiseArraysForAlgebraicModel CellmlFileRuntime::Impl::initialiseArraysForAlgebraicModel() const
{
    return mInitialiseArraysForAlgebraicModel;
}

CellmlFileRuntime::InitialiseArraysForDifferentialModel CellmlFileRuntime::Impl::initialiseArraysForDifferentialModel() const
{
    return mInitialiseArraysForDifferentialModel;
}

CellmlFileRuntime::ComputeComputedConstantsForAlgebraicModel CellmlFileRuntime::Impl::computeComputedConstantsForAlgebraicModel() const
{
    return mComputeComputedConstantsForAlgebraicModel;
}

CellmlFileRuntime::ComputeComputedConstantsForDifferentialModel CellmlFileRuntime::Impl::computeComputedConstantsForDifferentialModel() const
{
    return mComputeComputedConstantsForDifferentialModel;
}

CellmlFileRuntime::ComputeRates CellmlFileRuntime::Impl::computeRates() const
{
    return mComputeRates;
}

CellmlFileRuntime::ComputeVariablesForAlgebraicModel CellmlFileRuntime::Impl::computeVariablesForAlgebraicModel() const
{
    return mComputeVariablesForAlgebraicModel;
}

CellmlFileRuntime::ComputeVariablesForDifferentialModel CellmlFileRuntime::Impl::computeVariablesForDifferentialModel() const
{
    return mComputeVariablesForDifferentialModel;
}
#endif

CellmlFileRuntime::CellmlFileRuntime(const CellmlFilePtr &pCellmlFile, const SolverNlaPtr &pNlaSolver)
    : Logger(std::make_unique<Impl>(pCellmlFile, pNlaSolver))
{
#ifdef CODE_COVERAGE_ENABLED
    (void)pimpl();
#endif
}

CellmlFileRuntime::~CellmlFileRuntime() = default;

CellmlFileRuntime::Impl *CellmlFileRuntime::pimpl()
{
    return static_cast<Impl *>(Logger::mPimpl.get());
}

const CellmlFileRuntime::Impl *CellmlFileRuntime::pimpl() const
{
    return static_cast<const Impl *>(Logger::mPimpl.get());
}

CellmlFileRuntimePtr CellmlFileRuntime::create(const CellmlFilePtr &pCellmlFile, const SolverNlaPtr &pNlaSolver)
{
    return CellmlFileRuntimePtr {new CellmlFileRuntime {pCellmlFile, pNlaSolver}};
}

#ifdef __EMSCRIPTEN__
void CellmlFileRuntime::initialiseWorkerWasm() const
{
    pimpl()->initialiseWorkerWasm();
}

void CellmlFileRuntime::cleanupWorkerWasm() const
{
    pimpl()->cleanupWorkerWasm();
}

void CellmlFileRuntime::setNlaSolverAddress(uintptr_t pAddress) const
{
    pimpl()->setNlaSolverAddress(pAddress);
}
#endif

CellmlFileRuntime::InitialiseArraysForAlgebraicModel CellmlFileRuntime::initialiseArraysForAlgebraicModel() const
{
    return pimpl()->initialiseArraysForAlgebraicModel();
}

CellmlFileRuntime::InitialiseArraysForDifferentialModel CellmlFileRuntime::initialiseArraysForDifferentialModel() const
{
    return pimpl()->initialiseArraysForDifferentialModel();
}

CellmlFileRuntime::ComputeComputedConstantsForAlgebraicModel CellmlFileRuntime::computeComputedConstantsForAlgebraicModel() const
{
    return pimpl()->computeComputedConstantsForAlgebraicModel();
}

CellmlFileRuntime::ComputeComputedConstantsForDifferentialModel CellmlFileRuntime::computeComputedConstantsForDifferentialModel() const
{
    return pimpl()->computeComputedConstantsForDifferentialModel();
}

CellmlFileRuntime::ComputeRates CellmlFileRuntime::computeRates() const
{
    return pimpl()->computeRates();
}

CellmlFileRuntime::ComputeVariablesForAlgebraicModel CellmlFileRuntime::computeVariablesForAlgebraicModel() const
{
    return pimpl()->computeVariablesForAlgebraicModel();
}

CellmlFileRuntime::ComputeVariablesForDifferentialModel CellmlFileRuntime::computeVariablesForDifferentialModel() const
{
    return pimpl()->computeVariablesForDifferentialModel();
}

} // namespace libOpenCOR
