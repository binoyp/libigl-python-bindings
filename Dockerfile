# Use Ubuntu 24.04 as base image
FROM ubuntu:24.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install system dependencies including latest GCC and Python 3.13 build requirements
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc-13 \
    g++-13 \
    cmake \
    git \
    wget \
    libssl-dev \
    zlib1g-dev \
    libbz2-dev \
    libreadline-dev \
    libsqlite3-dev \
    libncursesw5-dev \
    xz-utils \
    tk-dev \
    libxml2-dev \
    libxmlsec1-dev \
    libffi-dev \
    liblzma-dev \
    && rm -rf /var/lib/apt/lists/*

# Set GCC 13 as default
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 100 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-13 100

# Install Python 3.13 from source
RUN wget https://www.python.org/ftp/python/3.13.1/Python-3.13.1.tgz && \
    tar -xf Python-3.13.1.tgz && \
    cd Python-3.13.1 && \
    ./configure --enable-optimizations --with-ensurepip=install && \
    make -j$(nproc) && \
    make altinstall && \
    cd .. && \
    rm -rf Python-3.13.1 Python-3.13.1.tgz

# Create symlinks for python3.13
RUN ln -sf /usr/local/bin/python3.13 /usr/local/bin/python3 && \
    ln -sf /usr/local/bin/python3.13 /usr/local/bin/python && \
    ln -sf /usr/local/bin/pip3.13 /usr/local/bin/pip3 && \
    ln -sf /usr/local/bin/pip3.13 /usr/local/bin/pip

# Set working directory
WORKDIR /workspace

# Copy project files
COPY . /workspace/

# Upgrade pip and install build tools
RUN pip install --upgrade pip setuptools wheel build

# Build the wheel with limited parallel jobs to avoid OOM
ENV CMAKE_BUILD_PARALLEL_LEVEL=2
RUN python -m build --wheel --outdir /workspace/dist

# Output directory contains the wheels
CMD ["ls", "-lh", "/workspace/dist"]
