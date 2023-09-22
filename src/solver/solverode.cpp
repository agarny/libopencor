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

#include "solverode_p.h"

namespace libOpenCOR {

bool SolverOde::Impl::initialise(size_t pSize, double *pStates, double *pRates, double *pVariables,
                                 ComputeRates pComputeRates)
{
    mSize = pSize;

    mStates = pStates;
    mRates = pRates;
    mVariables = pVariables;

    mComputeRates = pComputeRates;

    return true;
}

SolverOde::SolverOde(Impl *pPimpl)
    : Solver(pPimpl)
{
}

SolverOde::Impl *SolverOde::pimpl()
{
    return static_cast<Impl *>(Solver::pimpl());
}

const SolverOde::Impl *SolverOde::pimpl() const
{
    return static_cast<const Impl *>(Solver::pimpl());
}

} // namespace libOpenCOR