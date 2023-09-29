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

#include "model.h"
#include "solvercvode.h"
#include "utils.h"

#include "gtest/gtest.h"

#include "tests/utils.h"

#include <libopencor>

TEST(CvodeSolverTest, maximumStepValueWithNonNumber)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Maximum step" property has an invalid value ("abc"). It must be a floating point number greater or equal to zero.)"},
    };

    solver->setProperty("Maximum step", "abc");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, maximumStepValueWithInvalidNumber)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Maximum step" property has an invalid value ("-1.234"). It must be a floating point number greater or equal to zero.)"},
    };

    solver->setProperty("Maximum step", "-1.234");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, maximumNumberOfStepsValueWithNonNumber)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Maximum number of steps" property has an invalid value ("abc"). It must be a floating point number greater than zero.)"},
    };

    solver->setProperty("Maximum number of steps", "abc");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, maximumNumberOfStepsValueWithInvalidNumber)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Maximum number of steps" property has an invalid value ("0"). It must be a floating point number greater than zero.)"},
    };

    solver->setProperty("Maximum number of steps", "0");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, integrationMethodValueWithUnknownValue)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Integration method" property has an invalid value ("Unknown integration method"). It must be equal to either "Adams-Moulton" or "BDF".)"},
    };

    solver->setProperty("Integration method", "Unknown integration method");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, iterationTypeValueWithUnknownValue)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Iteration type" property has an invalid value ("Unknown iteration type"). It must be equal to either "Functional" or "Newton".)"},
    };

    solver->setProperty("Iteration type", "Unknown iteration type");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, linearSolverValueWithUnknownValue)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Linear solver" property has an invalid value ("Unknown linear solver"). It must be equal to either "Dense", "Banded", "Diagonal", "GMRES", "BiCGStab", or "TFQMR".)"},
    };

    solver->setProperty("Linear solver", "Unknown linear solver");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, preconditionerValueWithUnknownValue)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Preconditioner" property has an invalid value ("Unknown preconditioner"). It must be equal to either "None" or "Banded".)"},
    };

    solver->setProperty("Linear solver", "GMRES");
    solver->setProperty("Preconditioner", "Unknown preconditioner");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, bandedLinearSolverAndUpperHalfBandwidthValueWithNonNumber)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Upper half-bandwidth" property has an invalid value ("abc"). It must be an integer number between 0 and 4.)"},
    };

    solver->setProperty("Linear solver", "Banded");
    solver->setProperty("Upper half-bandwidth", "abc");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, gmresLinearSolverBandedPreconditionerAndUpperHalfBandwidthValueWithNumberTooSmall)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Upper half-bandwidth" property has an invalid value ("-1"). It must be an integer number between 0 and 4.)"},
    };

    solver->setProperty("Linear solver", "GMRES");
    solver->setProperty("Preconditioner", "Banded");
    solver->setProperty("Upper half-bandwidth", "-1");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, gmresLinearSolverBandedPreconditionerAndUpperHalfBandwidthValueWithNumberTooBig)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Upper half-bandwidth" property has an invalid value ("5"). It must be an integer number between 0 and 4.)"},
    };

    solver->setProperty("Linear solver", "GMRES");
    solver->setProperty("Preconditioner", "Banded");
    solver->setProperty("Upper half-bandwidth", "5");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, bandedLinearSolverAndLowerHalfBandwidthValueWithNonNumber)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Lower half-bandwidth" property has an invalid value ("abc"). It must be an integer number between 0 and 4.)"},
    };

    solver->setProperty("Linear solver", "Banded");
    solver->setProperty("Lower half-bandwidth", "abc");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, gmresLinearSolverBandedPreconditionerAndLowerHalfBandwidthValueWithNumberTooSmall)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Lower half-bandwidth" property has an invalid value ("-1"). It must be an integer number between 0 and 4.)"},
    };

    solver->setProperty("Linear solver", "GMRES");
    solver->setProperty("Preconditioner", "Banded");
    solver->setProperty("Lower half-bandwidth", "-1");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, gmresLinearSolverBandedPreconditionerAndLowerHalfBandwidthValueWithNumberTooBig)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Lower half-bandwidth" property has an invalid value ("5"). It must be an integer number between 0 and 4.)"},
    };

    solver->setProperty("Linear solver", "GMRES");
    solver->setProperty("Preconditioner", "Banded");
    solver->setProperty("Lower half-bandwidth", "5");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, relativeToleranceValueWithNonNumber)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Relative tolerance" property has an invalid value ("abc"). It must be a floating point number greater or equal to zero.)"},
    };

    solver->setProperty("Relative tolerance", "abc");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, relativeToleranceValueWithInvalidNumber)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Relative tolerance" property has an invalid value ("-1.234"). It must be a floating point number greater or equal to zero.)"},
    };

    solver->setProperty("Relative tolerance", "-1.234");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, absoluteToleranceValueWithNonNumber)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Absolute tolerance" property has an invalid value ("abc"). It must be a floating point number greater or equal to zero.)"},
    };

    solver->setProperty("Absolute tolerance", "abc");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, absoluteToleranceValueWithInvalidNumber)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Absolute tolerance" property has an invalid value ("-1.234"). It must be a floating point number greater or equal to zero.)"},
    };

    solver->setProperty("Absolute tolerance", "-1.234");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, interpolateSolutionValueWithNonBoolean)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Customise and initialise our solver using a step value that is not a floating point number.

    static const libOpenCOR::ExpectedIssues EXPECTED_ISSUES = {
        {libOpenCOR::Issue::Type::ERROR, R"(The "Interpolate solution" property has an invalid value ("Non boolean"). It must be equal to either "True" or "False".)"},
    };

    solver->setProperty("Interpolate solution", "Non boolean");

    EXPECT_FALSE(solver->initialise(0.0, STATE_COUNT, states, rates, variables, computeRates));
    EXPECT_EQ_ISSUES(solver, EXPECTED_ISSUES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, solve)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Compute our model.

