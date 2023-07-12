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

#include "unittestingexport.h"

#include "libopencor/logger.h"

#include <memory>

namespace libOpenCOR {

class CombineArchive;
using CombineArchivePtr = std::shared_ptr<CombineArchive>;

class LIBOPENCOR_UNIT_TESTING_EXPORT CombineArchive: public Logger
{
public:
    CombineArchive() = delete;
    ~CombineArchive();

    CombineArchive(const CombineArchive &pOther) = delete;
    CombineArchive(CombineArchive &&pOther) noexcept = delete;

    CombineArchive &operator=(const CombineArchive &pRhs) = delete;
    CombineArchive &operator=(CombineArchive &&pRhs) noexcept = delete;

    static CombineArchivePtr create(const FilePtr &pFile);

private:
    class Impl;

    explicit CombineArchive(const FilePtr &pFile);

    Impl *pimpl();
    const Impl *pimpl() const;
};

} // namespace libOpenCOR
