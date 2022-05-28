How to run the code:
1. before running code make sure you closed the tests' server!
2. First of all compile with "make all"
3. Second run the server: ./server
4. Third run the client: ./client
5. Fourth choose command- PUSH/POP/TOP/EXIT
6. At the end remove all o files with "make clean" :)

Notes:
1. Neta Roth 207603978 and Reut Hadad 305170219
2. For malloc and free implementation we used
http://tharikasblogs.blogspot.com/p/how-to-write-your-own-malloc-and-free.html

Assumptions:
1. available memory: 40000 chars (malloc mmap)
2. available stack memory: 1024 chars (#define MAXDATASIZE 1024 in funcs.h)

How to run the test:
1. before running tests make sure you closed the code's server!
2. run from terminal 1: ./server
3. run from terminal 2: ./tests
