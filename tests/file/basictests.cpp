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

#include "gtest/gtest.h"

#include "tests/utils.h"

#include <libopencor>

TEST(BasicFileTest, windowsLocalFile)
{
    auto file = libOpenCOR::File::create(libOpenCOR::WINDOWS_LOCAL_FILE);

    EXPECT_EQ(file->type(), libOpenCOR::File::Type::IRRETRIEVABLE_FILE);
    EXPECT_EQ(file->fileName(), libOpenCOR::WINDOWS_LOCAL_FILE);
    EXPECT_EQ(file->url(), "");
    EXPECT_TRUE(file->contents().empty());
}

TEST(BasicFileTest, unixLocalFile)
{
    auto file = libOpenCOR::File::create(libOpenCOR::UNIX_LOCAL_FILE);

    EXPECT_EQ(file->type(), libOpenCOR::File::Type::IRRETRIEVABLE_FILE);
    EXPECT_EQ(file->fileName(), libOpenCOR::UNIX_LOCAL_FILE);
    EXPECT_EQ(file->url(), "");
    EXPECT_TRUE(file->contents().empty());
}

TEST(BasicFileTest, urlBasedWindowsLocalFile)
{
    auto file = libOpenCOR::File::create("file:///C:/some/path/file.txt");

    EXPECT_EQ(file->type(), libOpenCOR::File::Type::IRRETRIEVABLE_FILE);
    EXPECT_EQ(file->fileName(), libOpenCOR::WINDOWS_LOCAL_FILE);
    EXPECT_EQ(file->url(), "");
    EXPECT_TRUE(file->contents().empty());
}

TEST(BasicFileTest, urlBasedUnixLocalFile)
{
    auto file = libOpenCOR::File::create("file:///some/path/file.txt");

    EXPECT_EQ(file->type(), libOpenCOR::File::Type::IRRETRIEVABLE_FILE);
    EXPECT_EQ(file->fileName(), libOpenCOR::UNIX_LOCAL_FILE);
    EXPECT_EQ(file->url(), "");
    EXPECT_TRUE(file->contents().empty());
}

TEST(BasicFileTest, remoteFile)
{
    auto file = libOpenCOR::File::create(libOpenCOR::REMOTE_FILE);

    EXPECT_EQ(file->type(), libOpenCOR::File::Type::CELLML_FILE);
    EXPECT_NE(file->fileName(), "");
    EXPECT_EQ(file->url(), libOpenCOR::REMOTE_FILE);
    EXPECT_FALSE(file->contents().empty());
}

TEST(BasicFileTest, localVirtualFile)
{
    auto file = libOpenCOR::File::create(libOpenCOR::UNIX_LOCAL_FILE,
                                         libOpenCOR::charArrayToVector(libOpenCOR::SOME_UNKNOWN_CONTENTS));

    EXPECT_EQ(file->type(), libOpenCOR::File::Type::UNKNOWN_FILE);
    EXPECT_EQ(file->fileName(), libOpenCOR::UNIX_LOCAL_FILE);
    EXPECT_EQ(file->url(), "");
    EXPECT_TRUE(!strcmp(reinterpret_cast<const char *>(file->contents().data()), libOpenCOR::SOME_UNKNOWN_CONTENTS));
}

TEST(BasicFileTest, remoteVirtualFile)
{
    auto file = libOpenCOR::File::create(libOpenCOR::REMOTE_FILE,
                                         libOpenCOR::charArrayToVector(libOpenCOR::SOME_UNKNOWN_CONTENTS));

    EXPECT_EQ(file->type(), libOpenCOR::File::Type::UNKNOWN_FILE);
    EXPECT_EQ(file->fileName(), "");
    EXPECT_EQ(file->url(), libOpenCOR::REMOTE_FILE);
    EXPECT_TRUE(!strcmp(reinterpret_cast<const char *>(file->contents().data()), libOpenCOR::SOME_UNKNOWN_CONTENTS));
}
