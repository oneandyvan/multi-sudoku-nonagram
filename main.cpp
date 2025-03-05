//blank spaces are stored as -1. 
//Otherwise digits are stored as their integer value.

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>

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

    //Checks if placement of number is valid with sudoku rules and current info
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

    //Attempts to solve sudoku via recursion
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

};


int main(int argc, char *argv[])
{
    Sudoku sudoku;

    if (argc > 1)
    {
        sudoku.readInput(argv[1]);
    }

    cout << "Current board:\n";
    sudoku.printBoard(sudoku.currentBoard);

    cout << "Completed board:\n";
    sudoku.printBoard(sudoku.completedBoard);

    if(sudoku.isCorrect()){
        cout << "The current board is correct.\n";
    }

    if(sudoku.solve()){
        cout << "The current board was solved.\n";
        sudoku.printBoard(sudoku.currentBoard);
    }else{
        cout << "The current board was NOT solved.\n";
    }
    return 0;
}
