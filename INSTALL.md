# Greetings

Welcome to the installation!

**Contents**

  1. Requirements
  2. Installation
  3. Debugging

# 1 Requirements
You will need the following libraries, headers and tools:

  * `g++>=4.3` or `clang>=3.3` (clang is suggested. The compiler must be C++03 compatible.)
  * C++ ?.? or higher
  * GNU `bison`
  * GNU `flex`
  * `doxygen` if you want to build the documentation

# 2 Installation of the commandline interface

Export the variables
  * `BISON_BIN` (suggested: `/usr/bin/bison`)
  * `FLEX_BIN` (suggested: `/usr/bin/flex`)
  * `CC` (suggested: `clang`)
  * `CXX` (suggested: `clang++`)
Set the variable `LD_LIBRARY_PATH` to `$(readlink -e src)`.
Then type `make`.

All in all, you may want to type:
```sh
export BISON_BIN=/usr/bin/bison \
  && export FLEX_BIN=/usr/bin/flex \
  && export CC=clang \
  && export CXX=clang++ \
  && export LD_LIBRARY_PATH=$(readlink -e src)
make -j <Number of your CPU cores x 1.5>
```

# 3 Debugging
TODO: describe compiler flags, output etc.
