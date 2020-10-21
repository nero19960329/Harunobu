bazel build :harunobu -c opt --copt="-fopenmp" --linkopt="-lgomp"
bazel-bin/harunobu $1