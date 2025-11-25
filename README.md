#  Server Project – Key-Value Store with TTL and List Support

[![Platform](https://img.shields.io/badge/platform-UNIX-informational)](https://github.com/HusseinSerag)
[![Build](https://img.shields.io/badge/build-CMake-blue)](https://cmake.org)
[![Language](https://img.shields.io/badge/language-C++17-blue.svg)](https://en.cppreference.com)

##  Overview

A fast, minimal, and extensible key-value store written in C++ for UNIX-based systems. Built with low-level control in mind, it uses custom non-blocking sockets and an efficient hashtable for performance. The store supports multiple types, including ordered list types using red-black trees, and introduces TTL (Time-To-Live) support for expirable keys. There are no external dependencies.

The architecture is designed for simplicity and speed, focusing on deep control of memory, sockets, file I/O and multi-threading.

---

##  Features

*  Low-level non-blocking socket server using `poll`, `bind`, and `setsockopt`
*  Custom non-blocking event loop, no threads or callbacks
*  Strong type system: `int64_t`, `double`, `std::string`, and their list counterparts
*  Ordered list types (`list<int>`, `list<double>`, `list<string>`) implemented using red-black trees for efficient `O(log n)` operations
*  TTL (Time-To-Live) support for scalar and list values
*  Optional persistence with binary dump/load support
*  Log file support (enabled by default)
*  Python script for load testing
*  Built with CMake for easy build setup
*  Clean codebase separation (client, server, shared logic)
*  Producer-Consumer Pattern for logging using Semaphores

---

##  Client Communication Format

The client sends requests as a sequence of strings.

Format:

```
[n] [len1][str1] [len2][str2] ... [lenN][strN]
```

Where:

* `n` is the number of strings.
* Each string is prefixed with its length.

This format is simple to parse and avoids complex serialization logic.

---

##  Default Configuration

The server is launched via **command-line arguments**.

Default values:

* **Port:** `3000`
* **Persistence:** `1` (enabled)
* **Log Enabled:** `1` (enabled)
* **Data file:** `data/storage.dat`
* **Log file:** `logs/operations.log`

---

##  Project Structure

```
project_root/
├── client/             # Client application (client.cpp)
├── include/            # Server headers
├── script/             # Python load testing script
│   └── main.py
├── shared/             # Shared code between client and server
├── src/                # Core server source code
├── CMakeLists.txt      # CMake build script
└── .gitignore          # Git ignore file
```

---

##  Build and Run

**Supported Platforms:** UNIX (Linux/macOS). Windows is not supported.

###  Clone the Repository

```bash
git clone https://github.com/HusseinSerag/your-repo.git
cd your-repo
```

###  Build with CMake

```bash
mkdir build && cd build
cmake ..
make
```

###  Run Server

```bash
./server_exec               # Uses default configuration
# Or manually:
./server_exec <port> <persistence> <data_file> <log_file> <log_enabled>

# Example:
./server_exec --port=3000 --persistence=1 --data_file=data/storage.dat --log_file=logs/operations.log --log_enabled=1
```

###  Run Client

```bash
./client
```

###  Run Load Test

```bash
cd script
python3 main.py
```

---

##  Error Handling

* Malformed or invalid operations result in connection termination.
* Division by zero and similar invalid operations raise exceptions.
* The log file records all operations and results (if logging is enabled).
* TTL keys are automatically purged after expiration.

---

##  Load Testing

The `script/main.py` simulates multiple clients performing rapid operations to benchmark the server. On a local test, the server handled **19,000 `SET` operations** per second.

---

## ❤️ Author

Built with care by [HusseinSerag](https://github.com/HusseinSerag). The project emphasizes clean, low-level design, and extensibility—adding new types or extending functionality is straightforward.

---

## 🤝 Contributing

Feel free to fork and submit a PR. Suggestions, issues, and improvements are always welcome.

---

## 📄 License

MIT License.
