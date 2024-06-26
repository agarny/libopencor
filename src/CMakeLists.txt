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

# Enable compiler caching.

if(LIBOPENCOR_COMPILER_CACHING)
    if(BUILDCACHE_EXE)
        set(CMAKE_C_COMPILER_LAUNCHER ${BUILDCACHE_EXE})
        set(CMAKE_CXX_COMPILER_LAUNCHER ${BUILDCACHE_EXE})
    elseif(CLCACHE_EXE)
        set(CLCACHEWRAPPER ${CMAKE_BINARY_DIR}/clcachewrapper)

        try_compile(CLCACHEWRAPPER_EXE ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/cmake/clcachewrapper.c
                    COPY_FILE ${CLCACHEWRAPPER})

        if(CLCACHEWRAPPER_EXE)
            set(CMAKE_C_COMPILER_LAUNCHER ${CLCACHEWRAPPER})
            set(CMAKE_CXX_COMPILER_LAUNCHER ${CLCACHEWRAPPER})
        endif()
    else()
        set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_EXE})
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_EXE})
    endif()
endif()

# Replace some compiler flags when building using MSVC.

if(BUILDING_USING_MSVC)
    # Replace the /Zi option (i.e. enable debugging information), if present, with the /Z7 option (i.e. enable old-style
    # debugging information) otherwise caching (be it with buildcache or clcache) won't work.

    if(BUILDCACHE_EXE OR CLCACHE_EXE)
        replace_compiler_flag("/Zi" "/Z7")
    endif()
endif()

# Build our different third-party libraries.

add_subdirectory(3rdparty)

if(ONLY_BUILD_THIRD_PARTY_LIBRARIES)
    add_executable(${CMAKE_PROJECT_NAME} dummy.cpp)

    add_subdirectory(bindings)

    return()
endif()

# Configure the version file.

foreach(VERSION_PART PROJECT_VERSION_MAJOR PROJECT_VERSION_MINOR PROJECT_VERSION_PATCH)
    if(${${VERSION_PART}} LESS 10)
        set(${VERSION_PART}_PAD 0)
    endif()
endforeach()

