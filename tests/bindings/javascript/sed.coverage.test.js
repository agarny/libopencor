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

import libOpenCOR from "./libopencor.js";
import * as utils from "./utils.js";
import { expectIssues } from "./utils.js";

const libopencor = await libOpenCOR();

describe("Sed coverage tests", () => {
  let someSolverOdeContents;

  beforeAll(() => {
    someSolverOdeContents = utils.allocateMemory(
      libopencor,
      utils.SOME_SOLVER_ODE_CONTENTS,
    );
  });

  afterAll(() => {
    utils.freeMemory(libopencor, someSolverOdeContents);
  });

  function sedTaskExpectedSerialisation(withProperties) {
    return `<?xml version="1.0" encoding="UTF-8"?>
<sedML xmlns="http://sed-ml.org/sed-ml/level1/version4" level="1" version="4">
  <listOfTasks>
    <task id="task1"${withProperties ? ` modelReference="model1" simulationReference="simulation1"` : ``}/>
  </listOfTasks>
</sedML>
`;
  }

  test("Initialise", () => {
    const expectedSerialisation = `<?xml version="1.0" encoding="UTF-8"?>
<sedML xmlns="http://sed-ml.org/sed-ml/level1/version4" level="1" version="4"/>
`;
    const document = new libopencor.SedDocument();

    expect(document.serialise()).toBe(expectedSerialisation);

    document.delete();
  });

  test("Models", () => {
    const document = new libopencor.SedDocument();

    expect(document.hasModels()).toBe(false);
    expect(document.modelCount()).toBe(0);
    expect(document.models().size()).toBe(0);
    expect(document.addModel(null)).toBe(false);

    const file = new libopencor.File(utils.SEDML_FILE);
    const model = new libopencor.SedModel(document, file);

    expect(model.file()).toStrictEqual(file);

    expect(document.addModel(model)).toBe(true);

    expect(document.hasModels()).toBe(true);
    expect(document.modelCount()).toBe(1);
    expect(document.models().size()).toBe(1);
    expect(document.models().get(0)).toStrictEqual(model);
    expect(document.model(0)).toStrictEqual(model);
    expect(document.model(1)).toBeNull();

    expect(document.addModel(model)).toBe(false);
    expect(document.removeModel(model)).toBe(true);

    expect(document.hasModels()).toBe(false);
    expect(document.modelCount()).toBe(0);
    expect(document.models().size()).toBe(0);

    expect(document.removeModel(null)).toBe(false);

    model.delete();
    file.delete();
    document.delete();
  });

  test("Simulations", () => {
    const document = new libopencor.SedDocument();

    expect(document.hasSimulations()).toBe(false);
    expect(document.simulationCount()).toBe(0);
    expect(document.simulations().size()).toBe(0);
    expect(document.addSimulation(null)).toBe(false);

    const uniform_time_course = new libopencor.SedUniformTimeCourse(document);
    const one_step = new libopencor.SedOneStep(document);
    const steady_state = new libopencor.SedSteadyState(document);
    const analysis = new libopencor.SedAnalysis(document);

    expect(document.addSimulation(uniform_time_course)).toBe(true);
    expect(document.addSimulation(one_step)).toBe(true);
    expect(document.addSimulation(steady_state)).toBe(true);
    expect(document.addSimulation(analysis)).toBe(true);

    expect(document.hasSimulations()).toBe(true);
    expect(document.simulationCount()).toBe(4);
    expect(document.simulations().size()).toBe(4);
    expect(document.simulations().get(0)).toStrictEqual(uniform_time_course);
    expect(document.simulations().get(1)).toStrictEqual(one_step);
    expect(document.simulations().get(2)).toStrictEqual(steady_state);
    expect(document.simulations().get(3)).toStrictEqual(analysis);
    expect(document.simulation(0)).toStrictEqual(uniform_time_course);
    expect(document.simulation(1)).toStrictEqual(one_step);
    expect(document.simulation(2)).toStrictEqual(steady_state);
    expect(document.simulation(3)).toStrictEqual(analysis);
    expect(document.simulation(4)).toBeNull();

    expect(document.addSimulation(uniform_time_course)).toBe(false);
    expect(document.removeSimulation(uniform_time_course)).toBe(true);

    expect(document.addSimulation(one_step)).toBe(false);
    expect(document.removeSimulation(one_step)).toBe(true);

    expect(document.addSimulation(steady_state)).toBe(false);
    expect(document.removeSimulation(steady_state)).toBe(true);

    expect(document.addSimulation(analysis)).toBe(false);
    expect(document.removeSimulation(analysis)).toBe(true);

    expect(document.hasSimulations()).toBe(false);
    expect(document.simulationCount()).toBe(0);
    expect(document.simulations().size()).toBe(0);

    expect(document.removeSimulation(null)).toBe(false);

    steady_state.delete();
    uniform_time_course.delete();
    document.delete();
  });

  test("Tasks", () => {
    const document = new libopencor.SedDocument();

    expect(document.hasTasks()).toBe(false);
    expect(document.taskCount()).toBe(0);
    expect(document.tasks().size()).toBe(0);
    expect(document.addTask(null)).toBe(false);

    const file = new libopencor.File(utils.SEDML_FILE);
    const model = new libopencor.SedModel(document, file);
    const simulation = new libopencor.SedUniformTimeCourse(document);
    const task = new libopencor.SedTask(document, model, simulation);

    expect(task.model).not.toBe(null);
    expect(task.simulation).not.toBe(null);

    expect(document.addTask(task)).toBe(true);

    expect(document.hasTasks()).toBe(true);
    expect(document.taskCount()).toBe(1);
    expect(document.tasks().size()).toBe(1);
    expect(document.tasks().get(0)).toStrictEqual(task);
    expect(document.task(0)).toStrictEqual(task);
    expect(document.task(1)).toBeNull();

    expect(document.serialise()).toBe(sedTaskExpectedSerialisation(true));

    task.model = null;
    task.simulation = null;

    expect(task.model).toBe(null);
    expect(task.simulation).toBe(null);

    expect(document.serialise()).toBe(sedTaskExpectedSerialisation(false));

    const instance = document.instantiate();

    expectIssues(libopencor, instance, [
      [libopencor.Issue.Type.ERROR, "Task 'task1' requires a model."],
      [libopencor.Issue.Type.ERROR, "Task 'task1' requires a simulation."],
    ]);

    expect(document.addTask(task)).toBe(false);
    expect(document.removeTask(task)).toBe(true);

    expect(document.hasTasks()).toBe(false);
    expect(document.taskCount()).toBe(0);
    expect(document.tasks().size()).toBe(0);

    expect(document.removeTask(null)).toBe(false);

    task.delete();
    simulation.delete();
    model.delete();
    file.delete();
    document.delete();
  });

  test("ODE solver", () => {
    const document = new libopencor.SedDocument();
    const simulation = new libopencor.SedUniformTimeCourse(document);

    expect(simulation.odeSolver).toBe(null);

    const solver = new libopencor.SolverCvode();

    simulation.odeSolver = solver;

    expect(simulation.odeSolver).toStrictEqual(solver);

    simulation.odeSolver = null;

    expect(simulation.odeSolver).toBe(null);

    solver.delete();
    simulation.delete();
    document.delete();
  });

  test("NLA solver", () => {
    const document = new libopencor.SedDocument();
    const simulation = new libopencor.SedUniformTimeCourse(document);

    expect(simulation.nlaSolver).toBe(null);

    const solver = new libopencor.SolverKinsol();

    simulation.nlaSolver = solver;

    expect(simulation.nlaSolver).toStrictEqual(solver);

    simulation.nlaSolver = null;

    expect(simulation.nlaSolver).toBe(null);

    solver.delete();
    simulation.delete();
    document.delete();
  });

  test("SedOneStep", () => {
    const file = new libopencor.File(utils.resourcePath(utils.CELLML_FILE));
    const document = new libopencor.SedDocument(file);
    const simulation = new libopencor.SedOneStep(document);

    expect(simulation.step).toBe(1.0);

    simulation.step = 1.23;

    expect(simulation.step).toBe(1.23);

    simulation.delete();
    document.delete();
    file.delete();
  });

  test("SedUniformTimeCourse", () => {
    const file = new libopencor.File(utils.resourcePath(utils.CELLML_FILE));
    const document = new libopencor.SedDocument(file);
    const simulation = new libopencor.SedUniformTimeCourse(document);

    expect(simulation.initialTime).toBe(0.0);
    expect(simulation.outputStartTime).toBe(0.0);
    expect(simulation.outputEndTime).toBe(1000.0);
    expect(simulation.numberOfSteps).toBe(1000);

    simulation.initialTime = 1.23;
    simulation.outputStartTime = 4.56;
    simulation.outputEndTime = 7.89;
    simulation.numberOfSteps = 10;

    expect(simulation.initialTime).toBe(1.23);
    expect(simulation.outputStartTime).toBe(4.56);
    expect(simulation.outputEndTime).toBe(7.89);
    expect(simulation.numberOfSteps).toBe(10);

    simulation.delete();
    document.delete();
    file.delete();
  });

  test("SedInstanceAndSedInstanceTask", () => {
    const file = new libopencor.File(utils.resourcePath(utils.CELLML_FILE));

    file.setContents(
      someSolverOdeContents,
      utils.SOME_SOLVER_ODE_CONTENTS.length,
    );

    const document = new libopencor.SedDocument(file);
    const solver = document.simulations().get(0).odeSolver;

    solver.linearSolver = libopencor.SolverCvode.LinearSolver.BANDED;
    solver.upperHalfBandwidth = -1;

    const instance = document.instantiate();
    const instanceTask = instance.tasks().get(0);

    expect(instanceTask.voi().size()).toBe(0);
    expect(instanceTask.voiAsArray()).toStrictEqual([]);
    expect(instanceTask.voiName()).toBe("environment/time");
    expect(instanceTask.voiUnit()).toBe("millisecond");

    expect(instanceTask.stateCount()).toBe(4);
    expect(instanceTask.state(0).size()).toBe(0);
    expect(instanceTask.stateAsArray(0)).toStrictEqual([]);
    expect(instanceTask.state(4).size()).toBe(0);
    expect(instanceTask.stateAsArray(4)).toStrictEqual([]);
    expect(instanceTask.stateName(0)).toBe("membrane/V");
    expect(instanceTask.stateName(4)).toBe("");
    expect(instanceTask.stateUnit(0)).toBe("millivolt");
    expect(instanceTask.stateUnit(4)).toBe("");

    expect(instanceTask.rateCount()).toBe(4);
    expect(instanceTask.rate(0).size()).toBe(0);
    expect(instanceTask.rateAsArray(0)).toStrictEqual([]);
    expect(instanceTask.rate(4).size()).toBe(0);
    expect(instanceTask.rateAsArray(4)).toStrictEqual([]);
    expect(instanceTask.rateName(0)).toBe("membrane/V'");
    expect(instanceTask.rateName(4)).toBe("");
    expect(instanceTask.rateUnit(0)).toBe("millivolt/millisecond");
    expect(instanceTask.rateUnit(4)).toBe("");

    expect(instanceTask.constantCount()).toBe(5);
    expect(instanceTask.constant(0).size()).toBe(0);
    expect(instanceTask.constantAsArray(0)).toStrictEqual([]);
    expect(instanceTask.constant(5).size()).toBe(0);
    expect(instanceTask.constantAsArray(5)).toStrictEqual([]);
    expect(instanceTask.constantName(0)).toBe("membrane/Cm");
    expect(instanceTask.constantName(5)).toBe("");
    expect(instanceTask.constantUnit(0)).toBe("microF_per_cm2");
    expect(instanceTask.constantUnit(5)).toBe("");

    expect(instanceTask.computedConstantCount()).toBe(3);
    expect(instanceTask.computedConstant(0).size()).toBe(0);
    expect(instanceTask.computedConstantAsArray(0)).toStrictEqual([]);
    expect(instanceTask.computedConstant(3).size()).toBe(0);
    expect(instanceTask.computedConstantAsArray(3)).toStrictEqual([]);
    expect(instanceTask.computedConstantName(0)).toBe("leakage_current/E_L");
    expect(instanceTask.computedConstantName(3)).toBe("");
    expect(instanceTask.computedConstantUnit(0)).toBe("millivolt");
    expect(instanceTask.computedConstantUnit(3)).toBe("");

    expect(instanceTask.algebraicCount()).toBe(10);
    expect(instanceTask.algebraic(0).size()).toBe(0);
    expect(instanceTask.algebraicAsArray(0)).toStrictEqual([]);
    expect(instanceTask.algebraic(10).size()).toBe(0);
    expect(instanceTask.algebraicAsArray(10)).toStrictEqual([]);
    expect(instanceTask.algebraicName(0)).toBe("membrane/i_Stim");
    expect(instanceTask.algebraicName(10)).toBe("");
    expect(instanceTask.algebraicUnit(0)).toBe("microA_per_cm2");
    expect(instanceTask.algebraicUnit(10)).toBe("");

    instance.run();

    expectIssues(libopencor, instance, [
      [
        libopencor.Issue.Type.ERROR,
        "The upper half-bandwidth cannot be equal to -1. It must be between 0 and 3.",
      ],
    ]);

    instance.delete();
    document.delete();
    file.delete();
  });

  test("SedDocument", () => {
    let file = new libopencor.File(
      utils.resourcePath(utils.HTTP_REMOTE_CELLML_FILE),
    );
    let document = new libopencor.SedDocument(file);

    document.delete();
    file.delete();

    file = new libopencor.File(
      utils.resourcePath(utils.HTTP_REMOTE_SEDML_FILE),
    );
    document = new libopencor.SedDocument(file);

    document.delete();
    file.delete();

    file = new libopencor.File(
      utils.resourcePath(utils.HTTP_REMOTE_COMBINE_ARCHIVE),
    );
    document = new libopencor.SedDocument(file);

    document.delete();
    file.delete();
  });

  test("Solver", () => {
    // Get the duplicate() method of different solvers to be covered.

    const file = new libopencor.File(utils.resourcePath(utils.CELLML_FILE));

    file.setContents(
      someSolverOdeContents,
      utils.SOME_SOLVER_ODE_CONTENTS.length,
    );

    const document = new libopencor.SedDocument(file);
    const simulation = document.simulations().get(0);
    let solver = new libopencor.SolverForwardEuler();

    simulation.odeSolver = solver;

    let instance = document.instantiate();

    instance.run();

    expect(instance.hasIssues()).toBe(false);

    instance.delete();
    solver.delete();

    solver = new libopencor.SolverFourthOrderRungeKutta();

    simulation.odeSolver = solver;

    instance = document.instantiate();

    instance.run();

    expect(instance.hasIssues()).toBe(false);

    instance.delete();
    solver.delete();

    solver = new libopencor.SolverHeun();

    simulation.odeSolver = solver;

    instance = document.instantiate();

    instance.run();

    expect(instance.hasIssues()).toBe(false);

    instance.delete();
    solver.delete();

    solver = new libopencor.SolverSecondOrderRungeKutta();

    simulation.odeSolver = solver;

    instance = document.instantiate();

    instance.run();

    expect(instance.hasIssues()).toBe(false);

    instance.delete();
    solver.delete();
    document.delete();
    file.delete();
  });
});
