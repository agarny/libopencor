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

void sedApi()
{
    // SedBase API.

    emscripten::class_<libOpenCOR::SedBase, emscripten::base<libOpenCOR::Logger>>("SedBase");

    // SedAbstractTask API.

    emscripten::class_<libOpenCOR::SedAbstractTask, emscripten::base<libOpenCOR::SedBase>>("SedAbstractTask")
        .smart_ptr<libOpenCOR::SedAbstractTaskPtr>("SedAbstractTask");

    // SedDataDescription API.

    emscripten::class_<libOpenCOR::SedDataDescription, emscripten::base<libOpenCOR::SedBase>>("SedDataDescription");

    // SedDataGenerator API.

    emscripten::class_<libOpenCOR::SedDataGenerator, emscripten::base<libOpenCOR::SedBase>>("SedDataGenerator");

    // SedDocument API.

    emscripten::class_<libOpenCOR::SedDocument, emscripten::base<libOpenCOR::Logger>>("SedDocument")
        .smart_ptr<libOpenCOR::SedDocumentPtr>("SedDocument")
        .constructor(&libOpenCOR::SedDocument::create)
        .constructor(&libOpenCOR::SedDocument::defaultCreate)
        .function("serialise", emscripten::select_overload<std::string() const>(&libOpenCOR::SedDocument::serialise))
        .function("serialise", emscripten::select_overload<std::string(const std::string &) const>(&libOpenCOR::SedDocument::serialise))
        .function("hasModels", &libOpenCOR::SedDocument::hasModels)
        .function("models", &libOpenCOR::SedDocument::models)
        .function("addModel", &libOpenCOR::SedDocument::addModel)
        .function("removeModel", &libOpenCOR::SedDocument::removeModel)
        .function("hasSimulations", &libOpenCOR::SedDocument::hasSimulations)
        .function("simulations", &libOpenCOR::SedDocument::simulations)
        .function("addSimulation", &libOpenCOR::SedDocument::addSimulation)
        .function("removeSimulation", &libOpenCOR::SedDocument::removeSimulation)
        .function("hasTasks", &libOpenCOR::SedDocument::hasTasks)
        .function("tasks", &libOpenCOR::SedDocument::tasks)
        .function("addTask", &libOpenCOR::SedDocument::addTask)
        .function("removeTask", &libOpenCOR::SedDocument::removeTask)
        .function("run", &libOpenCOR::SedDocument::run);

    // SedModel API.

    emscripten::class_<libOpenCOR::SedModel, emscripten::base<libOpenCOR::SedBase>>("SedModel")
        .smart_ptr_constructor("SedModel", &libOpenCOR::SedModel::create);

    // SedOutput API.

    emscripten::class_<libOpenCOR::SedOutput, emscripten::base<libOpenCOR::SedBase>>("SedOutput");

    // SedRepeatedTask API.

    /*---GRY---
        emscripten::class_<libOpenCOR::SedRepeatedTask, emscripten::base<libOpenCOR::SedAbstractTask>>("SedRepeatedTask")
            .smart_ptr_constructor("SedRepeatedTask", &libOpenCOR::SedRepeatedTask::create);
    */

    // SedSimulation API.

    emscripten::class_<libOpenCOR::SedSimulation, emscripten::base<libOpenCOR::SedBase>>("SedSimulation")
        .smart_ptr<libOpenCOR::SedSimulationPtr>("SedSimulation")
        .function("odeSolver", &libOpenCOR::SedSimulation::odeSolver)
        .function("setOdeSolver", &libOpenCOR::SedSimulation::setOdeSolver)
        .function("nlaSolver", &libOpenCOR::SedSimulation::nlaSolver)
        .function("setNlaSolver", &libOpenCOR::SedSimulation::setNlaSolver);

    // SedSimulationOneStep API.

    emscripten::class_<libOpenCOR::SedSimulationOneStep, emscripten::base<libOpenCOR::SedSimulation>>("SedSimulationOneStep")
        .smart_ptr_constructor("SedSimulationOneStep", &libOpenCOR::SedSimulationOneStep::create)
        .function("step", &libOpenCOR::SedSimulationOneStep::step)
        .function("setStep", &libOpenCOR::SedSimulationOneStep::setStep);

    // SedSimulationSteadyState API.

    emscripten::class_<libOpenCOR::SedSimulationSteadyState, emscripten::base<libOpenCOR::SedSimulation>>("SedSimulationSteadyState")
        .smart_ptr_constructor("SedSimulationSteadyState", &libOpenCOR::SedSimulationSteadyState::create);

    // SedSimulationUniformTimeCourse API.

    emscripten::class_<libOpenCOR::SedSimulationUniformTimeCourse, emscripten::base<libOpenCOR::SedSimulation>>("SedSimulationUniformTimeCourse")
        .smart_ptr_constructor("SedSimulationUniformTimeCourse", &libOpenCOR::SedSimulationUniformTimeCourse::create)
        .function("initialTime", &libOpenCOR::SedSimulationUniformTimeCourse::initialTime)
        .function("setInitialTime", &libOpenCOR::SedSimulationUniformTimeCourse::setInitialTime)
        .function("outputStartTime", &libOpenCOR::SedSimulationUniformTimeCourse::outputStartTime)
        .function("setOutputStartTime", &libOpenCOR::SedSimulationUniformTimeCourse::setOutputStartTime)
        .function("outputEndTime", &libOpenCOR::SedSimulationUniformTimeCourse::outputEndTime)
        .function("setOutputEndTime", &libOpenCOR::SedSimulationUniformTimeCourse::setOutputEndTime)
        .function("numberOfSteps", &libOpenCOR::SedSimulationUniformTimeCourse::numberOfSteps)
        .function("setNumberOfSteps", &libOpenCOR::SedSimulationUniformTimeCourse::setNumberOfSteps);

    // SedStyle API.

    emscripten::class_<libOpenCOR::SedStyle, emscripten::base<libOpenCOR::SedBase>>("SedStyle");

    // SedTask API.

    emscripten::class_<libOpenCOR::SedTask, emscripten::base<libOpenCOR::SedAbstractTask>>("SedTask")
        .smart_ptr_constructor("SedTask", &libOpenCOR::SedTask::create)
        .function("model", &libOpenCOR::SedTask::model)
        .function("setModel", &libOpenCOR::SedTask::setModel)
        .function("simulation", &libOpenCOR::SedTask::simulation)
        .function("setSimulation", &libOpenCOR::SedTask::setSimulation);
}
