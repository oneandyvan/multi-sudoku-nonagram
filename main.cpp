//blank spaces are stored as -1. 
//Otherwise digits are stored as their integer value.

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;

const int WIDTH = 9;
const int HEIGHT = 9;

class Sudoku
{
public:
    int currentBoard[WIDTH][HEIGHT];
    int completedBoard[WIDTH][HEIGHT];

    Sudoku()
    {
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                currentBoard[i][j] = -1;
                completedBoard[i][j] = -1;
            }
        }
    }

    //reads input from .txt file
    void readInput(const char *filename)
    {
        char value;
        ifstream file(filename);

        // Read current board
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                file >> value;
                currentBoard[i][j] = isdigit(value) ? value - '0' : -1;
            }
        }

        // Read completed board
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                file >> value;
                completedBoard[i][j] = isdigit(value) ? value - '0' : -1;
            }
        }

        file.close();
    }
    
    //prints a board
    void printBoard(const int board[WIDTH][HEIGHT])
    {
        for (int i = 0; i < HEIGHT; i++)
        {
            if (i % 3 == 0)
            {
                cout << "------------------------------\n";
            }

            for (int j = 0; j < WIDTH; j++)
            {
                if (board[i][j] == -1)
                {
                    cout << setw(2) << "_" << " ";
                }
                else
                {
                    cout << setw(2) << board[i][j] << " ";
                }

                if (j % 3 == 2)
                {
                    cout << "|";
                }
            }
            cout << endl;
        }
        cout << "------------------------------\n";
    }

    //returns true if there are no mistakes in the current board (blank spaces are ok).
    bool isCorrect(){
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                if(isdigit(currentBoard[i][j]) && currentBoard[i][j] != completedBoard[i][j]){
                    return false;
                }
            }
        }
        return true;
    }

    bool isValid(int row, int col, int num){
        //checks row
        for(int i = 0; i < WIDTH; i++){
            if(currentBoard[row][i] == num){
                return false;
            }
        }

        //checks col
        for(int i = 0; i < HEIGHT; i++){
            if(currentBoard[i][col] == num){
                return false;
            }
        }

        //checks 3x3 grid
        int rowLoc = row - row % 3;
        int colLoc = col - col % 3;

        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(currentBoard[i + rowLoc][j + colLoc] == num){
                    return false;
                }
            }
        }

        return true;
    }

    //  Checks if placement of number is valid with sudoku rules and current info using (Parallel)
    bool isValidParallel(int row, int col, int num){
        bool rowValid = true, colValid = true, subgridValid = true;
        vector<thread> threads;

        // Create threads for row, column, and subgrid checks
        threads.emplace_back(&Sudoku::isRowValid, this, row, num, ref(rowValid));
        threads.emplace_back(&Sudoku::isColValid, this, col, num, ref(colValid));
        threads.emplace_back(&Sudoku::isSubgridValid, this, row, col, num, ref(subgridValid));

        // Wait for threads to finish
        for (auto &t : threads) {
            t.join();
        }

        // Return true if all checks passed
        return rowValid && colValid && subgridValid;
    }

    //Attempts to solve sudoku via recursion (Parallel)
    bool solve(){
        for(int i = 0; i < HEIGHT; i++)
        {
            for(int j = 0; j < WIDTH; j++)
            {
                //Tries 1-9 on blank space
                if(currentBoard[i][j] == -1)
                {
                    for(int num = 1; num <= 9; num++)
                    {
                        if(isValid(i,j,num))
                        {

                            currentBoard[i][j] = num;

                            if(solve())
                            {
                                return true;
                            }

                            currentBoard[i][j] = -1;
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

    //Attempts to solve sudoku via recursion (Parallel)
    bool solveParallel(){
        for(int i = 0; i < HEIGHT; i++)
        {
            for(int j = 0; j < WIDTH; j++)
            {
                //Tries 1-9 on blank space
                if(currentBoard[i][j] == -1)
                {
                    for(int num = 1; num <= 9; num++)
                    {
                        if(isValidParallel(i,j,num))
                        {

                            currentBoard[i][j] = num;

                            if(solveParallel())
                            {
                                return true;
                            }

                            currentBoard[i][j] = -1;
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

private:
    //  Checks Row Constraint (Split for Multi-threading)
    void isRowValid(int row, int num, bool &result)
    {
        for(int i = 0; i < WIDTH; i++){
            if(currentBoard[row][i] == num){
                result = false;
                return;
            }
        }
    }

    //  Checks Column Constraint (Split for Multi-threading)
    void isColValid(int col, int num, bool &result)
    {
        for(int i = 0; i < HEIGHT; i++){
            if(currentBoard[i][col] == num){
                result = false;
                return;
            }
        }
    }

    //  Checks Subgrid (3x3) Constraint (Split for Multi-threading)
    void isSubgridValid(int row, int col, int num, bool &result)
    {
        int rowLoc = row - row % 3;
        int colLoc = col - col % 3;

        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(currentBoard[i + rowLoc][j + colLoc] == num){
                    result = false;
                    return;
                }
            }
        }
    }
};

class Nonogram
{

};

void runSudoku(char* fileName)
{
    Sudoku sudoku;
    Sudoku sudokuSequential;

    //  Sequential Sudoku
    sudoku.readInput(fileName);

    cout << "Current board:\n";
    sudoku.printBoard(sudoku.currentBoard);

    cout << "Completed board:\n";
    sudoku.printBoard(sudoku.completedBoard);

    if(sudoku.isCorrect()){
        cout << "The current board is correct.\n";
    }
    else{
        cout << "The current board is not correct.\n";
    }

    auto startTimeSudoku = chrono::high_resolution_clock::now();
    if(sudoku.solve()){
        cout << "The current board was solved.\n";
        sudoku.printBoard(sudoku.currentBoard);
    }else{
        cout << "The current board was NOT solved.\n";
    }

    auto durationSudoku = chrono::high_resolution_clock::now() - startTimeSudoku;
    cout << "Sudoku Sudoku Time = " << chrono::duration_cast<chrono::milliseconds>(durationSudoku).count() << " ms\n" << endl;


    //  Sequential Sudoku
    sudokuSequential.readInput(fileName);

    cout << "Current board:\n";
    sudokuSequential.printBoard(sudokuSequential.currentBoard);

    cout << "Completed board:\n";
    sudokuSequential.printBoard(sudokuSequential.completedBoard);

    if(sudokuSequential.isCorrect()){
        cout << "The current board is correct.\n";
    }
    else{
        cout << "The current board is not correct.\n";
    }

    auto startTimeParSudoku = chrono::high_resolution_clock::now();
    if(sudokuSequential.solveParallel()){
        cout << "The current board was solved.\n";
        sudokuSequential.printBoard(sudokuSequential.currentBoard);
    }else{
        cout << "The current board was NOT solved.\n";
    }

    auto parDurationSudoku = chrono::high_resolution_clock::now() - startTimeParSudoku;
    cout << "Parallel Sudoku Time = " << chrono::duration_cast<chrono::milliseconds>(parDurationSudoku).count() << " ms\n" << endl;
}

void runNonogram()
{
    
}


int main(int argc, char *argv[])
{
    if(argc > 2)
    {
        if(strcmp(argv[1], "sudoku") == 0)
        {
            runSudoku(argv[2]);
        }   
        else if(strcmp(argv[1], "nonogram") == 0)
        {

        }
        else
        {
            cout << "Invalid puzzle option!";
        }
    }

    return 0;
}
