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

#include "libopencor/sedchange.h"

namespace libOpenCOR {

/**
 * @brief The SedChangeAttribute class.
 *
 * The SedChangeAttribute class is used to describe a change attribute in the context of a simulation experiment
 * description.
 */

class LIBOPENCOR_EXPORT SedChangeAttribute: public SedChange
{
    friend class SedInstanceTask;

public:
    /**
     * Constructors, destructor, and assignment operators.
     */

    ~SedChangeAttribute() override; /**< Destructor, @private. */

    SedChangeAttribute(const SedChangeAttribute &pOther) = delete; /**< No copy constructor allowed, @private. */
    SedChangeAttribute(SedChangeAttribute &&pOther) noexcept = delete; /**< No move constructor allowed, @private. */

    SedChangeAttribute &operator=(const SedChangeAttribute &pRhs) = delete; /**< No copy assignment operator allowed, @private. */
    SedChangeAttribute &operator=(SedChangeAttribute &&pRhs) noexcept = delete; /**< No move assignment operator allowed, @private. */

    /**
     * @brief Create a @ref SedChangeAttribute object.
     *
     * Factory method to create a @ref SedChangeAttribute object:
     *
     * ```
     * auto simulation = libOpenCOR::SedChangeAttribute::create(component, variable, newValue);
     * ```
     *
     * @param pComponentName The name of the component, as a @c std::string, where the target is located.
     * @param pVariableName The name of the variable, as a @c std::string, corresponding to the target.
     * @param pNewValue The new value, as a @c std::string, for the target.
     *
     * @return A smart pointer to a @ref SedChangeAttribute object.
     */

    static SedChangeAttributePtr create(const std::string &pComponentName, const std::string &pVariableName,
                                        const std::string &pNewValue);

    /**
     * @brief Get the name of the component.
     *
     * Get the name of the component.
     *
     * @return The name of the component as a @c std::string.
     */

    std::string componentName() const;

    /**
     * @brief Set the name of the component.
     *
     * Set the name of the component.
     *
     * @param pComponentName The name of the component as a @c std::string.
     */

    void setComponentName(const std::string &pComponentName);

    /**
     * @brief Get the name of the variable.
     *
     * Get the name of the variable.
     *
     * @return The name of the variable as a @c std::string.
     */

    std::string variableName() const;

    /**
     * @brief Set the name of the variable.
     *
     * Set the name of the variable.
     *
     * @param pVariableName The name of the variable as a @c std::string.
     */

    void setVariableName(const std::string &pVariableName);

    /**
     * @brief Get the new value.
     *
     * Get the new value.
     *
     * @return The new value as a @c std::string.
     */

    std::string newValue() const;

    /**
     * @brief Set the new value.
     *
     * Set the new value.
     *
     * @param pNewValue The new value as a @c std::string.
     */

    void setNewValue(const std::string &pNewValue);

private:
    class Impl; /**< Forward declaration of the implementation class, @private. */

    explicit SedChangeAttribute(const std::string &pComponentName, const std::string &pVariableName,
                                const std::string &pNewValue); /**< Constructor @private. */

    Impl *pimpl(); /**< Private implementation pointer, @private. */
    const Impl *pimpl() const; /**< Constant private implementation pointer, @private. */
};

} // namespace libOpenCOR
