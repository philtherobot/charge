#!/usr/bin/env sh

LIBS="-lboost_filesystem -lboost_system -lyaml-cpp"

g++ --std=c++17 -g -I. src/charge.cpp $LIBS -o charge
