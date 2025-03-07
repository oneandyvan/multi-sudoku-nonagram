# Multi-threaded Solver for Combinatorial Logic Puzzles

## Overview
This project is for **COP4520 (Spring 2025)** and aims to implement a **multi-threaded solver** for **Sudoku** and **Nonogram** puzzles. The project focuses on leveraging parallel algorithms to enhance performance while solving these combinatorial logic puzzles.

## Challenges, Tasks, and Goals
- Implement efficient **multi-threaded** solvers for **Sudoku** and **Nonograms**.
- Analyze and optimize different algorithms, such as:
  - **Backtracking**
  - **Dancing Links**
  - **2-SAT Problem Reduction**
  - **Depth-First Search (DFS)**
- Implement **parallel methodologies** to improve execution time.
- Ensure correctness while maintaining a **balance between performance and computational complexity**.
- Compare and contrast different approaches to solving these NP-complete problems.
- Conduct extensive benchmarking to analyze the speedup from multi-threading.
- Identify the **limitations and tradeoffs** of different algorithmic approaches.

---

## Report
Here is a rough draft of the [paper](https://docs.google.com/document/d/1dcpnFOEYQ8LSIxTercKv7PKMg142GDkCQVmDmy0G9bc/edit?usp=sharing)
This is a live document for collaboration purposes. For the milestone assignment, check the attached PDF in the submission to have an accurate view of work up to that point.

### Authors:
- **Andrew Browning** - Undergrad, Dept. Computer Science, University of Central Florida
- **Ohm Patel** - Undergrad, Dept. Computer Science, University of Central Florida
- **Kyle Seney** - Undergrad, Dept. Computer Science, University of Central Florida
- **Andy Van** - Undergrad, Dept. Computer Science, University of Central Florida

---

## Abstract
Both **Sudoku** and **Nonograms** are puzzles that can be examined through **combinatorial** lenses and abstracted into similar **computer science** problems. We analyze various algorithms used to solve these puzzles, focusing on **brute-force** approaches and **decision pruning**. Our goal is to **apply multi-threading** to improve performance and analyze the tradeoffs involved. 

---

## Compilation Instructions

1. **Compile the Solver**  
   ```bash
   g++ main.cpp -o sudoku
   ```
   This will produce an executable named `sudoku`.

2. **Run the Solver**  
   ```bash
   ./sudoku test1.txt
   ```
   Replace `test1.txt` with the path to your puzzle file. The file should have:
   - **First 9 lines:** The current (incomplete) Sudoku board, where underscores (`_`) or other non-digit characters are interpreted as blank spaces (`-1`).
   - **Next 9 lines:** The completed Sudoku board for validation.

3. **Output**  
   - The program prints the current board and the completed board.  
   - It checks if the current board is correct (i.e., no contradictions so far).  
   - If possible, it attempts to solve the Sudoku using a backtracking algorithm and prints the solved board.

4. **Notes**  
   - Blank spaces are stored as `-1` internally.  
   - Digits are stored at their integer values (`1` through `9`).  
   - The code checks row, column, and 3×3 box constraints before placing any number.  
   - The solver uses recursion to fill in valid candidates until the puzzle is solved or deemed unsolvable.
---

## Introduction
### Sudoku
**Sudoku** is a 9×9 matrix divided into 3×3 subsections, where:
- Each cell can hold an integer between **1 and 9**.
- Some cells contain **immutable hints**.
- A valid solution must satisfy:
  1. Each **3×3 subsection** contains **each digit exactly once**.
  2. Each **row** contains **each digit exactly once**.
  3. Each **column** contains **each digit exactly once**.

**Complexity:** The generalized **n² Sudoku problem** belongs to **NP-complete** class.

### Nonograms
Also known as **Picross**, **Hanjie**, or **Paint by Numbers**, Nonograms consist of **n×m grids** with **clues** for each **row** and **column**:
- Clues specify **groups of consecutive filled cells**.
- Cells are either **filled or unfilled**.
- The goal is to create a **valid binary image** following the given constraints.

Nonograms, like Sudoku, are **NP-complete** and may have **multiple solutions** or **no solution**.

---

## Background
### **Sudoku Solving Approaches**
#### 1. Backtracking
- **Recursive brute-force** method that tries each value in a cell.
- If an invalid state is reached, the algorithm **backtracks** to the previous step.
- Used for **constraint satisfaction problems (CSPs)**.

#### 2. Exact Cover Problem and Dancing Links
- **Sudoku can be transformed** into an **exact cover problem**.
- **Dancing Links (DLX)** is an efficient technique to solve exact cover problems.
- DLX provides **faster pruning** than brute-force backtracking.

---

### **Nonogram Solving Approaches**
#### 1. Depth-First Search (DFS)
- DFS explores all possible placements **depth-first** and prunes **invalid states** early.

#### 2. 2-SAT Problem Reduction
- Nonogram constraints can be represented using **boolean satisfiability (SAT)**.
- **2-SAT solvers** provide an efficient way to validate puzzle constraints.

---

## Parallel Methodology
- **Multi-threaded Backtracking**
- **Parallel Dancing Links**
- **Parallel DFS for Nonograms**
- **Optimized constraint propagation with multi-threading**

---

## Implementation
### Sudoku:
- Implemented **backtracking solver** with **multi-threading**.
- Implemented **Dancing Links (DLX)** for improved performance.

### Nonograms:
- Used **DFS** and **constraint propagation** techniques.
- Applied **2-SAT reduction** to improve efficiency.

---

## Results
- **Comparison of single-threaded vs. multi-threaded performance.**
- **Benchmarking of different solving techniques.**
- **Tradeoff analysis for parallel execution.**

---

## Limitations
- **Increased memory usage** due to thread management.
- **Difficulties in parallelizing constraint-heavy solvers**.
- **Scalability issues** for very large puzzles.

---

## Conclusion
- **Multi-threading significantly improves performance** for solving Sudoku and Nonograms.
- **Certain algorithms (like DLX) outperform brute-force methods** in many cases.
- **Further optimizations** can be explored, including **GPU acceleration**.

---

## References
- *The Effect of Guess Choices on the Efficiency of a Backtracking Algorithm in a Sudoku Solver* ([IEEE Xplore](https://ieeexplore.ieee.org/document/6845190))
- *Techniques for Solving Sudoku Puzzles* ([arXiv](https://arxiv.org/pdf/1203.2295))
- *Solving Sudoku efficiently with Dancing Links* ([DiVA Portal](https://www.diva-portal.org/smash/record.jsf?pid=diva2%3A770655&dswid=2627))
- *Solving Nonograms by Combining Relaxations* ([ScienceDirect](https://www.sciencedirect.com/science/article/pii/S0031320308005153))
- *Algorithms Effectiveness Comparison in Solving Nonogram Boards* ([ScienceDirect](https://www.sciencedirect.com/science/article/pii/S1877050921016902))

---

## License
This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.

---

## Contributors
- **Andrew Browning**
- **Ohm Patel**
- **Kyle Seney**
- **Andy Van**
