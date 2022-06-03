References:
1. for queue code- used https://www.geeksforgeeks.org/queue-linked-list-implementation/

Assumptions:
1. string's max size as an argument to the queue is 1024 bytes.
2. the string "EXIT" is invalid and used to disconnect from the server.
3. ! will be returned from function when invalid (not alphabetic) char is being inserted.
4. reactor struct can contains maximum 1000 connections.

Notes:
1. to run pipeline:
- first run "make all"
- second run "./main"
- for new string run "./client"
    - then enter the string you would like ir 'EXIT' to exit