#if defined(BUILDING_ON_WINDOWS)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015422246861901662, 0.59605559394022689, 0.053035160895529773, 0.31777058539774627};
#elif defined(BUILDING_ON_LINUX)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.01542143979535311, 0.59605559223195803, 0.053035155728368123, 0.31777058661072155};
#elif defined(BUILDING_ON_INTEL)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015423275579583704, 0.5960556037980016, 0.053035167225244692, 0.31777058077104786};
#else
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015420735373646405, 0.59605558367032696, 0.05303515120051977, 0.31777058753168086};
#endif

    computeModel(solver, states, rates, variables, FINAL_STATES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, solveWithAdamsMoultonIntegrationMethod)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Compute our model.

#if defined(BUILDING_ON_WINDOWS)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015422246861901662, 0.59605559394022689, 0.053035160895529773, 0.31777058539774627};
#elif defined(BUILDING_ON_LINUX)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.01542143979535311, 0.59605559223195803, 0.053035155728368123, 0.31777058661072155};
#elif defined(BUILDING_ON_INTEL)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015423275579583704, 0.5960556037980016, 0.053035167225244692, 0.31777058077104786};
#else
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015419524192581236, 0.59605557879092774, 0.053035145613795749, 0.31777058994460156};
#endif

    solver->setProperty("Integration method", "Adams-Moulton");

    computeModel(solver, states, rates, variables, FINAL_STATES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, solveWithFunctionalIterationType)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Compute our model.

#if defined(BUILDING_ON_WINDOWS)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015422246861901662, 0.59605559394022689, 0.053035160895529773, 0.31777058539774627};
#elif defined(BUILDING_ON_LINUX)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.01542143979535311, 0.59605559223195803, 0.053035155728368123, 0.31777058661072155};
#elif defined(BUILDING_ON_INTEL)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015423275579583704, 0.5960556037980016, 0.053035167225244692, 0.31777058077104786};
#else
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015419450943615409, 0.5960555863622764, 0.053035126367920661, 0.31777058514038992};
#endif

    solver->setProperty("Iteration type", "Functional");

    computeModel(solver, states, rates, variables, FINAL_STATES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, solveWithBandedLinearSolver)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Compute our model.

#if defined(BUILDING_ON_WINDOWS)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015422246861901662, 0.59605559394022689, 0.053035160895529773, 0.31777058539774627};
#elif defined(BUILDING_ON_LINUX)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.01542143979535311, 0.59605559223195803, 0.053035155728368123, 0.31777058661072155};
#elif defined(BUILDING_ON_INTEL)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015423275579583704, 0.5960556037980016, 0.053035167225244692, 0.31777058077104786};
#else
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015297069679523696, 0.5961402839536516, 0.053034618540828818, 0.3177776000625106};
#endif

    solver->setProperty("Linear solver", "Banded");

    computeModel(solver, states, rates, variables, FINAL_STATES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, solveWithDiagonalLinearSolver)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Compute our model.

