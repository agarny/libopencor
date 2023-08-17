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

void fileApi()
{
    emscripten::enum_<libOpenCOR::File::Type>("File.Type")
        .value("UNKNOWN_FILE", libOpenCOR::File::Type::UNKNOWN_FILE)
        .value("CELLML_FILE", libOpenCOR::File::Type::CELLML_FILE)
        .value("SEDML_FILE", libOpenCOR::File::Type::SEDML_FILE)
        .value("COMBINE_ARCHIVE", libOpenCOR::File::Type::COMBINE_ARCHIVE);

    emscripten::class_<libOpenCOR::File, emscripten::base<libOpenCOR::Logger>>("File")
        .smart_ptr_constructor("File", emscripten::optional_override([](const std::string &pFileNameOrUrl, uintptr_t pContents, size_t pSize) {
                                   auto contents = reinterpret_cast<unsigned char *>(pContents);

                                   return libOpenCOR::File::create(pFileNameOrUrl,
                                                                   std::vector<unsigned char>(contents, contents + pSize));
                               }))
        .function("type", &libOpenCOR::File::type)
        .function("fileName", &libOpenCOR::File::fileName)
        .function("url", &libOpenCOR::File::url)
        .function("path", &libOpenCOR::File::path)
        .function("contents", &libOpenCOR::File::jsContents);

    EM_ASM({
        Module['File']['Type'] = Module['File.Type'];

        delete Module['File.Type'];
    });
}
