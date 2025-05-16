### Use this flag in CLion

build

```
docker build -t ubuntu-dev-env .
```

Build, add this flag to clion

add to cmake args

```
-DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_CXX_FLAGS="-fsanitize=address -g"
```

map in toolchain->docker->setting (gear icon)

virtual machine path:
/mnt/from_mac
local path:
/Users/ktt/plot/malasim_v6/Calibration/Run_1_v6_calibrate_z_kappa_test_5

Run
```
docker run --rm -it \
  -v /Users/ktt/CLionProjects/malasim:/home/dev/malasim \
  -v /Users/ktt/plot/malasim_v6/Calibration/Run_1_v6_calibrate_z_kappa_test_5/input:/home/dev/malasim/cmake-build-debug-docker/input \
  -w /home/dev/malasim/cmake-build-debug-docker/bin \
  ubuntu-dev-env \
  ./MalaSim --input=/home/dev/malasim/cmake-build-debug-docker/input/input_6.yml --reporter=ValidationReporter
```

To run docker container and debug on docker desktop:

```
docker compose -f docker-compose.yml build
docker-compose up -d
```
then see log in docker desktop

To access docker and debug:

```
docker run --rm -it ubuntu-dev-env bash
```

or 

```
docker run --rm -it \
  -v /Users/ktt/CLionProjects/malasim:/home/dev/malasim \
  -v /Users/ktt/plot/malasim_v6/.../input:/home/dev/malasim/cmake-build-debug-docker/input \
  -w /home/dev/malasim/cmake-build-debug-docker/bin \
  ubuntu-dev-env \
  bash

```