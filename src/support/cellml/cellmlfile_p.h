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

#include "logger_p.h"

#include "cellmlfile.h"

#include <libcellml>

namespace libOpenCOR {

class CellmlFile::Impl: public Logger::Impl
{
public:
    libcellml::ModelPtr mModel;
    libcellml::AnalyserModelPtr mAnalyserModel;

    explicit Impl(const FilePtr &pFile, const libcellml::ModelPtr &pModel);
    ~Impl() = default;
};

} // namespace libOpenCOR
