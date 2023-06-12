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

const WINDOWS_LOCAL_FILE = "C:\\some\\path\\file.txt";
const UNIX_LOCAL_FILE = "/some/path/file.txt";

describe("File basic tests", () => {
    test("Basic Windows file", () => {
        const file = new libopencor.File(WINDOWS_LOCAL_FILE);

        expect(file.type().value).toBe(libopencor.File.Type.UNRESOLVED.value);
        expect(file.fileName()).toBe(WINDOWS_LOCAL_FILE);
        expect(file.url()).toBe("");
    });

    test("Basic UNIX file", () => {
        const file = new libopencor.File(UNIX_LOCAL_FILE);

        expect(file.type().value).toBe(libopencor.File.Type.UNRESOLVED.value);
        expect(file.fileName()).toBe(UNIX_LOCAL_FILE);
        expect(file.url()).toBe("");
    });

    test("Basic URL-based Windows file", () => {
        const file = new libopencor.File("file:///C:/some/path/file.txt");

        expect(file.type().value).toBe(libopencor.File.Type.UNRESOLVED.value);
        expect(file.fileName()).toBe(WINDOWS_LOCAL_FILE);
        expect(file.url()).toBe("");
    });

    test("Basic URL-based UNIX file", () => {
        const file = new libopencor.File("file:///some/path/file.txt");

        expect(file.type().value).toBe(libopencor.File.Type.UNRESOLVED.value);
        expect(file.fileName()).toBe(UNIX_LOCAL_FILE);
        expect(file.url()).toBe("");
    });

    test("Basic remote file", () => {
        const file = new libopencor.File(utils.REMOTE_FILE);

        expect(file.type().value).toBe(libopencor.File.Type.UNRESOLVED.value);
        expect(file.fileName()).toBe("");
        expect(file.url()).toBe(utils.REMOTE_FILE);
    });
});