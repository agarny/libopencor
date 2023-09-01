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

#include <libopencor>

void solverApi()
{
    emscripten::enum_<libOpenCOR::Solver::Type>("Solver.Type")
        .value("ODE", libOpenCOR::Solver::Type::ODE)
        .value("NLA", libOpenCOR::Solver::Type::NLA);

    emscripten::enum_<libOpenCOR::Solver::Method>("Solver.Method")
        .value("CVODE", libOpenCOR::Solver::Method::CVODE)
        .value("FORWARD_EULER", libOpenCOR::Solver::Method::FORWARD_EULER)
        .value("FOURTH_ORDER_RUNGE_KUTTA", libOpenCOR::Solver::Method::FOURTH_ORDER_RUNGE_KUTTA)
        .value("HEUN", libOpenCOR::Solver::Method::HEUN)
        .value("KINSOL", libOpenCOR::Solver::Method::KINSOL)
        .value("SECOND_ORDER_RUNGE_KUTTA", libOpenCOR::Solver::Method::SECOND_ORDER_RUNGE_KUTTA);

    emscripten::class_<libOpenCOR::Solver, emscripten::base<libOpenCOR::Logger>>("Solver")
        .smart_ptr_constructor("Solver", &libOpenCOR::Solver::create)
        .function("type", &libOpenCOR::Solver::type)
        .function("method", &libOpenCOR::Solver::method)
        .function("name", &libOpenCOR::Solver::name);

    emscripten::class_<libOpenCOR::SolverForwardEuler, emscripten::base<libOpenCOR::Solver>>("SolverForwardEuler");

    EM_ASM({
        Module['Solver']['Type'] = Module['Solver.Type'];
        Module['Solver']['Method'] = Module['Solver.Method'];

        delete Module['Solver.Type'];
        delete Module['Solver.Method'];
    });
}
