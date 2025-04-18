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

#include "solverodefixedstep_p.h"

#include "libopencor/solverfourthorderrungekutta.h"

namespace libOpenCOR {

class SolverFourthOrderRungeKutta::Impl: public SolverOdeFixedStep::Impl
{
public:
    double *mK1 = nullptr;
    double *mK2 = nullptr;
    double *mK3 = nullptr;
    double *mYk = nullptr;

    Doubles mK1Doubles;
    Doubles mK2Doubles;
    Doubles mK3Doubles;
    Doubles mYkDoubles;

    explicit Impl();

    SolverPtr duplicate() override;

    bool initialise(double pVoi, size_t pSize, double *pStates, double *pRates,
                    double *pConstants, double *pComputedConstants, double *pAlgebraic,
                    CellmlFileRuntime::ComputeCompiledRates pComputeCompiledRates,
                    CellmlFileRuntime::ComputeInterpretedRates pComputeInterpretedRates) override;

    bool solve(double &pVoi, double pVoiEnd) override;
};

} // namespace libOpenCOR
