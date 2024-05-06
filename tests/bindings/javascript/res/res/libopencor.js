export function showPage(page) {
  document.querySelectorAll(".nav-link").forEach((crtPage) => {
    if (crtPage.id === "nav" + page) {
      crtPage.classList.add("active");
    } else {
      crtPage.classList.remove("active");
    }
  });

  document.querySelectorAll(".page").forEach((crtPage) => {
    crtPage.style.display = crtPage.id === "page" + page ? "block" : "none";
  });
}

function showError(error) {
  if (!error.endsWith(".")) {
    error += ".";
  }

  document.getElementById("errorMessage").innerHTML = error;

  updateFileUi(false, false, true, true);
}

function updateFileUi(
  fileInfoDisplay,
  fileIssuesDisplay,
  fileErrorDisplay,
  resetButtonDisplay
) {
  document.getElementById("fileInfo").style.display = fileInfoDisplay
    ? "block"
    : "none";
  document.getElementById("fileIssues").style.display = fileIssuesDisplay
    ? "block"
    : "none";
  document.getElementById("fileError").style.display = fileErrorDisplay
    ? "block"
    : "none";
  document.getElementById("resetFile").style.display = resetButtonDisplay
    ? "block"
    : "none";
  document.getElementById("simulation").style.display = fileInfoDisplay && !fileIssuesDisplay
    ? "block"
    : "none";
}

export function resetFile() {
  document.getElementById("dropAreaInput").value = "";

  updateFileUi(false, false, false, false);
}

function addAxisElement(axis, name) {
  if (name === "--") {
    axis.append("<hr>");
  } else {
    axis.append("<option>" + name + "</option>");
  }
}

function populateAxis(axisId) {
  const axis = $("#" + axisId);

  axis.empty();

  addAxisElement(axis, instance.voiName());
  addAxisElement(axis, "--");

  for (let i = 0; i < instance.stateCount(); ++i) {
    addAxisElement(axis, instance.stateName(i));
    addAxisElement(axis, instance.rateName(i));
  }

  for (let i = 0; i < instance.variableCount(); ++i) {
    addAxisElement(axis, instance.variableName(i));
  }
}

let sed = null;
let simulation = null;
let instance = null;
const { lightningChart } = lcjs;
const lc = lightningChart({
  license:
    "0002-nzAQF3zqMCpblLS99rf02G/6gxAtKwAxEC5o8jYpT4yyvi4PLN2GbqtlRwIftmLnHvzQLnGyUSxM3ZeY/K0T8CYy-MEUCIGFCtwYUZqBfv+B7Lu63gSSRGgNZ+XjiFIrVTIUzFYrPAiEAq8ycNenFAtDe4FEgMRaiR7qZSkoLp0mvXbtdOLhZ+0A=",
  licenseInformation: {
    appTitle: "LightningChart JS Trial",
    company: "LightningChart Ltd.",
  },
});
const chart = lc
  .ChartXY({
    container: document.getElementById("plottingArea"),
  })
  .setTitle("")
  .setAnimationsEnabled(false);
const lineSeries = chart.addLineSeries().setName("");

export function run() {
  console.time("Elapsed time");
  instance.run();
  console.timeEnd("Elapsed time");

  const voi = instance.voi();
  const state = instance.state(0);
  let dataSet = [];

  for (let i = 0; i < voi.size(); ++i) {
    dataSet.push({ x: voi.get(i), y: state.get(i) });
  }

  lineSeries.clear();
  lineSeries.add(dataSet);

  chart.getDefaultAxisX().fit();
  chart.getDefaultAxisY().fit();
}

function formattedIssueDescription(issue) {
  return issue.charAt(0).toLowerCase() + issue.slice(1);
}

// Make sure that the DOM is loaded before we do anything else.

import libOpenCOR from "../libopencor.js";

