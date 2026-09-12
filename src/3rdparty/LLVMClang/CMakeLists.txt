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

# The libraries that libOpenCOR doesn't need.
# Note: they are only needed by the various LLVM+Clang tools and/or libclang, none of which we use. So, we don't want
#       them to be part of our package, to be linked into libOpenCOR, or to be merged into its static version.

set(LLVMCLANG_EXCLUDE_LIBRARIES
    clangAnalysisFlowSensitive
    clangAnalysisFlowSensitiveModels
    clangAnalysisScalable
    clangCrossTU
    clangDependencyScanning
    clangDirectoryWatcher
    clangDynamicASTMatchers
    clangExtractAPI
    clangFormat
    clangFrontendTool
    clangIndex
    clangIndexSerialization
    clangInstallAPI
    clangInterpreter
    clangRewrite
    clangRewriteFrontend
    clangStaticAnalyzerCheckers
    clangStaticAnalyzerCore
    clangStaticAnalyzerFrontend
    clangTooling
    clangToolingASTDiff
    clangToolingCore
    clangToolingInclusions
    clangToolingInclusionsStdlib
    clangToolingRefactoring
    clangToolingSyntax
    clangTransformer
    LLVMAArch64AsmParser
    LLVMAArch64Disassembler
    LLVMABI
    LLVMCAS
    LLVMCFIVerify
    LLVMDebugInfoBTF
    LLVMDebuginfod
    LLVMDebugInfoGSYM
    LLVMDebugInfoLogicalView
    LLVMDebugInfoMSF
    LLVMDebugInfoPDB
    LLVMDiff
    LLVMDlltoolDriver
    LLVMDTLTO
    LLVMDWARFCFIChecker
    LLVMDWARFLinker
    LLVMDWARFLinkerClassic
    LLVMDWARFLinkerParallel
    LLVMDWP
    LLVMExegesis
    LLVMExtensions
    LLVMFileCheck
    LLVMFrontendOpenACC
    LLVMFuzzerCLI
    LLVMFuzzMutate
    LLVMInterfaceStub
    LLVMInterpreter
    LLVMLibDriver
    LLVMLineEditor
    LLVMMCA
    LLVMMCJIT
    LLVMMIRParser
    LLVMObjCopy
    LLVMOptDriver
    LLVMOrcDebugging
    LLVMSupportLSP
    LLVMSymbolize
    LLVMTableGen
    LLVMTableGenBasic
    LLVMTableGenCommon
    LLVMTelemetry
    LLVMTextAPIBinaryReader
    LLVMWindowsManifest
    LLVMX86AsmParser
    LLVMX86Disassembler
    LLVMXRay
)

# Either retrieve or build our package.

if(LIBOPENCOR_PREBUILT_LLVMCLANG)
    if(EMSCRIPTEN)
        retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                         ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                         e089448b11e1147260b91ef3605e138d709a8125)
    else()
        if(WIN32)
            if(RELEASE_MODE)
                if(INTEL_MODE)
                    retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                     ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                     38c12f6b973d2ce9cae44c65e80bc075f96204e1)
                else()
                    retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                     ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                     7cef17e6ddde780d8824efa77c04c0d4c579831b)
                endif()
            else()
                if(INTEL_MODE)
                    retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                     ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                     865057e2236df680e676f784eae070182440d107)
                else()
                    retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                     ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                     657eaf3ad56455c1d6f6d9dce1e3ca5582458809)
                endif()
            endif()
        elseif(APPLE)
            if(INTEL_MODE)
                retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                 ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                 079e8bb6ba42171e39501fbeca3874791a59e90f)
            else()
                retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                 ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                 f1975382fd202158cdce094ccfae885f728636ec)
            endif()
        else()
            if(INTEL_MODE)
                retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                 ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                 0022ab49c4cf74fa195fdff0adf86635c54fcedc)
            else()
                retrieve_package(${PACKAGE_NAME} ${PACKAGE_VERSION}
                                 ${PACKAGE_REPOSITORY} ${RELEASE_TAG}
                                 e5fa5ff7c9b081d97a74fdbfc7b8e6964c9bf941)
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

    # Build LLVM+Clang.

    build_package(${PACKAGE_NAME}
        URL
            https://github.com/opencor/${PACKAGE_REPOSITORY}/archive/refs/tags/${RELEASE_TAG}.tar.gz
        DOWNLOAD_NO_PROGRESS ON
        SOURCE_SUBDIR
            llvm
        PATCH_COMMAND
            ${CMAKE_COMMAND} -DLLVMCLANG_SOURCE_DIR=<SOURCE_DIR>
                             -DLLVMCLANG_PATCHES_DIR=${CMAKE_CURRENT_SOURCE_DIR}/patches
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
    # # Note: we also package llvm-tblgen and clang-tblgen since we need them to build our JavaScript bindings.

    # if(NOT EMSCRIPTEN)
    #     set(TABLEGEN_EXES_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/build/LLVMClang-prefix/src/LLVMClang-build)

    #     file(GLOB TABLEGEN_EXES "${TABLEGEN_EXES_BUILD_DIR}/bin/*tblgen")

    #     foreach(TABLEGEN_EXE ${TABLEGEN_EXES})
    #         file(COPY ${TABLEGEN_EXE} DESTINATION ${INSTALL_DIR}/bin)
    #     endforeach()
    # endif()

    # # Remove the files that we don't need from our package.
    # # Note: we only need llvm-tblgen and clang-tblgen (to build our JavaScript bindings), so we remove everything else
    # #       from the bin directory (e.g. llvm-min-tblgen, which is only used to bootstrap LLVM+Clang's own build).
    # #       (We cannot do the same for the libraries in LLVMCLANG_EXCLUDE_LIBRARIES since LLVM+Clang's installed CMake
    # #       files reference them all, so removing them would break find_package(LLVM/Clang). They are instead excluded
    # #       from LLVMCLANG_LIBRARIES, so that they are neither linked into libOpenCOR nor merged into its static
    # #       version.)

    # file(GLOB BIN_FILES ${INSTALL_DIR}/bin/*)

    # foreach(BIN_FILE ${BIN_FILES})
    #     get_filename_component(BIN_FILE_NAME ${BIN_FILE} NAME)

    #     if(NOT BIN_FILE_NAME MATCHES "^(llvm|clang)-tblgen(\\.exe)?$")
    #         file(REMOVE ${BIN_FILE})
    #     endif()
    # endforeach()

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

# Remove from the list above the libraries that we don't need, then cache the resulting list, so that it can be used
# by the rest of libOpenCOR's building system (see LLVMCLANG_EXCLUDE_LIBRARIES above).

list(REMOVE_ITEM LLVMCLANG_LIBRARIES ${LLVMCLANG_EXCLUDE_LIBRARIES})

set(LLVMCLANG_LIBRARIES ${LLVMCLANG_LIBRARIES} CACHE INTERNAL "${PACKAGE_NAME}'s libraries." FORCE)

set(LLVMCLANG_EXCLUDE_LIBRARIES ${LLVMCLANG_EXCLUDE_LIBRARIES} CACHE INTERNAL
    "LLVM+Clang libraries that are not needed by libOpenCOR." FORCE)
