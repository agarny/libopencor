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
#include <pybind11/stl.h>

namespace py = pybind11;

void fileApi(py::module_ &m)
{
    // File API.

    py::class_<libOpenCOR::File, libOpenCOR::Logger, libOpenCOR::FilePtr> file(m, "File");

    py::enum_<libOpenCOR::File::Type>(file, "Type")
        .value("UnknownFile", libOpenCOR::File::Type::UNKNOWN_FILE)
        .value("CellmlFile", libOpenCOR::File::Type::CELLML_FILE)
        .value("SedmlFile", libOpenCOR::File::Type::SEDML_FILE)
        .value("CombineArchive", libOpenCOR::File::Type::COMBINE_ARCHIVE)
        .value("IrretrievableFile", libOpenCOR::File::Type::IRRETRIEVABLE_FILE)
        .export_values();

    file.def(py::init(&libOpenCOR::File::create), "Create a File object.", py::arg("file_name_or_url"))
        .def_property_readonly("type", &libOpenCOR::File::type, "Get the type of this File object.")
        .def_property_readonly("file_name", &libOpenCOR::File::fileName, "Get the file name for this File object.")
        .def_property_readonly("url", &libOpenCOR::File::url, "Get the URL for this File object.")
        .def_property_readonly("path", &libOpenCOR::File::path, "Get the path for this File object.")
        .def_property("contents", &libOpenCOR::File::contents, &libOpenCOR::File::setContents, "The contents of this File object.");
}
