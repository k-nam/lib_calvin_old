# lib_calvin

## Overview
* It Implements various data structures and algorithms in C++, including major STL container interfaces (vector/set/map) and fundamental algorithms (sorting, graph, string, matrix).
* It includes practical applications like neural_network, rainbow_table, lexer, parser, subcc (sub-C compiler; compiler system for C-like toy language), CUDA matrix multiplication.
* The focus is on efficient and readable source code. 

## Directories
* lib_calvin
  * Visual C++ solution which contains almost all source codes. 
  * You can see the benchmarks of algorithms implemented in this project against other libraries in [here](http://bench.minami.im). The 'benchmark' and 'benchmark_string' project in the solution is for running benchmarks and produce data.

* bench_upload_script
  * Ruby script to upload JSON files produced by 'benchmark' and 'benchmark_string' projects to Rails web server.

* rainbow_table_db_script
  * Scala script for 'rainbow_table' project, taking care of file and DB operations. 


## How to run
* **Source files in this repository do not compile themselves**, because there are data files and library files which were excluded from git due to their size. 
* Download the zip file to get all files. Open lib_calvin.sln in "lib_calvin/repo/lib_calvin" folder in Visual Studio. [download](https://s3.ap-northeast-2.amazonaws.com/calvin-download/lib_calvin.zip)
* The 'matrix' project uses Intel's MKL and it needs [libiomp5md.dll](https://s3.ap-northeast-2.amazonaws.com/calvin-download/libiomp5md.dll) file to be in your path.

