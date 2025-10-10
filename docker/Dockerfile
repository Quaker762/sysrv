#
# Dockerfile for System-V Development
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

# Use ubuntu 24.04
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive \
    PYTHONBUFFERED=1 \
    PYTHON_VENV_PATH=/opt/sysrv-venv

# Make sure we're totally up to date
RUN apt update -y
RUN apt upgrade -y
RUN apt install -y build-essential \
                   python3-full \
                   python3-pip \
                   python-is-python3 \
                   doxygen \
                   tar \
                   qemu-system-misc \
                   git \
                   bash-completion \
                   clang-tidy-19 \
                   clang-format-19 \
                   sudo \
                   vim \
                   curl \
                   wget \
                   unzip \
                   device-tree-compiler \
                   ninja-build \
                   cmake \
                   && apt clean \
                   && rm -rf /var/lib/apt

# Download the cross compiler
ADD https://github.com/riscv-collab/riscv-gnu-toolchain/releases/download/2024.12.16/riscv64-elf-ubuntu-24.04-gcc-nightly-2024.12.16-nightly.tar.xz /tmp/riscv64-elf-ubuntu-24.04-gcc-nightly-2024.12.16-nightly.tar.xz

# Extract it
RUN mkdir /opt/riscv64-gcc && \
    tar -xvf /tmp/riscv64-elf-ubuntu-24.04-gcc-nightly-2024.12.16-nightly.tar.xz -C /opt/riscv64-gcc --strip-components=1 && \
    rm -rf /tmp/*

# Create a Virtual Environment and install compiledb
RUN python3 -m venv $PYTHON_VENV_PATH
ENV PATH="$PYTHON_VENV_PATH/bin:$PATH"

# Update the path
ENV PATH="$PATH:/opt/riscv64-gcc/bin"

# Add a non-root user that has sudo access
# Set the default user to be our non-root user

ENTRYPOINT ["/bin/bash"]
