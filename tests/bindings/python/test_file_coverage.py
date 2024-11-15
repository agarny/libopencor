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


import libopencor as oc
import utils


def test_empty_file():
    file = oc.File(utils.LOCAL_FILE)

    file.contents = utils.string_to_list(utils.NO_CONTENTS)

    assert file.type == oc.File.Type.UnknownFile


def test_http_remote_file():
    oc.File(utils.HTTP_REMOTE_FILE)


def test_irretrievable_remote_file():
    oc.File(utils.IRRETRIEVABLE_REMOTE_FILE)


def test_same_local_file():
    file1 = oc.File(utils.LOCAL_FILE)
    file2 = oc.File(utils.LOCAL_FILE)

    assert file1.__subclasshook__ == file2.__subclasshook__


def test_same_remote_file():
    file1 = oc.File(utils.REMOTE_FILE)
    file2 = oc.File(utils.REMOTE_FILE)

    assert file1.__subclasshook__ == file2.__subclasshook__
