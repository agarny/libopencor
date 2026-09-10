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

set(PACKAGE_NAME LLVMClang)
set(PACKAGE_VERSION 22.1.8)
set(PACKAGE_REPOSITORY llvm-project)
set(RELEASE_TAG llvmorg-22.1.8)
set(INSTALL_DIR ${PREBUILT_DIR}/${PACKAGE_NAME})

# Either retrieve or build our package.

if(LIBOPENCOR_PREBUILT_LLVMCLANG)
    if(EMSCRIPTEN)
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         05cd2766a9164a86fefeed6c54af6ff177024a70)
    else()
        if(WIN32)
            if(RELEASE_MODE)
                if(INTEL_MODE)
                    retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                     ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                     9b4c654acab324912b28f30765697477312a83cf)
                else()
                    retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                     ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                     974ec78f371bf89ce5a475a0855fb428f5724cd6)
                endif()
            else()
                if(INTEL_MODE)
                    retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                     ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                     f57bcc2c99a3cddc423e09d06870ad4243e514bd)
                else()
                    retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                     ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                     bd417f60cd0182a33ef4c4c20fc23c03ece70a58)
                endif()
            endif()
        elseif(APPLE)
            if(INTEL_MODE)
                retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                 ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                 38cfd874eb9c386e6cfc978979d55fcfed210e3f)
            else()
                retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                 ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                 de2862e2f2c238a122e2573e16af2bbcdc37d17f)
            endif()
        else()
            if(INTEL_MODE)
                retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                 ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                 7cf85ece2ac15636c82e42d285fa4ab49420a8b3)
            else()
                retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                 ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                 a439ad9559ea909f42c192ba1c2033bd0f564877)
            endif()
        endif()
    endif()
