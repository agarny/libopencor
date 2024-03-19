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

#include "sedjob_p.h"

namespace libOpenCOR {

SedJobPtr SedJob::Impl::create()
{
    return SedJobPtr {new SedJob()};
}

SedJob::SedJob()
    : Logger(new Impl())
{
}

SedJob::~SedJob()
{
    delete pimpl();
}

SedJob::Impl *SedJob::pimpl()
{
    return reinterpret_cast<Impl *>(Logger::pimpl());
}

/*---GRY---
const SedJob::Impl *SedJob::pimpl() const
{
    return reinterpret_cast<const Impl *>(Logger::pimpl());
}
*/

} // namespace libOpenCOR
