TODO: write this boi
- what is it about
- how much is implemented, what is not implemented fully and what wasn't implemented at all
- authors
- notes

**********************************************************************************************
I recommend using the following architectural and design patterns for the Pac-Man application:

- Model-View-Controller (MVC) Pattern: 
This pattern will allow you to separate the core logic (model) 
from the user interface (view) and the way they interact (controller). 
This separation will make it easier to maintain and modify the code in the future.
The model will include the maze, Pac-Man, ghosts, and keys; the view will handle the graphical representation of the game, 
and the controller will process user inputs and update the model accordingly.

- Observer Pattern: 
This pattern can be used to notify the view whenever there's a change in the model. 
For instance, when Pac-Man moves or a ghost moves, the view will be updated to reflect the new positions. 
You can implement this using signals and slots in Qt.

- Command Pattern: 
This pattern can be used to handle the various user inputs (keyboard or mouse) and 
execute the corresponding actions in the model. This approach will make it easier to modify or 
add new commands without affecting the existing code.

- Singleton Pattern: 
This pattern can be used to manage the game state, ensuring that there's only one instance of the game. 
This instance will hold information about the current level, score, lives, and other relevant game data.

- Factory Pattern: 
This pattern can be used to create various maze elements (such as walls, ghosts, and keys) 
based on the input text file. This approach will make it easier to add new maze elements in the future 
without modifying the existing code.

Here's a high-level implementation plan for the Pac-Man application:

1. Define the maze, Pac-Man, ghosts, and keys classes (model) based on their required properties and behaviors.

2. Implement the view using Qt for creating the game window, rendering the maze, and displaying interactive elements (such as the ghost's biography, number of steps, etc.).

3. Implement the controller to handle user inputs (keyboard or mouse) and execute the corresponding commands in the model (e.g., move Pac-Man, collect keys, etc.).

4. Implement the Observer pattern using Qt's signals and slots to update the view whenever there's a change in the model.

5. Create a game state class using the Singleton pattern to manage the game's data (current level, score, lives, etc.).

6. Implement the Factory pattern to create maze elements (walls, ghosts, keys) based on the input text file.

7. Implement the functionality for logging the game progress and playing back the saved game in the required modes (smooth stepping, sequential stepping, etc.).

8. Ensure the application meets the minimum requirements for credit and is compatible with both Linux and Windows platforms.

9. Document the program design in source code format for the "doxygen" program and use appropriate design patterns as described in the assignment. 