set(LIBOPENCOR_VERSION 0x${PROJECT_VERSION_MAJOR_PAD}${PROJECT_VERSION_MAJOR}${PROJECT_VERSION_MINOR_PAD}${PROJECT_VERSION_MINOR}${PROJECT_VERSION_PATCH_PAD}${PROJECT_VERSION_PATCH})
set(LIBOPENCOR_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

set(VERSION_HEADER_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in)
set(VERSION_HEADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/version.h)

configure_file(${VERSION_HEADER_FILE_IN} ${VERSION_HEADER_FILE})

# Build libOpenCOR.

if(NOT WIN32 AND NOT APPLE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

set(GIT_API_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/file.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/issue.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/logger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/sedabstracttask.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/sedbase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/seddatadescription.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/seddatagenerator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/seddocument.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/sedinstance.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/sedinstancetask.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/sedmodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/sedonestep.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/sedoutput.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/sedrepeatedtask.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/sedsimulation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/sedsteadystate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/sedstyle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/sedtask.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/seduniformtimecourse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/solver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/solvercvode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/solverforwardeuler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/solverfourthorderrungekutta.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/solverheun.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/solverkinsol.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/solvernla.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/solverode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/solverodefixedstep.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/solversecondorderrungekutta.h
    ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/version.h
)

set(API_EXPORT_HEADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/api/${CMAKE_PROJECT_NAME_LC}/export.h)

set(API_HEADER_FILES
    ${GIT_API_HEADER_FILES}
    ${API_EXPORT_HEADER_FILE}
)

set(GIT_API_MODULE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/module/${CMAKE_PROJECT_NAME_LC})
set(API_MODULE_FILE ${GIT_API_MODULE_FILE})

set(INTERNAL_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/file/filemanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solvercvode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solverforwardeuler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solverfourthorderrungekutta.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solverheun.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solverkinsol.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solvernla.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solverode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solverodefixedstep.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solversecondorderrungekutta.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/support/cellml/cellmlfile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/support/cellml/cellmlfileruntime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/support/combine/combinearchive.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/support/sedml/sedmlfile.cpp
)

if(NOT EMSCRIPTEN)
    list(APPEND INTERNAL_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/compiler.cpp
    )
endif()

set(GIT_API_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/file/file.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/issue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedabstracttask.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedbase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/seddatadescription.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/seddatagenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/seddocument.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedinstance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedinstancetask.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedonestep.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedoutput.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedrepeatedtask.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedsimulation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedsteadystate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedstyle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedtask.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/seduniformtimecourse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/version.cpp
)

set(GIT_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/file/file_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/issue_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/logger/logger_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedabstracttask_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedbase_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/seddatadescription_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/seddatagenerator_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/seddocument_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedinstance_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedinstancetask_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedmodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedonestep_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedoutput_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedrepeatedtask_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedsimulation_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedsteadystate_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedstyle_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/sedtask_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sed/seduniformtimecourse_p.h
    ${GIT_API_SOURCE_FILES}
    ${INTERNAL_SOURCE_FILES}
)

set(SOURCE_FILES
    ${GIT_API_SOURCE_FILES}
    ${INTERNAL_SOURCE_FILES}
)

set(INTERNAL_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/file/filemanager_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/file/filemanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solver_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solvercvode_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solverforwardeuler_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solverfourthorderrungekutta_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solverheun_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solverkinsol_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solvernla_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solverode_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solverodefixedstep_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/solver/solversecondorderrungekutta_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/support/cellml/cellmlfile_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/support/cellml/cellmlfile.h
    ${CMAKE_CURRENT_SOURCE_DIR}/support/cellml/cellmlfileruntime_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/support/cellml/cellmlfileruntime.h
    ${CMAKE_CURRENT_SOURCE_DIR}/support/combine/combinearchive_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/support/combine/combinearchive.h
    ${CMAKE_CURRENT_SOURCE_DIR}/support/sedml/sedmlfile_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/support/sedml/sedmlfile.h
    ${CMAKE_CURRENT_SOURCE_DIR}/unittestingexport.h
)

if(NOT EMSCRIPTEN)
    list(APPEND INTERNAL_HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/compiler_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/compiler.h
    )
endif()

set(GIT_HEADER_FILES
    ${INTERNAL_HEADER_FILES}
    ${THIRD_PARTY_HEADER_FILES}
    ${VERSION_HEADER_FILE_IN}
)

set(HEADER_FILES
    ${INTERNAL_HEADER_FILES}
    ${VERSION_HEADER_FILE}
)

include_directories(file
                    logger
                    misc
                    solver
                    support/cellml
                    support/combine
                    support/sedml
                    ${CMAKE_CURRENT_BINARY_DIR})

set(JAVASCRIPT_SOURCE_DIR tests/bindings/javascript)

if(EMSCRIPTEN)
    set(JAVASCRIPT_BINDINGS_DIR ${CMAKE_BINARY_DIR}/../../../../${JAVASCRIPT_SOURCE_DIR})

    get_filename_component(JAVASCRIPT_BINDINGS_DIR "${JAVASCRIPT_BINDINGS_DIR}" REALPATH)
else()
    set(JAVASCRIPT_BINDINGS_DIR ${CMAKE_BINARY_DIR}/${JAVASCRIPT_SOURCE_DIR})
endif()

if(EMSCRIPTEN)
    add_executable(${CMAKE_PROJECT_NAME}
                   ${API_HEADER_FILES}
                   ${SOURCE_FILES}
                   ${HEADER_FILES})

    # Set the flags for our generated .js and .wasm files.
    # Note #1: we use -O3 to get the best performance, -sALLOW_MEMORY_GROWTH to allow the memory to grow dynamically,
    #          -sEXPORT_ES6=1 to export ES6 classes, -sEXPORTED_FUNCTIONS to export the functions we need,
    #          -sINLINING_LIMIT to increase the inlining limit, -sSTACK_SIZE to increase the stack size, and --bind to
    #          generate the necessary bindings.
    # Note #2: by default, Emscripten uses 64KB of stack, but this is not enough for "complex" models such as the Zhang
    #          et al. model (see https://github.com/agarny/COR/blob/master/Models/zhang_SAN_model_2000_all.cellml). For
    #          that model, we need about 108KB of stack. However, Emscripten originally used to use 5MB of stack (see
    #          https://github.com/emscripten-core/emscripten/pull/18191/files). So, here, we are using 4MB of stack,
    #          which should be more than enough for any model.

    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
                          COMPILE_FLAGS "-O3"
                          LINK_FLAGS "-O3 -sALLOW_MEMORY_GROWTH -sEXPORT_ES6=1 -sEXPORTED_FUNCTIONS=[_malloc,_free] -sINLINING_LIMIT -sSTACK_SIZE=4MB --bind")

    # Create a ZIP file for our generated .js and .wasm files.

    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E tar "-cfv" "${CMAKE_BINARY_DIR}/../../../../${REAL_CMAKE_PROJECT_NAME}-${PROJECT_VERSION_MAJOR}-${PROJECT_VERSION_MINOR}-${PROJECT_VERSION_PATCH}-WebAssembly.zip" --format=zip
                                                       "${CMAKE_PROJECT_NAME}.js"
                                                       "${CMAKE_PROJECT_NAME}.wasm")

    # Copy our generated .js and .wasm files over.

    foreach(FILE_EXTENSION js wasm)
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.${FILE_EXTENSION}
                                                            ${JAVASCRIPT_BINDINGS_DIR}/${CMAKE_PROJECT_NAME}.${FILE_EXTENSION})
    endforeach()

    # Let people know how to run our test web page.

    if(LIBOPENCOR_JAVASCRIPT_UNIT_TESTING)
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E echo "-- JavaScript test web page has been copied to: ${JAVASCRIPT_BINDINGS_DIR}/index.html"
                           COMMAND ${CMAKE_COMMAND} -E echo "                          and can be run using: emrun --browser chrome ${JAVASCRIPT_BINDINGS_DIR}/index.html")
    endif()
