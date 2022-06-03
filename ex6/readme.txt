References:
1. for queue code- used https://www.geeksforgeeks.org/queue-linked-list-implementation/
2. for guard code - used the code that Yuval taught us on practice lesson. Answer for 4th question down below.

Assumptions:
1. string's max size as an argument to the queue is 1024 bytes.
2. the string "EXIT" is invalid and used to disconnect from the server.
3. ! will be returned from function when invalid (not alphabetic) char is being inserted.
4. reactor struct can contain maximum 100 file descriptors (closed or open).
5. if the same file descriptor is placed again it will be placed in a different cell
and when removing it will be removed from the first cell it appeared on.

Notes:
1. to run pipeline:
- first run "make all"
- second run "./main"
- for new string run "./client"
    - then enter the string you would like ir 'EXIT' to exit

Answer for 4th question:
according to this:
https://stackoverflow.com/questions/20820937/strtok-function-and-multithreading
'strtok' modifies and potentially uses global (or static, whatever) variables,
and does so in a probably-not-thread-safe way, so even if we don't rely on
repeating calls to 'strtok' to get successive "tokens" from the same string,
it may not be safe to use it in a multi-threaded environment without guards, etc.

Important note:
Did not use the poll function to monitor incoming recv data from socket
https://stackoverflow.com/questions/26517373/use-poll-timeout-only-for-new-data-on-socket
according to this it is not possible to monitor such events on poll!