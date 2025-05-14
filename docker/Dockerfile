FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install build tools and GCC-12
RUN apt-get update && apt-get install -y \
    software-properties-common \
    build-essential \
    cmake \
    clang \
    make \
    curl \
    zip \
    unzip \
    git \
    sudo \
    vim \
    gdb \
    gcc-12 \
    g++-12 \
    pkg-config \
    valgrind \
    && apt-get clean

# Set GCC-12 as default
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 100 \
 && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 100

# Add user before changing vcpkg ownership
RUN useradd -m -s /bin/bash dev && echo "dev ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

# Install vcpkg and assign ownership
RUN git clone https://github.com/microsoft/vcpkg.git /opt/vcpkg \
 && /opt/vcpkg/bootstrap-vcpkg.sh \
 && chown -R dev:dev /opt/vcpkg

ENV VCPKG_ROOT=/opt/vcpkg
ENV PATH="${VCPKG_ROOT}:${PATH}"

USER dev
WORKDIR /home/dev

CMD ["/bin/bash"]
