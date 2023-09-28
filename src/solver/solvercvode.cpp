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

#include "solvercvode_p.h"
#include "utils.h"

namespace libOpenCOR {

// Integration methods.

const std::string SolverCvode::Impl::ADAMS_MOULTON_METHOD = "Adams-Moulton"; // NOLINT
const std::string SolverCvode::Impl::BDF_METHOD = "BDF"; // NOLINT

// Iteration types.

const std::string SolverCvode::Impl::FUNCTIONAL_ITERATION_TYPE = "Functional"; // NOLINT
const std::string SolverCvode::Impl::NEWTON_ITERATION_TYPE = "Newton"; // NOLINT

// Linear solvers.

const std::string SolverCvode::Impl::DENSE_LINEAR_SOLVER = "Dense"; // NOLINT
const std::string SolverCvode::Impl::BANDED_LINEAR_SOLVER = "Banded"; // NOLINT
const std::string SolverCvode::Impl::DIAGONAL_LINEAR_SOLVER = "Diagonal"; // NOLINT
const std::string SolverCvode::Impl::GMRES_LINEAR_SOLVER = "GMRES"; // NOLINT
const std::string SolverCvode::Impl::BICGSTAB_LINEAR_SOLVER = "BiCGStab"; // NOLINT
const std::string SolverCvode::Impl::TFQMR_LINEAR_SOLVER = "TFQMR"; // NOLINT

// Preconditioners.

const std::string SolverCvode::Impl::NO_PRECONDITIONER = "None"; // NOLINT
const std::string SolverCvode::Impl::BANDED_PRECONDITIONER = "Banded"; // NOLINT

// Properties information.

const std::string SolverCvode::Impl::ID = "KISAO:0000019"; // NOLINT
const std::string SolverCvode::Impl::NAME = "CVODE"; // NOLINT

const std::string SolverCvode::Impl::MAXIMUM_STEP_ID = "KISAO:0000467"; // NOLINT
const std::string SolverCvode::Impl::MAXIMUM_STEP_NAME = "Maximum step"; // NOLINT

const std::string SolverCvode::Impl::MAXIMUM_NUMBER_OF_STEPS_ID = "KISAO:0000415"; // NOLINT
const std::string SolverCvode::Impl::MAXIMUM_NUMBER_OF_STEPS_NAME = "Maximum number of steps"; // NOLINT

const std::string SolverCvode::Impl::INTEGRATION_METHOD_ID = "KISAO:0000475"; // NOLINT
const std::string SolverCvode::Impl::INTEGRATION_METHOD_NAME = "Integration method"; // NOLINT
const std::string SolverCvode::Impl::INTEGRATION_METHOD_DEFAULT_VALUE = SolverCvode::Impl::BDF_METHOD; // NOLINT

const std::string SolverCvode::Impl::ITERATION_TYPE_ID = "KISAO:0000476"; // NOLINT
const std::string SolverCvode::Impl::ITERATION_TYPE_NAME = "Iteration type"; // NOLINT
const std::string SolverCvode::Impl::ITERATION_TYPE_DEFAULT_VALUE = SolverCvode::Impl::NEWTON_ITERATION_TYPE; // NOLINT

const std::string SolverCvode::Impl::LINEAR_SOLVER_ID = "KISAO:0000477"; // NOLINT
const std::string SolverCvode::Impl::LINEAR_SOLVER_NAME = "Linear solver"; // NOLINT
const std::string SolverCvode::Impl::LINEAR_SOLVER_DEFAULT_VALUE = SolverCvode::Impl::DENSE_LINEAR_SOLVER; // NOLINT

const std::string SolverCvode::Impl::PRECONDITIONER_ID = "KISAO:0000478"; // NOLINT
const std::string SolverCvode::Impl::PRECONDITIONER_NAME = "Preconditioner"; // NOLINT
const std::string SolverCvode::Impl::PRECONDITIONER_DEFAULT_VALUE = SolverCvode::Impl::BANDED_PRECONDITIONER; // NOLINT

const std::string SolverCvode::Impl::UPPER_HALF_BANDWIDTH_ID = "KISAO:0000479"; // NOLINT
const std::string SolverCvode::Impl::UPPER_HALF_BANDWIDTH_NAME = "Upper half-bandwidth"; // NOLINT

const std::string SolverCvode::Impl::LOWER_HALF_BANDWIDTH_ID = "KISAO:0000480"; // NOLINT
const std::string SolverCvode::Impl::LOWER_HALF_BANDWIDTH_NAME = "Lower half-bandwidth"; // NOLINT

const std::string SolverCvode::Impl::RELATIVE_TOLERANCE_ID = "KISAO:0000209"; // NOLINT
const std::string SolverCvode::Impl::RELATIVE_TOLERANCE_NAME = "Relative tolerance"; // NOLINT

const std::string SolverCvode::Impl::ABSOLUTE_TOLERANCE_ID = "KISAO:0000211"; // NOLINT
const std::string SolverCvode::Impl::ABSOLUTE_TOLERANCE_NAME = "Absolute tolerance"; // NOLINT

const std::string SolverCvode::Impl::INTERPOLATE_SOLUTION_ID = "KISAO:0000481"; // NOLINT
const std::string SolverCvode::Impl::INTERPOLATE_SOLUTION_NAME = "Interpolate solution"; // NOLINT

// Solver.

SolverPtr SolverCvode::Impl::create()
{
    return std::shared_ptr<SolverCvode> {new SolverCvode {}};
}

std::vector<SolverPropertyPtr> SolverCvode::Impl::propertiesInfo()
{
    return {
        Solver::Impl::createProperty(SolverProperty::Type::DoubleGe0, MAXIMUM_STEP_ID, MAXIMUM_STEP_NAME,
                                     {},
                                     toString(MAXIMUM_STEP_DEFAULT_VALUE),
                                     true),
        Solver::Impl::createProperty(SolverProperty::Type::IntegerGt0, MAXIMUM_NUMBER_OF_STEPS_ID, MAXIMUM_NUMBER_OF_STEPS_NAME,
                                     {},
                                     toString(MAXIMUM_NUMBER_OF_STEPS_DEFAULT_VALUE),
                                     false),
        Solver::Impl::createProperty(SolverProperty::Type::List, INTEGRATION_METHOD_ID, INTEGRATION_METHOD_NAME,
                                     {ADAMS_MOULTON_METHOD,
                                      BDF_METHOD},
                                     INTEGRATION_METHOD_DEFAULT_VALUE,
                                     false),
        Solver::Impl::createProperty(SolverProperty::Type::List, ITERATION_TYPE_ID, ITERATION_TYPE_NAME,
                                     {FUNCTIONAL_ITERATION_TYPE,
                                      NEWTON_ITERATION_TYPE},
                                     ITERATION_TYPE_DEFAULT_VALUE,
                                     false),
        Solver::Impl::createProperty(SolverProperty::Type::List, LINEAR_SOLVER_ID, LINEAR_SOLVER_NAME,
                                     {DENSE_LINEAR_SOLVER,
                                      BANDED_LINEAR_SOLVER,
                                      DIAGONAL_LINEAR_SOLVER,
                                      GMRES_LINEAR_SOLVER,
                                      BICGSTAB_LINEAR_SOLVER,
                                      TFQMR_LINEAR_SOLVER},
                                     LINEAR_SOLVER_DEFAULT_VALUE,
                                     false),
        Solver::Impl::createProperty(SolverProperty::Type::List, PRECONDITIONER_ID, PRECONDITIONER_NAME,
                                     {NO_PRECONDITIONER,
                                      BANDED_PRECONDITIONER},
                                     PRECONDITIONER_DEFAULT_VALUE,
                                     false),
        Solver::Impl::createProperty(SolverProperty::Type::IntegerGe0, UPPER_HALF_BANDWIDTH_ID, UPPER_HALF_BANDWIDTH_NAME,
                                     {},
                                     toString(UPPER_HALF_BANDWIDTH_DEFAULT_VALUE),
                                     false),
        Solver::Impl::createProperty(SolverProperty::Type::IntegerGe0, LOWER_HALF_BANDWIDTH_ID, LOWER_HALF_BANDWIDTH_NAME,
                                     {},
                                     toString(LOWER_HALF_BANDWIDTH_DEFAULT_VALUE),
                                     false),
        Solver::Impl::createProperty(SolverProperty::Type::DoubleGe0, RELATIVE_TOLERANCE_ID, RELATIVE_TOLERANCE_NAME,
                                     {},
                                     toString(RELATIVE_TOLERANCE_DEFAULT_VALUE),
                                     false),
        Solver::Impl::createProperty(SolverProperty::Type::DoubleGe0, ABSOLUTE_TOLERANCE_ID, ABSOLUTE_TOLERANCE_NAME,
                                     {},
                                     toString(ABSOLUTE_TOLERANCE_DEFAULT_VALUE),
                                     false),
        Solver::Impl::createProperty(SolverProperty::Type::Boolean, INTERPOLATE_SOLUTION_ID, INTERPOLATE_SOLUTION_NAME,
                                     {},
                                     toString(INTERPOLATE_SOLUTION_DEFAULT_VALUE),
                                     false),
    };
}

SolverCvode::Impl::Impl()
    : SolverOde::Impl()
{
    mIsValid = true;

    mProperties[MAXIMUM_STEP_ID] = toString(MAXIMUM_STEP_DEFAULT_VALUE);
    mProperties[MAXIMUM_NUMBER_OF_STEPS_ID] = toString(MAXIMUM_NUMBER_OF_STEPS_DEFAULT_VALUE);
    mProperties[INTEGRATION_METHOD_ID] = INTEGRATION_METHOD_DEFAULT_VALUE;
    mProperties[ITERATION_TYPE_ID] = ITERATION_TYPE_DEFAULT_VALUE;
    mProperties[LINEAR_SOLVER_ID] = LINEAR_SOLVER_DEFAULT_VALUE;
    mProperties[PRECONDITIONER_ID] = PRECONDITIONER_DEFAULT_VALUE;
    mProperties[UPPER_HALF_BANDWIDTH_ID] = toString(UPPER_HALF_BANDWIDTH_DEFAULT_VALUE);
    mProperties[LOWER_HALF_BANDWIDTH_ID] = toString(LOWER_HALF_BANDWIDTH_DEFAULT_VALUE);
    mProperties[RELATIVE_TOLERANCE_ID] = toString(RELATIVE_TOLERANCE_DEFAULT_VALUE);
    mProperties[ABSOLUTE_TOLERANCE_ID] = toString(ABSOLUTE_TOLERANCE_DEFAULT_VALUE);
    mProperties[INTERPOLATE_SOLUTION_ID] = toString(INTERPOLATE_SOLUTION_DEFAULT_VALUE);
}

std::map<std::string, std::string> SolverCvode::Impl::propertiesId() const
{
    static const std::map<std::string, std::string> PROPERTIES_ID = {
        {MAXIMUM_STEP_NAME, MAXIMUM_STEP_ID},
        {MAXIMUM_NUMBER_OF_STEPS_NAME, MAXIMUM_NUMBER_OF_STEPS_ID},
        {INTEGRATION_METHOD_NAME, INTEGRATION_METHOD_ID},
        {ITERATION_TYPE_NAME, ITERATION_TYPE_ID},
        {LINEAR_SOLVER_NAME, LINEAR_SOLVER_ID},
        {PRECONDITIONER_NAME, PRECONDITIONER_ID},
        {UPPER_HALF_BANDWIDTH_NAME, UPPER_HALF_BANDWIDTH_ID},
        {LOWER_HALF_BANDWIDTH_NAME, LOWER_HALF_BANDWIDTH_ID},
        {RELATIVE_TOLERANCE_NAME, RELATIVE_TOLERANCE_ID},
        {ABSOLUTE_TOLERANCE_NAME, ABSOLUTE_TOLERANCE_ID},
        {INTERPOLATE_SOLUTION_NAME, INTERPOLATE_SOLUTION_ID},
    };

    return PROPERTIES_ID;
}

bool SolverCvode::Impl::initialise(size_t pSize, double *pStates, double *pRates, double *pVariables,
                                   ComputeRates pComputeRates)
{
    removeAllIssues();

    // Retrieve the solver's properties.

    /*---GRY---
        bool ok = true;
        auto step = toDouble(mProperties[STEP_ID], ok);

        if (ok && (step > 0.0)) {
            mStep = step;
        } else {
            addError(R"(The "Step" property has an invalid value (")" + mProperties[STEP_ID] + R"("). It must be a floating point number greater than zero.)");

            return false;
        }
    */

    // Initialise the ODE solver itself.

    return SolverOde::Impl::initialise(pSize, pStates, pRates, pVariables, pComputeRates);
}

bool SolverCvode::Impl::solve(double &pVoi, double pVoiEnd) const
{
    //---GRY--- TO BE DONE.

    pVoi = pVoiEnd;

    return true;
}

SolverCvode::SolverCvode()
    : SolverOde(new Impl())
{
}

SolverCvode::~SolverCvode()
{
    delete pimpl();
}

SolverCvode::Impl *SolverCvode::pimpl()
{
    return static_cast<Impl *>(SolverOde::pimpl());
}

const SolverCvode::Impl *SolverCvode::pimpl() const
{
    return static_cast<const Impl *>(SolverOde::pimpl());
}

bool SolverCvode::initialise(size_t pSize, double *pStates, double *pRates, double *pVariables,
                             ComputeRates pComputeRates)
{
    return pimpl()->initialise(pSize, pStates, pRates, pVariables, pComputeRates);
}

bool SolverCvode::solve(double &pVoi, double pVoiEnd) const
{
    return pimpl()->solve(pVoi, pVoiEnd);
}

} // namespace libOpenCOR
