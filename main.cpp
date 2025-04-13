//blank spaces are stored as -1. 
//Otherwise digits are stored as their integer value.

#include <iostream>
#include <sstream>
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

    //  reads input from .txt file
    void readInput(const char *filename)
    {
        char value;
        ifstream file(filename);

        //  Read current board
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                file >> value;
                currentBoard[i][j] = isdigit(value) ? value - '0' : -1;
            }
        }

        //  Read completed board
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
    
    //  prints a board
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

    //  returns true if there are no mistakes in the current board (blank spaces are ok).
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
        //  checks row
        for(int i = 0; i < WIDTH; i++){
            if(currentBoard[row][i] == num){
                return false;
            }
        }

        //  checks col
        for(int i = 0; i < HEIGHT; i++){
            if(currentBoard[i][col] == num){
                return false;
            }
        }

        //  checks 3x3 grid
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

        //  Create threads for row, column, and subgrid checks
        threads.emplace_back(&Sudoku::isRowValid, this, row, num, ref(rowValid));
        threads.emplace_back(&Sudoku::isColValid, this, col, num, ref(colValid));
        threads.emplace_back(&Sudoku::isSubgridValid, this, row, col, num, ref(subgridValid));

        //  Wait for threads to finish
        for (auto &t : threads) {
            t.join();
        }

        //  Return true if all checks passed
        return rowValid && colValid && subgridValid;
    }

    //Attempts to solve sudoku via recursion 
    bool solve(){
        for(int i = 0; i < HEIGHT; i++)
        {
            for(int j = 0; j < WIDTH; j++)
            {
                //  Tries 1-9 on blank space
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

    //  Attempts to solve sudoku via recursion using parallel valid check
    bool solveParallel(){
        for(int i = 0; i < HEIGHT; i++)
        {
            for(int j = 0; j < WIDTH; j++)
            {
                //  Tries 1-9 on blank space
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
public:
    //  Dimensions of the Nonogram board
    int rows;
    int cols;
    vector<vector<int>> rowConstraints;
    vector<vector<int>> colConstraints;
    vector<vector<int>> workingBoard;
    vector<vector<int>> completedBoard;

    Nonogram() {
        rows = 0;
        cols = 0;
    }

    //  reads input from .txt file
    void readInput(const char *filename) {
        char value;
        ifstream file(filename);

        //  Read current Nonogram board
        file >> rows >> cols;
        file.ignore();

        rowConstraints.resize(rows);
        colConstraints.resize(cols);
        workingBoard.resize(rows, vector<int>(cols, -1));   //  Represents unsolved
        completedBoard.resize(rows, vector<int>(cols));

        for(int i = 0; i < rows; i++)
        {
            string line;
            getline(file, line);
            istringstream iss(line);
            int num;
            while (iss >> num)
            {                
                rowConstraints[i].push_back(num);
            }
        }

        for(int i = 0; i < cols; i++)
        {
            string line;
            getline(file, line);
            istringstream iss(line);
            int num;
            while (iss >> num)
            {
                colConstraints[i].push_back(num);
            }
        }

        //  Read completed Nonogram board
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                file >> completedBoard[i][j];
            }
        }

        file.close();
    }

    //  Print Nonogram board, where filled cells are `#` and blank are `.`
    void printBoard(vector<vector<int>> &board) {
        for (auto& row : board) 
        {
            for (int cell : row) 
            {
                cout << (cell == 1 ? '#' : '.') << ' ';
            }
            cout << '\n';
        }
    }

    //  DFS initial call on first row
    bool solve() {
        return fillRow(0);
    }

private:

    //  For each row line, driver for DFS algorithm
    bool fillRow(int r) {
        //  Base case
        if(r == rows)
        {
            return validateAllColumns();
        }
        
        //  Find possibilities for each row line
        vector<vector<int>> possibilityList;
        vector<int> row(cols, 0);
        findPossibleRows(0, rowConstraints[r], 0, row, possibilityList);

        for (auto& config : possibilityList) {
            workingBoard[r] = config;
            if (validateColumns(r) && fillRow(r + 1)) 
            {
                return true;
            }
        }
        return false;
    }

    // Generates all valid row possibilities for the given row constraint
    void findPossibleRows(int idx, const vector<int>& constraint, int pos, vector<int>& row, vector<vector<int>>& possibilityList) {
        
        //  Base case
        if (idx == constraint.size()) 
        {
            // Fill remaining cells in row with blank `0`
            for (int i = pos; i < cols; i++)
            {
                row[i] = 0;
            }
            possibilityList.push_back(row);
            return;
        }

        //  Filling a row 
        for (int i = pos; i + constraint[idx] <= cols; ++i) 
        {
            // Fill blank `0` up to i
            for (int j = pos; j < i; j++) 
            {
                row[j] = 0;
            }

            // Fill `1` for the section
            for (int j = i; j < i + constraint[idx]; j++) 
            {
                row[j] = 1;
            }

            // Add a blank `0` after section if not last clue
            if (i + constraint[idx] < cols) 
            {
                row[i + constraint[idx]] = 0;
            }

            findPossibleRows(idx + 1, constraint, i + constraint[idx] + 1, row, possibilityList);
        }
    }

    // Verifies working board doesn't violate column constraints so far
    bool validateColumns(int r) {
        for(int j = 0; j < cols; j++)
        {
            int clueIndex = 0;
            int blockLength = 0;
            bool inBlock = false;
    
            for (int i = 0; i <= r; i++)
            {
                int cell = workingBoard[i][j];
    
                if (cell == 1) 
                {
                    //  Either start of block or continue
                    if (!inBlock) 
                    {
                        inBlock = true;
                        blockLength = 1;
    
                        if (clueIndex >= colConstraints[j].size()) 
                        {
                            return false; // too many block sections
                        }
                        if (blockLength > colConstraints[j][clueIndex]) 
                        {
                            return false; //  block length too long
                        }
                    } 
                    else 
                    {
                        blockLength++;
    
                        if (blockLength > colConstraints[j][clueIndex]) 
                        {
                            return false; //  block length too long
                        }
                    }
                } 
                else 
                { 
                    //  Reset "gap" between clue blockss
                    if (inBlock) 
                    {
                        if (blockLength != colConstraints[j][clueIndex]) 
                        {
                            return false; // premature or mismatched block end
                        }
                        clueIndex++;
                        inBlock = false;
                        blockLength = 0;
                    }
                }
            }
    
            // For up to row `r`, we check early:
            // Either unfinished block or finished blocks that don't exceed clue length
            if (inBlock) 
            {
                if (blockLength > colConstraints[j][clueIndex]) 
                {
                    return false;
                }
            }
        }
        
        return true;
    }

    //  Validate column constraints for entire board
    bool validateAllColumns() {
        for(int j = 0; j < cols; j++) 
        {
            vector<int> blockLengths;
            int count = 0;
    
            for(int i = 0; i < rows; i++)
            {
                if(workingBoard[i][j] == 1) 
                {
                    count++;
                } 
                else if(count > 0) 
                {
                    blockLengths.push_back(count);
                    count = 0;
                }
            }

            if(count > 0) 
            {
                blockLengths.push_back(count);
            }
    
            if(blockLengths != colConstraints[j])
            {
                return false;
            }
        }

        return true;
    }
};

void runSudokuSolver(char* fileName)
{
    Sudoku sudokuParallel;
    Sudoku sudokuSequential;

    //  Parallel Sudoku
    sudokuParallel.readInput(fileName);

    cout << "Current board:\n";
    sudokuParallel.printBoard(sudokuParallel.currentBoard);

    cout << "Completed board:\n";
    sudokuParallel.printBoard(sudokuParallel.completedBoard);

    if(sudokuParallel.isCorrect()){
        cout << "The current board is valid.\n";
    }
    else{
        cout << "The current board is not correct.\n";
    }

    auto startTimeSudoku = chrono::high_resolution_clock::now();
    if(sudokuParallel.solveParallel()){
        cout << "\nThe current board was solved. (Parallel)\n";
        sudokuParallel.printBoard(sudokuParallel.currentBoard);
    }else{
        cout << "\nThe current board was NOT solved. (Parallel)\n";
    }

    auto durationSudoku = chrono::high_resolution_clock::now() - startTimeSudoku;
    cout << "Parallel Sudoku Time = " << chrono::duration_cast<chrono::milliseconds>(durationSudoku).count() << " ms\n\n" << endl;


    //  Sequential Sudoku
    sudokuSequential.readInput(fileName);

    cout << "Current board:\n";
    sudokuSequential.printBoard(sudokuSequential.currentBoard);

    cout << "Completed board:\n";
    sudokuSequential.printBoard(sudokuSequential.completedBoard);

    if(sudokuSequential.isCorrect()){
        cout << "The current board is valid.\n";
    }
    else{
        cout << "The current board is not valid.\n";
    }

    auto startTimeSeqSudoku = chrono::high_resolution_clock::now();
    if(sudokuSequential.solve()){
        cout << "\nThe current board was solved. (Sequential)\n";
        sudokuSequential.printBoard(sudokuSequential.currentBoard);
    }else{
        cout << "\nThe current board was NOT solved. (Sequential)\n";
    }

    auto durationSeqSudoku = chrono::high_resolution_clock::now() - startTimeSeqSudoku;
    cout << "Sequential Sudoku Time = " << chrono::duration_cast<chrono::milliseconds>(durationSeqSudoku).count() << " ms" << endl;
}

void runNonogramSolver(char* fileName)
{
    Nonogram seqNonogram;
    Nonogram parNonogram;

    //  Parallel Sudoku
    parNonogram.readInput(fileName);

    //  Sequential Sudoku
    seqNonogram.readInput(fileName);

    cout << "Current board:\n";
    seqNonogram.printBoard(seqNonogram.workingBoard);

    cout << "Completed board:\n";
    seqNonogram.printBoard(seqNonogram.completedBoard);

    auto start_seqNonogram = chrono::high_resolution_clock::now();
    if(seqNonogram.solve()){
        cout << "\nThe current board was solved. (Sequential)\n";
        seqNonogram.printBoard(seqNonogram.workingBoard);
    }else{
        cout << "\nThe current board was NOT solved. (Sequential)\n";
    }

    auto duration_seqNonogram = chrono::high_resolution_clock::now() - start_seqNonogram;
    cout << "Sequential Nonogram Time = " << chrono::duration_cast<chrono::milliseconds>(duration_seqNonogram).count() << " ms" << endl;
}


int main(int argc, char *argv[])
{
    if(argc > 2)
    {
        if(strcmp(argv[1], "sudoku") == 0)
        {
            runSudokuSolver(argv[2]);
        }   
        else if(strcmp(argv[1], "nonogram") == 0)
        {
            runNonogramSolver(argv[2]);
        }
        else
        {
            cout << "Invalid puzzle option!";
        }
    }

    return 0;
}
