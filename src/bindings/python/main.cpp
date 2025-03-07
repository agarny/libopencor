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

#include <nanobind/nanobind.h>

namespace nb = nanobind;

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

void fileApi(nb::module_ &m);
void loggerApi(nb::module_ &m);
void sedApi(nb::module_ &m);
void solverApi(nb::module_ &m);
void versionApi(nb::module_ &m);

NB_MODULE(module, m)
{
    // Version.

    m.attr("__version__") = MACRO_STRINGIFY(PROJECT_VERSION);

    // Documentation.

    m.doc() = "libOpenCOR is the backend library to OpenCOR, an open source cross-platform modelling environment.";

    // APIs.

    loggerApi(m); // Note: it needs to be first since it is used by some other APIs.

    fileApi(m);
    sedApi(m);
    solverApi(m);
    versionApi(m);
}
