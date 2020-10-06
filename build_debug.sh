mkdir -p ./build
cd build
rm -rf ./debug
mkdir debug
cd debug
cmake ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-D_FLOAT64"
make -j8