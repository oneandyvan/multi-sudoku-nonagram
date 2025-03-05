//blank spaces are stored as -1. 
//Otherwise digits are stored as their integer value.

#include <iostream>
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

    return 0;
}
