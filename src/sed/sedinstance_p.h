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

#include "libopencor/sedinstance.h"

namespace libOpenCOR {

class SedInstance::Impl: public Logger::Impl
{
public:
    SedInstanceTaskPtrs mTasks;

    static SedInstancePtr create(const SedDocumentPtr &pDocument, bool pCompiled);

    explicit Impl(const SedDocumentPtr &pDocument, bool pCompiled);

    double run();

    bool hasTasks() const;
    size_t taskCount() const;
    SedInstanceTaskPtrs tasks() const;
    SedInstanceTaskPtr task(size_t pIndex) const;
};

} // namespace libOpenCOR
