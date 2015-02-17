# Project : Openclassrooms Mario Sokoban

Mario Sokoban game coded in C

### Dependencies
```
apt-get install libsdl1.2-dev libsdl1.2-image
```

### Needed options when building with Code::blocks :

#### Link librairies : 

SDL_image
SDL_ttf

#### Other linker options :

-lSDLmain
-lSDL

### Files list :

* main.c : The file containing the main function.
* prototypes.h : The file containing the prototypes.

### Files content
main.c :

The window showing the game must be devided into squares. We will start off
with a window with a fixed size. 

The window will be devided into 12x12 squares.

Each square will be filled with an appropriate sprite (box, Mario, box
objective, and free space).

The player will use the arrow keys to move Mario. Mario can only move
square per square, and only on squares containing a box objective sprite
or a free space sprite.

The first step of the programming will be to initialize a white window with
the Mario sprite on it.

Then, we will divide the window into 12x12 squares.

Then...

Steps finished :

1. Divide the window into 144 squares (12x12).
2. With the map data, load all the images of the map with the right size and the right position.
3. Show all the loaded images on the screen (Blitsurface).
4. Free all the surfaces containing an image using a loop (SDL_FreeSurface).
5. Put Mario on the screen, then add the possibility to make him move on the map square by square.
6. Prevent Mario to move outside the window.
7. Mario must be enable to go in wall and objectives squares.
8. Give the possibility to Mario to move boxes.
9. Boxes placed on objectives cannot be moved and their image change.
10. Add a starting menu.
11. Added the game in a single function.
12. Add a functional main menu.
13. Added a new window for the map editor.
14. Divided the map editor window into black border blank squares.
15. Added a space to the right of the map editor window to place the different sprites.
16. Program a map editor.

### To do

1. Propose a button to save the edited map in the map editor.
