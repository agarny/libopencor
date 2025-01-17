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
import os
import utils


def do_test_dataset(number, specific_child_file_names):
    file = File(utils.resource_path("api/file/dataset_" + str(number) + ".omex"))

    assert file.has_child_files == True
    assert file.child_file_count == len(specific_child_file_names) + 1
    assert len(file.child_file_names) == len(specific_child_file_names) + 1
    assert len(file.child_files) == len(specific_child_file_names) + 1

    index = 0
    simulation_file = file.child_file("simulation.json")

    assert file.child_file(index) is not None
    assert simulation_file is not None

    for specific_child_file_name in specific_child_file_names:
        index += 1

        assert file.child_file(index) is not None
        assert file.child_file(specific_child_file_name) is not None

    assert file.child_file(index + 1) is None
    assert file.child_file(utils.resource_path(utils.UnknownFile)) is None

    assert utils.to_string(simulation_file.contents) == utils.text_file_contents(
        utils.resource_path("api/file/dataset_" + str(number) + ".json")
    )


def test_type_no_child_files():
    file = File(utils.resource_path(utils.UnknownFile))

    assert file.has_child_files == False
    assert file.child_file_count == 0
    assert len(file.child_file_names) == 0
    assert len(file.child_files) == 0
    assert file.child_file(0) is None
    assert file.child_file("unknown_file") is None


def test_type_dataset_135():
    do_test_dataset(135, ["HumanSAN_Fabbri_Fantini_Wilders_Severi_2017.cellml"])


def test_type_dataset_157():
    do_test_dataset(
        157,
        [
            "fabbri_et_al_based_composite_SAN_model.cellml",
            "fabbri_et_al_based_composite_SAN_model.sedml",
        ],
    )