elseif(NOT ONLY_BUILD_JAVASCRIPT_THIRD_PARTY_LIBRARIES)
    # Build our package.
    # Note: libOpenCOR's WASM version doesn't use LLVM+Clang's typeinfo (e.g., the JIT parts of compiler.cpp are not
    #       compiled for WebAssembly), so we can disable RTTI to reduce the size of our WASM package. On the other hand,
    #       libOpenCOR's native versions do use LLVM+Clang's typeinfo (e.g., compiler.cpp instantiates LLVM+Clang
    #       templates whose typeinfo references LLVM+Clang's own typeinfo), so we must enable RTTI for them.

    if(EMSCRIPTEN)
        set(LLVM_TARGETS_TO_BUILD WebAssembly)
        set(LLVM_HOST_TRIPLE wasm32-unknown-emscripten)

        set(LLVM_ENABLE_LIBCXX
            -DLLVM_ENABLE_LIBCXX=ON
        )

        set(LLVM_ENABLE_RTTI
            -DLLVM_ENABLE_RTTI=OFF
        )
    else()
        if(INTEL_MODE)
            set(LLVM_TARGETS_TO_BUILD X86)

            if(WIN32)
                set(LLVM_HOST_TRIPLE x86_64-pc-windows-msvc)
            elseif(APPLE)
                set(LLVM_HOST_TRIPLE x86_64-apple-darwin)
            else()
                set(LLVM_HOST_TRIPLE x86_64-pc-linux-gnu)
            endif()
        else()
            set(LLVM_TARGETS_TO_BUILD AArch64)

            if(WIN32)
                set(LLVM_HOST_TRIPLE aarch64-pc-windows-msvc)
            elseif(APPLE)
                set(LLVM_HOST_TRIPLE aarch64-apple-darwin)
            else()
                set(LLVM_HOST_TRIPLE aarch64-pc-linux-gnu)
            endif()
        endif()

        set(LLVM_ENABLE_RTTI
            -DLLVM_ENABLE_RTTI=ON
        )
    endif()

    # Build LLVM+Clang with size optimisation.
    # Note: this doesn't affect the quality of the code that LLVM+Clang generates for us. It only affects its own size
    #       and, to some extent, how fast it can compile code.

    if(MSVC AND NOT EMSCRIPTEN)
        set(LLVMCLANG_RELEASE_FLAGS "/O1 /DNDEBUG")
    else()
        set(LLVMCLANG_RELEASE_FLAGS "-Os -DNDEBUG")
    endif()

    # The patches to apply to LLVM+Clang.

    file(GLOB_RECURSE LLVMCLANG_PATCH_FILES "${CMAKE_CURRENT_SOURCE_DIR}/patches/*.patch")

    # Build LLVM+Clang.

    build_package(${PACKAGE_NAME}
        URL
            https://github.com/opencor/${PACKAGE_REPOSITORY}/archive/refs/tags/${RELEASE_TAG}.tar.gz
        DOWNLOAD_NO_PROGRESS ON
        SOURCE_SUBDIR
            llvm
        PATCH_COMMAND
            ${CMAKE_COMMAND} -DLLVMCLANG_SOURCE_DIR=<SOURCE_DIR>
                             -DLLVMCLANG_PATCH_FILES="${LLVMCLANG_PATCH_FILES}"
                             -P ${CMAKE_CURRENT_SOURCE_DIR}/applypatches.cmake
        CMAKE_ARGS
            -DCLANG_BUILD_TOOLS=OFF
            -DCLANG_ENABLE_INTERPRETER=OFF
            -DCLANG_ENABLE_LIBXML2=OFF
            -DCLANG_ENABLE_STATIC_ANALYZER=OFF
            -DCLANG_ENABLE_TOOLING_LIBS=OFF
            -DCLANG_INCLUDE_DOCS=OFF
            -DCLANG_INCLUDE_TESTS=OFF
            -DCLANG_PLUGIN_SUPPORT=OFF
            -DCLANG_TOOL_CLANG_DIFF_BUILD=OFF
            -DCLANG_TOOL_CLANG_IMPORT_TEST_BUILD=OFF
            -DCLANG_TOOL_CLANG_SHLIB_BUILD=OFF
            -DCLANG_TOOL_DICTIONARY_BUILD=OFF
            -DCLANG_TOOL_HANDLE_CXX_BUILD=OFF
            -DCLANG_TOOL_HANDLE_LLVM_BUILD=OFF
            -DCLANG_TOOL_LIBCLANG_BUILD=OFF
            ${CMAKE_ARGS}
            -DCMAKE_CXX_FLAGS_RELEASE=${LLVMCLANG_RELEASE_FLAGS}
            -DCMAKE_C_FLAGS_RELEASE=${LLVMCLANG_RELEASE_FLAGS}
            -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
            -DLLVM_BUILD_LLVM_C_DYLIB=OFF
            -DLLVM_BUILD_RUNTIME=OFF
            -DLLVM_BUILD_RUNTIMES=OFF
            -DLLVM_BUILD_TOOLS=OFF
            -DLLVM_BUILD_UTILS=OFF
            -DLLVM_ENABLE_ASSERTIONS=OFF
            -DLLVM_ENABLE_BACKTRACES=OFF
            -DLLVM_ENABLE_BINDINGS=OFF
            -DLLVM_ENABLE_CRASH_OVERRIDES=OFF
            -DLLVM_ENABLE_DIA_SDK=OFF
            -DLLVM_ENABLE_LIBEDIT=OFF
            -DLLVM_ENABLE_LIBPFM=OFF
            -DLLVM_ENABLE_LIBXML2=OFF
            ${LLVM_ENABLE_LIBCXX}
            -DLLVM_ENABLE_MODULE_DEBUGGING=OFF
            -DLLVM_ENABLE_OCAMLDOC=OFF
            -DLLVM_ENABLE_ONDISK_CAS=OFF
            -DLLVM_ENABLE_PEDANTIC=OFF
            -DLLVM_ENABLE_PLUGINS=OFF
            -DLLVM_ENABLE_PROJECTS=clang
            ${LLVM_ENABLE_RTTI}
            -DLLVM_ENABLE_TELEMETRY=OFF
            -DLLVM_ENABLE_UNWIND_TABLES=OFF
            -DLLVM_ENABLE_WARNINGS=OFF
            -DLLVM_ENABLE_ZLIB=OFF
            -DLLVM_ENABLE_ZSTD=OFF
            -DLLVM_HOST_TRIPLE=${LLVM_HOST_TRIPLE}
            -DLLVM_INCLUDE_BENCHMARKS=OFF
            -DLLVM_INCLUDE_DOCS=OFF
            -DLLVM_INCLUDE_EXAMPLES=OFF
            -DLLVM_INCLUDE_RUNTIMES=OFF
            -DLLVM_INCLUDE_TESTS=OFF
            -DLLVM_INCLUDE_TOOLS=ON
            -DLLVM_INCLUDE_UTILS=OFF
            -DLLVM_TARGETS_TO_BUILD=${LLVM_TARGETS_TO_BUILD}
            -DLLVM_TOOL_LTO_BUILD=OFF
            -DLLVM_TOOL_REMARKS_SHLIB_BUILD=OFF
    )

    # Create our package.
    # Note: we also package llvm-tblgen and clang-tblgen since we need them to build our JavaScript bindings.

    if(NOT EMSCRIPTEN)
        set(TABLEGEN_EXES_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/build/LLVMClang-prefix/src/LLVMClang-build)

        file(GLOB TABLEGEN_EXES "${TABLEGEN_EXES_BUILD_DIR}/bin/*tblgen")

        foreach(TABLEGEN_EXE ${TABLEGEN_EXES})
            file(COPY ${TABLEGEN_EXE} DESTINATION ${INSTALL_DIR}/bin)
        endforeach()
    endif()

    create_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                   ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                   bin include lib)
