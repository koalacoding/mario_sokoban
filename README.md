# Project : Openclassrooms Mario Sokoban
![mario_sokoban](https://github.com/koalacoding/images/blob/master/mario_sokoban.png?raw=true)

https://github.com/koalacoding/mario_sokoban.git

Mario Sokoban is a 2d game coded in C.

You must place all the boxes on the green objectives to finish a level.

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

### Finished steps

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
17. Added a button to save the edited map in the map editor.
18. Added a button in the map editor to go back to the menu.
19. Every new saved map is stored in an unique file.
20. Added a select map window.
21. Give the possibility to the player to choose the map.
22. Make a vertical navigation bar in the select map window to give the opportunity to the player to choose between all the maps.
23. Give the possibility to the player to go to the main menu from the game.
24. Delete the SDL_Surface *window parameter in the function load_main_window.
25. Added Mario starting position as an sprite proposition in the map editor.
26. Mario can only be blited at one position in the map editor.
27. Mario starting position is now shown in the select map window.
28. When all boxes are placed, we are redirected to the main window.
29. Key repeating is now enabled in game.
30. Bug fixed : In game, Mario can no longer walk into the right menu (i.e go outside the right border of the map).

### To do