document.addEventListener("DOMContentLoaded", () => {
  // Make sure that libOpenCOR is loaded before we do anything else.

  libOpenCOR().then((libopencor) => {
    // Simulation page.

    const input = document.getElementById("dropAreaInput");
    const dropArea = document.getElementById("dropArea");
    let hasValidFile = false;

    input.addEventListener("change", (event) => {
      if (hasValidFile) {
        let inputFile = input.files[0];
        let fileReader = new FileReader();

        input.value = ""; // Allow the user to select the same file again.

        fileReader.readAsArrayBuffer(inputFile);

        fileReader.onload = async () => {
          try {
            // Retrieve the contents of the file.

            const fileArrayBuffer = await inputFile.arrayBuffer();
            const memPtr = libopencor._malloc(inputFile.size);
            const mem = new Uint8Array(
              libopencor.HEAPU8.buffer,
              memPtr,
              inputFile.size
            );

            mem.set(new Uint8Array(fileArrayBuffer));

            const file = new libopencor.File(inputFile.name);

            file.setContents(memPtr, inputFile.size);

            // Determine the type of the file.

            let fileType = "unknown";

            if (file.type() === libopencor.File.Type.CELLML_FILE) {
              fileType = "CellML";
            } else if (file.type() === libopencor.File.Type.SEDML_FILE) {
              fileType = "SED-ML";
            }

            document.getElementById("fileName").innerHTML = inputFile.name;
            document.getElementById("fileType").innerHTML = fileType;

            // Display any issues with the file or run it.

            let showIssues = false;

            if (file.type() === libopencor.File.Type.CELLML_FILE) {
              if (file.hasIssues()) {
                const issuesElement = document.getElementById("issues");
                const fileIssues = file.issues();

                issuesElement.replaceChildren();

                for (let i = 0; i < fileIssues.size(); ++i) {
                  const issue = fileIssues.get(i);
                  const issueElement = document.createElement("li");

                  issueElement.innerHTML =
                    String.raw`<span class="bold">` +
                    issue.typeAsString() +
                    ":</span> " +
                    formattedIssueDescription(issue.description());

                  issuesElement.appendChild(issueElement);
                }

                showIssues = true;
              } else {
                // Retrieve some information about the simulation.

                sed = new libopencor.SedDocument(file);
                simulation = sed.simulations().get(0);

                if (simulation.constructor.name === "SedUniformTimeCourse") {
                  simulation.setOutputEndTime(50.0);
                  simulation.setNumberOfSteps(50000);
                  // simulation.setOutputEndTime(1.0);
                  // simulation.setNumberOfSteps(1000);
                }

                instance = sed.createInstance();

                // Populate the X and Y axis dropdown lists.

                populateAxis("xAxis");
                populateAxis("yAxis");

                // Reset the plotting area.

                lineSeries.clear();

                // Run the model.

                run();
              }
            }

            updateFileUi(true, showIssues, false, true);
          } catch (exception) {
            showError(exception.message);
          }
        };

        fileReader.onerror = () => {
          showError(fileReader.error.message);
        };
      } else {
        resetFile();
      }
    });

    ["dragenter", "dragover"].forEach((eventName) => {
      dropArea.addEventListener(eventName, (event) => {
        if (event.dataTransfer.items.length === 1) {
          hasValidFile = true;

          dropArea.classList.add("drop-area-valid-file");
        } else {
          hasValidFile = false;

          dropArea.classList.add("drop-area-invalid-file-s");
        }
      });
    });

    ["dragleave", "drop"].forEach((eventName) => {
      dropArea.addEventListener(eventName, () => {
        dropArea.classList.remove("drop-area-valid-file");
        dropArea.classList.remove("drop-area-invalid-file-s");
      });
    });

    // Versions page.

    document.getElementById("version").innerHTML = libopencor.version();
    document.getElementById("versionString").innerHTML =
      libopencor.versionString();
    document.getElementById("libcellmlVersion").innerHTML =
      libopencor.libcellmlVersion();
    document.getElementById("libcellmlVersionString").innerHTML =
      libopencor.libcellmlVersionString();
    document.getElementById("libcombineVersion").innerHTML =
      libopencor.libcombineVersion();
    document.getElementById("libcombineVersionString").innerHTML =
      libopencor.libcombineVersionString();
    document.getElementById("libsedmlVersion").innerHTML =
      libopencor.libsedmlVersion();
    document.getElementById("libsedmlVersionString").innerHTML =
      libopencor.libsedmlVersionString();
    document.getElementById("sundialsVersion").innerHTML =
      libopencor.sundialsVersion();
    document.getElementById("sundialsVersionString").innerHTML =
      libopencor.sundialsVersionString();

    // Start with our simulation page.

    showPage("Simulation");
  });
});
