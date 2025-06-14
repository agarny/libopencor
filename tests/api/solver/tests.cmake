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

set(TEST solver)

list(APPEND TESTS ${TEST})

set(${TEST}_CATEGORY api)
set(${TEST}_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/basictests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/coveragetests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/cvodetests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/forwardeulertests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/fourthorderrungekuttatests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/heuntests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/kinsoltests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/odemodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/secondorderrungekuttatests.cpp
)
set(${TEST}_HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/odemodel.h
)
