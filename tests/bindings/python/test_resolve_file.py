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

from libopencor import File
import utils


def test_resolve_non_retrievable_file():
    f = File(utils.resource_path(utils.NON_RETRIEVABLE_FILE))

    assert f.resolve() == File.Status.NonRetrievableFile
    assert f.type == File.Type.Unresolved


def test_resolve_unknown_file():
    f = File(utils.resource_path(utils.UNKNOWN_FILE))

    assert f.resolve() == File.Status.Ok
    assert f.type == File.Type.UnknownFile


def test_resolve_sbml_file():
    f = File(utils.resource_path(utils.SBML_FILE))

    assert f.resolve() == File.Status.Ok
    assert f.type == File.Type.UnknownFile


def test_resolve_error_sedml_file():
    f = File(utils.resource_path(utils.ERROR_SEDML_FILE))

    assert f.resolve() == File.Status.Ok
    assert f.type == File.Type.SedmlFile


def test_resolve_cellml_1_x_file():
    f = File(utils.resource_path(utils.CELLML_1_X_FILE))

    assert f.resolve() == File.Status.Ok
    assert f.type == File.Type.CellmlFile


def test_resolve_sedml_1_x_file():
    f = File(utils.resource_path(utils.SEDML_1_X_FILE))

    assert f.resolve() == File.Status.Ok
    assert f.type == File.Type.SedmlFile


def test_resolve_combine_1_x_archive():
    f = File(utils.resource_path(utils.COMBINE_1_X_ARCHIVE))

    assert f.resolve() == File.Status.Ok
    assert f.type == File.Type.CombineArchive


def test_resolve_cellml_2_file():
    f = File(utils.resource_path(utils.CELLML_2_FILE))

    assert f.resolve() == File.Status.Ok
    assert f.type == File.Type.CellmlFile


def test_resolve_sedml_2_file():
    f = File(utils.resource_path(utils.SEDML_2_FILE))

    assert f.resolve() == File.Status.Ok
    assert f.type == File.Type.SedmlFile


def test_resolve_combine_2_archive():
    f = File(utils.resource_path(utils.COMBINE_2_ARCHIVE))

    assert f.resolve() == File.Status.Ok
    assert f.type == File.Type.CombineArchive