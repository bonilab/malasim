# Core Random

## Overview

The `Random` class encapsulates random number generation functionalities using the [GNU Scientific Library (GSL)](https://www.gnu.org/software/gsl/). It provides a comprehensive interface for generating random numbers across various distributions, ensuring proper resource management and ease of use in simulations.

## Features

- **Various Distributions:** Generate random numbers following Poisson, Uniform, Normal (both integer and double), Beta, Gamma, and Binomial distributions.
- **Truncated Normal Distribution:** Generate normally distributed numbers truncated within ±3 standard deviations to avoid extreme values.
- **Multinomial Distribution:** Generate multinomially distributed random variables.
- **CDF Computations:** Compute the Cumulative Distribution Function (CDF) and its inverse for the Gamma distribution and the standard normal distribution.
- **Shuffling Capabilities:** Shuffle elements in an array using the current random generator.
- **Resource Management:** Utilizes `std::unique_ptr` with a custom deleter to manage the GSL RNG resource.
- **Seed Control:** Allows setting and retrieving the RNG seed for reproducibility.
- **Templated Methods:** Provides templated methods for generating uniformly and normally distributed random numbers with flexible types.

## Dependencies

- [GNU Scientific Library (GSL)](https://www.gnu.org/software/gsl/)
- [fmt](https://fmt.dev/) (Optional, used for formatting logs)

## Installation

### Prerequisites

Ensure that the following dependencies are installed on your system:

- **GNU Scientific Library (GSL):**
  ```bash
  sudo apt-get install libgsl-dev
  ```
- **fmt Library:** (Optional, only if you intend to use logging features)
  ```bash
  sudo apt-get install libfmt-dev
  ```

### Building the Project

Assuming you're using CMake for your project, ensure that you link against GSL and fmt (if used):

```cmake
find_package(GSL REQUIRED)
find_package(fmt REQUIRED) # Optional

add_executable(your_executable main.cpp Random.cpp)
target_include_directories(your_executable PRIVATE ${GSL_INCLUDE_DIRS})
target_link_libraries(your_executable PRIVATE ${GSL_LIBRARIES} fmt::fmt) # fmt is optional
```

## Usage

### Initialization

```cpp
#include "Random.h"

// Initialize with default RNG
Random rng;

// Initialize with an external GSL RNG
gsl_rng* external_rng = gsl_rng_alloc(gsl_rng_mt19937);
Random rng_with_external(external_rng);
```

### Generating Random Numbers

#### Poisson Distribution

```cpp
double lambda = 4.5;
int poisson_sample = rng.random_poisson(lambda);
```

#### Uniform Distribution

```cpp
// Uniform integer in [0, 100)
uint64_t uniform_int = rng.random_uniform(100);

// Uniform integer in [50, 150)
uint64_t uniform_int_range = rng.random_uniform<uint64_t>(50, 150);

// Uniform double in [0.0, 1.0)
double uniform_double = rng.random_uniform();

// Uniform double in [5.0, 10.0)
double uniform_double_range = rng.random_uniform(5.0, 10.0);
```

#### Normal Distribution

```cpp
double mean = 0.0;
double std_dev = 1.0;

// Normal double
double normal_sample = rng.random_normal(mean, std_dev);

// Truncated normal double within ±3 standard deviations
double truncated_normal = rng.random_normal_truncated(mean, std_dev);

// Normal integer
int normal_int = rng.random_normal<int>(5, 2);

// Truncated normal integer within ±3 standard deviations
int truncated_normal_int = rng.random_normal_truncated(5, 2);
```

#### Beta Distribution

```cpp
double alpha = 2.0;
double beta_param = 5.0;
double beta_sample = rng.random_beta(alpha, beta_param);
```

#### Gamma Distribution

```cpp
double shape = 2.0;
double scale = 1.5;
double gamma_sample = rng.random_gamma(shape, scale);

// CDF of Gamma distribution
double gamma_cdf = rng.cdf_gamma_distribution(3.0, shape, scale);

// Inverse CDF (Quantile) of Gamma distribution
double gamma_quantile = rng.cdf_gamma_distribution_inverse(0.95, shape, scale);
```

#### Binomial Distribution

```cpp
double probability = 0.5;
unsigned int trials = 10;
unsigned int binomial_sample = rng.random_binomial(probability, trials);
```

#### Multinomial Distribution

```cpp
std::size_t categories = 3;
unsigned trials = 100;
double probabilities[] = {0.2, 0.5, 0.3};
unsigned results[3];

rng.random_multinomial(categories, trials, probabilities, results);

// results now contains the number of occurrences in each category
```

#### Shuffling an Array

```cpp
#include <algorithm>
#include <array>
#include <cstddef>

std::array<int, 5> arr = {1, 2, 3, 4, 5};
rng.shuffle(arr.data(), arr.size(), sizeof(int));

// arr is now shuffled
```

### Seed Management

```cpp
// Retrieve current seed
uint64_t current_seed = rng.get_seed();

// Set a new seed
rng.set_seed(123456789);
```

### Move Semantics

> **Note:** The `Random` class **does not** support move construction or move assignment. Both copy and move operations are deleted to ensure unique ownership of the RNG resource.

```cpp
Random rng1;
// The following operations are **not** allowed and will result in a compile-time error
// Random rng2 = std::move(rng1); // Move constructor - Deleted
// rng2 = std::move(rng1); // Move assignment - Deleted
```

## Example

```cpp
#include "Random.h"
#include <iostream>

int main() {
    try {
        // Initialize RNG
        Random rng;

        // Generate a Poisson-distributed number with lambda = 3.0
        int poisson = rng.random_poisson(3.0);
        std::cout << "Poisson sample: " << poisson << std::endl;

        // Generate a uniform double in [0, 1)
        double uniform = rng.random_uniform();
        std::cout << "Uniform sample: " << uniform << std::endl;

        // Generate a normal-distributed double with mean=0 and std_dev=1
        double normal = rng.random_normal(0.0, 1.0);
        std::cout << "Normal sample: " << normal << std::endl;

        // Shuffle an array
        int data[] = {1, 2, 3, 4, 5};
        rng.shuffle(data, 5, sizeof(int));
        std::cout << "Shuffled array: ";
        for (const auto& num : data) {
            std::cout << num << ' ';
        }
        std::cout << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
    return 0;
}
```

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request for any enhancements or bug fixes.

## License

[MIT License](LICENSE)

## Acknowledgments

- [GNU Scientific Library (GSL)](https://www.gnu.org/software/gsl/)
- [fmt Library](https://fmt.dev/)

---

### Changelog

#### v1.1.0

- **Removed Move Semantics:** The `Random` class no longer supports move construction or move assignment to ensure unique ownership of the RNG resource.
- **Added Templated Methods:** Introduced templated methods `random_uniform` and `random_normal` for greater flexibility in generating random numbers of different types.
- **Updated Documentation:** Revised the README to accurately reflect the class's capabilities and limitations.
- **Dependency Clarifications:** Specified that the `fmt` library is optional and primarily used for logging purposes.

#### v1.0.0

- Initial release with comprehensive random number generation functionalities using GSL.

---

_This README was generated and updated by ChatGPT based on the provided `Random.cpp` and `Random.h` implementations._
