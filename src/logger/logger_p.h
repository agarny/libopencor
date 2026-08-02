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

#include "libcellml/issue.h"
#include "libcellml/logger.h"

#include "libopencor/issue.h"
#include "libopencor/logger.h"

#include <atomic>
#include <mutex>

namespace libOpenCOR {

class Logger::Impl
{
public:
    mutable std::mutex mMutex;

    IssuePtrs mIssues;
    IssuePtrs mErrors;
    IssuePtrs mWarnings;

    // Note: we use atomic variables for the issue counts so that they can be read without locking the mutex, which is
    //       otherwise needed to access the issue vectors themselves. This is because the issue counts can be read
    //       frequently during a simulation and we want to avoid the overhead of locking the mutex. To ensure that a
    //       reader never sees an updated count before the corresponding issue vectors have been updated, the counts are
    //       written with std::memory_order_release and read with std::memory_order_acquire. The vectors themselves are
    //       still protected by the mutex.

    std::atomic<size_t> mIssueCount {0};
    std::atomic<size_t> mErrorCount {0};
    std::atomic<size_t> mWarningCount {0};

    virtual ~Impl() = default;

    bool hasIssues() const;
    size_t issueCount() const;
    IssuePtrs issues() const;
    IssuePtr issue(size_t pIndex) const;

    bool hasErrors() const;
    size_t errorCount() const;
    IssuePtrs errors() const;
    IssuePtr error(size_t pIndex) const;

    bool hasWarnings() const;
    size_t warningCount() const;
    IssuePtrs warnings() const;
    IssuePtr warning(size_t pIndex) const;

    void addIssues(const LoggerPtr &pLogger, const std::string &pContext);
    void addIssues(const libcellml::LoggerPtr &pLogger, const std::string &pContext);

    void addIssue(Issue::Type pType, const std::string &pDescription, const std::string &pContext = "");
    void addError(const std::string &pDescription);
    void addWarning(const std::string &pDescription);

    void removeAllIssues();
};

} // namespace libOpenCOR
