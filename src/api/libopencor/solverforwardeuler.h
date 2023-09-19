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

#pragma once

#include "solverode.h"

namespace libOpenCOR {

/**
 * @brief The SolverForwardEuler class.
 *
 * The SolverForwardEuler class implements the Foward Euler method, a first-order numerical technique for solving a
 * system of ODEs.
 */

class LIBOPENCOR_EXPORT SolverForwardEuler: public SolverOde
{
    friend class Solver;

public:
    /**
     * Constructors, destructor, and assignment operators.
     */

    ~SolverForwardEuler() override; /**< Destructor, @private. */

    SolverForwardEuler(const SolverForwardEuler &pOther) = delete; /**< No copy constructor allowed, @private. */
    SolverForwardEuler(SolverForwardEuler &&pOther) noexcept = delete; /**< No move constructor allowed, @private. */

    SolverForwardEuler &operator=(const SolverForwardEuler &pRhs) = delete; /**< No copy assignment operator allowed, @private. */
    SolverForwardEuler &operator=(SolverForwardEuler &&pRhs) noexcept = delete; /**< No move assignment operator allowed, @private. */

    /**
     * @brief Initialise the solver.
     *
     * Initialise the solver, which means keeping track of the various arrays and of the compute rates method, as well
     * as checking its properties to initialise the solver itself.
     *
     * @param pSize The size, as a @c size_t, of the system of ODEs.
     * @param pStates The states, as an array of @c double.
     * @param pRates The rates, as an array of @c double.
     * @param pVariables The other variables, as an array of @c double.
     * @param pComputeRates The compute rates method, as a @ref ComputeRates.
     *
     * @return @c true if the solver is initialised, @c false otherwise.
     */

    bool initialise(size_t pSize, double *pStates, double *pRates, double *pVariables,
                    ComputeRates pComputeRates) override;

    /**
     * @brief Solve the system of ODEs.
     *
     * Solve the system of ODEs between @p pVoi and @p pVoiEnd.
     *
     * @param pVoi The variable of integration, as a @c double.
     * @param pVoiEnd The end value of the variable of integration, as a @c double.
     */

    void solve(double &pVoi, double pVoiEnd) const override;

private:
    class Impl; /**< Forward declaration of the implementation class, @private. */

    Impl *pimpl(); /**< Private implementation pointer, @private. */
    const Impl *pimpl() const; /**< Constant private implementation pointer, @private. */

    explicit SolverForwardEuler(); /**< Constructor, @private. */
};

} // namespace libOpenCOR
