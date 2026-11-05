# 📷 Camera Module Test Software

A automated inspection framework using OpenCV for real-time camera module quality evaluation and manufacturing yield reporting.

---

## 🛠️ Tech Stack & Dependencies

* **Language:** C++17
* **Computer Vision:** OpenCV 5.0.0 (or 4.x)
* **Build System:** CMake 3.15+
* **IDE / Toolchain:** VS Code / MSVC (Visual Studio 2019/2022) or MinGW GCC
* **Analytics:** Python 3 (`pandas`, `matplotlib`)

---

## 📁 Project Architecture

```text
CameraTest/
├── CMakeLists.txt                # CMake configuration script
├── README.md                     # Project documentation
├── include/
│   ├── ImageAnalyzer.h           # Core image metric algorithm header
│   ├── Logger.h                  # Thread-safe CSV logging handler
│   └── TestTypes.h               # Shared data structures (TestResult, ModuleReport)
├── src/
│   ├── main.cpp                  # Directory scanner & batch execution loop
│   ├── ImageAnalyzer.cpp         # Inspection algorithm implementations
│   └── Logger.cpp                # File logging implementation
├── data/                         # Sample datasets and generated test reports
└── dashboard/
    └── yield_analysis.py         # Python yield metrics & Pareto chart generator
```

---

## 🔬 Inspection Metrics & Criteria

| Test Metric | Algorithm / Method | Pass Threshold |
| :--- | :--- | :--- |
| **Sharpness** | Laplacian Variance ($\sigma^2$) | $\ge 100.0$ |
| **Luminance Uniformity** | Corner-to-Center Mean Ratio | $\ge 0.70$ (70%) |
| **Dead / Hot Pixels** | Bright pixel thresholding ($> 250$) | $\le 5$ pixels |
| **Color Balance** | BGR Channel Mean Variance | $\le 20.0$ |

---

## 🚀 Building & Running

### 1. Configure and Compile
Open PowerShell in the project root folder:

```powershell
# Generate build configuration
cmake -B build

# Compile project executable
cmake --build build
```

### 2. Run Batch Inspection
Execute the inspection engine on the `data/` image dataset:

* **Visual Studio MSVC (Windows):**
  ```powershell
  .\build\Debug\CameraTest.exe data/
  ```

* **MinGW / GCC / Linux:**
  ```powershell
  .\build\CameraTest.exe data/
  ```

---

## 📊 Reporting

After running batch inspections, execute the Python analytics script to view First-Pass Yield (FPY) metrics and plot the failure Pareto chart:

```powershell
python dashboard/analysis.py
```