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

void fileApi();
void issueApi();
void loggerApi();
void sedAbstractTaskApi();
void sedAlgorithmParameterApi();
void sedBaseApi();
void sedDataDescriptionApi();
void sedDataGeneratorApi();
void sedDocumentApi();
void sedModelApi();
void sedOutputApi();
void sedSimulationApi();
void sedStyleApi();
void solverApi();
void versionApi();

EMSCRIPTEN_BINDINGS(libOpenCOR)
{
    emscripten::register_vector<std::string>("Strings");

    loggerApi(); // Note: it needs to be first since it is used by some other APIs.
    sedBaseApi(); // Note: it needs to be second since it is used by some other APIs.

    fileApi();
    issueApi();
    sedAbstractTaskApi();
    sedAlgorithmParameterApi();
    sedDataDescriptionApi();
    sedDataGeneratorApi();
    sedDocumentApi();
    sedModelApi();
    sedOutputApi();
    sedSimulationApi();
    sedStyleApi();
    solverApi();
    versionApi();
}
