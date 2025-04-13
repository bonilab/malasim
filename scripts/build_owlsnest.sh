#!/bin/bash

# Check if the script is executed inside the scripts folder
if [[ $(basename "$PWD") == "scripts" ]]; then
  echo "Error: This script must be run from the project folder, not the 'scripts' folder."
  exit 1
fi

module load gcc/12.2.0
module load cmake/3.27.7
module load git/2.34.1

git clone https://github.com/microsoft/vcpkg.git
export VCPKG_ROOT=$(pwd)/vcpkg
make setup-vcpkg
make generate-gcc-12-owlsnest
make build
make test