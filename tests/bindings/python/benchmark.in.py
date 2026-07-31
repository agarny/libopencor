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


import libopencor as loc
import os
import resource
import statistics
import sys
import time

ONE_MILLION_STEPS = 1000000
ONE_HUNDRED_THOUSAND_STEPS = 100000
THIRTY_THOUSAND_STEPS = 30000
NANOSECONDS_PER_MILLISECOND = 1.0e6
BYTES_PER_KILOBYTE = 1024

ResourceLocation = "@RESOURCE_LOCATION@"


def resource_path(relative_path=""):
    return os.path.realpath(ResourceLocation + "/" + relative_path)


def peak_memory_usage():
    usage = resource.getrusage(resource.RUSAGE_SELF)

    # Note: ru_maxrss is in bytes on macOS and in kilobytes on Linux.

    if sys.platform == "darwin":
        return usage.ru_maxrss

    return BYTES_PER_KILOBYTE * usage.ru_maxrss


def run_benchmark(name, resource_file, ode_solver=None, number_of_steps=0):
    # Create our document, using the given model, and customise its simulation, if needed.

    file = loc.File(resource_path(resource_file))
    document = loc.SedDocument(file)
    simulation = document.simulations[0]

    if ode_solver is not None:
        simulation.ode_solver = ode_solver

    if number_of_steps > 0:
        simulation.output_end_time = (
            simulation.output_end_time * number_of_steps / simulation.number_of_steps
        )
        simulation.number_of_steps = number_of_steps

    # Instantiate our document and time how long it takes (this includes JIT compiling the model).

    instantiate_start = time.perf_counter()
    instance = document.instantiate()
    instantiate_ms = 1000.0 * (time.perf_counter() - instantiate_start)

    if instance.has_issues:
        print(
            f"ERROR: the {name} benchmark could not be instantiated:", file=sys.stderr
        )

        for i in range(instance.issue_count):
            print(f" - {instance.issue(i).description}", file=sys.stderr)

        return False

    # Warm up (i.e. a first, untimed, run) and then time several runs.

    instance.run()

    if instance.has_issues:
        print(f"ERROR: the {name} benchmark could not be run.", file=sys.stderr)

        return False

    repetition_count = 5
    times = []

    for _ in range(repetition_count):
        start = time.perf_counter()

        instance.run()

        times.append(1000.0 * (time.perf_counter() - start))

    times.sort()

    mean = statistics.mean(times)
    steps = simulation.number_of_steps
    peak_memory_mb = peak_memory_usage() / (BYTES_PER_KILOBYTE * BYTES_PER_KILOBYTE)

    print(
        f"{name:<16s} | {steps:7d} | {instantiate_ms:9.3f} | {times[0]:8.3f} "
        f"| {times[len(times) // 2]:11.3f} | {mean:9.3f} | {mean * NANOSECONDS_PER_MILLISECOND / steps:14.1f} "
        f"| {peak_memory_mb:9.1f}"
    )

    return True


def main():
    try:
        print(
            "Benchmark        |   Steps | Inst (ms) | Min (ms) | Median (ms) | Mean (ms) | Mean (ns/step) | Peak (MB)"
        )
        print(
            "-----------------+---------+-----------+----------+-------------+-----------+----------------+----------"
        )

        ok = True

        ok = run_benchmark("cvode-50k", "cellml_2.sedml") and ok
        ok = (
            run_benchmark(
                "cvode-1m", "cellml_2.sedml", number_of_steps=ONE_MILLION_STEPS
            )
            and ok
        )
        ok = (
            run_benchmark(
                "euler-1m",
                "cellml_2.sedml",
                loc.SolverForwardEuler(),
                ONE_MILLION_STEPS,
            )
            and ok
        )
        ok = (
            run_benchmark(
                "heun-1m", "cellml_2.sedml", loc.SolverHeun(), ONE_MILLION_STEPS
            )
            and ok
        )
        ok = (
            run_benchmark(
                "rk2-1m",
                "cellml_2.sedml",
                loc.SolverSecondOrderRungeKutta(),
                ONE_MILLION_STEPS,
            )
            and ok
        )
        ok = (
            run_benchmark(
                "rk4-1m",
                "cellml_2.sedml",
                loc.SolverFourthOrderRungeKutta(),
                ONE_MILLION_STEPS,
            )
            and ok
        )
        ok = run_benchmark("dae-cvode", "api/sed/dae/model.sedml") and ok
        ok = run_benchmark("tt04-3k", "benchmark/tt04.sedml") and ok
        ok = (
            run_benchmark(
                "tt04-100k",
                "benchmark/tt04.sedml",
                number_of_steps=ONE_HUNDRED_THOUSAND_STEPS,
            )
            and ok
        )
        ok = run_benchmark("hypercapnea", "benchmark/hypercapnea.sedml") and ok
        ok = (
            run_benchmark(
                "hypercapnea-30k",
                "benchmark/hypercapnea.sedml",
                number_of_steps=THIRTY_THOUSAND_STEPS,
            )
            and ok
        )

        return 0 if ok else 1
    except Exception:
        print("ERROR: an unexpected exception occurred.", file=sys.stderr)

        return 1


if __name__ == "__main__":
    sys.exit(main())
