#!/usr/bin/env bash
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && cmake --build build
ln -f -s build/compile_commands.json .
