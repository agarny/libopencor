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


from libopencor import (
    File,
    Issue,
    SedDocument,
    SedModel,
    SedOneStep,
    SedTask,
    SedUniformTimeCourse,
    SolverCvode,
    SolverForwardEuler,
    SolverFourthOrderRungeKutta,
    SolverHeun,
    SolverKinsol,
    SolverSecondOrderRungeKutta,
)
import utils
from utils import assert_issues


def test_initialise():
    expected_serialisation = """<?xml version="1.0" encoding="UTF-8"?>
<sedML xmlns="http://sed-ml.org/sed-ml/level1/version4" level="1" version="4"/>
"""

    sed = SedDocument()

    assert sed.serialise() == expected_serialisation


def test_models():
    sed = SedDocument()

    assert sed.has_models == False
    assert sed.add_model(None) == False

    file = File(utils.LOCAL_FILE)
    model = SedModel(sed, file)

    assert sed.add_model(model) == True

    assert len(sed.models) == 1
    assert sed.models[0] == model

    assert sed.add_model(model) == False
    assert sed.remove_model(model) == True

    assert sed.has_models == False

    assert sed.remove_model(None) == False


def test_simulations():
    sed = SedDocument()

    assert sed.has_simulations == False
    assert sed.add_simulation(None) == False

    simulation = SedUniformTimeCourse(sed)

    assert sed.add_simulation(simulation) == True

    assert len(sed.simulations) == 1
    assert sed.simulations[0] == simulation

    assert sed.add_simulation(simulation) == False
    assert sed.remove_simulation(simulation) == True

    assert sed.has_simulations == False

    assert sed.remove_simulation(None) == False


def sed_task_expected_serialisation(with_properties):
    properties = (
        ' modelReference="model1" simulationReference="simulation1"'
        if with_properties
        else ""
    )

    return f"""<?xml version="1.0" encoding="UTF-8"?>
<sedML xmlns="http://sed-ml.org/sed-ml/level1/version4" level="1" version="4">
  <listOfTasks>
    <task id="task1"{properties}/>
  </listOfTasks>
</sedML>
"""


def test_tasks():
    sed = SedDocument()

    assert sed.has_tasks == False
    assert sed.add_task(None) == False

    file = File(utils.LOCAL_FILE)
    model = SedModel(sed, file)
    simulation = SedUniformTimeCourse(sed)
    task = SedTask(sed, model, simulation)

    assert task.model != None
    assert task.simulation != None

    assert sed.add_task(task) == True

    assert len(sed.tasks) == 1
    assert sed.tasks[0] == task

    assert sed.serialise() == sed_task_expected_serialisation(True)

    task.model = None
    task.simulation = None

    assert task.model == None
    assert task.simulation == None

    assert sed.serialise() == sed_task_expected_serialisation(False)

    expected_issues = [
        [
            Issue.Type.Error,
            "Task 'task1' requires a model.",
        ],
        [
            Issue.Type.Error,
            "Task 'task1' requires a simulation.",
        ],
    ]

    instance = sed.create_instance()

    assert_issues(instance, expected_issues)

    assert sed.add_task(task) == False
    assert sed.remove_task(task) == True

    assert sed.has_tasks == False

    assert sed.remove_task(None) == False


def test_ode_solver():
    sed = SedDocument()
    simulation = SedUniformTimeCourse(sed)

    assert simulation.ode_solver == None

    solver = SolverCvode()

    simulation.ode_solver = solver

    assert simulation.ode_solver == solver

    simulation.ode_solver = None

    assert simulation.ode_solver == None


def test_nla_solver():
    sed = SedDocument()
    simulation = SedUniformTimeCourse(sed)

    assert simulation.nla_solver == None

    solver = SolverKinsol()

    simulation.nla_solver = solver

    assert simulation.nla_solver == solver

    simulation.nla_solver = None

    assert simulation.nla_solver == None


def test_sed_simulation_one_step():
    file = File(utils.resource_path(utils.CELLML_2_FILE))
    sed = SedDocument(file)
    simulation = SedOneStep(sed)

    assert simulation.step == 1.0

    simulation.step = 1.23

    assert simulation.step == 1.23


def test_sed_simulation_uniform_time_course():
    file = File(utils.resource_path(utils.CELLML_2_FILE))
    sed = SedDocument(file)
    simulation = SedUniformTimeCourse(sed)

    assert simulation.initial_time == 0.0
    assert simulation.output_start_time == 0.0
    assert simulation.output_end_time == 1000.0
    assert simulation.number_of_steps == 1000

    simulation.initial_time = 1.23
    simulation.output_start_time = 4.56
    simulation.output_end_time = 7.89
    simulation.number_of_steps = 10

    assert simulation.initial_time == 1.23
    assert simulation.output_start_time == 4.56
    assert simulation.output_end_time == 7.89
    assert simulation.number_of_steps == 10


def test_solver():
    # Get the duplicate() method of different solvers to be covered.

    file = File(utils.resource_path(utils.CELLML_2_FILE))
    sed = SedDocument(file)

    sed.simulations[0].ode_solver = SolverForwardEuler()

    instance = sed.create_instance()

    instance.run()

    assert instance.has_issues == False

    sed.simulations[0].ode_solver = SolverFourthOrderRungeKutta()

    instance = sed.create_instance()

    instance.run()

    assert instance.has_issues == False

    sed.simulations[0].ode_solver = SolverHeun()

    instance = sed.create_instance()

    instance.run()

    assert instance.has_issues == False

    sed.simulations[0].ode_solver = SolverSecondOrderRungeKutta()

    instance = sed.create_instance()

    instance.run()

    assert instance.has_issues == False


def test_sed_instance_task():
    expected_issues = [
        [
            Issue.Type.Error,
            "The upper half-bandwidth cannot be equal to -1. It must be between 0 and 2.",
        ],
    ]

    file = File(utils.resource_path(utils.CELLML_2_FILE))
    sed = SedDocument(file)
    solver = sed.simulations[0].ode_solver

    solver.linear_solver = SolverCvode.LinearSolver.Banded
    solver.upper_half_bandwidth = -1

    instance = sed.create_instance()

    instance.run()

    assert_issues(instance, expected_issues)
