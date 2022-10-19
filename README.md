# Bloomfilter Class Project

## Installation

We assume a linux kernel and bash is used.

Install using:

```
git clone --recurse-submodules https://github.com/zachross015/bloomfilter
cd bloomfilter
```

Submodules are necessary since we use a different random library than the
traditional c++ library. Once the project has been downloaded, run

```
mkdir build
cd build
cmake ..
cmake --build .
cd ..
```

To run the program then, simply run:

```
build/bloomfilter
```

## Usage

The program requires 3 inputs: the size of the bloomfilter, the number of hash
functions used, and the path to the file containing all the commands used in the
file.

