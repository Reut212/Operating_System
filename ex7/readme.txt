Assumptions:
1. any file is connected to a directory,
In order to open a file you would need to specify the directory name as well.
2. Valid pathname contains '/' and not '\'.
3. Note that 'home/reut/file.txt' is not the same as 'reut/file.txt'.
4. Note that when you open 'home/reut/file.txt' it opens:
    - home (path- "")
    - reut (path- "home")
    - file.txt (path- "home/reut")
But when you close 'home/reut/file.txt' it is only closes:
    - file.txt (path- "home/reut")
just like in a normal file system
5. all of the filesystem is being written to 'filesystem.txt'.

Notes:
1. Ignore all errors in the tests - They're supposed to be there
2. For myfscanf and myfprintf in part 2 we worked according this tutorial- as writen on exercise:
https://www.cprogramming.com/tutorial/c/lesson17.html
3. Got help from https://www.youtube.com/watch?v=n2AAhiujAqs&t=3432s&ab_channel=drdelhart for part A.