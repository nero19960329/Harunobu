#!/bin/sh

set -e

if [ $1 = "build" ]; then
    bazel build :harunobu
fi
if [ $1 = "run" ]; then
    bazel build :harunobu
    bazel-bin/harunobu $2 $3
fi