endif()

# Make our package accessible.

if(EMSCRIPTEN)
    set(EXTRA_LLVMCLANG_LIBRARIES
        LLVMWebAssemblyAsmParser
        LLVMWebAssemblyCodeGen
        LLVMWebAssemblyDesc
        LLVMWebAssemblyDisassembler
        LLVMWebAssemblyInfo
        LLVMWebAssemblyUtils
    )
else()
    if(INTEL_MODE)
        set(EXTRA_LLVMCLANG_LIBRARIES
            LLVMX86AsmParser
            LLVMX86CodeGen
            LLVMX86Desc
            LLVMX86Disassembler
            LLVMX86Info
        )
    else()
        set(EXTRA_LLVMCLANG_LIBRARIES
            LLVMAArch64AsmParser
            LLVMAArch64CodeGen
            LLVMAArch64Desc
            LLVMAArch64Disassembler
            LLVMAArch64Info
            LLVMAArch64Utils
        )
    endif()
endif()

set(LLVMCLANG_ROOT ${INSTALL_DIR} CACHE INTERNAL "${PACKAGE_NAME}'s root directory.")
set(LLVMCLANG_CMAKE_DIR ${INSTALL_DIR}/lib/cmake/clang CACHE INTERNAL "${PACKAGE_NAME}'s CMake directory.")
set(LLVMCLANG_CMAKE_DIRS ${INSTALL_DIR}/lib/cmake/llvm ${INSTALL_DIR}/lib/cmake/clang CACHE INTERNAL "${PACKAGE_NAME}'s CMake directories.")
set(LLVMCLANG_CMAKE_PACKAGE_NAME Clang CACHE INTERNAL "${PACKAGE_NAME}'s CMake package name.")
set(LLVMCLANG_CMAKE_PACKAGE_NAMES LLVM Clang CACHE INTERNAL "${PACKAGE_NAME}'s CMake package names.")
set(LLVMCLANG_INCLUDE_DIR ${INSTALL_DIR}/include CACHE INTERNAL "${PACKAGE_NAME}'s include directory.")
set(LLVMCLANG_LIBRARIES
    clangAnalysis
    clangAnalysisFlowSensitive
    clangAnalysisFlowSensitiveModels
    clangAnalysisLifetimeSafety
    clangAnalysisScalable
    clangAPINotes
    clangAST
    clangASTMatchers
    clangBasic
    clangCodeGen
    clangCrossTU
    clangDependencyScanning
    clangDirectoryWatcher
    clangDriver
    clangDynamicASTMatchers
    clangEdit
    clangExtractAPI
    clangFormat
    clangFrontend
    clangFrontendTool
    clangIndex
    clangIndexSerialization
    clangInstallAPI
    clangInterpreter
    clangLex
    clangOptions
    clangParse
    clangRewrite
    clangRewriteFrontend
    clangSema
    clangSerialization
    clangStaticAnalyzerCheckers
    clangStaticAnalyzerCore
    clangStaticAnalyzerFrontend
    clangSupport
    clangTooling
    clangToolingASTDiff
    clangToolingCore
    clangToolingInclusions
    clangToolingInclusionsStdlib
    clangToolingRefactoring
    clangToolingSyntax
    clangTransformer
    LLVMABI
    LLVMAggressiveInstCombine
    LLVMAnalysis
    LLVMAsmParser
    LLVMAsmPrinter
    LLVMBinaryFormat
    LLVMBitReader
    LLVMBitstreamReader
    LLVMBitWriter
    LLVMCAS
    LLVMCFGuard
    LLVMCFIVerify
    LLVMCGData
    LLVMCodeGen
    LLVMCodeGenTypes
    LLVMCore
    LLVMCoroutines
    LLVMCoverage
    LLVMDebugInfoBTF
    LLVMDebugInfoCodeView
    LLVMDebuginfod
    LLVMDebugInfoDWARF
    LLVMDebugInfoDWARFLowLevel
    LLVMDebugInfoGSYM
    LLVMDebugInfoLogicalView
    LLVMDebugInfoMSF
    LLVMDebugInfoPDB
    LLVMDemangle
    LLVMDiff
    LLVMDlltoolDriver
    LLVMDTLTO
    LLVMDWARFCFIChecker
    LLVMDWARFLinker
    LLVMDWARFLinkerClassic
    LLVMDWARFLinkerParallel
    LLVMDWP
    LLVMExecutionEngine
    LLVMExegesis
    LLVMExtensions
    LLVMFileCheck
    LLVMFrontendAtomic
    LLVMFrontendDirective
    LLVMFrontendDriver
    LLVMFrontendHLSL
    LLVMFrontendOffloading
    LLVMFrontendOpenACC
    LLVMFrontendOpenMP
    LLVMFuzzerCLI
    LLVMFuzzMutate
    LLVMGlobalISel
    LLVMHipStdPar
    LLVMInstCombine
    LLVMInstrumentation
    LLVMInterfaceStub
    LLVMInterpreter
    LLVMipo
    LLVMIRPrinter
    LLVMIRReader
    LLVMJITLink
    LLVMLibDriver
    LLVMLineEditor
    LLVMLinker
    LLVMLTO
    LLVMMC
    LLVMMCA
    LLVMMCDisassembler
    LLVMMCJIT
    LLVMMCParser
    LLVMMIRParser
    LLVMObjCARCOpts
    LLVMObjCopy
    LLVMObject
    LLVMObjectYAML
    LLVMOptDriver
    LLVMOption
    LLVMOrcDebugging
    LLVMOrcJIT
    LLVMOrcShared
    LLVMOrcTargetProcess
    LLVMPasses
    LLVMPlugins
    LLVMProfileData
    LLVMRemarks
    LLVMRuntimeDyld
    LLVMSandboxIR
    LLVMScalarOpts
    LLVMSelectionDAG
    LLVMSupport
    LLVMSupportLSP
    LLVMSymbolize
    LLVMTableGen
    LLVMTableGenBasic
    LLVMTableGenCommon
    LLVMTarget
    LLVMTargetParser
    LLVMTelemetry
    LLVMTextAPI
    LLVMTextAPIBinaryReader
    LLVMTransformUtils
    LLVMVectorize
    LLVMWindowsDriver
    LLVMWindowsManifest
    LLVMXRay
    ${EXTRA_LLVMCLANG_LIBRARIES}
    CACHE INTERNAL "${PACKAGE_NAME}'s libraries."
)
