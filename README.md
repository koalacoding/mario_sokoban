# Project : Openclassrooms Mario Sokoban

Mario Sokoban game coded in C

### Dependencies
```
apt-get install libsdl1.2-dev libsdl1.2-image
```

### Needed options when building with Code::blocks :

#### Link librairies : 

SDL_image

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

1) Divide the window into 144 squares. (12x12)

### Code example
hello this is some code, if you 
```c
int main() {
  printf("hello world");
  return 0;
  }
```
