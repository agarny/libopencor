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

#include "version_p.h"

#include "curl/curlver.h"

#include "libcellml/version.h"

#include "libcombinebegin.h"
#include "omex/common/libcombine-version.h"
#include "libcombineend.h"

#include "libopencor/version.h"

#include "libsedmlbegin.h"
#include "sedml/common/libsedml-version.h"
#include "libsedmlend.h"

#include "sundialsbegin.h"
#include "sundials/sundials_version.h"
#include "sundialsend.h"

#include <array>

namespace libOpenCOR {

unsigned int version()
{
    return LIBOPENCOR_VERSION;
}

std::string versionString()
{
    return LIBOPENCOR_VERSION_STRING;
}

unsigned int libcellmlVersion()
{
    return libcellml::version();
}

std::string libcellmlVersionString()
{
    return libcellml::versionString();
}

unsigned int libcombineVersion()
{
    return static_cast<unsigned int>(libcombine::getLibCombineVersion());
}

std::string libcombineVersionString()
{
    return libcombine::getLibCombineDottedVersion();
}

unsigned int libcurlVersion()
{
    return LIBCURL_VERSION_NUM;
}

std::string libcurlVersionString()
{
    return LIBCURL_VERSION;
}

unsigned int libsedmlVersion()
{
    return static_cast<unsigned int>(libsedml::getLibSEDMLVersion());
}

std::string libsedmlVersionString()
{
    return libsedml::getLibSEDMLDottedVersion();
}

unsigned int sundialsVersion()
{
    static const int MAJOR = 65536;
    static const int MINOR = 256;
    static const int LABEL_SIZE = 10;

    int major = 0;
    int minor = 0;
    int patch = 0;
    std::array<char, LABEL_SIZE> label {""};

    SUNDIALSGetVersionNumber(&major, &minor, &patch, label.data(), LABEL_SIZE);

    return static_cast<unsigned int>(MAJOR * major + MINOR * minor + patch);
}

std::string sundialsVersionString()
{
    static const int VERSION_SIZE = 25;

    std::array<char, VERSION_SIZE> version {""};

    SUNDIALSGetVersion(version.data(), VERSION_SIZE);

    std::string res {std::begin(version), std::end(version)};

    res.erase(res.find_last_not_of('\0') + 1, std::string::npos);

    return res;
}

} // namespace libOpenCOR
