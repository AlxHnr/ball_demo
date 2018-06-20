![preview.gif](https://raw.github.com/AlxHnr/ball_demo/master/preview.gif)

This repository contains a copy of the
[DejaVu Sans Mono](https://dejavu-fonts.github.io) font and graphics
created in [GIMP](https://www.gimp.org).

### Controls

* **escape** quit program
* **+** spawn ball
* **-** remove ball
* **f** spawn 50 balls
* **d** remove 50 balls
* **r** remove all balls but one
* **z** increase ball size
* **u** decrease ball size
* **t** reset ball size
* **i** toggle milliseconds per frame and ball count
* **b** toggle background image

## Building

This program requires the development files of the following libraries:

* SDL (_not_ SDL2)
* SDL\_image
* SDL\_gfx
* SDL\_ttf

Build and run the game using `make run`.
