# Introduction

This repository holds the source code, documentation and tests for the file syncing library `libfilesync`.

This `README` gives you the steps to build the library, the (unit and integration) tests and examples.
You will find information regarding the project structure.

Documentation is provided in terms of <em>UML</em> diagrams located in the `doc` sub directories.

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

Set the `BUILD_UNIT_TESTS` and/or the `BUILD_INTEGRATION_TESTS` options to build tests.

```shell
cd libfilesync
mkdir build
cd build
cmake -DBUILD_UNIT_TESTS=ON -DBUILD_INTEGRATION_TESTS=ON ..
cmake --build .
```

Run the tests using `ctest`
```shell
make test
```

Unit test names are preceded by `UT` and integration test names are preceded by `IT` in the result view. Integration tests potentially require an available test server that must be configured in `tests/config/config.h` in order to run. This test server is expected to have a certain configuration (e.g. allow creation of directories, refer to the integration test objects for details).

## Debug build

Use the `CMAKE_BUILD_TYPE` option to enable debug build.

```shell
cd libfilesync
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=DEBUG ..
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
|   +- doc
|       +- *.puml           # UML diagram 
|   +- CMakeLists.txt
|   +- *.cpp
|   +- *.hpp
|   +- <em>component1</em>                       # File sync library component sub directory
|       +- doc
|           +- *.puml                   # UML diagram
|       +- CMakeLists.txt
|       +- *.cpp                        # Library source file
|       +- *.hpp                        # Library header file
|       +- *.tpp                        # Library template implementation file
|       +- *.test.cpp                   # Unit tests source file
|       +- *.test.hpp                   # Unit test header file
|   +- <em>component2</em>                       # File sync library component sub directory
|       +- doc
|           +- *.puml                   
|       +- CMakeLists.txt
|       +- *.cpp
|       +- *.hpp
|       +- *.test.cpp
|       +- *.test.hpp
+- tests                            # Integration tests
|   +- CMakeLists.txt
|   +- <em>test1</em>                        # Integration test sub directory
|       +- CMakeLists.txt
|       +- *.cpp                    # Integration test source file
|       +- *.hpp                    # Integration test header file
+- CMakeLists.txt
</pre>