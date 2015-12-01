Rage Against the Machines
=========================

A Robot Game.

The player (indicated by @) plays in a rectangular arena filled 
with killer robots (indicated by R, or a number which means 
the number of robots). At each turn, the user will select 
an action for the player to take. The player will take the 
action, and then each robot will move one step in a random 
direction. If a robot lands on the position occupied by 
the player, the player dies.

*This project is an extension of a in-class project*

Before you play the game or run the test, be sure to make the 
following files executable with the command `chmod +x <filename>`

- check_output.py
- play
- test
- experiment/test_robots.py

**To play the game, run `./play`**

The game is configured with a default board 
of 7 &#xd7; 8 and 25 robots, which is kind 
of challenging. To run a custom version, 
run it with command line arguments 
`./play <row size> <column size> <robot number>`.
For example, `./play 3 4 2`

This project is also bundled with a test,
to run the test, first make all members of
all classes in robots.h file public, and run `./test`

There is also a experimental test bundled with
the project under the folder `experiment/`. The
experimental test handles everything more automatically
with only one python script. To run the experimental
test, navigate to the folder `experiment/`, and run
`./test_robots.py`

The make process is automatically handled for you,
so you don't need to worry anything about compiling or
cleaning up.
