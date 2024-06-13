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
    SedOneStep,
    SolverCvode,
    SolverForwardEuler,
    SolverKinsol,
)
import platform
import utils
from utils import assert_issues


def cvode_expected_serialisation(source, parameters=None):
    integration_method = (
        parameters["KISAO:0000475"]
        if parameters and "KISAO:0000475" in parameters
        else "BDF"
    )
    iteration_type = (
        parameters["KISAO:0000476"]
        if parameters and "KISAO:0000476" in parameters
        else "Newton"
    )
    linear_solver = (
        parameters["KISAO:0000477"]
        if parameters and "KISAO:0000477" in parameters
        else "Dense"
    )
    preconditioner = (
        parameters["KISAO:0000478"]
        if parameters and "KISAO:0000478" in parameters
        else "Banded"
    )
    interpolate_solution = (
        parameters["KISAO:0000481"]
        if parameters and "KISAO:0000481" in parameters
        else "true"
    )

    return f"""<?xml version="1.0" encoding="UTF-8"?>
<sedML xmlns="http://sed-ml.org/sed-ml/level1/version4" level="1" version="4">
  <listOfModels>
    <model id="model1" language="urn:sedml:language:cellml" source="{source}"/>
  </listOfModels>
  <listOfSimulations>
    <uniformTimeCourse id="simulation1" initialTime="0" outputStartTime="0" outputEndTime="1000" numberOfSteps="1000">
      <algorithm kisaoID="KISAO:0000019">
        <listOfAlgorithmParameters>
          <algorithmParameter kisaoID="KISAO:0000209" value="1e-07"/>
          <algorithmParameter kisaoID="KISAO:0000211" value="1e-07"/>
          <algorithmParameter kisaoID="KISAO:0000415" value="500"/>
          <algorithmParameter kisaoID="KISAO:0000467" value="0"/>
          <algorithmParameter kisaoID="KISAO:0000475" value="{integration_method}"/>
          <algorithmParameter kisaoID="KISAO:0000476" value="{iteration_type}"/>
          <algorithmParameter kisaoID="KISAO:0000477" value="{linear_solver}"/>
          <algorithmParameter kisaoID="KISAO:0000478" value="{preconditioner}"/>
          <algorithmParameter kisaoID="KISAO:0000479" value="0"/>
          <algorithmParameter kisaoID="KISAO:0000480" value="0"/>
          <algorithmParameter kisaoID="KISAO:0000481" value="{interpolate_solution}"/>
        </listOfAlgorithmParameters>
      </algorithm>
    </uniformTimeCourse>
  </listOfSimulations>
  <listOfTasks>
    <task id="task1" modelReference="model1" simulationReference="simulation1"/>
  </listOfTasks>
</sedML>
"""


def kinsol_expected_serialisation(parameters=None):
    linear_solver = (
        parameters["KISAO:0000477"]
        if parameters and "KISAO:0000477" in parameters
        else "Dense"
    )

    return f"""<?xml version="1.0" encoding="UTF-8"?>
<sedML xmlns="http://sed-ml.org/sed-ml/level1/version4" level="1" version="4">
  <listOfModels>
    <model id="model1" language="urn:sedml:language:cellml" source="api/sed/nla.cellml"/>
  </listOfModels>
  <listOfSimulations>
    <steadyState id="simulation1">
      <algorithm kisaoID="KISAO:0000282">
        <listOfAlgorithmParameters>
          <algorithmParameter kisaoID="KISAO:0000477" value="{linear_solver}"/>
          <algorithmParameter kisaoID="KISAO:0000479" value="0"/>
          <algorithmParameter kisaoID="KISAO:0000480" value="0"/>
          <algorithmParameter kisaoID="KISAO:0000486" value="200"/>
        </listOfAlgorithmParameters>
      </algorithm>
    </steadyState>
  </listOfSimulations>
  <listOfTasks>
    <task id="task1" modelReference="model1" simulationReference="simulation1"/>
  </listOfTasks>
</sedML>
"""