else()
    add_library(${CMAKE_PROJECT_NAME}
                ${API_HEADER_FILES}
                ${SOURCE_FILES}
                ${HEADER_FILES})

    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
                          CXX_VISIBILITY_PRESET hidden
                          DEBUG_POSTFIX d
                          PREFIX ""
                          VERSION ${PROJECT_VERSION}
                          VISIBILITY_INLINES_HIDDEN ON)
endif()

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/api>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>)
target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api/${CMAKE_PROJECT_NAME_LC}/module>
                           $<INSTALL_INTERFACE:include/>
                           $<INSTALL_INTERFACE:include/${CMAKE_PROJECT_NAME_LC}/module>)

# Configure libOpenCOR.

configure_target(${CMAKE_PROJECT_NAME})

# Generate the export header file, or don't use any if we are building our JavaScript bindings.

if(EMSCRIPTEN)
    configure_file(noexports.h ${API_EXPORT_HEADER_FILE} COPYONLY)
else()
    include(GenerateExportHeader)

    generate_export_header(${CMAKE_PROJECT_NAME}
                           EXPORT_FILE_NAME ${API_EXPORT_HEADER_FILE}
                           BASE_NAME LIBOPENCOR)
endif()

# Copy over our export header file for unit testing.

configure_file(unittestingexport.h unittestingexport.h COPYONLY)

# Don't allow unresolved symbols.
# Note: MSVC doesn't allow unresolved symbols, so we only need to look for them if we are building using GNU or Clang.

if(BUILDING_USING_GNU)
    SET(CMAKE_SHARED_LINKER_FLAGS -Wl,--no-undefined)
elseif(BUILDING_USING_CLANG)
    # On macOS, only use the -Wl,--no-undefined option if we are not using Xcode 15.0 or later since otherwise the
    # linker is going to generate some warnings about -undefined error being deprecated.

    if(APPLE)
        # Get the Xcode version.
        # Note: CMake has XCODE_VERSION, but it's only available when using the Xcode generator. So, we need to get the
        #       Xcode version ourselves. And, depending on how Xcode was installed, we may need to get the Xcode version
        #       using softwareupdate.

        execute_process(COMMAND xcodebuild -version
                        OUTPUT_VARIABLE XCODE_VERSION
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        RESULT_VARIABLE RESULT
                        ERROR_QUIET)

        if(NOT RESULT EQUAL 0)
            execute_process(COMMAND softwareupdate --history
                            COMMAND awk "/Command Line Tools for Xcode/ {print \$6}"
                            COMMAND tail -1
                            OUTPUT_VARIABLE XCODE_VERSION
                            OUTPUT_STRIP_TRAILING_WHITESPACE
                            ERROR_QUIET)
        endif()
    else()
        set(XCODE_VERSION 0)
    endif()

    if(XCODE_VERSION VERSION_LESS 15.0)
        SET(CMAKE_SHARED_LINKER_FLAGS -Wl,-undefined,error)
    endif()
