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

if(BUILDING_JAVASCRIPT_BINDINGS)
    # Add some sources to our WASM version of libOpenCOR so we can access its internals.

    target_sources(${CMAKE_PROJECT_NAME} PRIVATE
                   ${CMAKE_CURRENT_SOURCE_DIR}/version.cpp)
else()
    # Build our JavaScript bindings.

    set(JAVASCRIPT_BINDINGS_TARGET ${CMAKE_PROJECT_NAME}_JavaScript CACHE INTERNAL "JavaScript bindings target.")

    add_custom_target(${JAVASCRIPT_BINDINGS_TARGET}
                      COMMAND ${EMCMAKE_EXE} ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" -S ${CMAKE_SOURCE_DIR} -B build
                      COMMAND ${CMAKE_COMMAND} --build build
                      WORKING_DIRECTORY ${DOCUMENTATION_BUILD_DIR})

    add_dependencies(${CMAKE_PROJECT_NAME} ${JAVASCRIPT_BINDINGS_TARGET})
endif()