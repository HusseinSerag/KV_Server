# 🧠 Server Project – Binary Protocol Key-Value Store

[![Platform](https://img.shields.io/badge/platform-UNIX-informational)](https://github.com/HusseinSerag)
[![Build](https://img.shields.io/badge/build-CMake-blue)](https://cmake.org)
[![Language](https://img.shields.io/badge/language-C++17-blue.svg)](https://en.cppreference.com)

## 🚀 Overview

A fast, minimal, and extensible binary-protocol key-value store written in C++ for UNIX-based systems. This project avoids over-engineering and thread complexity by using a custom non-blocking event loop built around `poll`. No external dependencies. Designed to be efficient, easy to understand, and extensible.

The protocol supports `int64_t`, `double`, and `std::string`, with clear type boundaries, a hashtable-backed store, and persistence capabilities. It’s built from scratch with full control over memory, sockets, and file I/O.

---

## ✨ Features

* 🧱 Low-level socket programming (`bind`, `setsockopt`, `poll`, etc.)
* 🔄 Non-blocking custom event loop
* 🧠 Lightweight business logic, no threads or callbacks
* 📦 Binary protocol with type-aware commands
* 🧮 Hashtable implemented with templates and linked lists
* 🧬 `Type` and `Value` class hierarchy for parsing and storage
* 💾 Optional persistence with binary dump/load
* 📝 Operation logging to file
* 📤 Simple Python load testing script
* 🛠️ Built with CMake
* ✅ Clean separation of client, server, and shared logic

---

## 🔌 Binary Protocol

Each message sent from the client is structured as:

```
[4 bytes size][2 bytes type][payload]
```

### Types:

* `0x00` – Generic operations (e.g., keys, table ops)
* `0x01` – `double`
* `0x02` – `int64_t`
* `0x03` – `std::string`

Incorrect or malformed messages will result in the connection being closed.

---

## ⚙️ Default Configuration

The server is launched using **command-line arguments**, not a config file.

Default values (if no args provided):

* **Port:** `3000`
* **Persistence:** `1` (enabled)
* **Data file:** `data/storage.dat`
* **Log file:** `logs/operations.log`

---

## 🗂️ Project Structure

```
project_root/
├── client/             # Client application (client.cpp)
├── include/            # Server headers
├── script/             # Python script for load testing
│   └── main.py
├── shared/             # Code shared between client and server
├── src/                # Core server source code
├── CMakeLists.txt      # CMake build script
└── .gitignore          # Clean ignored files
```

---

## 🛠️ Build and Run

**Platform Support:** UNIX (Linux/macOS). Windows is not supported.

### 📥 Clone

```bash
git clone https://github.com/HusseinSerag/your-repo.git
cd your-repo
```

### 🔧 Build (CMake)

```bash
mkdir build && cd build
cmake ..
make
```

### 🖥️ Run Server

```bash
./server_exec               # Uses default configuration
# Or manually:
./server_exec <port> <persistence> <data_file> <log_file>

# Example:
./server_exec --port=3000 --persistence=1 --data_file=data/storage.dat --log_file=logs/operations.log
```

### 💻 Run Client

```bash
./client
```

### 📊 Run Load Test

```bash
cd script
python3 main.py
```

---

## 📃 Error Handling

* Invalid operations return appropriate status codes.
* Division by zero or other illegal operations throw exceptions.
* Log file records all operations and their results.
* Failed protocol handling (e.g. wrong type) closes the connection.

---

## 🧪 Testing

The `script/main.py` script is a Python load test tool that simulates multiple concurrent client operations to benchmark and verify event loop stability.

---

## ❤️ Author

Built with care by [HusseinSerag](https://github.com/HusseinSerag). This project reflects deep system-level control and an appreciation for simplicity without sacrificing power. Extending it (e.g., adding list support) is as easy as adding new `Type` and `Value` subclasses.

---

## 🤝 Contributing

Contributions, suggestions, and bug reports are welcome! Fork and PR!

---

## 📄 License

MIT License.
