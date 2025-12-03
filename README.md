# **httpserver**

A lightweight educational HTTP server written in C++.
Currently serves a static `index.html` (with CSS & JS) and focuses on helping me learn and experiment with **system calls**, low-level networking, and OS internals.


## Overview

`httpserver` is a simple HTTP server implemented from scratch in modern C++.
Its main purpose is **learning**, not production use:

At the moment, the server returns a single static page (`index.html`), but the project will evolve as I learn more.


## Features

* Minimal HTTP/1.0 implementation
* Uses raw system calls directly (no frameworks)
* Simple C++ build system using `make`
* Clean project structure

Planned:

* Routing
* Multi-threaded request handling
* MIME type recognition


## Project Structure

`src/` - C++ source files
`include/` -  Header files
`public/` - index.html, CSS, JS (site)
`build/` - Compiled binary output
`setup.sh` - Build preparation script (requires `bear`)
`Makefile`
`README.md`


## Installation & Setup

### **Clone the repo**

```sh
git clone https://github.com/yourname/httpserver.git
cd httpserver
```

### **Run setup**

`setup.sh` requires that you have `bear` installed.

```sh
./setup.sh
```

> `setup.sh` generates the compilation database (`compile_commands.json`) using bear.

### **Build**

After setup, run:

```sh
make
```

### **Run**

```sh
./build/http-server
```


## Usage

Start the server:

```sh
./build/http-server
```

Then open your browser and go to:

```
http://localhost:8080
```

You should see the static `index.html` page being served.


## Requirements

* Linux
* clang++
* make
* bear (for setup step)


## Contributing

This project is experimental and educational, but contributions, ideas, and pull requests are welcome.

