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

#include "cellmlfile.h"
#include "unittestingexport.h"

#include "libopencor/types.h"

#ifndef NDEBUG
#    include <cassert>
#endif
#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include <libxml/xmlstring.h>

namespace libOpenCOR {

static const std::string LIBOPENCOR_NAMESPACE = "https://opencor.ws/libopencor";

using ConstCharPtrs = std::vector<const char *>;
using Files = std::vector<File *>;

using StringStringMap = std::map<std::string, std::string>;

#ifdef NDEBUG
#    define ASSERT_EQ(x, y) \
        (void)x; \
        (void)y
#    define ASSERT_NE(x, y) \
        (void)x; \
        (void)y
#    define ASSERT_GT(x, y) \
        (void)x; \
        (void)y
#    define ASSERT_GE(x, y) \
        (void)x; \
        (void)y
#    define ASSERT_LT(x, y) \
        (void)x; \
        (void)y
#    define ASSERT_LE(x, y) \
        (void)x; \
        (void)y
#else
#    define ASSERT_EQ(x, y) assert(x == y)
#    define ASSERT_NE(x, y) assert(x != y)
#    define ASSERT_GT(x, y) assert(x > y)
#    define ASSERT_GE(x, y) assert(x >= y)
#    define ASSERT_LT(x, y) assert(x < y)
#    define ASSERT_LE(x, y) assert(x <= y)
#endif

bool LIBOPENCOR_UNIT_TESTING_EXPORT fuzzyCompare(double pNb1, double pNb2);

#ifdef BUILDING_USING_MSVC
std::string wideStringToString(const std::wstring &pString);

std::string forwardSlashPath(const std::string &pPath);
#endif

std::filesystem::path stringToPath(const std::string &pString);

std::tuple<bool, std::string> retrieveFileInfo(const std::string &pFileNameOrUrl);
std::string relativePath(const std::string &pPath, const std::string &pBasePath);
std::string urlPath(const std::string &pPath);

#ifndef __EMSCRIPTEN__
std::tuple<bool, std::filesystem::path> downloadFile(const std::string &pUrl);

UnsignedChars LIBOPENCOR_UNIT_TESTING_EXPORT fileContents(const std::filesystem::path &pFilePath);
#endif

char LIBOPENCOR_UNIT_TESTING_EXPORT *nlaSolverAddress(SolverNla *pNlaSolver);

std::string toString(int pNumber);
std::string toString(size_t pNumber);
std::string toString(double pNumber);
std::string LIBOPENCOR_UNIT_TESTING_EXPORT toString(const UnsignedChars &pBytes);

const xmlChar *toConstXmlCharPtr(const std::string &pString);

} // namespace libOpenCOR
