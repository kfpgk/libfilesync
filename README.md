# Introduction

This repository holds the source code for the file syncing library `libfilesync`.

Refer to the [examples](#examples) for guidance on the use of this library.

# Dependencies

- `libcurl`

# Build

## Library

Build the library:
```shell
cd libfilesync
mkdir build
cd build
cmake ..
cmake --build .
```

## Examples

Set the `BUILD_EXAMPLES` option to build example applications.

```shell
cd libfilesync
mkdir build
cd build
cmake -DBUILD_EXAMPLES=ON ..
cmake --build .
```

## Tests

Set the `BUILD_TESTING` option to build tests.

```shell
cd libfilesync
mkdir build
cd build
cmake -DBUILD_TESTING=ON ..
cmake --build .
```

Run the tests using `ctest`
```shell
make test
```

Unit test names are preceded by `UT`.
Integration test names are preceded by `IT`. Integration tests potentially require an available test server that must be configured in `tests/config/config.h` in order to run.

## Debug build

Use the `CMAKE_BUILD_TYPE` option to enable debug build.

```shell
cd libfilesync
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=debug ..
cmake --build .
```

# Directory structure

<pre>
(root)
+- doc                      # Plant UML diagrams
|   +- *.puml
+- examples                 # Examples using libfilesync
|   +- <em>example1</em>
|       +- CMakeLists.txt
|       +- *.cpp
+- libfilesync              # File sync library 
|   +- CMakeLists.txt
|   +- *.cpp
|   +- *.hpp
|   +- <em>component1</em>              # File sync library component sub directory
|       +. CMakeLists.txt
|       +- *.cpp                        # Library source file
|       +- *.hpp                        # Library header file
|       +- *.test.cpp                   # Unit tests source file
|       +- *.test.hpp                   # Unit test header file
|   +- <em>component2</em>              # File sync library component sub directory
|       +. CMakeLists.txt
|       +- *.cpp
|       +- *.hpp
|       +- *.test.cpp
|       +- *.test.hpp
+- tests                    # Integration tests
|   +- CMakeLists.txt
|   +- <em>test1</em>               # Integration test sub directory
|       +- CMakeLists.txt
|       +- *.cpp                    # Integration test source file
|       +- *.hpp                    # Integration test header file
+- CMakeLists.txt
</pre>

# Documentation

## Architecture

There are several <em>UML</em> diagrams describing the software architecture.

A [package diagram](doc/libfilesync-package.puml) and a [component diagram](doc/libfilesync-component.puml) show a high level view and dependencies between subsystems.

Check the [overall class diagram](doc/libfilesync-class.puml) for general relations. This diagram only contains a simplified view on the <em>curl</em> related classes. Refer to the [curl class diagram](doc/libfilesync-curl-class.puml) for more details on these.
For a low level view on the <b>data layer</b>, refer to the [data layer class diagram](doc/libfilesync-data-layer-class.puml).

Take a look at the [curl option class diagram](doc/libfilesync-curl-option-class.puml) and the [curl option sequence diagram](doc/libfilesync-curl-option-seq.puml) for low level handling of <em>curl</em> options.

The [protocol interface class diagram](doc/libfilesync-protocol-interface-class.puml) shows the <b>interface</b> provided by the `protocolClient` objects. 