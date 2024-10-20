# Chess Game

This project is a classic implementation of a chess game where the player can compete against an artificial intelligence. 
The game provides an intuitive visual interface: moves are displayed on the left-hand panel at the bottom, the current player is shown on the top left, and captured pieces are shown on the right.
The main goal of this project was to dive deeper into learning the Qt library and to demonstrate my skills in user interface development and artificial intelligence logic. 
The project also allows me to explore complex algorithms, such as Minimax with alpha-beta pruning, and apply them in a real game.

## Key Features of the Project

1. **Play against AI**: The player competes against an opponent controlled by the Minimax algorithm with alpha-beta pruning.
2. **Step-by-step interface**: All moves are displayed in the left sidebar.
3. **Captured pieces display**: The right panel shows the pieces that have been captured during the game.
4. **Current player indicator**: It indicates whose turn it is — white or black.
5. **Game Menu**: Includes buttons to start a new game or exit the game.
6. **Side Selection**: At the start of a new game, the player can choose which side they will play, either white or black.

## Screenshots of the game

- Choosing a side:
 
![Choosing a side](./Screenshots%20of%20the%20game/newGame.PNG)

- Start of the game:
 
![Start of the game](./Screenshots%20of%20the%20game/game%20start.PNG)

- Gameplay:
 
![Gameplay](./Screenshots%20of%20the%20game/gameplay.PNG)

- Game menu:
  
![Game menu](./Screenshots%20of%20the%20game/game%20menu.PNG)

## Technologies Used

### Programming Language:
- **C++**: The primary programming language used for developing the game logic, implementing all classes, and algorithms.

### Build System:
- **CMake**: Used as a cross-platform build system to manage the project's build process, ensuring that all dependencies are correctly compiled and linked.
 
### Libraries and Frameworks:
- **QGraphicsView, QGraphicsScene, QGraphicsRectItem, QGraphicsPixmapItem**: Used for rendering and managing the graphical elements of the game, including the chessboard and pieces.
- **QLabel**: For displaying text and captured pieces in the interface.
- **QPushButton**: Buttons for controlling the game, such as starting a new game or exiting.
- **QVBoxLayout, QGridLayout**: Used to organize UI elements such as buttons and labels.
- **QDialog**: To display message dialogs, like the end-of-game dialog.
- **QVector**: A dynamic array structure from the Qt library, used for storing and managing chess pieces and board cells efficiently.
- **QList**: A dynamic list structure from the Qt library, used to manage QLabel widgets representing captured pieces for efficient insertion and removal operations.
  
### Algorithm for AI:
- **Minimax Algorithm with Alpha-Beta Pruning**: Implemented for artificial intelligence, allowing the computer to calculate optimal moves efficiently by minimizing the number of nodes evaluated in the game tree.

## Used Resources

- **Minimax Algorithm with Alpha-Beta Pruning**:  
  [Skillbox Article on Habr](https://habr.com/ru/companies/skillbox/articles/437524/)
  
  [Alpha-Beta Pruning in AI - Medium Article](https://medium.com/edureka/alpha-beta-pruning-in-ai-b47ee5500f9a)
- **Official Qt Documentation**:  
  Extensively used to understand and implement various Qt features.
- **Video Tutorials on Qt**:  
  [YouTube Playlist by VoidRealms](https://www.youtube.com/watch?v=9lqhMLFHj3A&list=PLMgDVIa0Pg8WrI9WmZR09xAbfXyfkqKWy&index=1)  
  These were particularly helpful for working with the game scene.
- **Book**:  
  "Qt 5.10. Professional Programming in C++" by Max Schlee.  
  Provided in-depth insights into using Qt and C++ effectively.
