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

#include "libopencor/simulation.h"

#include "file_p.h"
#include "simulation_p.h"
#include "simulationdata_p.h"
#include "simulationresults_p.h"

#include "libopencor/file.h"

namespace libOpenCOR {

Simulation::Impl::Impl(const FilePtr &pFile)
    : mFile(pFile)
{
    // Create our simulation data and results objects.

    mData = SimulationData::Impl::create();
    mResults = SimulationResults::Impl::create();

    // Make sure that the given file is supported.

    switch (mFile->type()) {
    case File::Type::UNKNOWN_FILE:
        addError("A simulation cannot be created using an unknown file.");

        break;
    case File::Type::CELLML_FILE:
        mCellmlFile = mFile->pimpl()->mCellmlFile;

        break;
    case File::Type::SEDML_FILE:
        addError("A simulation cannot currently be created using a SED-ML file.");

        break;
#ifdef __EMSCRIPTEN__
    default: // File::Type::COMBINE_ARCHIVE.
        addError("A simulation cannot currently be created using a COMBINE archive.");

        break;
#else
    case File::Type::COMBINE_ARCHIVE:
        addError("A simulation cannot currently be created using a COMBINE archive.");

        break;
    default: // File::Type::IRRETRIEVABLE_FILE.
        addError("A simulation cannot be created using an irretrievable file.");

        break;
#endif
    }
}

bool Simulation::Impl::supportedFile() const
{
    return mCellmlFile != nullptr;
}

void Simulation::Impl::run()
{
    // Make sure that our file is supported.

    if (!supportedFile()) {
        return;
    }

    // Remove all our issues in case we had some from a previous run.

    removeAllIssues();

    // Retrieve a runtime for the CellML file and propagate any issues it may have.

    auto runtime = mCellmlFile->runtime();

    if (runtime->hasIssues()) {
        addIssues(runtime);

        return;
    }

    // Initialise our worker, if we don't already have one and if our simulation data are valid.

//---GRY---
#ifndef COVERAGE_ENABLED
    if ((mWorker == nullptr) && mData->isValid()) {
#endif
        mWorker = SimulationWorker::create();
//---GRY---
#ifndef COVERAGE_ENABLED
    }
#endif
}

//---GRY---
#ifndef COVERAGE_ENABLED
void Simulation::Impl::pause()
{
}
#endif

//---GRY---
#ifndef COVERAGE_ENABLED
void Simulation::Impl::resume()
{
}
#endif

//---GRY---
#ifndef COVERAGE_ENABLED
void Simulation::Impl::stop()
{
}
#endif

Simulation::Simulation(const FilePtr &pFile)
    : Logger(new Impl(pFile))
{
}

Simulation::~Simulation()
{
    delete pimpl();
}

Simulation::Impl *Simulation::pimpl()
{
    return reinterpret_cast<Impl *>(Logger::pimpl());
}

/*---GRY---
const Simulation::Impl *Simulation::pimpl() const
{
    return reinterpret_cast<const Impl *>(Logger::pimpl());
}
*/

SimulationPtr Simulation::create(const FilePtr &pFile)
{
    return std::shared_ptr<Simulation> {new Simulation {pFile}};
}

void Simulation::run()
{
    pimpl()->run();
}

//---GRY---
#ifndef COVERAGE_ENABLED
void Simulation::pause()
{
    pimpl()->pause();
}
#endif

//---GRY---
#ifndef COVERAGE_ENABLED
void Simulation::resume()
{
    pimpl()->resume();
}
#endif

//---GRY---
#ifndef COVERAGE_ENABLED
void Simulation::stop()
{
    pimpl()->stop();
}
#endif

} // namespace libOpenCOR
