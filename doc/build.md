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

Use the following command to build the full set of sources:
```bash
cmake -DCMAKE_INSTALL_PREFIX=${PWD}/../install ${PWD}/../sltbench -DCMAKE_BUILD_TYPE=Release -DSLT_BUILD_TESTS=ON -DSLT_BUILD_EXAMPLES=ON -DSLT_BUILD_HISTOGRAMMER=ON -DSLT_BUILD_AUTOLEARNING=ON
```