endif()

# Get ready for code coverage.

if(LIBOPENCOR_CODE_COVERAGE)
    add_target_property(${CMAKE_PROJECT_NAME} COMPILE_FLAGS ${CODE_COVERAGE_COMPILER_FLAGS})
    add_target_property(${CMAKE_PROJECT_NAME} LINK_FLAGS ${CODE_COVERAGE_LINKER_FLAGS})
endif()

# Build our different bindings.

add_subdirectory(bindings)

list(APPEND GIT_SOURCE_FILES
    ${JAVASCRIPT_SOURCE_FILES}
    ${PYTHON_SOURCE_FILES}
)

# Install libOpenCOR itself.

if(APPLE)
    # On macOS, we need to specify the COMPONENT option otherwise our package will be empty.
    # Note: we don't want to specify the COMPONENT option on Windows since NSIS will otherwise make it a selectable
    #       option which we don't want.

    set(LIBOPENCOR_COMPONENT_OPTION COMPONENT Runtime)
endif()

install(TARGETS ${CMAKE_PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}Targets
        ${LIBOPENCOR_COMPONENT_OPTION}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(FILES ${API_HEADER_FILES}
        DESTINATION include/${CMAKE_PROJECT_NAME_LC})

install(FILES ${API_MODULE_FILE}
        DESTINATION include/${CMAKE_PROJECT_NAME_LC}/module)

# Install libOpenCOR's CMake files.

include(CMakePackageConfigHelpers)

if(WIN32)
    set(CMAKE_PACKAGE_DIR cmake/${PROJECT_NAME})
else()
    set(CMAKE_PACKAGE_DIR lib/cmake/${PROJECT_NAME})
endif()

install(EXPORT ${CMAKE_PROJECT_NAME}Targets
        FILE ${CMAKE_PROJECT_NAME}Targets.cmake
        DESTINATION ${CMAKE_PACKAGE_DIR})

set(CONFIG_CMAKE_IN_FILE ${PROJECT_SOURCE_DIR}/cmake/packaging/config.cmake.in)
set(CONFIG_CMAKE_FILE ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake)

configure_package_config_file(${CONFIG_CMAKE_IN_FILE} ${CONFIG_CMAKE_FILE}
                              INSTALL_DESTINATION ${CMAKE_PACKAGE_DIR})

set(CONFIG_VERSION_CMAKE_FILE ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake)

write_basic_package_version_file(${CONFIG_VERSION_CMAKE_FILE}
                                 COMPATIBILITY SameMajorVersion)

install(FILES ${CONFIG_CMAKE_FILE}
              ${CONFIG_VERSION_CMAKE_FILE}
        DESTINATION ${CMAKE_PACKAGE_DIR})

# Uninstall libOpenCOR.

set(UNINSTALL_CMAKE_FILE ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake)

configure_file(${PROJECT_SOURCE_DIR}/cmake/packaging/uninstall.cmake.in ${UNINSTALL_CMAKE_FILE} @ONLY)

add_target(uninstall
           COMMAND ${CMAKE_COMMAND} -P ${UNINSTALL_CMAKE_FILE}
           COMMENT "Uninstall the project...")

# Share some file information with the rest of our building system.

set(GIT_API_HEADER_FILES ${GIT_API_HEADER_FILES} PARENT_SCOPE)
set(GIT_API_MODULE_FILE ${GIT_API_MODULE_FILE} PARENT_SCOPE)
set(GIT_SOURCE_FILES ${GIT_SOURCE_FILES} PARENT_SCOPE)
set(GIT_HEADER_FILES ${GIT_HEADER_FILES} PARENT_SCOPE)
