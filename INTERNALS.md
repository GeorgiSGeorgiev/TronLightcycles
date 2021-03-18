# INTERNALS

* main.cpp
  * basic initializations of the Main menu, the game Window (the whole playing field), the Grid, the main View (the camera settings), the different lightcycles, the AI, the Collidable class and the small User interface of the game
  * keyboard controls handling
  * the main while loop which drives the whole game: it switches between the start menu and the actual game, updates the game each frame and draws everything on the main window

* MainMenu.h/.cpp
  * definition of the menu buttons
  * different keyboard controls which change the button selection
  * variable which stores the selected game difficulty
* TheGrid.h/.cpp
  * definition of the game borders
  * definition of the drawing function which draws the "ground"
* Lightcycle.h/.cpp
  * definition of the lightcycle itself (its body)
  * definition of the lightcycle movement (+physics)
  * main lightcycle collision detection
  * lightcycle turning behavior and important constants
  * function that spawns the lighttrace and function that destroys it
  * function that determines the possible movement directions of the cycle (important for the AI)
  * the lightcycle explosion which stops the machine from moving

* Collidable.h/.cpp
  * a vector of pointers which point to different objects considered as obstacles and the lightcycles can crash into them
  * a function which takes a lightcycle and calculates the distance to the closest collidable in all of the desired directions (to the left, to the front, to the right)
  * a function which detects a collision between the lightcycle from the argument and a collidable. It simply calls the collision detection of the cycle but is very important because the Collidable class actually contains (in my case all of the) collidable objects

* AI.h/.cpp
  * contains the AI's lightcycle and a vector of its targets (I have just one target for each AI: the user's lightcycle)
  * update_ai updates and draws the AI's lightcycle
  * **make_turn** function which finds the closest collidables, calculates the values of each possible turn and at the end makes the turn with best value (this is the actual "brain" of the AI, so it's the most complicated functions mainly because of the constraints in it which try to describe as much as possible interesting situations)
  * timer which limits the number of turns made in a row (used to simulate human-like behavior)