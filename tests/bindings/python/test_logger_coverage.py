# Copyright libOpenCOR contributors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


import libopencor as loc
import utils


def test_has_issues():
    file = loc.File(utils.resource_path(utils.Cellml2File))

    assert not file.has_issues


def test_issue_count():
    file = loc.File(utils.resource_path(utils.Cellml2File))

    assert file.issue_count == 0


def test_issues():
    file = loc.File(utils.resource_path(utils.Cellml2File))

    assert len(file.issues) == 0


def test_issue():
    file = loc.File(utils.resource_path(utils.ErrorCellmlFile))

    assert file.issue(0) != None
    assert file.issue(file.issue_count) == None


def test_has_errors():
    file = loc.File(utils.resource_path(utils.Cellml2File))

    assert not file.has_errors


def test_error_count():
    file = loc.File(utils.resource_path(utils.Cellml2File))

    assert file.error_count == 0


def test_errors():
    file = loc.File(utils.resource_path(utils.Cellml2File))

    assert len(file.errors) == 0


def test_error():
    file = loc.File(utils.resource_path(utils.ErrorCellmlFile))

    assert file.error(0) != None
    assert file.error(file.error_count) == None


def test_has_warnings():
    file = loc.File(utils.resource_path(utils.Cellml2File))

    assert not file.has_warnings


def test_warning_count():
    file = loc.File(utils.resource_path(utils.Cellml2File))

    assert file.warning_count == 0


def test_warnings():
    file = loc.File(utils.resource_path(utils.Cellml2File))

    assert len(file.warnings) == 0


def test_warning():
    file = loc.File(utils.resource_path(utils.Sedml2File))
    document = loc.SedDocument(file)

    assert document.warning(0) != None
    assert document.warning(document.warning_count) == None
