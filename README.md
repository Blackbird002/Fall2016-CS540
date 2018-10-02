# Fall2016-CS540
Code Repository for assignments in CS 540 - Operating Systems @ Wichita State University

<h1>Program #1</h1>
This program uses the Collatz conjecture and applies it to any given positive integer n.

```
Collatz conjecture
n = n/2 if n is even
n = (3*n)+ 1 if n is odd
```

This ends when n converges to 1.
Uses the fork() system to call a child process
Parent process invokes the wait call to ensure process finishes


<h1>Programming #2</h1>
This program again uses the Collatz conjecture and applies it to a given positive number.
A child process is created to produce the sequence and writes it into shared memory. Later the
parent process will print out the generated collatz sequence from shared memory.

```
Collatz conjecture:
n = n/2 if n is even
n = (3*n) + 1 if n is odd
This ends when n converges to 1.
```
