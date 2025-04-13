#!/bin/bash

# Check if the script is executed inside the scripts folder
if [[ $(basename "$PWD") == "scripts" ]]; then
  echo "Error: This script must be run from the project folder, not the 'scripts' folder."
  exit 1
fi

git clone https://github.com/microsoft/vcpkg.git
export VCPKG_ROOT=$(pwd)/vcpkg
make setup-vcpkg
make generate
make build
make test