def test_local_cellml_file_with_base_path():
    file = File(utils.resource_path(utils.Cellml2File))
    document = SedDocument(file)

    assert document.serialise(utils.LocalBasePath) == cvode_expected_serialisation(
        "cellml_2.cellml"
    )


def test_local_cellml_file_without_base_path():
    file = File(utils.LocalFile)

    file.contents = utils.string_to_list(utils.SomeCellmlContents)

    document = SedDocument(file)

    if platform.system() == "Windows":
        assert document.serialise() == cvode_expected_serialisation(
            "file:///P:/some/path/file.txt"
        )
    else:
        assert document.serialise() == cvode_expected_serialisation(
            "file:///some/path/file.txt"
        )


def test_relative_local_cellml_file_with_base_path():
    file = File(utils.resource_path(utils.Cellml2File))
    document = SedDocument(file)

    assert document.serialise(
        utils.LocalBasePath + "/../.."
    ) == cvode_expected_serialisation("tests/res/cellml_2.cellml")


def test_relative_local_cellml_file_without_base_path():
    file = File(utils.Cellml2File)

    file.contents = utils.string_to_list(utils.SomeCellmlContents)

    document = SedDocument(file)

    assert document.serialise() == cvode_expected_serialisation("cellml_2.cellml")


def test_remote_cellml_file_with_base_path():
    file = File(utils.RemoteFile)
    document = SedDocument(file)

    assert document.serialise(utils.RemoteBasePath) == cvode_expected_serialisation(
        "cellml_2.cellml"
    )


def test_remote_cellml_file_without_base_path():
    file = File(utils.RemoteFile)
    document = SedDocument(file)

    assert document.serialise() == cvode_expected_serialisation(
        "https://raw.githubusercontent.com/opencor/libopencor/master/tests/res/cellml_2.cellml"
    )


def test_relative_remote_cellml_file_with_base_path():
    file = File(utils.RemoteFile)
    document = SedDocument(file)

    assert document.serialise(
        utils.RemoteBasePath + "/../.."
    ) == cvode_expected_serialisation("tests/res/cellml_2.cellml")


def test_dae_model():
    expected_serialisation = """<?xml version="1.0" encoding="UTF-8"?>
<sedML xmlns="http://sed-ml.org/sed-ml/level1/version4" level="1" version="4">
  <listOfModels>
    <model id="model1" language="urn:sedml:language:cellml" source="api/sed/dae.cellml"/>
  </listOfModels>
  <listOfSimulations>
    <uniformTimeCourse id="simulation1" initialTime="0" outputStartTime="0" outputEndTime="1000" numberOfSteps="1000">
      <algorithm kisaoID="KISAO:0000019">
        <listOfAlgorithmParameters>
          <algorithmParameter kisaoID="KISAO:0000209" value="1e-07"/>
          <algorithmParameter kisaoID="KISAO:0000211" value="1e-07"/>
          <algorithmParameter kisaoID="KISAO:0000415" value="500"/>
          <algorithmParameter kisaoID="KISAO:0000467" value="0"/>
          <algorithmParameter kisaoID="KISAO:0000475" value="BDF"/>
          <algorithmParameter kisaoID="KISAO:0000476" value="Newton"/>
          <algorithmParameter kisaoID="KISAO:0000477" value="Dense"/>
          <algorithmParameter kisaoID="KISAO:0000478" value="Banded"/>
          <algorithmParameter kisaoID="KISAO:0000479" value="0"/>
          <algorithmParameter kisaoID="KISAO:0000480" value="0"/>
          <algorithmParameter kisaoID="KISAO:0000481" value="true"/>
        </listOfAlgorithmParameters>
      </algorithm>
      <nlaAlgorithm xmlns="https://opencor.ws/libopencor" kisaoID="KISAO:0000282">
        <listOfAlgorithmParameters>
          <algorithmParameter kisaoID="KISAO:0000477" value="Dense"/>
          <algorithmParameter kisaoID="KISAO:0000479" value="0"/>
          <algorithmParameter kisaoID="KISAO:0000480" value="0"/>
          <algorithmParameter kisaoID="KISAO:0000486" value="200"/>
        </listOfAlgorithmParameters>
      </nlaAlgorithm>
    </uniformTimeCourse>
  </listOfSimulations>
  <listOfTasks>
    <task id="task1" modelReference="model1" simulationReference="simulation1"/>
  </listOfTasks>
</sedML>
"""

    file = File(utils.resource_path("api/sed/dae.cellml"))
    document = SedDocument(file)

    assert document.serialise(utils.LocalBasePath) == expected_serialisation


