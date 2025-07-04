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

#include "file_p.h"
#include "seddocument_p.h"
#include "sedinstance_p.h"
#include "sedmodel_p.h"
#include "sedsimulation_p.h"
#include "sedtask_p.h"

#include "utils.h"

#include <algorithm>
#include <sstream>

namespace libOpenCOR {

std::string SedDocument::Impl::uniqueId(const std::string &pPrefix)
{
    size_t counter = 0;
    std::stringstream stream;

    stream << pPrefix << ++counter;

    auto res = stream.str();

    while (mIds.contains(res)) {
        stream.str({});

        stream << pPrefix << ++counter;

        res = stream.str();
    }

    mIds.insert(res);

    return res;
}

void SedDocument::Impl::initialise(const SedDocumentPtr &pOwner, const FilePtr &pFile)
{
    // Make sure that we were given a file.

    if (pFile == nullptr) {
        return;
    }

    // Make sure that the given file is supported.
    // Note: we would normally have a switch statement, but then code coverage doesn't cover the lines where we check
    //       for __EMSCRIPTEN__. Could it be an issue with llvm-cov not properly handling C++ directives within a switch
    //       statement?

    auto fileType = pFile->type();

    if (fileType == File::Type::CELLML_FILE) {
        initialiseFromCellmlFile(pOwner, pFile);
    } else if (fileType == File::Type::SEDML_FILE) {
        initialiseFromSedmlFile(pOwner, pFile);
    } else if (fileType == File::Type::COMBINE_ARCHIVE) {
        initialiseFromCombineArchive(pOwner, pFile);
#ifndef __EMSCRIPTEN__
    } else if (fileType == File::Type::IRRETRIEVABLE_FILE) {
        addError("A simulation experiment description cannot be created using an irretrievable file.");
#endif
    } else { // File::Type::UNKNOWN_FILE.
        addError("A simulation experiment description cannot be created using an unknown file.");
    }
}

void SedDocument::Impl::initialiseFromCellmlFile(const SedDocumentPtr &pOwner, const FilePtr &pFile)
{
    pFile->pimpl()->mCellmlFile->populateDocument(pOwner);
}

void SedDocument::Impl::initialiseFromSedmlFile(const SedDocumentPtr &pOwner, const FilePtr &pFile)
{
    auto sedmlFile = pFile->pimpl()->mSedmlFile;

    sedmlFile->populateDocument(pOwner);

    addIssues(sedmlFile);
}

void SedDocument::Impl::initialiseFromCombineArchive(const SedDocumentPtr &pOwner, const FilePtr &pFile)
{
    auto masterFile = pFile->pimpl()->mCombineArchive->masterFile();

    if (masterFile == nullptr) {
        addError("A simulation experiment description cannot be created using a COMBINE archive with no master file.");
    } else {
        auto masterFileType = masterFile->type();

        if (masterFileType == File::Type::CELLML_FILE) {
            initialiseFromCellmlFile(pOwner, masterFile);
        } else if (masterFileType == File::Type::SEDML_FILE) {
            initialiseFromSedmlFile(pOwner, masterFile);
        } else {
            addError("A simulation experiment description cannot be created using a COMBINE archive with an unknown master file (only CellML and SED-ML master files are supported).");
        }
    }
}

void SedDocument::Impl::serialise(xmlNodePtr pNode) const
{
    xmlNewProp(pNode, toConstXmlCharPtr("xmlns"), toConstXmlCharPtr(mXmlns));
    xmlNewProp(pNode, toConstXmlCharPtr("level"), toConstXmlCharPtr(std::to_string(mLevel)));
    xmlNewProp(pNode, toConstXmlCharPtr("version"), toConstXmlCharPtr(std::to_string(mVersion)));
}

std::string SedDocument::Impl::serialise(const std::string &pBasePath) const
{
    // Serialise our SED-ML document.

    auto *doc = xmlNewDoc(toConstXmlCharPtr("1.0"));
    auto *node = xmlNewNode(nullptr, toConstXmlCharPtr("sedML"));

    serialise(node);

    xmlDocSetRootElement(doc, node);

    // Add the data descriptions, if any, to our SED-ML document.

    /*---GRY---
    if (!mDataDescriptions.empty()) {
        auto *sedListOfDataDescriptions = xmlNewNode(nullptr, toConstXmlCharPtr("listOfDataDescriptions"));

        xmlAddChild(node, sedListOfDataDescriptions);

        for (const auto &dataDescription : mDataDescriptions) {
            (void)dataDescription;
        }
    }
    */

    // Add the models, if any, to our SED-ML document.

    if (!mModels.empty()) {
        auto *sedListOfModels = xmlNewNode(nullptr, toConstXmlCharPtr("listOfModels"));

        xmlAddChild(node, sedListOfModels);

        for (const auto &model : mModels) {
            model->pimpl()->setBasePath(pBasePath);
            model->pimpl()->serialise(sedListOfModels);
        }
    }

    // Add the simulations, if any, to our SED-ML document.

    if (!mSimulations.empty()) {
        auto *sedListOfSimulations = xmlNewNode(nullptr, toConstXmlCharPtr("listOfSimulations"));

        for (const auto &simulation : mSimulations) {
            simulation->pimpl()->serialise(sedListOfSimulations);
        }

        xmlAddChild(node, sedListOfSimulations);
    }

    // Add the tasks, if any, to our SED-ML document.

    if (!mTasks.empty()) {
        auto *sedListOfTasks = xmlNewNode(nullptr, toConstXmlCharPtr("listOfTasks"));

        for (const auto &task : mTasks) {
            task->pimpl()->serialise(sedListOfTasks);
        }

        xmlAddChild(node, sedListOfTasks);
    }

    // Add the data generators, if any, to our SED-ML document.

    /*---GRY---
    if (!mDataGenerators.empty()) {
        auto *sedListOfDataGenerators = xmlNewNode(nullptr, toConstXmlCharPtr("listOfDataGenerators"));

        for (const auto &dataGenerator : mDataGenerators) {
            dataGenerator->pimpl()->serialise(sedListOfDataGenerators);
        }

        xmlAddChild(node, sedListOfDataGenerators);
    }
    */

    // Add the outputs, if any, to our SED-ML document.

    /*---GRY---
    if (!mOutputs.empty()) {
        auto *sedListOfOutputs = xmlNewNode(nullptr, toConstXmlCharPtr("listOfOutputs"));

        for (const auto &output : mOutputs) {
            output->pimpl()->serialise(sedListOfOutputs);
        }

        xmlAddChild(node, sedListOfOutputs);
    }
    */

    // Add the styles, if any, to our SED-ML document.

    /*---GRY---
    if (!mStyles.empty()) {
        auto *sedListOfStyles = xmlNewNode(nullptr, toConstXmlCharPtr("listOfStyles"));

        for (const auto &style : mStyles) {
            style->pimpl()->serialise(sedListOfStyles);
        }

        xmlAddChild(node, sedListOfStyles);
    }
    */

    // Convert our SED-ML document to a string and return it.

    xmlChar *buffer = nullptr;
    int size = 0;

    xmlDocDumpFormatMemoryEnc(doc, &buffer, &size, "UTF-8", 1);

    std::stringstream res;

    res << buffer;

    xmlFree(buffer);
    xmlFreeDoc(doc);

    return res.str();
}

bool SedDocument::Impl::hasModels() const
{
    return !mModels.empty();
}

size_t SedDocument::Impl::modelCount() const
{
    return mModels.size();
}

SedModelPtrs SedDocument::Impl::models() const
{
    return mModels;
}

SedModelPtr SedDocument::Impl::model(size_t pIndex) const
{
    if (pIndex >= mModels.size()) {
        return {};
    }

    return mModels[pIndex];
}

bool SedDocument::Impl::addModel(const SedModelPtr &pModel)
{
    if (pModel == nullptr) {
        return false;
    }

    auto model = std::ranges::find_if(mModels, [&](const auto &s) {
        return s == pModel;
    });

    if (model != mModels.end()) {
        return false;
    }

    mModels.push_back(pModel);

    return true;
}

bool SedDocument::Impl::removeModel(const SedModelPtr &pModel)
{
    auto model = std::ranges::find_if(mModels, [&](const auto &s) {
        return s == pModel;
    });

    if (model != mModels.end()) {
        mIds.erase((*model)->pimpl()->mId);
        mModels.erase(model);

        return true;
    }

    return false;
}

bool SedDocument::Impl::removeAllModels()
{
    if (!hasModels()) {
        return false;
    }

    while (!mModels.empty()) {
        removeModel(mModels.front());
    }

    return true;
}

bool SedDocument::Impl::hasSimulations() const
{
    return !mSimulations.empty();
}

size_t SedDocument::Impl::simulationCount() const
{
    return mSimulations.size();
}

SedSimulationPtrs SedDocument::Impl::simulations() const
{
    return mSimulations;
}

SedSimulationPtr SedDocument::Impl::simulation(size_t pIndex) const
{
    if (pIndex >= mSimulations.size()) {
        return {};
    }

    return mSimulations[pIndex];
}

bool SedDocument::Impl::addSimulation(const SedSimulationPtr &pSimulation)
{
    if (pSimulation == nullptr) {
        return false;
    }

    auto simulation = std::ranges::find_if(mSimulations, [&](const auto &s) {
        return s == pSimulation;
    });

    if (simulation != mSimulations.end()) {
        return false;
    }

    mSimulations.push_back(pSimulation);

    return true;
}

bool SedDocument::Impl::removeSimulation(const SedSimulationPtr &pSimulation)
{
    auto simulation = std::ranges::find_if(mSimulations, [&](const auto &s) {
        return s == pSimulation;
    });

    if (simulation != mSimulations.end()) {
        mIds.erase((*simulation)->pimpl()->mId);
        mSimulations.erase(simulation);

        return true;
    }

    return false;
}

bool SedDocument::Impl::removeAllSimulations()
{
    if (!hasSimulations()) {
        return false;
    }

    while (!mSimulations.empty()) {
        removeSimulation(mSimulations.front());
    }

    return true;
}

bool SedDocument::Impl::hasTasks() const
{
    return !mTasks.empty();
}

size_t SedDocument::Impl::taskCount() const
{
    return mTasks.size();
}

SedAbstractTaskPtrs SedDocument::Impl::tasks() const
{
    return mTasks;
}

SedAbstractTaskPtr SedDocument::Impl::task(size_t pIndex) const
{
    if (pIndex >= mTasks.size()) {
        return {};
    }

    return mTasks[pIndex];
}

bool SedDocument::Impl::addTask(const SedAbstractTaskPtr &pTask)
{
    if (pTask == nullptr) {
        return false;
    }

    auto task = std::ranges::find_if(mTasks, [&](const auto &t) {
        return t == pTask;
    });

    if (task != mTasks.end()) {
        return false;
    }

    mTasks.push_back(pTask);

    return true;
}

bool SedDocument::Impl::removeTask(const SedAbstractTaskPtr &pTask)
{
    auto task = std::ranges::find_if(mTasks, [&](const auto &t) {
        return t == pTask;
    });

    if (task != mTasks.end()) {
        mIds.erase((*task)->pimpl()->mId);
        mTasks.erase(task);

        return true;
    }

    return false;
}

bool SedDocument::Impl::removeAllTasks()
{
    if (!hasTasks()) {
        return false;
    }

    while (!mTasks.empty()) {
        removeTask(mTasks.front());
    }

    return true;
}

SedDocument::SedDocument()
    : Logger(new Impl {})
{
}

SedDocument::~SedDocument()
{
    delete pimpl();
}

SedDocument::Impl *SedDocument::pimpl()
{
    return static_cast<Impl *>(Logger::mPimpl);
}

const SedDocument::Impl *SedDocument::pimpl() const
{
    return static_cast<const Impl *>(Logger::mPimpl);
}

SedDocumentPtr SedDocument::create(const FilePtr &pFile)
{
    auto res = SedDocumentPtr {new SedDocument {}};

    res->pimpl()->initialise(res, pFile);

    return res;
}

#ifdef __EMSCRIPTEN__
SedDocumentPtr SedDocument::defaultCreate()
{
    return create({});
}
#endif

std::string SedDocument::serialise() const
{
    return pimpl()->serialise();
}

std::string SedDocument::serialise(const std::string &pBasePath) const
{
    return pimpl()->serialise(pBasePath);
}

bool SedDocument::hasModels() const
{
    return pimpl()->hasModels();
}

size_t SedDocument::modelCount() const
{
    return pimpl()->modelCount();
}

SedModelPtrs SedDocument::models() const
{
    return pimpl()->models();
}

SedModelPtr SedDocument::model(size_t pIndex) const
{
    return pimpl()->model(pIndex);
}

bool SedDocument::addModel(const SedModelPtr &pModel)
{
    return pimpl()->addModel(pModel);
}

bool SedDocument::removeModel(const SedModelPtr &pModel)
{
    return pimpl()->removeModel(pModel);
}

bool SedDocument::removeAllModels()
{
    return pimpl()->removeAllModels();
}

bool SedDocument::hasSimulations() const
{
    return pimpl()->hasSimulations();
}

size_t SedDocument::simulationCount() const
{
    return pimpl()->simulationCount();
}

SedSimulationPtrs SedDocument::simulations() const
{
    return pimpl()->simulations();
}

SedSimulationPtr SedDocument::simulation(size_t pIndex) const
{
    return pimpl()->simulation(pIndex);
}

bool SedDocument::addSimulation(const SedSimulationPtr &pSimulation)
{
    return pimpl()->addSimulation(pSimulation);
}

bool SedDocument::removeSimulation(const SedSimulationPtr &pSimulation)
{
    return pimpl()->removeSimulation(pSimulation);
}

bool SedDocument::removeAllSimulations()
{
    return pimpl()->removeAllSimulations();
}

bool SedDocument::hasTasks() const
{
    return pimpl()->hasTasks();
}

size_t SedDocument::taskCount() const
{
    return pimpl()->taskCount();
}

SedAbstractTaskPtrs SedDocument::tasks() const
{
    return pimpl()->tasks();
}

SedAbstractTaskPtr SedDocument::task(size_t pIndex) const
{
    return pimpl()->task(pIndex);
}

bool SedDocument::addTask(const SedAbstractTaskPtr &pTask)
{
    return pimpl()->addTask(pTask);
}

bool SedDocument::removeTask(const SedAbstractTaskPtr &pTask)
{
    return pimpl()->removeTask(pTask);
}

bool SedDocument::removeAllTasks()
{
    return pimpl()->removeAllTasks();
}

#ifdef __EMSCRIPTEN__
SedInstancePtr SedDocument::instantiate()
{
    return SedInstance::Impl::create(shared_from_this(), false);
}
#else
SedInstancePtr SedDocument::instantiate(bool pCompiled)
{
    return SedInstance::Impl::create(shared_from_this(), pCompiled);
}
#endif

} // namespace libOpenCOR
