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

import libOpenCOR from "./libopencor.js";
import * as utils from "./utils.js";

const libopencor = await libOpenCOR();

describe("File coverage tests", () => {
    test("http remote file", () => {
        new libopencor.File(utils.HTTP_REMOTE_FILE, utils.SOME_UNKNOWN_CONTENTS, utils.SOME_UNKNOWN_CONTENTS.size);
    });

    test("Not virtual file", () => {
        new libopencor.File(utils.REMOTE_FILE, utils.SOME_UNKNOWN_CONTENTS, 0);
    });
});
