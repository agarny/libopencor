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

#include "libopencor/issue.h"
#include "libopencor/logger.h"

#include <libcellml>
#include <vector>

namespace libOpenCOR {

class Logger::Impl
{
public:
    IssuePtrs mIssues;

    IssuePtrs mErrors;
    IssuePtrs mWarnings;
    IssuePtrs mMessages;

    bool hasIssues() const;
    bool hasErrors() const;
    bool hasWarnings() const;
    bool hasMessages() const;

    void addIssues(const LoggerPtr &pLogger);
    void addIssues(const libcellml::LoggerPtr &pLogger);

    void addIssue(Issue::Type pType, const std::string &pDescription);

    void addError(const std::string &pDescription);
    void addWarning(const std::string &pDescription);
    void addMessage(const std::string &pDescription);

    void removeAllIssues();
};

} // namespace libOpenCOR
