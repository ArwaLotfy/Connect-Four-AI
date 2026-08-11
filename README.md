# Connect Four AI

A C++ implementation of the classic **Connect Four** game featuring a custom graphical user interface and an intelligent AI opponent built using advanced algorithmic techniques.

The project combines **game development, object-oriented programming, and artificial intelligence**, using Alpha-Beta pruning, heuristic evaluation, move ordering, and Zobrist hashing to create an optimized game-playing AI.

## Features

* **Custom Graphical User Interface**
* **AI opponent** powered by Alpha-Beta pruning
* **Heuristic board evaluation**
* **Move ordering** to improve search efficiency
* **Transposition table** for caching previously evaluated positions
* **Zobrist hashing** for efficient board-state identification
* **Search performance statistics**
* Horizontal, vertical, and diagonal win detection
* Move undo functionality during AI search
* Input validation and game-state handling

## Graphical User Interface

The project includes a custom-built GUI that provides an interactive way to play Connect Four against the AI.

The interface handles:

* Board visualization
* Player and AI moves
* Turn management
* Game-state updates
* Win and draw notifications
* User interaction and input

The GUI separates the visual/gameplay layer from the underlying AI and board logic, allowing the algorithms to operate independently from the interface.

## How the AI Works

The AI uses **Alpha-Beta pruning**, an optimization of the Minimax algorithm, to search through possible future game states.

At each turn, the AI:

1. Checks for an immediate winning move.
2. Checks for immediate opponent threats.
3. Explores possible moves using Alpha-Beta search.
4. Evaluates non-terminal positions using a heuristic function.
5. Uses move ordering to examine promising moves first.
6. Stores evaluated board states in a transposition table.
7. Selects the move with the highest evaluated score.

### Alpha-Beta Pruning

Instead of exploring every possible branch of the game tree, Alpha-Beta pruning eliminates branches that cannot influence the final decision.

The algorithm maintains:

* **Alpha** — the best guaranteed score for the maximizing player.
* **Beta** — the best guaranteed score for the minimizing player.

When `alpha >= beta`, the remaining branches can be skipped.

The project also tracks explored and pruned nodes to measure search performance.

### Heuristic Evaluation

When the search reaches its depth limit, the AI evaluates the current board using a heuristic function.

The evaluation considers factors such as:

* Potential four-piece winning sequences
* Three-piece threats
* Two-piece opportunities
* Opponent threats
* Center-column control

The center of the board receives a higher weight because controlling the center generally creates more opportunities for horizontal and diagonal connections.

### Move Ordering

The AI prioritizes promising columns first, with the center of the board examined before the outer columns.

This improves Alpha-Beta pruning by allowing strong moves to establish better alpha/beta bounds earlier in the search.

### Transposition Table & Zobrist Hashing

Different sequences of moves can result in the same board position. Re-evaluating these positions unnecessarily increases the search cost.

To address this, the project uses a **transposition table** to cache previously evaluated positions.

Board states are identified using **Zobrist hashing**, which generates a compact hash representation of the current game state.

Stored information includes:

* Evaluated position score
* Search depth

This allows the AI to reuse previously calculated results when possible.

## Project Structure

```text
Connect-Four/
│
├── include/
│   ├── AlphaBeta.h
│   ├── Board.h
│   ├── Heuristic.h
│   ├── Statistics.h
│   └── TranspositionTable.h
│
├── src/
│   ├── AlphaBeta.cpp
│   ├── Board.cpp
│   ├── Heuristic.cpp
│   ├── Statistics.cpp
│   └── TranspositionTable.cpp
│
├── assets/
│   └── [GUI assets]
│
└── main.cpp
```

## Main Components

| Component            | Responsibility                                                |
| -------------------- | ------------------------------------------------------------- |
| `Board`              | Manages the game board, moves, win detection, and board state |
| `AlphaBeta`          | Implements the AI search algorithm                            |
| `Heuristic`          | Evaluates the quality of board positions                      |
| `TranspositionTable` | Caches previously evaluated board states                      |
| `Statistics`         | Tracks explored and pruned search nodes                       |
| `GUI`                | Handles the graphical interface and user interaction          |
| `main.cpp`           | Initializes and runs the application                          |

## Technologies & Concepts

### Technologies

* **C++**
* **Graphical User Interface**

### Algorithms

* Minimax / adversarial search
* Alpha-Beta pruning
* Heuristic evaluation
* Move ordering
* Zobrist hashing

### Data Structures

* 2D arrays
* Hash tables / `std::unordered_map`
* Game-state representations

### Programming Concepts

* Object-oriented programming
* Modular software design
* Recursion
* State-space search
* Algorithm optimization
* GUI development

## Running the Project

### 1. Clone the repository

```bash
git clone https://github.com/YOUR-USERNAME/connect-four-ai.git
cd connect-four-ai
```

### 2. Build the project

Compile the project according to the GUI framework and build configuration used by the project.

### 3. Run

Launch the generated executable to start the graphical Connect Four game.

## Gameplay

The GUI allows the player to interact directly with the Connect Four board and play against the AI.

The goal is to connect four pieces:

* Horizontally
* Vertically
* Diagonally

before the opponent.

The AI analyzes possible future game states and selects its move based on the search algorithm and heuristic evaluation.

## AI Search Depth

The AI uses a configurable search depth to determine how many moves ahead it analyzes.

Increasing the search depth allows the AI to consider more future possibilities, but also increases computational cost.

The combination of:

**Alpha-Beta pruning + move ordering + heuristic evaluation + transposition tables**

helps reduce unnecessary computation and makes deeper searches more practical.

## Performance Statistics

The AI tracks search statistics such as:

* **Explored nodes** — positions evaluated during the search.
* **Pruned nodes** — branches eliminated through Alpha-Beta pruning.

These statistics can be used to analyze the effectiveness of the optimization techniques.

## Learning Objectives

This project was developed to apply advanced algorithmic concepts to a practical game-playing problem.

Key learning outcomes include:

* Implementing adversarial search
* Understanding Minimax and Alpha-Beta pruning
* Designing heuristic evaluation functions
* Optimizing search through move ordering
* Implementing transposition tables
* Applying Zobrist hashing
* Measuring algorithm performance
* Developing an interactive graphical interface
* Structuring a larger C++ application into modular components

## Future Improvements

Possible extensions include:

* Iterative deepening
* Dynamic search depth
* More advanced heuristic evaluation
* Additional AI difficulty levels
* Player vs Player mode
* Improved GUI animations and effects
* Detailed AI benchmarking
* Enhanced transposition-table strategies

## Author

**Team Project — 6 members**

Developed as an advanced C++ algorithms project combining **game AI, algorithm optimization, and graphical interface development**.

