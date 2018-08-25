# klisp
My implementation of a lisp like language

![klisp cli](https://github.com/intothevoid/klisp/raw/master/res/2018-08-25.png)

Version History
===============
v0.1 Added prompt to accept user input

v0.2 Added MPC (parser combinator)

v0.3 Klisp now accepts polish notation and prints the AST! (⌐■_■)

v0.4 Klisp can now evaluate polish notation expressions entered by the user. See screenshot above.

Installation
============
gcc -std=c99 -Wall klisp.c mpc.c -o parsing

Notes
=====
The MPC parser combinator is copyrighted by Daniel Holden and is available under the BSD license https://github.com/orangeduck/mpc/blob/master/LICENSE.md
