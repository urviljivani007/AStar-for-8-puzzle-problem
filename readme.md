# AStar Algorithm for solving 8 puzzle problem.

- Solving 8 Puzzle Problem Using A\* Search with Modifications.

- This C++ program solves the classic 8 puzzle problem using A\* with some modifications.

- Using various heuristic algorithms like Manhattan, Mismatch.
- And measuring their performance using metrics like time required, fringe size and total nodes expanded.

- The only input required is the value depth. It represents how deep you want to search during one call for the `Expand` Function. The goal state is: - 1 2 3 4 5 6 7 8 0 ‘0’ specifying a blank tile.

- The initial states are generated randomly using my `generaterandomstate` function. These are first checked for being solvable before being passed to the `AStar` function.

- The Costs are calculated as: - Total Number of Nodes visited. Maximum size of Priority Queue(fringe).

- The default heuristics are: -
  1.  Manhattan Distance.
  2.  Tiles Mismatch
