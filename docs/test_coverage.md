# Generating Test Coverage Reports (macOS with Xcode Tools)

This guide explains how to generate code coverage reports for the `malasim_test` executable on macOS using the LLVM tools included with Xcode.

## 1. Configure CMake for Coverage

You need to enable coverage instrumentation during the build. Configure your CMake project by adding the `ENABLE_COVERAGE` option:

```bash
# Navigate to your build directory (create one if it doesn't exist)
# E.g., mkdir build && cd build

# Configure CMake with coverage enabled
cmake .. -DENABLE_COVERAGE=ON
```

This assumes your `CMakeLists.txt` files have been updated to include the necessary LLVM coverage flags when `ENABLE_COVERAGE` is ON:

*   **Flags added:**
    *   `-fprofile-instr-generate`
    *   `-fcoverage-mapping`
*   **Targets:** These flags should be added to both the `MalaSimCore` library and the `malasim_test` executable targets during compilation and linking. (See relevant `CMakeLists.txt` files for the `if(ENABLE_COVERAGE)` blocks).

## 2. Build the Project

Compile your project as usual. A clean build is recommended after enabling coverage flags for the first time.

```bash
# From your build directory
make # or ninja, or your chosen build tool
```

## 3. Run the Test Executable

Execute your test suite. Running the instrumented executable will generate raw profile data.

```bash
# From your build directory
./bin/malasim_test
```

After execution, look for a file named `default.profraw` (or potentially multiple `.profraw` files) in the directory where you ran the executable (likely your build directory).

## 4. Process Coverage Data

Use the tools provided by `xcrun` (Xcode's command-line tool runner) to process the raw data and generate reports.

### a. Merge Raw Profiles

Combine the `.profraw` files into a single indexed `.profdata` file using `llvm-profdata`.

```bash
xcrun llvm-profdata merge -sparse default.profraw -o coverage.profdata
# If you have multiple *.profraw files:
# xcrun llvm-profdata merge -sparse *.profraw -o coverage.profdata
```

This creates the `coverage.profdata` file.

### b. Generate Coverage Report

Use `llvm-cov` to create reports from the indexed profile data and the instrumented executable.

**i. Summary Report (Terminal)**

To get a quick overview of coverage per file:

```bash
xcrun llvm-cov report ./bin/malasim_test -instr-profile=coverage.profdata
```

**ii. Detailed HTML Report**

To generate a browsable HTML report showing line-by-line coverage:

```bash
# Create a directory for the report
mkdir coverage_html

# Generate the report
xcrun llvm-cov show ./bin/malasim_test -instr-profile=coverage.profdata -format=html -o coverage_html

# Optional: Specify specific source files or directories to include
# xcrun llvm-cov show ./bin/malasim_test -instr-profile=coverage.profdata path/to/source/file.cpp path/to/dir/ -format=html -o coverage_html
```

Now you can open the `index.html` file inside the `coverage_html` directory in your web browser.

## Troubleshooting

*   **No `.profraw` file:** Ensure CMake configuration (`-DENABLE_COVERAGE=ON`) and a clean build were successful. Check that the compilation commands included the `-fprofile-instr-generate` and `-fcoverage-mapping` flags.
*   **Incorrect Paths / Missing Files in Report:** Ensure you are running the `llvm-profdata` and `llvm-cov` commands from the correct directory (usually the build directory root) so that the paths to the executable and source files can be resolved correctly.
*   **Only Headers in Report:** Verify that your tests actually execute code within the `.cpp` implementation files, not just code defined entirely in headers.
