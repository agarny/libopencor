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

#include <libopencor>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <format>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#ifndef _WIN32
#    include <sys/resource.h>
#endif

namespace {

constexpr int ONE_MILLION_STEPS {1000000};
constexpr int ONE_HUNDRED_THOUSAND_STEPS {100000};
constexpr int THIRTY_THOUSAND_STEPS {30000};
constexpr double NANOSECONDS_PER_MILLISECOND {1.0e6};
constexpr size_t BYTES_PER_KILOBYTE {1024};

size_t peakMemoryUsage()
{
#ifdef _WIN32
    // Not currently supported on Windows.

    return 0;
#else
    rusage usage {};

    getrusage(RUSAGE_SELF, &usage);

    const auto maxRss {static_cast<size_t>(usage.ru_maxrss)}; // NOLINT
    // Note: ru_maxrss is in bytes on macOS and in kilobytes on Linux.

#    ifdef __APPLE__
    return maxRss;
#    else
    return BYTES_PER_KILOBYTE * maxRss;
#    endif
#endif
}

bool runBenchmark(const std::string &pName, const std::string &pResourcePath, const libOpenCOR::SolverOdePtr &pOdeSolver, int pNumberOfSteps)
{
    // Create our document, using the given model, and customise its simulation, if needed.

    auto file {libOpenCOR::File::create(std::string(BENCHMARK_RESOURCE_LOCATION) + "/" + pResourcePath)};
    auto document {libOpenCOR::SedDocument::create(file)};
    const auto &simulation {std::dynamic_pointer_cast<libOpenCOR::SedUniformTimeCourse>(document->simulations()[0])};

    if (pOdeSolver != nullptr) {
        simulation->setOdeSolver(pOdeSolver);
    }

    if (pNumberOfSteps > 0) {
        simulation->setOutputEndTime(simulation->outputEndTime() * static_cast<double>(pNumberOfSteps) / static_cast<double>(simulation->numberOfSteps()));
        simulation->setNumberOfSteps(pNumberOfSteps);
    }

    // Instantiate our document and time how long it takes (this includes JIT compiling the model).

    const auto instantiateStart {std::chrono::steady_clock::now()};
    auto instance {document->instantiate()};
    const auto instantiateMs {std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - instantiateStart).count()};

    if (instance->hasIssues()) {
        std::cerr << "ERROR: the " << pName << " benchmark could not be instantiated:\n";

        for (size_t i {0}; i < instance->issueCount(); ++i) {
            std::cerr << " - " << instance->issue(i)->description() << "\n";
        }

        return false;
    }

    // Warm up (i.e. a first, untimed, run) and then time several runs.

    instance->run();

    if (instance->hasIssues()) {
        std::cerr << "ERROR: the " << pName << " benchmark could not be run.\n";

        return false;
    }

    static constexpr size_t REPETITION_COUNT {5};

    std::vector<double> times;

    times.reserve(REPETITION_COUNT);

    for (size_t i {0}; i < REPETITION_COUNT; ++i) {
        const auto start {std::chrono::steady_clock::now()};

        instance->run();

        times.push_back(std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count());
    }

    std::ranges::sort(times);

    const auto mean {std::accumulate(times.begin(), times.end(), 0.0) / static_cast<double>(REPETITION_COUNT)};
    const auto numberOfSteps {simulation->numberOfSteps()};
    const auto peakMemoryMb {static_cast<double>(peakMemoryUsage()) / static_cast<double>(BYTES_PER_KILOBYTE * BYTES_PER_KILOBYTE)};

    std::cout << std::format("{:<16} | {:>7} | {:>9.3f} | {:>8.3f} | {:>11.3f} | {:>9.3f} | {:>14.1f} | {:>9.1f}\n",
                             pName, numberOfSteps, instantiateMs,
                             times.front(), times[times.size() / 2], mean,
                             mean * NANOSECONDS_PER_MILLISECOND / static_cast<double>(numberOfSteps), peakMemoryMb);

    return true;
}

} // namespace

int main()
{
    try {
        std::cout << "Benchmark        |   Steps | Inst (ms) | Min (ms) | Median (ms) | Mean (ms) | Mean (ns/step) | Peak (MB)\n";
        std::cout << "-----------------+---------+-----------+----------+-------------+-----------+----------------+----------\n";

        auto ok {true};

        ok = runBenchmark("cvode-50k", "cellml_2.sedml", nullptr, -1) && ok;
        ok = runBenchmark("cvode-1m", "cellml_2.sedml", nullptr, ONE_MILLION_STEPS) && ok;
        ok = runBenchmark("euler-1m", "cellml_2.sedml", libOpenCOR::SolverForwardEuler::create(), ONE_MILLION_STEPS) && ok;
        ok = runBenchmark("heun-1m", "cellml_2.sedml", libOpenCOR::SolverHeun::create(), ONE_MILLION_STEPS) && ok;
        ok = runBenchmark("rk2-1m", "cellml_2.sedml", libOpenCOR::SolverSecondOrderRungeKutta::create(), ONE_MILLION_STEPS) && ok;
        ok = runBenchmark("rk4-1m", "cellml_2.sedml", libOpenCOR::SolverFourthOrderRungeKutta::create(), ONE_MILLION_STEPS) && ok;
        ok = runBenchmark("dae-cvode", "api/sed/dae/model.sedml", nullptr, -1) && ok;
        ok = runBenchmark("tt04-3k", "benchmark/tt04.sedml", nullptr, -1) && ok;
        ok = runBenchmark("tt04-100k", "benchmark/tt04.sedml", nullptr, ONE_HUNDRED_THOUSAND_STEPS) && ok;
        ok = runBenchmark("hypercapnea", "benchmark/hypercapnea.sedml", nullptr, -1) && ok;
        ok = runBenchmark("hypercapnea-30k", "benchmark/hypercapnea.sedml", nullptr, THIRTY_THOUSAND_STEPS) && ok;

        return ok ? 0 : 1;
    } catch (...) {
        std::cerr << "ERROR: an unexpected exception occurred.\n";

        return 1;
    }
}
