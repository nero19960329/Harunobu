mkdir -p ./build
cd build
rm -rf ./release
mkdir release
cd release
cmake ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-D_FLOAT64"
make -j8