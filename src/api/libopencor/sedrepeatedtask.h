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

#pragma once

#include "libopencor/sedabstracttask.h"

namespace libOpenCOR {

/**
 * @brief The SedRepeatedTask class.
 *
 * The SedRepeatedTask class is used to describe a repeated task in the context of a simulation experiment description.
 */

class LIBOPENCOR_EXPORT SedRepeatedTask: public SedAbstractTask
{
public:
    /**
     * Constructors, destructor, and assignment operators.
     */

    ~SedRepeatedTask() override; /**< Destructor, @private. */

    SedRepeatedTask(const SedRepeatedTask &pOther) = delete; /**< No copy constructor allowed, @private. */
    SedRepeatedTask(SedRepeatedTask &&pOther) noexcept = delete; /**< No move constructor allowed, @private. */

    SedRepeatedTask &operator=(const SedRepeatedTask &pRhs) = delete; /**< No copy assignment operator allowed, @private. */
    SedRepeatedTask &operator=(SedRepeatedTask &&pRhs) noexcept = delete; /**< No move assignment operator allowed, @private. */

    /**
     * @brief Create a @ref SedRepeatedTask object.
     *
     * Factory method to create a @ref SedRepeatedTask object:
     *
     * ```
     * auto document = libOpenCOR::SedDocument::create();
     * auto task = libOpenCOR::SedRepeatedTask::create(document);
     * ```
     *
     * @param pDocument The @ref SedDocument object to which the @ref SedRepeatedTask object is to belong.
     *
     * @return A smart pointer to a @ref SedRepeatedTask object.
     */

    static SedRepeatedTaskPtr create(const SedDocumentPtr &pDocument);

private:
    class Impl; /**< Forward declaration of the implementation class, @private. */

    explicit SedRepeatedTask(const SedDocumentPtr &pDocument); /**< Constructor @private. */

    Impl *pimpl(); /**< Private implementation pointer, @private. */
    const Impl *pimpl() const; /**< Constant private implementation pointer, @private. */
};

} // namespace libOpenCOR
