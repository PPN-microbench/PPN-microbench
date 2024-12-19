# PPN-microbench

A (hopefully) portable microbenchmarking tool for compute node caracterisation.

## Requirements

| lib            | version | comment                                      |
| -------------- | ------- | -------------------------------------------- |
| nlohmann-json3 | 3.10    | https://github.com/nlohmann/json             |
| cmake          | >=3.25  | https://cmake.org/download/                  |
| clang-format   | 14.0    | https://clang.llvm.org/docs/ClangFormat.html |
| LLVM           | 14.0    | https://apt.llvm.org/llvm.sh                 |

## Installing

```sh
cmake -B build -G Ninja
cmake --build build
```
