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


from libopencor import Solver
from utils import assert_strings


def test_forward_euler_method():
    solver = Solver("Forward Euler")

    # ---GRY---
    # assert solver.type == Solver.Type.Ode
    # assert solver.name == "Forward Euler"
    # assert_strings(solver.properties, ["Step"])
