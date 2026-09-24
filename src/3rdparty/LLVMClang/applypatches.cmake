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

# Apply the libOpenCOR patches to the LLVM+Clang source code, if needed.
# Note #1: the patches are applied by libOpenCOR's CMakeLists.txt when building LLVM+Clang from source. They are also
#          meant to be applied (and committed) to the opencor/llvm-project repository so that our prebuilt packages
#          include them.
# Note #2: ExternalProject re-runs our patch step whenever our patches change (see LLVMCLANG_PATCHES_SHA1 in
#          CMakeLists.txt), but it does so in the source directory that it previously patched. So, we keep a copy of
#          the patches that we apply, so that we can revert them before applying the current ones, if needed.

if(NOT DEFINED LLVMCLANG_SOURCE_DIR)
    message(FATAL_ERROR "LLVMCLANG_SOURCE_DIR must be set to the LLVM+Clang source directory.")
endif()

if(NOT DEFINED LLVMCLANG_PATCHES_DIR)
    message(FATAL_ERROR "LLVMCLANG_PATCHES_DIR must be set to the directory that contains the libOpenCOR patches to be applied to LLVM+Clang.")
endif()

if(NOT EXISTS "${LLVMCLANG_PATCHES_DIR}")
    message(FATAL_ERROR "The directory that contains the libOpenCOR patches to be applied to LLVM+Clang could not be found (${LLVMCLANG_PATCHES_DIR}).")
endif()

if(NOT EXISTS "${LLVMCLANG_SOURCE_DIR}/llvm/CMakeLists.txt")
    message(FATAL_ERROR "The LLVM+Clang source directory could not be found (${LLVMCLANG_SOURCE_DIR}).")
endif()

find_program(GIT_EXECUTABLE NAMES git)

if(NOT GIT_EXECUTABLE)
    message(FATAL_ERROR "git could not be found, so the LLVM+Clang patches could not be applied.")
endif()

# Make sure that the source directory is a git repository, otherwise git apply would silently skip the patches (it
# only applies them if the current or a parent directory is a git repository, which is not something we can rely on,
# e.g. if libOpenCOR's source is a tarball rather than a clone).

execute_process(COMMAND ${GIT_EXECUTABLE} init -q
                WORKING_DIRECTORY "${LLVMCLANG_SOURCE_DIR}"
                RESULT_VARIABLE RESULT
                ERROR_VARIABLE ERROR)

if(NOT RESULT EQUAL 0)
    message(FATAL_ERROR "git init failed, so the LLVM+Clang patches could not be applied (${ERROR}).")
endif()

# The directories where we keep a copy of the patches that we are to apply and of those that we have applied, as well
# as the file that lists the latter in the order in which we applied them.

set(LLVMCLANG_CURRENT_PATCHES_DIR "${LLVMCLANG_SOURCE_DIR}/.libopencor/currentpatches")
set(LLVMCLANG_APPLIED_PATCHES_DIR "${LLVMCLANG_SOURCE_DIR}/.libopencor/appliedpatches")
set(LLVMCLANG_APPLIED_PATCHES_LIST "${LLVMCLANG_SOURCE_DIR}/.libopencor/appliedpatches.txt")

# Run git apply with the given arguments on the given patch file.
# Note: we don't want git to convert the line endings of the patched files (e.g. on Windows), so we disable any
#       automatic line ending conversion and ask git to ignore any remaining whitespace differences.

function(git_apply GIT_APPLY_RESULT GIT_APPLY_ERROR PATCH_FILE)
    execute_process(COMMAND ${GIT_EXECUTABLE} -c core.autocrlf=input apply ${ARGN} --whitespace=nowarn --ignore-space-change "${PATCH_FILE}"
                    WORKING_DIRECTORY "${LLVMCLANG_SOURCE_DIR}"
                    RESULT_VARIABLE RESULT
                    ERROR_VARIABLE ERROR)

    set(${GIT_APPLY_RESULT} ${RESULT} PARENT_SCOPE)
    set(${GIT_APPLY_ERROR} "${ERROR}" PARENT_SCOPE)
endfunction()

