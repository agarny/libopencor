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

#include "cellmlfile.h"

#include "cellmlfile_p.h"

namespace libOpenCOR {

CellmlFile::Impl::Impl(const FilePtr &pFile)
    : mFile(pFile)
{
}

CellmlFile::CellmlFile(const FilePtr &pFile)
    : Logger(new Impl(pFile))
{
}

CellmlFile::~CellmlFile()
{
    delete pimpl();
}

CellmlFile::Impl *CellmlFile::pimpl()
{
    return reinterpret_cast<Impl *>(Logger::pimpl());
}

/*---GRY---
const CellmlFile::Impl *CellmlFile::pimpl() const
{
    return reinterpret_cast<const Impl *>(Logger::pimpl());
}
*/

CellmlFilePtr CellmlFile::create(const FilePtr &pFile)
{
    return std::shared_ptr<CellmlFile> {new CellmlFile {pFile}};
}

} // namespace libOpenCOR
