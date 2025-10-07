# Stream-FS

**Stream-FS** is a modular, high-performance file system framework written in **C++**, designed around a **stream-oriented storage model**.
It provides a **Virtual File System (VFS)** layer with pluggable backends such as a simulated hard-disk (HDD) driver for testing, benchmarking, and research in file system design — without direct disk access.

---

## 🚀 Features

* **Virtual File System (VFS)** — clean abstraction for mounting and managing file systems.
* **HDD Simulator** — safe, file-backed disk emulation for development and benchmarking.
* **Modular architecture** — easily extendable to support multiple backends (RAM disk, network FS, etc.).
* **Written in Modern C++ (C++20)** — clean, type-safe design with RAII and smart resource management.
* **Cross-platform** — builds on both Linux and Windows using CMake.
* **Testing and Benchmarking** — includes GoogleTest and Google Benchmark integration.

---

## ⚙️ Build Instructions

### Prerequisites

* **CMake ≥ 3.20**
* **C++20 compiler** (GCC ≥ 11 or Clang ≥ 13)
* **Git** (for submodules and CI)
* *(Optional)* GoogleTest and Google Benchmark (automatically fetched via CMake)

### Build

```bash
git clone https://github.com/amanp243/stream-fs.git
cd stream-fs
cmake -B build -S .
cmake --build build -- -j
```

### Run Tests

```bash
cd build
ctest --output-on-failure
```

### Run Benchmarks

```bash
./build/benchmarks/benchmark_streamfs
```

---

## 🧠 Architecture Overview

* **VFS Layer**
  Provides a unified API for file operations (open, read, write, delete) independent of the underlying device.

* **HDD Simulator**
  Implements a block-device interface (`IBlockDevice`) using a file-backed storage image, allowing safe development and reproducible testing.

* **Stream-Oriented File System (Future Work)**
  Will manage data as continuous streams, optimized for sequential and real-time access patterns (ideal for multimedia or cloud pipelines).

---

## 🧰 Future Roadmap

* [ ] Implement full VFS metadata layer (inodes, directories)
* [ ] Add journaling and crash recovery support
* [ ] Integrate caching and async I/O
* [ ] Add RAM-based block device simulator
* [ ] Develop distributed streaming support

---

**Stream-FS** — *exploring modern file system design from the ground up in C++.*
