#! /usr/bin/env python

from __future__ import print_function

import re
import sys
import subprocess


SUCCESS_MSG = (
    "\n"
    "*************"
    "** SUCCESS **"
    "*************"
    "\n"
)

FAILURE_MSG = (
    "\n"
    "*************"
    "** FAILURE **"
    "*************"
    "\n"
)

PLAYER_TEST = [
"""\x1b[2J\x1b[H.....
.....
.....
..2@.
...R.


There are 3 robots remaining.
""",

"""\x1b[2J\x1b[H.....
.....
.....
.RR@.
...R.


There are 3 robots remaining.
The player has lasted 1 steps.
""",

"""\x1b[2J\x1b[H.....
.....
.....
.2.@.
...R.


There are 3 robots remaining.
The player has lasted 2 steps.
""",

"""\x1b[2J\x1b[H.....
.....
.....
.2.@.
.....


There are 2 robots remaining.
The player has lasted 3 steps.
""",

"""\x1b[2J\x1b[H.....
.....
.....
.2@..
.....


There are 2 robots remaining.
The player has lasted 4 steps.
""",

"""\x1b[2J\x1b[H.....
.....
.....
.R@..
.....


There are 1 robots remaining.
The player has lasted 5 steps.
""",

"""\x1b[2J\x1b[H.....
.....
.....
..@..
.....


There are 0 robots remaining.
The player has lasted 6 steps.
""",

"""\x1b[2J\x1b[HR@.


There are 1 robots remaining.
""",

"""\x1b[2J\x1b[H.*.


There are 1 robots remaining.
The player is dead.
"""
]

ARENA_TEST = [
"""\x1b[2J\x1b[H.R...
.....
...9.
..R@.
.....


There are 12 robots remaining.
""",

"""\x1b[2J\x1b[H.R...
.....
...9.
..R@.
.....


There are 12 robots remaining.
""",

"""\x1b[2J\x1b[H.R...
.....
...9.
..R*.
.....


There are 13 robots remaining.
The player is dead.
""",

"""\x1b[2J\x1b[H..R.
.R@.
.R..


There are 3 robots remaining.
""",

"""\x1b[2J\x1b[H..R.
R.@.
.R..


There are 3 robots remaining.
""",

"""\x1b[2J\x1b[H....
R.@.
.R..


There are 2 robots remaining.
""",

"""\x1b[2J\x1b[H....
R.@.
.R..


There are 2 robots remaining.
""",

"""\x1b[2J\x1b[H*


There are 1 robots remaining.
""",

"""\x1b[2J\x1b[H*


There are 1 robots remaining.
The player is dead.
"""
]

EXPECTED_OUTPUT = ''.join(PLAYER_TEST) + \
                  ''.join(ARENA_TEST) + \
                  "all assertion test passed\n"

ENCODING = 'utf-8'

COMPILER = 'g++'
FLAG     = '-std=c++11'
FILE     = 'test.cpp'
EXEC     = 'test'
SOURCE   = 'robots.cpp'
HEADER   = 'robots.h'

# match private member declaration of class definition
PATTERN_PRIVATE = re.compile(r'private\s*:')
# match main function signiture
PATTERN_MAIN    = re.compile(r'[(int)(void)]\s+main\s*\(')
MAIN            = re.compile(r'main')


def find_diff(output):
    print("NOTICE: the report below is incomprehensive\n")

    # find the first index where output does not agree
    diff_index = -1
    # print(output)
    for c1, c2 in zip(output, EXPECTED_OUTPUT):
        diff_index += 1
        if c1 != c2:
            break

    test_str_index = 0

    for p_index in range(len(PLAYER_TEST)):
        test_str_index += len(PLAYER_TEST[p_index])
        if test_str_index > diff_index:
            start = test_str_index - len(PLAYER_TEST[p_index])
            end =  test_str_index
            print('\n'.join([
                "In class Player, {}th output\n".format(p_index+1),
                "EXPECTED:\n",
                PLAYER_TEST[p_index],
                "\nGOT:\n",
                output[start:end]
            ]))
            # assert PLAYER_TEST[p_index] != output[start:end]
            return

    for a_index in range(len(ARENA_TEST)):
        test_str_index += len(ARENA_TEST[a_index])
        if test_str_index > diff_index:
            start = test_str_index - len(ARENA_TEST[a_index])
            end = test_str_index
            print('\n'.join([
                "In class Arena, {}th output\n".format(a_index+1),
                "EXPECTED:\n",
                ARENA_TEST[a_index],
                "\nGOT:\n",
                output[start:end],
            ]))
            # assert ARENA_TEST[a_index] != output[start:end]
            return


def main():

    header = open(HEADER, 'w')
    
    with open(SOURCE, 'r') as source:
        for line in source:

            # make all members public 
            if PATTERN_PRIVATE.search(line):
                # print(line)
                header.write('//' + line)

            # find the main function
            # and rename it to something harmless
            elif PATTERN_MAIN.search(line):
                # print(line)
                header.write(MAIN.sub('main_', line))

            else:
                header.write(line)

    header.flush()
    header.close()

    # compile the test file
    # if compile fails, exit the program
    compile_cmd = ' '.join([COMPILER, '-o', EXEC, FILE, FLAG])
    if subprocess.call(compile_cmd,
                       # stdout=subprocess.PIPE,
                       # stderr=subprocess.PIPE,
                       shell=True) != 0:
        # subprocess.call(' '.join(['rm', '-f', HEADER, EXEC]), shell=True)
        sys.exit()

    output = subprocess.check_output('./' + EXEC, shell=True).decode(ENCODING)
    if EXPECTED_OUTPUT == output:
        print(SUCCESS_MSG)
    else:
        print(FAILURE_MSG)
        # print(output)
        find_diff(output)

    subprocess.call(' '.join(['rm', '-f', HEADER, EXEC]), shell=True)


if __name__ == '__main__':
    main()
