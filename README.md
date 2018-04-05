# lib_calvin

## Overview
* Implementing various data structures and algorithms in C++. 
* Major STL container interfaces (vector/set/map) and fundamental algorithms (sorting, graph, string, matrix)
* Some practical applications (neural_network, rainbow_table, lexer, parser, subcc (sub-C compiler; compier system of C-like toy language), CUDA matrix multiplication).
* Focus is on efficient and readable source code. 

## Directories

* lib_calvin
  * Visual C++ solution which contains almost all source codes. 
  * You can see the benchmarks of algorithms implemented in this project against other libraries in [here](http://bench.minami.im). The 'benchmark' and 'benchmark_string' project in the solution is for running benchmarks and produce data.

* bench_upload_script
  * Ruby script to upload JSON files produced by 'benchmark' and 'benchmark_string' projects to Rails web server.

* rainbow_table_db_script
  * Scala script for 'rainbow_table' project, taking care of file and DB operations. 


## How to run
* **Source files in this repository do not compile themselves**, because there are other data files and library files which were excluded from git due to their size. 
* Download below zip file to get all files. Open lib_calvin.sln in "lib_calvin/repo/lib_calvin" folder in Visual Studio.
* 'matrix' project uses Intel's MKL, so **matrix project will not run unless you install MKL on your computer.**
* [download](https://s3.ap-northeast-2.amazonaws.com/calvin-download/lib_calvin.zip)
