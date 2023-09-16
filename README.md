# Some `ncurses` Snakes!

**Developing a simple version of Snake in various programming languages!**

Each folder contains a version of the game Snake for terminal that interfaces with `ncurses` to provide the game UI, or some other library in case it's not available.

I thought of this idea to learn Go, but now I want to expand it a little, just to give it a touch of many different languages.

**Make sure to check the implementation-specific READMEs, not just this one!**

## Current Implementations

+ C
+ That's it for now :smile:

## C Version

The possible Snake algorithms I had thought of for the game were:

+ Store a list of snake node coordinates, direction, and next_direction
+ **Store a list of snake rotations and only the head coordinates, then draw backward and rotate when meeting rotation points**
+ Use a delayed tail approach, which moves the same as the head but after N steps and clears nodes instead of applying them

Since C can be considered lower level than other popular languages, I wanted to make something a bit more interesting algorithmically, so I decided to implement the snake by using a linked list of rotations (the second one). This allows the program to use memory way more efficiently to store the Snake!

Inside the `c/` folder. You can find a few `.c` and `.h` and build script for Snake. Just execute the script and run `snake` once complete!

Here is an Asciinema Demo of the C version:

<a href="https://asciinema.org/a/608243" target="_blank"><img src="https://asciinema.org/a/608243.svg" /></a>