#if defined(BUILDING_ON_WINDOWS)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015422246861901662, 0.59605559394022689, 0.053035160895529773, 0.31777058539774627};
#elif defined(BUILDING_ON_LINUX)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.01542143979535311, 0.59605559223195803, 0.053035155728368123, 0.31777058661072155};
#elif defined(BUILDING_ON_INTEL)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015423275579583704, 0.5960556037980016, 0.053035167225244692, 0.31777058077104786};
#else
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015420959503828912, 0.59605530714545696, 0.053035148843076509, 0.31777055784271463};
#endif

    solver->setProperty("Linear solver", "Diagonal");

    computeModel(solver, states, rates, variables, FINAL_STATES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, solveWithGmresLinearSolver)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Compute our model.

#if defined(BUILDING_ON_WINDOWS)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015422246861901662, 0.59605559394022689, 0.053035160895529773, 0.31777058539774627};
#elif defined(BUILDING_ON_LINUX)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.01542143979535311, 0.59605559223195803, 0.053035155728368123, 0.31777058661072155};
#elif defined(BUILDING_ON_INTEL)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015423275579583704, 0.5960556037980016, 0.053035167225244692, 0.31777058077104786};
#else
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015021211583899895, 0.59605801825378757, 0.053032741816368641, 0.31777318543482858};
#endif

    solver->setProperty("Linear solver", "GMRES");

    computeModel(solver, states, rates, variables, FINAL_STATES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, solveWithBicgstabLinearSolver)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Compute our model.

#if defined(BUILDING_ON_WINDOWS)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015422246861901662, 0.59605559394022689, 0.053035160895529773, 0.31777058539774627};
#elif defined(BUILDING_ON_LINUX)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.01542143979535311, 0.59605559223195803, 0.053035155728368123, 0.31777058661072155};
#elif defined(BUILDING_ON_INTEL)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015423275579583704, 0.5960556037980016, 0.053035167225244692, 0.31777058077104786};
#else
    static const libOpenCOR::Doubles FINAL_STATES = {-0.01501735489017168, 0.59605794815048185, 0.053032731566004181, 0.31777323174139266};
#endif

    solver->setProperty("Linear solver", "BiCGStab");

    computeModel(solver, states, rates, variables, FINAL_STATES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, solveWithTfqmrLinearSolver)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Compute our model.

#if defined(BUILDING_ON_WINDOWS)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015422246861901662, 0.59605559394022689, 0.053035160895529773, 0.31777058539774627};
#elif defined(BUILDING_ON_LINUX)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.01542143979535311, 0.59605559223195803, 0.053035155728368123, 0.31777058661072155};
#elif defined(BUILDING_ON_INTEL)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015423275579583704, 0.5960556037980016, 0.053035167225244692, 0.31777058077104786};
#else
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015019630087747961, 0.59605796103564324, 0.053032734038507313, 0.3177732093055472};
#endif

    solver->setProperty("Linear solver", "TFQMR");

    computeModel(solver, states, rates, variables, FINAL_STATES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}

TEST(CvodeSolverTest, solveWithNoPreconditioner)
{
    // Create and initialise our various arrays and create our solver.

    const auto [solver, states, rates, variables] = createAndInitialiseArraysAndCreateSolver("CVODE");

    // Compute our model.

#if defined(BUILDING_ON_WINDOWS)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015422246861901662, 0.59605559394022689, 0.053035160895529773, 0.31777058539774627};
#elif defined(BUILDING_ON_LINUX)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.01542143979535311, 0.59605559223195803, 0.053035155728368123, 0.31777058661072155};
#elif defined(BUILDING_ON_INTEL)
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015423275579583704, 0.5960556037980016, 0.053035167225244692, 0.31777058077104786};
#else
    static const libOpenCOR::Doubles FINAL_STATES = {-0.015419131974789706, 0.5960556246035319, 0.053035133913277126, 0.31777055729945081};
#endif

    solver->setProperty("Linear solver", "GMRES");
    solver->setProperty("Preconditioner", "None");

    computeModel(solver, states, rates, variables, FINAL_STATES);

    // Clean up after ourselves.

    deleteArrays(states, rates, variables);
}
