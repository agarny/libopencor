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

void issueApi(py::module_ &m)
{
    // Issue API.

    py::class_<libOpenCOR::Issue, std::shared_ptr<libOpenCOR::Issue>> issue(m, "Issue");

    py::enum_<libOpenCOR::Issue::Type>(issue, "Type")
        .value("Error", libOpenCOR::Issue::Type::ERROR)
        .value("Warning", libOpenCOR::Issue::Type::WARNING)
        .value("Message", libOpenCOR::Issue::Type::MESSAGE)
        .export_values();

    issue.def_property_readonly("type", &libOpenCOR::Issue::type, "Get the type of this Issue object.")
        .def_property_readonly("description", &libOpenCOR::Issue::description, "Get the description for this Issue object.");
}
