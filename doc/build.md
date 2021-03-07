# Build instructions


## Linux && Mac

```bash
# clone git repo
git clone https://github.com/ivafanas/sltbench.git
cd sltbench
git submodule init
git submodule update
cd ..

# create temp directories for building
mkdir build install

# generate, build, install
cd build
cmake -DCMAKE_INSTALL_PREFIX=${PWD}/../install ${PWD}/../sltbench -DCMAKE_BUILD_TYPE=Release
cmake --build . --target install

# prepared sltbench lib is here:
# ${PWD}/../install
```

## Windows

```bat
git clone https://github.com/ivafanas/sltbench.git
cd sltbench
git submodule init
git submodule update
cd ..

mkdir build install
cd build
cmake -DCMAKE_INSTALL_PREFIX=..\install ..\sltbench
msbuild.exe INSTALL.vcxproj /p:Configuration=Release
```

## For sltbench maintainers:

##### how to build full set of sources

Use the following command to build the full set of sources:

```bash
cmake -DCMAKE_INSTALL_PREFIX=${PWD}/../install ${PWD}/../sltbench -DCMAKE_BUILD_TYPE=Release -DSLT_BUILD_TESTS=ON -DSLT_BUILD_EXAMPLES=ON -DSLT_BUILD_HISTOGRAMMER=ON -DSLT_BUILD_AUTOLEARNING=ON
```

##### sanitizers support

In order to make sanitized builds to test sltbench library code the following cmake options are supported:

* `SLT_ENABLE_ASAN` - address sanitizer
* `SLT_ENABLE_LSAN` - leak sanitizer
* `SLT_ENABLE_MSAN` - memory sanitizer
* `SLT_ENABLE_TSAN` - thread sanitizer
* `SLT_ENABLE_UBSAN` - undefined behavior sanitizer

Example how to inject ASAN into the output binaries:

```bash
cmake -DCMAKE_INSTALL_PREFIX=${PWD}/../install ${PWD}/../sltbench -DCMAKE_BUILD_TYPE=Release -DSLT_BUILD_TESTS=ON -DSLT_BUILD_EXAMPLES=ON -DSLT_ENABLE_ASAN=ON
```

Please note that sanitizers are supported for Linux build only.

There is no sense to enable sanitizers for auxiliary utilities like histogrammer or autolearning, but they might be enabled anyway. Unit tests and examples must be covered with sanitizers. Other library parts are not required to be covered.

