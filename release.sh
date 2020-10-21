#!/bin/sh

set -e

if [ $1 = "build" ]; then
    bazel build :harunobu -c opt --copt="-fopenmp" --linkopt="-lgomp"
fi
if [ $1 = "run" ]; then
    bazel build :harunobu -c opt --copt="-fopenmp" --linkopt="-lgomp"
    bazel-bin/harunobu $2 $3
fi