def test_nla_model():
    expected_serialisation = """<?xml version="1.0" encoding="UTF-8"?>
<sedML xmlns="http://sed-ml.org/sed-ml/level1/version4" level="1" version="4">
  <listOfModels>
    <model id="model1" language="urn:sedml:language:cellml" source="api/sed/nla.cellml"/>
  </listOfModels>
  <listOfSimulations>
    <steadyState id="simulation1">
      <algorithm kisaoID="KISAO:0000282">
        <listOfAlgorithmParameters>
          <algorithmParameter kisaoID="KISAO:0000477" value="Dense"/>
          <algorithmParameter kisaoID="KISAO:0000479" value="0"/>
          <algorithmParameter kisaoID="KISAO:0000480" value="0"/>
          <algorithmParameter kisaoID="KISAO:0000486" value="200"/>
        </listOfAlgorithmParameters>
      </algorithm>
    </steadyState>
  </listOfSimulations>
  <listOfTasks>
    <task id="task1" modelReference="model1" simulationReference="simulation1"/>
  </listOfTasks>
</sedML>
"""

    file = File(utils.resource_path("api/sed/nla.cellml"))
    document = SedDocument(file)

    assert document.serialise(utils.LocalBasePath) == expected_serialisation


def test_algebraic_model():
    expected_serialisation = """<?xml version="1.0" encoding="UTF-8"?>
<sedML xmlns="http://sed-ml.org/sed-ml/level1/version4" level="1" version="4">
  <listOfModels>
    <model id="model1" language="urn:sedml:language:cellml" source="api/sed/algebraic.cellml"/>
  </listOfModels>
  <listOfSimulations>
    <steadyState id="simulation1"/>
  </listOfSimulations>
  <listOfTasks>
    <task id="task1" modelReference="model1" simulationReference="simulation1"/>
  </listOfTasks>
</sedML>
"""

    file = File(utils.resource_path("api/sed/algebraic.cellml"))
    document = SedDocument(file)

    assert document.serialise(utils.LocalBasePath) == expected_serialisation


def test_fixed_step_ode_solver():
    expected_serialisation = """<?xml version="1.0" encoding="UTF-8"?>
<sedML xmlns="http://sed-ml.org/sed-ml/level1/version4" level="1" version="4">
  <listOfModels>
    <model id="model1" language="urn:sedml:language:cellml" source="cellml_2.cellml"/>
  </listOfModels>
  <listOfSimulations>
    <uniformTimeCourse id="simulation1" initialTime="0" outputStartTime="0" outputEndTime="1000" numberOfSteps="1000">
      <algorithm kisaoID="KISAO:0000030">
        <listOfAlgorithmParameters>
          <algorithmParameter kisaoID="KISAO:0000483" value="1"/>
        </listOfAlgorithmParameters>
      </algorithm>
    </uniformTimeCourse>
  </listOfSimulations>
  <listOfTasks>
    <task id="task1" modelReference="model1" simulationReference="simulation1"/>
  </listOfTasks>
</sedML>
"""

    file = File(utils.resource_path(utils.Cellml2File))
    document = SedDocument(file)
    simulation = document.simulations[0]

    simulation.ode_solver = SolverForwardEuler()

    assert document.serialise(utils.LocalBasePath) == expected_serialisation


