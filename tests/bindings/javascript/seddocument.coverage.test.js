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

const libopencor = await libOpenCOR();

describe("SedDocument coverage tests", () => {
  test("Initialise", () => {
    const expectedSerialisation = `<?xml version="1.0" encoding="UTF-8"?>
<sed xmlns="http://sed-ml.org/sed-ml/level1/version4" level="1" version="4"/>
`;
    const sed = new libopencor.SedDocument();

    expect(sed.serialise()).toBe(expectedSerialisation);
  });

  test("Model", () => {
    const sed = new libopencor.SedDocument();

    expect(sed.models().size()).toBe(0);
    expect(sed.addModel(null)).toBe(false);

    const file = new libopencor.File(utils.LOCAL_FILE);
    const model = new libopencor.SedModel(file, sed);

    expect(sed.addModel(model)).toBe(true);

    expect(sed.models().size()).toBe(1);
    expect(sed.models().get(0)).toStrictEqual(model);

    expect(sed.addModel(model)).toBe(false);
    expect(sed.removeModel(model)).toBe(true);

    expect(sed.models().size()).toBe(0);

    expect(sed.removeModel(null)).toBe(false);
  });

  test("Simulations", () => {
    const sed = new libopencor.SedDocument();

    expect(sed.simulations().size()).toBe(0);
    expect(sed.addSimulation(null)).toBe(false);

    const simulation = new libopencor.SedSimulationUniformTimeCourse(sed);

    expect(sed.addSimulation(simulation)).toBe(true);

    expect(sed.simulations().size()).toBe(1);
    expect(sed.simulations().get(0)).toStrictEqual(simulation);

    expect(sed.addSimulation(simulation)).toBe(false);
    expect(sed.removeSimulation(simulation)).toBe(true);

    expect(sed.simulations().size()).toBe(0);

    expect(sed.removeSimulation(null)).toBe(false);
  });

  test("ODE solver", () => {
    const sed = new libopencor.SedDocument();
    const simulation = new libopencor.SedSimulationUniformTimeCourse(sed);

    expect(simulation.odeSolver()).toBe(null);

    const solver = new libopencor.SolverCvode();

    simulation.setOdeSolver(solver);

    expect(simulation.odeSolver()).toStrictEqual(solver);

    simulation.setOdeSolver(null);

    expect(simulation.odeSolver()).toBe(null);
  });

  test("NLA solver", () => {
    const sed = new libopencor.SedDocument();
    const simulation = new libopencor.SedSimulationUniformTimeCourse(sed);

    expect(simulation.nlaSolver()).toBe(null);

    const solver = new libopencor.SolverKinsol();

    simulation.setNlaSolver(solver);

    expect(simulation.nlaSolver()).toStrictEqual(solver);

    simulation.setNlaSolver(null);

    expect(simulation.nlaSolver()).toBe(null);
  });
});
