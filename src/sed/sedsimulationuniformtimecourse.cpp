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

#include "seddocument_p.h"
#include "sedsimulationuniformtimecourse_p.h"
#include "utils.h"

namespace libOpenCOR {

static constexpr auto ID_PREFIX = "simulation";

SedSimulationUniformTimeCourse::Impl::Impl(double pInitialTime, double pOutputStartTime, double pOutputEndTime,
                                           int pNumberOfSteps, const SedDocumentPtr &pDocument)
    : mInitialTime(pInitialTime)
    , mOutputStartTime(pOutputStartTime)
    , mOutputEndTime(pOutputEndTime)
    , mNumberOfSteps(pNumberOfSteps)
{
    mId = pDocument->pimpl()->uniqueId(ID_PREFIX); //---GRY--- THIS SHOULD PROBABLY BE DONE ONLY WHEN SERIALISING...?
}

void SedSimulationUniformTimeCourse::Impl::serialise(xmlNodePtr pNode) const
{
    // Serialise the uniform time course simulation.

    auto *node = xmlNewNode(nullptr, constXmlCharPtr("uniformTimeCourse"));

    xmlNewProp(node, constXmlCharPtr("id"), constXmlCharPtr(mId));
    xmlNewProp(node, constXmlCharPtr("initialTime"), constXmlCharPtr(toString(mInitialTime)));
    xmlNewProp(node, constXmlCharPtr("outputStartTime"), constXmlCharPtr(toString(mOutputStartTime)));
    xmlNewProp(node, constXmlCharPtr("outputEndTime"), constXmlCharPtr(toString(mOutputEndTime)));
    xmlNewProp(node, constXmlCharPtr("numberOfSteps"), constXmlCharPtr(toString(mNumberOfSteps)));

    xmlAddChild(pNode, node);

    // Serialise the rest of the simulation.

    SedSimulation::Impl::serialise(node);
}

SedSimulationUniformTimeCourse::SedSimulationUniformTimeCourse(double pInitialTime, double pOutputStartTime,
                                                               double pOutputEndTime, int pNumberOfSteps,
                                                               const SedDocumentPtr &pDocument)
    : SedSimulation(new Impl(pInitialTime, pOutputStartTime, pOutputEndTime, pNumberOfSteps, pDocument))
{
}

SedSimulationUniformTimeCourse::~SedSimulationUniformTimeCourse()
{
    delete pimpl();
}

SedSimulationUniformTimeCourse::Impl *SedSimulationUniformTimeCourse::pimpl()
{
    return reinterpret_cast<SedSimulationUniformTimeCourse::Impl *>(SedSimulation::pimpl());
}

/*---GRY---
const SedSimulationUniformTimeCourse::Impl *SedSimulationUniformTimeCourse::pimpl() const
{
    return reinterpret_cast<const Impl *>(SedSimulation::pimpl());
}
*/

} // namespace libOpenCOR
