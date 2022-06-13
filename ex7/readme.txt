Assumptions:
1. any file is connected to a directory,
In order to open a file you would need to specify the directory name as well.
2. Valid pathname contains '/' and not '\'.
3. Note that 'home/reut/file.txt' is not the same as 'reut/file.txt'
4. Note that when you open 'home/reut/file.txt' it opens:
    - home (path- "")
    - reut (path- "home")
    - file.txt (path- "home/reut")
But when you close 'home/reut/file.txt' it is only closes:
    - file.txt (path- "home/reut")
just like in a normal file system