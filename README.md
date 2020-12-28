# C++ High Performance, Second Edition
This is the code repository for [C++ High Performance, Second Edition](https://www.packtpub.com/application-development/), published by [Packt](https://www.packtpub.com/?utm_source=github). It contains all the supporting project files necessary to work through the book from start to finish.

## About the Book

C++ High Performance, Second Edition guides you through optimizing the performance of your C++ apps. This allows them to run faster and consume fewer resources on the device they're running on without compromising the readability of your codebase.

## About the Code

There are a lot of code examples in this book. Some are taken from the real world, but most of them are artificial or vastly simplified examples to prove a concept, rather than provide you with production-ready code.

I have put all the code examples in source files divided by chapter so that it is fairly easy to find the examples you want to experiment with. For example, code examples from chapter 1 resides in the directory named `Chapter01`.

 Some chapters contains benchmarks. This code is collected under a subdirectory named `benchmarks`. For example, the benchmark code from chapter 2 are placed in the directory `Chapter02/benchmarks`.

If you open up the source files, you will note that I have replaced most of the `main()` functions from the examples with test cases written with the Google Test framework. I hope that this will help you rather than confuse you. It allowed me to write helpful descriptions for each example, and it also makes it easier to run all the examples from one chapter at once.

If you want to exclude some of the chapters when building, you can comment out some of the chapters in the file `CMakeList.txt` located in the root of the project.


## Compilers, build tools, and libraries 

In order to build and run the examples you need a recent C++ compiler and [CMake](https://cmake.org) version 3.12 or above.

The code examples have been compiled using GCC 10.2, Clang 11, and Microsoft Visual C++ 2019. Not all C++20 features are supported by these compilers. The code files use [feature-test macros](https://en.cppreference.com/w/cpp/feature_test) to filter out code that are not supported among the various compilers. An excellent up-to-date summary of the current status of the major C++ compilers can be found at [https://en.cppreference.com/w/cpp/compiler_support](https://en.cppreference.com/w/cpp/compiler_support). You can also test various compiler versions online at [https://godbolt.org](https://godbolt.org).

The code in this repository depends on the following third party libraries:

- [Google Test framework](https://github.com/google/googletest)
- [Google Benchmark](https://github.com/google/benchmark)
- [Boost](https://boost.org)

All of these third party dependencies are installed and configured automatically by the CMake script via the [Hunter package manager](https://github.com/cpp-pm/hunter). Note that downloading and building third party libraries can take significant amount of time.


## Build Instructions for Windows/Visual Studio
Several steps are required to build the code examples.

### Step 1: Generate a Visual Studio solution

Go to the folder with the code examples and create a folder named `build`:
```
cd <cpp-high-performance-repo>
mkdir build
cd build
```

Run CMake to generate a Visual Studio solution:
```
cmake .. -G "Visual Studio 16 2019" -A x64
```
This step will also download and build all third party dependencies (Boost, Benchmark, and Google test) by using Hunter. This may take a while but will only be needed the first time you run CMake.

### Step 2: Open the Visual Studio solution
Open the solution in Visual Studio. There you can find one Visual Studio project for each chapter and several special projects, among them `ALL_BUILD` and `RUN_TESTS`.

### Step 3: Build and run the examples
Build `ALL_BUILD` project, and as a result all other projects will be built automatically.

Build `RUN_TESTS` project, and as a result all other projects will be executed. The result of running the tests will be printed to the output window.

## Build instructions for macOS/Clang
The examples have been compiled and verified on macOS Catalina using clang 11. The installation instructions assume that you have installed the package manager [Homebrew](https://brew.sh).

### Step 1: Install CMake
First of all you will need CMake version 3.12 or later in order to use the provided CMake scripts. Open a terminal and install CMake using Homebrew:

```
$ brew install cmake
```

### Step 2: Install LLVM/Clang
You will need a recent version of the Clang compiler in order to build all examples. You can download pre-built binaries for macOS of LLVM/Clang at [https://releases.llvm.org/download.html](https://releases.llvm.org/download.html) or use Homebrew to install it:

```
$ brew install llvm
```

Verify that you have the correct version of Clang installed:

```
$ /usr/local/opt/llvm/bin/clang++
clang version 11.0.0
...
```

In order to have CMake use the newly installed version of Clang, you need to set two environment variables: `CC` and `CXX`. If you are using the Z shell (the standard shell in macOS) you can set the environment variables in your ```.zshrc``` file, located under your home directory. Add the following two lines to `~/.zshrc`:

```
export CC=/usr/local/opt/llvm/bin/clang
export CXX=/usr/local/opt/llvm/bin/clang++
```

The environment variables will now be set the next time you open a terminal. Or if you source the `.zshrc` file immediately:

```
source ~/.zshrc
```

### Step 3: Generate Makefiles

Run CMake to generate Makefiles:
```
cd <cpp-high-performance-repo>
mkdir build
cd build
cmake .. -G "Unix Makefiles"
```

This step will also download and build all third party dependencies (Boost, Benchmark, and Google test) by using Hunter. This may take a while but will only be needed the first time you run CMake.


### Step 4: Build and run the examples
Now you are ready to compile the example codes using the Makefiles generated in the `build` directory. First compile the code:

```
$ make
```

It's now possible to execute each chapter on its own, like this:
```
$ Chapter01/Chapter01-A_Brief_Introduction_to_C++
```

Or you can run all the examples using the command:

```
$ ctest
```

### Step 5: Build and run benchmarks in Release mode

If you want to run the benchmarks it's recommended to build the code in `Release` mode (with optimizations turned on). You can do that by specifying `CMAKE_BUILD_TYPE` when issuing the `cmake` command.

Create a new build folder called `release-build`:

```
$ cd <cpp-high-performance-repo>
$ mkdir release-build
$ cd release-build
```

Generate Makefiles and compile the code with optimizations:
```
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make 
```

After the code has been built in `Release` mode, you can now run an individual benchmark like this:
``` 
$ ./Chapter02/benchmarks/Chapter02-Essential_C++_Techniques_Benchmarks 
```




### Generate Xcode project files (optional)

Instead of generating Makefiles you can also generate Xcode project files with CMake:

```
$ cd <cpp-high-performance-repo>
$ mkdir xcode-build
$ cd xcode-build
$ cmake .. -G Xcode
```

Assuming you have Xcode correctly installed, you can open the project by this command:

```
$ open xcode-build/CppHighPerformanceCodeExamples.xcodeproj
```

Note that Xcode will use the Clang version shipped with Xcode (and not the one installed by Homebrew).


## Build instructions for Ubuntu/GCC

The examples have been compiled and verified on Ubuntu 20.04 using GCC 10.2.

### Step 1: Install CMake

Use `apt` to install CMake:

```sh
$ sudo apt install cmake
```

Verify that you have a at least version 3.12 installed:

```sh
$ cmake --version
cmake version 3.16.3
```

### Step 2: Install GCC

Use `apt` to install GCC 10:
```
sudo apt install g++-10
```

Configure your system to use GCC 10 by default:
```
$ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 100
$ sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 100
```

Verify that you are using the correct version:
```
$ g++ --version
g++ (Ubuntu 10.2.0-5ubuntu1~20.04) 10.2.0
...
```



### Step 3: Generate Makefiles

Run CMake to generate Makefiles:
```
cd <cpp-high-performance-repo>
mkdir build
cd build
cmake .. 
```

This step will also download and build all third party dependencies (Boost, Benchmark, and Google test) by using Hunter. This may take a while but will only be needed the first time you run CMake.


### Step 4: Build and run the examples
Now you are ready to compile the example codes using the Makefiles generated in the `build` directory. First compile the code:

```
$ make
```

You can now execute each chapter on its own, like this:

```
$ Chapter01/Chapter01-A_Brief_Introduction_to_C++
```

Or you can run all the examples using the command:

```
$ ctest
```


### Step 5: Build and run benchmarks in Release mode

If you want to run the benchmarks it's recommended to build the code in `Release` mode (with optimizations turned on). You can do that by specifying `CMAKE_BUILD_TYPE` when issuing the `cmake` command.

Create a new build folder called `release-build`:

```
$ cd <cpp-high-performance-repo>
$ mkdir release-build
$ cd release-build
```

Generate Makefiles and compile the code with optimizations:
```
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make 
```

After the code has been built in `Release` mode, you can now run an individual benchmark like this:
``` 
$ ./Chapter02/benchmarks/Chapter02-Essential_C++_Techniques_Benchmarks 
```


