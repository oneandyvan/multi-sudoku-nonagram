// -1 denotes a blank space
// 0-9 denotes a number in the board

const int WIDTH = 9;
const int HEIGHT = 9;

#include <iostream>
#include <fstream>
#include <iomanip>

class Game {
    public:
        //2d array to store the cells of our board
        int board [WIDTH][HEIGHT];

        //initializes board with -1 values
        Game(){ 
            for(int i = 0; i < HEIGHT; i++){
                for(int j = 0; j < WIDTH; j++){
                    board[i][j] = -1;
                }
            }
        }

        //takes in filepath to test case and reads the input into the board
        void readInput(char* filename){
            char value;
            std::ifstream file;
            file.open(filename);
            if(file.is_open()){
                for (int i = 0; i < HEIGHT; i++){
                    for(int j = 0; j < WIDTH; j++){
                        file >> value;
                        if(isdigit(value)){
                            board[i][j] = value - '0';
                        }else{
                            board[i][j] = -1;
                        }
                    }
                }
            }
            file.close();
        }

        //prints board with equal spacing
        void printBoard(){
            for (int i = 0; i < HEIGHT; i++){
                if (i % 3 == 0){
                    std::cout << "------------------------------\n";
                }

                for(int j = 0; j < WIDTH; j++){
                    if(board[i][j] == -1){
                        std::cout << std::setw(2) << "_" << " ";
                    }else{ 
                        std::cout << std::setw(2) << board[i][j] << " ";
                    }

                    if(j % 3 == 2){
                        std::cout << "|";
                    }
                }
                std::cout << std::endl;
            }
        }
};

int main(int argc, char* argv[]){
    Game game;

    if(argc > 1){
        game.readInput(argv[1]);
    }

    game.printBoard();
    return 0;
}