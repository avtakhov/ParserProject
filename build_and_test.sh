mkdir build
cd build
cmake .. -G "Unix Makefiles"
make
export GTEST_COLOR=1
make test ARGS="-V"

