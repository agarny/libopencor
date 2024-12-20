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

void loggerApi()
{
    // Logger API.

    emscripten::class_<libOpenCOR::Logger>("Logger")
        .function("hasIssues", &libOpenCOR::Logger::hasIssues)
        .function("issueCount", &libOpenCOR::Logger::issueCount)
        .function("issues", &libOpenCOR::Logger::issues)
        .function("issue", &libOpenCOR::Logger::issue)
        .function("hasErrors", &libOpenCOR::Logger::hasErrors)
        .function("errorCount", &libOpenCOR::Logger::errorCount)
        .function("errors", &libOpenCOR::Logger::errors)
        .function("error", &libOpenCOR::Logger::error)
        .function("hasWarnings", &libOpenCOR::Logger::hasWarnings)
        .function("warningCount", &libOpenCOR::Logger::warningCount)
        .function("warnings", &libOpenCOR::Logger::warnings)
        .function("warning", &libOpenCOR::Logger::warning);

    // Issue API.

    emscripten::enum_<libOpenCOR::Issue::Type>("Issue.Type")
        .value("ERROR", libOpenCOR::Issue::Type::ERROR)
        .value("WARNING", libOpenCOR::Issue::Type::WARNING);

    emscripten::class_<libOpenCOR::Issue>("Issue")
        .smart_ptr<libOpenCOR::IssuePtr>("Issue")
        .function("type", &libOpenCOR::Issue::type)
        .function("typeAsString", &libOpenCOR::Issue::typeAsString)
        .function("description", &libOpenCOR::Issue::description);

    EM_ASM({
        Module["Issue"]["Type"] = Module["Issue.Type"];

        delete Module["Issue.Type"];
    });
}
