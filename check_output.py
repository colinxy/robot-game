#! /usr/bin/env python

import sys

SUCCESS = (
    "\n"
    "*************"
    "** SUCCESS **"
    "*************"
    "\n"
)

FAILURE = (
    "\n"
    "*************"
    "** FAILURE **"
    "*************"
    "\n"
)

PLAYER_TEST = \
"""\x1b[2J\x1b[H.....
.....
.....
..2@.
...R.


There are 3 robots remaining.
\x1b[2J\x1b[H.....
.....
.....
.RR@.
...R.


There are 3 robots remaining.
The player has lasted 1 steps.
\x1b[2J\x1b[H.....
.....
.....
.2.@.
...R.


There are 3 robots remaining.
The player has lasted 2 steps.
\x1b[2J\x1b[H.....
.....
.....
.2.@.
.....


There are 2 robots remaining.
The player has lasted 3 steps.
\x1b[2J\x1b[H.....
.....
.....
.2@..
.....


There are 2 robots remaining.
The player has lasted 4 steps.
\x1b[2J\x1b[H.....
.....
.....
.R@..
.....


There are 1 robots remaining.
The player has lasted 5 steps.
\x1b[2J\x1b[H.....
.....
.....
..@..
.....


There are 0 robots remaining.
The player has lasted 6 steps.
\x1b[2J\x1b[HR@.


There are 1 robots remaining.
\x1b[2J\x1b[H.*.


There are 1 robots remaining.
The player is dead.
"""

ARENA_TEST = \
"""\x1b[2J\x1b[H.R...
.....
...9.
..R@.
.....


There are 12 robots remaining.
\x1b[2J\x1b[H.R...
.....
...9.
..R@.
.....


There are 12 robots remaining.
\x1b[2J\x1b[H.R...
.....
...9.
..R*.
.....


There are 13 robots remaining.
The player is dead.
\x1b[2J\x1b[H..R.
.R@.
.R..


There are 3 robots remaining.
\x1b[2J\x1b[H..R.
R.@.
.R..


There are 3 robots remaining.
\x1b[2J\x1b[H....
R.@.
.R..


There are 2 robots remaining.
\x1b[2J\x1b[H....
R.@.
.R..


There are 2 robots remaining.
\x1b[2J\x1b[H*


There are 1 robots remaining.
\x1b[2J\x1b[H*


There are 1 robots remaining.
The player is dead.
"""

OUTPUT = PLAYER_TEST + ARENA_TEST + "all test passed\n"

stdin = sys.stdin.read()

if stdin == OUTPUT:
    print SUCCESS
else:
    print FAILURE
    print stdin