def test_cvode_solver_with_adams_moulton_interation_method():
    file = File(utils.resource_path(utils.Cellml2File))
    document = SedDocument(file)
    simulation = document.simulations[0]
    solver = simulation.ode_solver

    solver.integration_method = SolverCvode.IntegrationMethod.AdamsMoulton

    assert document.serialise(utils.LocalBasePath) == cvode_expected_serialisation(
        "cellml_2.cellml", {"KISAO:0000475": "Adams-Moulton"}
    )


def test_cvode_solver_with_functional_iteration_type():
    file = File(utils.resource_path(utils.Cellml2File))
    document = SedDocument(file)
    simulation = document.simulations[0]
    solver = simulation.ode_solver

    solver.iteration_type = SolverCvode.IterationType.Functional

    assert document.serialise(utils.LocalBasePath) == cvode_expected_serialisation(
        "cellml_2.cellml", {"KISAO:0000476": "Functional"}
    )


def test_cvode_solver_with_banded_linear_solver():
    file = File(utils.resource_path(utils.Cellml2File))
    document = SedDocument(file)
    simulation = document.simulations[0]
    solver = simulation.ode_solver

    solver.linear_solver = SolverCvode.LinearSolver.Banded

    assert document.serialise(utils.LocalBasePath) == cvode_expected_serialisation(
        "cellml_2.cellml", {"KISAO:0000477": "Banded"}
    )


def test_cvode_solver_with_diagonal_linear_solver():
    file = File(utils.resource_path(utils.Cellml2File))
    document = SedDocument(file)
    simulation = document.simulations[0]
    solver = simulation.ode_solver

    solver.linear_solver = SolverCvode.LinearSolver.Diagonal

    assert document.serialise(utils.LocalBasePath) == cvode_expected_serialisation(
        "cellml_2.cellml", {"KISAO:0000477": "Diagonal"}
    )


def test_cvode_solver_with_gmres_linear_solver():
    file = File(utils.resource_path(utils.Cellml2File))
    document = SedDocument(file)
    simulation = document.simulations[0]
    solver = simulation.ode_solver

    solver.linear_solver = SolverCvode.LinearSolver.Gmres

    assert document.serialise(utils.LocalBasePath) == cvode_expected_serialisation(
        "cellml_2.cellml", {"KISAO:0000477": "GMRES"}
    )


def test_cvode_solver_with_bicgstab_linear_solver():
    file = File(utils.resource_path(utils.Cellml2File))
    document = SedDocument(file)
    simulation = document.simulations[0]
    solver = simulation.ode_solver

    solver.linear_solver = SolverCvode.LinearSolver.Bicgstab

    assert document.serialise(utils.LocalBasePath) == cvode_expected_serialisation(
        "cellml_2.cellml", {"KISAO:0000477": "BiCGStab"}
    )


def test_cvode_solver_with_tfqmr_linear_solver():
    file = File(utils.resource_path(utils.Cellml2File))
    document = SedDocument(file)
    simulation = document.simulations[0]
    solver = simulation.ode_solver

    solver.linear_solver = SolverCvode.LinearSolver.Tfqmr

    assert document.serialise(utils.LocalBasePath) == cvode_expected_serialisation(
        "cellml_2.cellml", {"KISAO:0000477": "TFQMR"}
    )


def test_cvode_solver_with_no_preconditioner():
    file = File(utils.resource_path(utils.Cellml2File))
    document = SedDocument(file)
    simulation = document.simulations[0]
    solver = simulation.ode_solver

    solver.preconditioner = SolverCvode.Preconditioner.No

    assert document.serialise(utils.LocalBasePath) == cvode_expected_serialisation(
        "cellml_2.cellml", {"KISAO:0000478": "No"}
    )


