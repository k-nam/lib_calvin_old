# lib_calvin

## Overview
* It Implements various data structures and algorithms in C++, including major STL container interfaces (vector/set/map) and fundamental algorithms (sorting, graph, string, matrix).
* The focus is on efficient and readable source code. 

## Directories
* src
  * Visual C++ solution which contains almost all source codes. 
  * You can see the benchmarks of algorithms implemented in this project against other libraries in [here](http://bench.minami.im). The 'benchmark' and 'benchmark_string' project in the solution is for running benchmarks and produce data.

* scripts/bench_upload_script
  * Upload JSON files produced by '/src/benchmark' to Rails web server. (see bench.minami.link)


## How to run
* **Source files in this repository do not compile by themselves**, because there are data files and library files which were excluded from git due to their size. 
* Download the zip file to get all files. Open lib_calvin.sln in "lib_calvin/repo/lib_calvin" folder in Visual Studio. [download](https://s3.ap-northeast-2.amazonaws.com/calvin-download/lib_calvin.zip)
* The 'matrix' project uses Intel's MKL and it needs [libiomp5md.dll](https://s3.ap-northeast-2.amazonaws.com/calvin-download/libiomp5md.dll) file to be in your path.

