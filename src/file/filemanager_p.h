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

#include "libopencor/filemanager.h"

namespace libOpenCOR {

using Files = std::vector<File *>;

class FileManager::Impl
{
public:
    Files mFiles;

    static Impl &instance();

    void manage(File *pFile);
    void unmanage(File *pFile);

    void reset();

    bool hasFiles() const;
    size_t fileCount() const;
    FilePtrs files() const;
    FilePtr file(size_t pIndex) const;
#ifdef __EMSCRIPTEN__
    FilePtr fileFromFileNameOrUrl(const std::string &pFileNameOrUrl) const;
#else
    FilePtr file(const std::string &pFileNameOrUrl) const;
#endif
};

} // namespace libOpenCOR