def test_cvode_solver_with_no_interpolate_solution():
    file = File(utils.resource_path(utils.Cellml2File))
    document = SedDocument(file)
    simulation = document.simulations[0]
    solver = simulation.ode_solver

    solver.interpolate_solution = False

    assert document.serialise(utils.LocalBasePath) == cvode_expected_serialisation(
        "cellml_2.cellml", {"KISAO:0000481": "false"}
    )


def test_kinsol_solver_with_banded_linear_solver():
    file = File(utils.resource_path("api/sed/nla.cellml"))
    document = SedDocument(file)
    simulation = document.simulations[0]
    solver = simulation.nla_solver

    solver.linear_solver = SolverKinsol.LinearSolver.Banded

    assert document.serialise(utils.LocalBasePath) == kinsol_expected_serialisation(
        {"KISAO:0000477": "Banded"}
    )


def test_kinsol_solver_with_gmres_linear_solver():
    file = File(utils.resource_path("api/sed/nla.cellml"))
    document = SedDocument(file)
    simulation = document.simulations[0]
    solver = simulation.nla_solver

    solver.linear_solver = SolverKinsol.LinearSolver.Gmres

    assert document.serialise(utils.LocalBasePath) == kinsol_expected_serialisation(
        {"KISAO:0000477": "GMRES"}
    )


def test_kinsol_solver_with_bicgstab_linear_solver():
    file = File(utils.resource_path("api/sed/nla.cellml"))
    document = SedDocument(file)
    simulation = document.simulations[0]
    solver = simulation.nla_solver

    solver.linear_solver = SolverKinsol.LinearSolver.Bicgstab

    assert document.serialise(utils.LocalBasePath) == kinsol_expected_serialisation(
        {"KISAO:0000477": "BiCGStab"}
    )


def test_kinsol_solver_with_tfqmr_linear_solver():
    file = File(utils.resource_path("api/sed/nla.cellml"))
    document = SedDocument(file)
    simulation = document.simulations[0]
    solver = simulation.nla_solver

    solver.linear_solver = SolverKinsol.LinearSolver.Tfqmr

    assert document.serialise(utils.LocalBasePath) == kinsol_expected_serialisation(
        {"KISAO:0000477": "TFQMR"}
    )


def test_one_step_simulation():
    expected_serialisation = """<?xml version="1.0" encoding="UTF-8"?>
<sedML xmlns="http://sed-ml.org/sed-ml/level1/version4" level="1" version="4">
  <listOfModels>
    <model id="model1" language="urn:sedml:language:cellml" source="cellml_2.cellml"/>
  </listOfModels>
  <listOfSimulations>
    <oneStep id="simulation1" step="1"/>
  </listOfSimulations>
  <listOfTasks>
    <task id="task1" modelReference="model1" simulationReference="simulation1"/>
  </listOfTasks>
</sedML>
"""

    file = File(utils.resource_path(utils.Cellml2File))
    document = SedDocument(file)

    document.remove_simulation(document.simulations[0])

    simulation = SedOneStep(document)

    document.add_simulation(simulation)

    assert document.serialise(utils.LocalBasePath) == expected_serialisation


def test_sedml_file():
    expected_issues = [
        [
            Issue.Type.Warning,
            "The model 'cellml_2.cellml' could not be found. It has been automatically added, but it is empty.",
        ],
    ]
    expected_serialisation = """<?xml version="1.0" encoding="UTF-8"?>
<sedML xmlns="http://sed-ml.org/sed-ml/level1/version4" level="1" version="4">
  <listOfModels>
    <model id="model" language="urn:sedml:language:cellml" source="cellml_2.cellml"/>
  </listOfModels>
  <listOfSimulations>
    <uniformTimeCourse id=\"simulation1\" initialTime=\"0\" outputStartTime=\"0\" outputEndTime=\"50\" numberOfSteps=\"50000\"/>
  </listOfSimulations>
</sedML>
"""

    file = File(utils.resource_path(utils.Sedml2File))
    document = SedDocument(file)

    assert_issues(document, expected_issues)
    assert document.serialise(utils.LocalBasePath) == expected_serialisation
