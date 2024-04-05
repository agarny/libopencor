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

describe("Sed run tests", () => {
  let someCellmlContentsPtr;
  let someErrorCellmlContentsPtr;
  let someOverconstrainedContentsPtr;
  let someUnderconstrainedContentsPtr;
  let someUnsuitablyConstrainedContentsPtr;
  let someAlgebraicContentsPtr;
  let someNlaContentsPtr;
  let someDaeContentsPtr;

  beforeAll(() => {
    someCellmlContentsPtr = utils.allocateMemory(
      libopencor,
      utils.SOME_CELLML_CONTENTS,
    );
    someErrorCellmlContentsPtr = utils.allocateMemory(
      libopencor,
      utils.SOME_ERROR_CELLML_CONTENTS,
    );
    someOverconstrainedContentsPtr = utils.allocateMemory(
      libopencor,
      utils.SOME_OVERCONSTRAINED_CONTENTS,
    );
    someUnderconstrainedContentsPtr = utils.allocateMemory(
      libopencor,
      utils.SOME_UNDERCONSTRAINED_CONTENTS,
    );
    someUnsuitablyConstrainedContentsPtr = utils.allocateMemory(
      libopencor,
      utils.SOME_UNSUITABLY_CONSTRAINED_CONTENTS,
    );
    someAlgebraicContentsPtr = utils.allocateMemory(
      libopencor,
      utils.SOME_ALGEBRAIC_CONTENTS,
    );
    someNlaContentsPtr = utils.allocateMemory(
      libopencor,
      utils.SOME_NLA_CONTENTS,
    );
    someDaeContentsPtr = utils.allocateMemory(
      libopencor,
      utils.SOME_DAE_CONTENTS,
    );
  });

  afterAll(() => {
    utils.freeMemory(libopencor, someCellmlContentsPtr);
    utils.freeMemory(libopencor, someErrorCellmlContentsPtr);
    utils.freeMemory(libopencor, someOverconstrainedContentsPtr);
    utils.freeMemory(libopencor, someUnderconstrainedContentsPtr);
    utils.freeMemory(libopencor, someUnsuitablyConstrainedContentsPtr);
    utils.freeMemory(libopencor, someAlgebraicContentsPtr);
    utils.freeMemory(libopencor, someNlaContentsPtr);
    utils.freeMemory(libopencor, someDaeContentsPtr);
  });

  test("No file", () => {
    const sed = new libopencor.SedDocument();
    const instance = sed.createInstance();

    expectIssues(instance, [
      [
        libopencor.Issue.Type.ERROR,
        "The simulation experiment description does not contain any tasks to run.",
      ],
    ]);
  });

  test("Invalid CellML file", () => {
    const file = new libopencor.File(utils.LOCAL_FILE);

    file.setContents(
      someErrorCellmlContentsPtr,
      utils.SOME_ERROR_CELLML_CONTENTS.length,
    );

    const sed = new libopencor.SedDocument(file);
    const instance = sed.createInstance();

    expectIssues(instance, [
      [libopencor.Issue.Type.ERROR, "The CellML file is invalid."],
      [
        libopencor.Issue.Type.ERROR,
        "Equation 'x+y+z' in component 'my_component' is not an equality statement (i.e. LHS = RHS).",
      ],
    ]);
  });

  test("Overconstrained CellML file", () => {
    const file = new libopencor.File(utils.LOCAL_FILE);

    file.setContents(
      someOverconstrainedContentsPtr,
      utils.SOME_OVERCONSTRAINED_CONTENTS.length,
    );

    const sed = new libopencor.SedDocument(file);
    const instance = sed.createInstance();

    expectIssues(instance, [
      [libopencor.Issue.Type.ERROR, "The CellML file is overconstrained."],
      [
        libopencor.Issue.Type.ERROR,
        "Variable 'x' in component 'my_component' is computed more than once.",
      ],
    ]);
  });

  test("Underconstrained CellML file", () => {
    const file = new libopencor.File(utils.LOCAL_FILE);

    file.setContents(
      someUnderconstrainedContentsPtr,
      utils.SOME_UNDERCONSTRAINED_CONTENTS.length,
    );

    const sed = new libopencor.SedDocument(file);
    const instance = sed.createInstance();

    expectIssues(instance, [
      [libopencor.Issue.Type.ERROR, "The CellML file is underconstrained."],
      [
        libopencor.Issue.Type.ERROR,
        "The type of variable 'x' in component 'my_component' is unknown.",
      ],
    ]);
  });

  test("UnsuitablyConstrained CellML file", () => {
    const file = new libopencor.File(utils.LOCAL_FILE);

    file.setContents(
      someUnsuitablyConstrainedContentsPtr,
      utils.SOME_UNSUITABLY_CONSTRAINED_CONTENTS.length,
    );

    const sed = new libopencor.SedDocument(file);
    const instance = sed.createInstance();

    expectIssues(instance, [
      [
        libopencor.Issue.Type.ERROR,
        "The CellML file is unsuitably constrained.",
      ],
      [
        libopencor.Issue.Type.ERROR,
        "Variable 'y' in component 'my_component' is computed more than once.",
      ],
      [
        libopencor.Issue.Type.ERROR,
        "The type of variable 'x' in component 'my_component' is unknown.",
      ],
    ]);
  });

  test("Algebraic model", () => {
    const file = new libopencor.File(utils.LOCAL_FILE);

    file.setContents(
      someAlgebraicContentsPtr,
      utils.SOME_ALGEBRAIC_CONTENTS.length,
    );

    const sed = new libopencor.SedDocument(file);
    const instance = sed.createInstance();

    instance.run();

    expect(instance.hasIssues()).toBe(false);
  });

  test("ODE model", () => {
    const file = new libopencor.File(utils.LOCAL_FILE);

    file.setContents(someCellmlContentsPtr, utils.SOME_CELLML_CONTENTS.length);

    const sed = new libopencor.SedDocument(file);
    const simulation = sed.simulations().get(0);
    const cvode = simulation.odeSolver();

    /*---GRY--- TO BE RE-ENABLED ONCE WE CAN RUN A SIMULATION FROM JavaScript.
    cvode.setMaximumNumberOfSteps(10);

    const instance = sed.createInstance();

    expect(instance.hasIssues()).toBe(false);

    instance.run();

    expectIssues(instance, [
      [
        libopencor.Issue.Type.ERROR,
        "At t = 0.00140014, mxstep steps taken before reaching tout.",
      ],
    ]);
    */

    cvode.setMaximumNumberOfSteps(500);

    const instance = sed.createInstance();

    instance.run();

    expect(instance.hasIssues()).toBe(false);
  });

  test("ODE model with no ODE solver", () => {
    const file = new libopencor.File(utils.LOCAL_FILE);

    file.setContents(someCellmlContentsPtr, utils.SOME_CELLML_CONTENTS.length);

    const sed = new libopencor.SedDocument(file);

    sed.simulations().get(0).setOdeSolver(null);

    const instance = sed.createInstance();

    expectIssues(instance, [
      [
        libopencor.Issue.Type.ERROR,
        "Simulation 'simulation1' is to be used with model 'model1' which requires an ODE solver but none is provided.",
      ],
    ]);
  });

  test("NLA model", () => {
    const file = new libopencor.File(utils.LOCAL_FILE);

    file.setContents(someNlaContentsPtr, utils.SOME_NLA_CONTENTS.length);

    const sed = new libopencor.SedDocument(file);
    const simulation = sed.simulations().get(0);
    const kinsol = simulation.nlaSolver();

    kinsol.setLinearSolver(libopencor.SolverKinsol.LinearSolver.BANDED);
    kinsol.setUpperHalfBandwidth(-1);

    let instance = sed.createInstance();

    /*---GRY--- TO BE RE-ENABLED ONCE WE CAN RUN A SIMULATION FROM JavaScript.
    expectIssues(instance, [
      [
        libopencor.Issue.Type.ERROR,
        "The upper half-bandwidth cannot be equal to -1. It must be between 0 and 0.",
      ],
    ]);
    */

    kinsol.setLinearSolver(libopencor.SolverKinsol.LinearSolver.DENSE);

    instance = sed.createInstance();

    expect(instance.hasIssues()).toBe(false);
  });

  test("NLA model with no NLA solver", () => {
    const file = new libopencor.File(utils.LOCAL_FILE);

    file.setContents(someNlaContentsPtr, utils.SOME_NLA_CONTENTS.length);

    const sed = new libopencor.SedDocument(file);

    sed.simulations().get(0).setNlaSolver(null);

    const instance = sed.createInstance();

    expectIssues(instance, [
      [
        libopencor.Issue.Type.ERROR,
        "Simulation 'simulation1' is to be used with model 'model1' which requires an NLA solver but none is provided.",
      ],
    ]);
  });

  test("DAE model", () => {
    const file = new libopencor.File(utils.LOCAL_FILE);

    file.setContents(someDaeContentsPtr, utils.SOME_DAE_CONTENTS.length);

    const sed = new libopencor.SedDocument(file);
    const simulation = sed.simulations().get(0);
    const kinsol = simulation.nlaSolver();

    kinsol.setLinearSolver(libopencor.SolverKinsol.LinearSolver.BANDED);
    kinsol.setUpperHalfBandwidth(-1);

    let instance = sed.createInstance();

    /*---GRY--- TO BE RE-ENABLED ONCE WE CAN RUN A SIMULATION FROM JavaScript.
    expectIssues(instance, [
      [
        libopencor.Issue.Type.ERROR,
        "The upper half-bandwidth cannot be equal to -1. It must be between 0 and 0.",
      ],
    ]);
    */

    instance.run();

    /*---GRY--- TO BE RE-ENABLED ONCE WE CAN RUN A SIMULATION FROM JavaScript.
    expectIssues(instance, [
      [
        libopencor.Issue.Type.ERROR,
        "The upper half-bandwidth cannot be equal to -1. It must be between 0 and 0.",
      ],
    ]);
    */

    kinsol.setLinearSolver(libopencor.SolverKinsol.LinearSolver.DENSE);

    instance = sed.createInstance();

    instance.run();

    expect(instance.hasIssues()).toBe(false);
  });

  test("DAE model with no ODE or NLA solver", () => {
    const file = new libopencor.File(utils.LOCAL_FILE);

    file.setContents(someDaeContentsPtr, utils.SOME_DAE_CONTENTS.length);

    const sed = new libopencor.SedDocument(file);
    const simulation = sed.simulations().get(0);

    simulation.setOdeSolver(null);
    simulation.setNlaSolver(null);

    const instance = sed.createInstance();

    expectIssues(instance, [
      [
        libopencor.Issue.Type.ERROR,
        "Simulation 'simulation1' is to be used with model 'model1' which requires an ODE solver but none is provided.",
      ],
      [
        libopencor.Issue.Type.ERROR,
        "Simulation 'simulation1' is to be used with model 'model1' which requires an NLA solver but none is provided.",
      ],
    ]);
  });
});
