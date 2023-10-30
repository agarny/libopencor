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

#include <pybind11/pybind11.h>

namespace py = pybind11;

void sedApi(py::module_ &m)
{
    // SedBase API.

    py::class_<libOpenCOR::SedBase, libOpenCOR::Logger, std::shared_ptr<libOpenCOR::SedBase>> sedBase(m, "SedBase");

    // SedAbstractTask API.

    py::class_<libOpenCOR::SedAbstractTask, libOpenCOR::SedBase, std::shared_ptr<libOpenCOR::SedAbstractTask>> sedAbstractTask(m, "SedAbstractTask");

    // SedAlgorithmParameter API.

    py::class_<libOpenCOR::SedAlgorithmParameter, libOpenCOR::SedBase, std::shared_ptr<libOpenCOR::SedAlgorithmParameter>> sedAlgorithmParameter(m, "SedAlgorithmParameter");

    // SedDataDescription API.

    py::class_<libOpenCOR::SedDataDescription, libOpenCOR::SedBase, std::shared_ptr<libOpenCOR::SedDataDescription>> sedDataDescription(m, "SedDataDescription");

    // SedDataGenerator API.

    py::class_<libOpenCOR::SedDataGenerator, libOpenCOR::SedBase, std::shared_ptr<libOpenCOR::SedDataGenerator>> sedDataGenerator(m, "SedDataGenerator");

    // SedDocument API.

    py::class_<libOpenCOR::SedDocument, libOpenCOR::SedBase, std::shared_ptr<libOpenCOR::SedDocument>> sedDocument(m, "SedDocument");

    sedDocument.def(py::init(&libOpenCOR::SedDocument::create), "Create a SedDocument object.")
        .def("initialise", &libOpenCOR::SedDocument::initialise, "Initialise this SedDocument object.", py::arg("file"))
        .def("serialise", py::overload_cast<>(&libOpenCOR::SedDocument::serialise, py::const_), "Get the serialised version of this SedDocument object.")
        .def("serialise", py::overload_cast<const std::string &>(&libOpenCOR::SedDocument::serialise, py::const_), "Get the serialised version of this SedDocument object.", py::arg("base_path"));

    // SedModel API.

    py::class_<libOpenCOR::SedModel, libOpenCOR::SedBase, std::shared_ptr<libOpenCOR::SedModel>> sedModel(m, "SedModel");

    // SedOutput API.

    py::class_<libOpenCOR::SedOutput, libOpenCOR::SedBase, std::shared_ptr<libOpenCOR::SedOutput>> sedOutput(m, "SedOutput");

    // SedSimulation API.

    py::class_<libOpenCOR::SedSimulation, libOpenCOR::SedBase, std::shared_ptr<libOpenCOR::SedSimulation>> sedSimulation(m, "SedSimulation");

    // SedStyle API.

    py::class_<libOpenCOR::SedStyle, libOpenCOR::SedBase, std::shared_ptr<libOpenCOR::SedStyle>> sedStyle(m, "SedStyle");

    // SedUniformTimeCourse API.

    py::class_<libOpenCOR::SedUniformTimeCourse, libOpenCOR::SedSimulation, std::shared_ptr<libOpenCOR::SedUniformTimeCourse>> SedUniformTimeCourse(m, "SedUniformTimeCourse");
}
