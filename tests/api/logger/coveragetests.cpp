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

#include "tests/utils.h"

#include <libopencor>

TEST(CoverageLoggerTest, hasIssues)
{
    auto file = libOpenCOR::File::create(libOpenCOR::resourcePath(libOpenCOR::CELLML_2_FILE));

    EXPECT_FALSE(file->hasIssues());
}

TEST(CoverageLoggerTest, issueCount)
{
    auto file = libOpenCOR::File::create(libOpenCOR::resourcePath(libOpenCOR::CELLML_2_FILE));

    EXPECT_EQ(file->issueCount(), 0);
}

TEST(CoverageLoggerTest, issues)
{
    auto file = libOpenCOR::File::create(libOpenCOR::resourcePath(libOpenCOR::CELLML_2_FILE));

    EXPECT_TRUE(file->issues().empty());
}

TEST(CoverageLoggerTest, issue)
{
    auto file = libOpenCOR::File::create(libOpenCOR::resourcePath(libOpenCOR::ERROR_CELLML_FILE));

    EXPECT_NE(file->issue(0), nullptr);
    EXPECT_EQ(file->issue(file->issueCount()), nullptr);
}

TEST(CoverageLoggerTest, hasErrors)
{
    auto file = libOpenCOR::File::create(libOpenCOR::resourcePath(libOpenCOR::CELLML_2_FILE));

    EXPECT_FALSE(file->hasErrors());
}

TEST(CoverageLoggerTest, errorCount)
{
    auto file = libOpenCOR::File::create(libOpenCOR::resourcePath(libOpenCOR::CELLML_2_FILE));

    EXPECT_EQ(file->errorCount(), 0);
}

TEST(CoverageLoggerTest, errors)
{
    auto file = libOpenCOR::File::create(libOpenCOR::resourcePath(libOpenCOR::CELLML_2_FILE));

    EXPECT_TRUE(file->errors().empty());
}

TEST(CoverageLoggerTest, error)
{
    auto file = libOpenCOR::File::create(libOpenCOR::resourcePath(libOpenCOR::ERROR_CELLML_FILE));

    EXPECT_NE(file->error(0), nullptr);
    EXPECT_EQ(file->error(file->errorCount()), nullptr);
}

TEST(CoverageLoggerTest, hasWarnings)
{
    auto file = libOpenCOR::File::create(libOpenCOR::resourcePath(libOpenCOR::CELLML_2_FILE));

    EXPECT_FALSE(file->hasWarnings());
}

TEST(CoverageLoggerTest, warningCount)
{
    auto file = libOpenCOR::File::create(libOpenCOR::resourcePath(libOpenCOR::CELLML_2_FILE));

    EXPECT_EQ(file->warningCount(), 0);
}

TEST(CoverageLoggerTest, warnings)
{
    auto file = libOpenCOR::File::create(libOpenCOR::resourcePath(libOpenCOR::CELLML_2_FILE));

    EXPECT_TRUE(file->warnings().empty());
}

TEST(CoverageLoggerTest, warning)
{
    auto file = libOpenCOR::File::create(libOpenCOR::resourcePath(libOpenCOR::SEDML_2_FILE));
    auto document = libOpenCOR::SedDocument::create(file);

    EXPECT_NE(document->warning(0), nullptr);
    EXPECT_EQ(document->warning(document->warningCount()), nullptr);
}
