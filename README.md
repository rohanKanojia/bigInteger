[![Build Status](https://travis-ci.org/rohanKanojia/bigInteger.svg?branch=master)](https://travis-ci.org/rohanKanojia/bigInteger)

# BigInteger
A bigInteger Library designed in C++

Usually on 32, 16 bit platforms size of integer is restricted to 4 bytes/2 bytes. This
implementation of the BigInteger library provides feature similar to Java's BigInteger.
It is basically performs calculations using character arrays as the base type in order
to represent the big integers. Right now it can handle digits upto 1000 characters.

There is also a nice terminal based ui application that was initially what this project
was about. It provides nice visualization of the BigInteger operations using Ncurses with
C++.

## Screenshots:
1.) Addition:
![Addition](screenshots/2015-06-23-115549_1024x768_scrot.png)

2.) Multiplication:
![Multiplication](screenshots/2015-06-23-115605_1024x768_scrot.png)

3.) Division:
![Division](screenshots/2015-06-23-115638_1024x768_scrot.png)

4.) Subtraction:
![Subtraction](screenshots/2015-06-23-115821_1024x768_scrot.png)