# Determine the libOpenCOR patches to be applied to LLVM+Clang and normalise their line endings, as well as those of
# the files that they patch, if needed.
# Note: on Windows, git checks out text files (such as our patch files) with CRLF line endings by default, while the
#       LLVM+Clang source files (as extracted from their archive) have LF line endings. git apply cannot apply a patch
#       with CRLF line endings to files with LF line endings (and vice versa), so we make sure that both the patch file
#       and the files to be patched use LF line endings.

file(GLOB_RECURSE LLVMCLANG_PATCH_FILES "${LLVMCLANG_PATCHES_DIR}/*.patch")

file(REMOVE_RECURSE "${LLVMCLANG_CURRENT_PATCHES_DIR}")

set(LLVMCLANG_PATCH_NAMES)

foreach(LLVMCLANG_PATCH_FILE IN LISTS LLVMCLANG_PATCH_FILES)
    get_filename_component(LLVMCLANG_PATCH_NAME "${LLVMCLANG_PATCH_FILE}" NAME)

    list(APPEND LLVMCLANG_PATCH_NAMES ${LLVMCLANG_PATCH_NAME})

    file(READ "${LLVMCLANG_PATCH_FILE}" LLVMCLANG_PATCH_CONTENT)
    string(REPLACE "\r\n" "\n" LLVMCLANG_PATCH_CONTENT "${LLVMCLANG_PATCH_CONTENT}")
    file(WRITE "${LLVMCLANG_CURRENT_PATCHES_DIR}/${LLVMCLANG_PATCH_NAME}" "${LLVMCLANG_PATCH_CONTENT}")

    # Normalise the line endings of the files to be patched. They are the files listed in the patch file.

    string(REGEX MATCHALL "diff --git a/[^ ]+ b/[^ ]+"
           LLVMCLANG_PATCHED_FILES "${LLVMCLANG_PATCH_CONTENT}")

    foreach(LLVMCLANG_PATCHED_FILE ${LLVMCLANG_PATCHED_FILES})
        string(REPLACE "diff --git a/" "" LLVMCLANG_PATCHED_FILE "${LLVMCLANG_PATCHED_FILE}")
        string(REGEX REPLACE " b/.*" "" LLVMCLANG_PATCHED_FILE "${LLVMCLANG_PATCHED_FILE}")

        # Note: we only rewrite a file if it has CRLF line endings, so that its timestamp doesn't change (and it doesn't
        #       get recompiled) for nothing.

        if(EXISTS "${LLVMCLANG_SOURCE_DIR}/${LLVMCLANG_PATCHED_FILE}")
            file(READ "${LLVMCLANG_SOURCE_DIR}/${LLVMCLANG_PATCHED_FILE}" LLVMCLANG_PATCHED_FILE_CONTENT)
            string(FIND "${LLVMCLANG_PATCHED_FILE_CONTENT}" "\r\n" LLVMCLANG_CRLF_INDEX)

            if(NOT LLVMCLANG_CRLF_INDEX EQUAL -1)
                string(REPLACE "\r\n" "\n" LLVMCLANG_PATCHED_FILE_CONTENT "${LLVMCLANG_PATCHED_FILE_CONTENT}")
                file(WRITE "${LLVMCLANG_SOURCE_DIR}/${LLVMCLANG_PATCHED_FILE}" "${LLVMCLANG_PATCHED_FILE_CONTENT}")
            endif()
        endif()
    endforeach()
endforeach()

# Check whether we have already applied the patches that we are to apply, in which case there is nothing for us to do.
# Otherwise, revert the patches that we have applied, from the last one to the first one.

