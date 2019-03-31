mkdir build
cd build
cmake ../src -G "Unix Makefiles"
make
export GTEST_COLOR=1
make test ARGS="-V"