if(EXISTS "${LLVMCLANG_APPLIED_PATCHES_LIST}")
    file(STRINGS "${LLVMCLANG_APPLIED_PATCHES_LIST}" LLVMCLANG_APPLIED_PATCH_NAMES)

    set(LLVMCLANG_PATCHES_ALREADY_APPLIED FALSE)

    if("${LLVMCLANG_APPLIED_PATCH_NAMES}" STREQUAL "${LLVMCLANG_PATCH_NAMES}")
        set(LLVMCLANG_PATCHES_ALREADY_APPLIED TRUE)

        foreach(LLVMCLANG_PATCH_NAME IN LISTS LLVMCLANG_PATCH_NAMES)
            file(SHA1 "${LLVMCLANG_CURRENT_PATCHES_DIR}/${LLVMCLANG_PATCH_NAME}" LLVMCLANG_CURRENT_PATCH_SHA1)
            file(SHA1 "${LLVMCLANG_APPLIED_PATCHES_DIR}/${LLVMCLANG_PATCH_NAME}" LLVMCLANG_APPLIED_PATCH_SHA1)

            if(NOT "${LLVMCLANG_CURRENT_PATCH_SHA1}" STREQUAL "${LLVMCLANG_APPLIED_PATCH_SHA1}")
                set(LLVMCLANG_PATCHES_ALREADY_APPLIED FALSE)

                break()
            endif()
        endforeach()
    endif()

    if(LLVMCLANG_PATCHES_ALREADY_APPLIED)
        foreach(LLVMCLANG_PATCH_NAME IN LISTS LLVMCLANG_PATCH_NAMES)
            message(STATUS "Applying the LLVM+Clang patch ${LLVMCLANG_PATCH_NAME} - already applied")
        endforeach()

        file(REMOVE_RECURSE "${LLVMCLANG_CURRENT_PATCHES_DIR}")

        return()
    endif()

    list(REVERSE LLVMCLANG_APPLIED_PATCH_NAMES)

    foreach(LLVMCLANG_APPLIED_PATCH_NAME IN LISTS LLVMCLANG_APPLIED_PATCH_NAMES)
        git_apply(RESULT ERROR "${LLVMCLANG_APPLIED_PATCHES_DIR}/${LLVMCLANG_APPLIED_PATCH_NAME}" --reverse)

        if(NOT RESULT EQUAL 0)
            message(FATAL_ERROR "The LLVM+Clang patch ${LLVMCLANG_APPLIED_PATCH_NAME} could not be reverted (${ERROR}). Please delete the LLVM+Clang source directory (${LLVMCLANG_SOURCE_DIR}), so that it gets extracted and patched afresh.")
        endif()

        message(STATUS "Reverting the LLVM+Clang patch ${LLVMCLANG_APPLIED_PATCH_NAME} - Success")
    endforeach()

    file(REMOVE_RECURSE "${LLVMCLANG_APPLIED_PATCHES_DIR}")
    file(REMOVE "${LLVMCLANG_APPLIED_PATCHES_LIST}")
endif()

# Apply the patches, keeping a copy of each of them so that we can revert them later, if needed.

file(MAKE_DIRECTORY "${LLVMCLANG_APPLIED_PATCHES_DIR}")
file(WRITE "${LLVMCLANG_APPLIED_PATCHES_LIST}" "")

foreach(LLVMCLANG_PATCH_NAME IN LISTS LLVMCLANG_PATCH_NAMES)
    set(LLVMCLANG_PATCH_FILE "${LLVMCLANG_CURRENT_PATCHES_DIR}/${LLVMCLANG_PATCH_NAME}")

    # Check whether the patch has already been applied (e.g. it may have been committed to the opencor/llvm-project
    # repository that we download from). Indeed, if the patch can be reverse-applied, then it has already been applied.

    git_apply(RESULT ERROR "${LLVMCLANG_PATCH_FILE}" --reverse --check)

    if(RESULT EQUAL 0)
        message(STATUS "Applying the LLVM+Clang patch ${LLVMCLANG_PATCH_NAME} - already applied")
    else()
        git_apply(RESULT ERROR "${LLVMCLANG_PATCH_FILE}")

        if(NOT RESULT EQUAL 0)
            message(FATAL_ERROR "The LLVM+Clang patch ${LLVMCLANG_PATCH_NAME} could not be applied (${ERROR}). If the LLVM+Clang source directory (${LLVMCLANG_SOURCE_DIR}) was patched with a different version of the patch, then please delete that directory, so that it gets extracted and patched afresh.")
        endif()

        message(STATUS "Applying the LLVM+Clang patch ${LLVMCLANG_PATCH_NAME} - Success")
    endif()

    file(COPY_FILE "${LLVMCLANG_PATCH_FILE}" "${LLVMCLANG_APPLIED_PATCHES_DIR}/${LLVMCLANG_PATCH_NAME}")
    file(APPEND "${LLVMCLANG_APPLIED_PATCHES_LIST}" "${LLVMCLANG_PATCH_NAME}\n")
endforeach()

file(REMOVE_RECURSE "${LLVMCLANG_CURRENT_PATCHES_DIR}")
