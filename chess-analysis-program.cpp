#include <iostream>
#include <conio.h>
#include <fstream>
#include <cctype>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

class Checker {
public:
    Checker(char color, char figure, int row, int col) : color(color), figure(figure), row(row), col(col) {}
    ~Checker(){}

    char getColor() const { return color; }
    char getFigure() const { return figure; }
    int getRow() const { return row; }
    int getCol() const { return col; }

private:
    char color;
    char figure;
    int row;
    int col;
};

int LetterOfRow(char column_char){
    if (column_char == 'A'){
        return 1;
    }if (column_char == 'B'){
        return 2;
    }if (column_char == 'C'){
        return 3;
    }if (column_char == 'D'){
        return 4;
    }if (column_char == 'E'){
        return 5;
    }if (column_char == 'F'){
        return 6;
    }if (column_char == 'G'){
        return 7;
    }if (column_char == 'H'){
        return 8;
    }
    return 0;
}

class Chessboard {
public:

    Chessboard() {}
    ~Chessboard() {}
    void SetChessboardSize(int size){
        count = size;
    }

    //function to get full name if figure
    void figureName(char figure){
        if (figure == 'P') {
            cout << "Pawn";
        }
        if (figure == 'H') {
            cout << "Knight";
        }
        if (figure == 'B') {
            cout << "Bishop";
        }
        if (figure == 'R') {
            cout << "Rook";
        }
        if (figure == 'Q') {
            cout << "Queen";
        }
        if (figure == 'K') {
            cout << "King";
        }
    }

    //function to get Letter from coordinate(column)
    void RowToLetter(int column){
        if (column == 1) {
            cout << "A";
        }
        if (column == 2) {
            cout << "B";
        }
        if (column == 3) {
            cout << "C";
        }
        if (column == 4) {
            cout << "D";
        }
        if (column == 5) {
            cout << "E";
        }
        if (column == 6) {
            cout << "F";
        }
        if (column == 7) {
            cout << "G";
        }
        if (column == 9) {
            cout << "H";
        }
    }

    //function to load data about figures and represent them at terminal
    int loadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            char color, figure, column;
            int row, col;
            while (file >> color >> figure >> column >> row) {
                col = LetterOfRow(column);
                checkers.push_back(Checker(toupper(color), toupper(figure), row, col));
            }
            file.close();
        }
        cout << "Two points of view: " << endl;

        Sides();
        cout << "Black";
        Sides();
        cout << " | ";
        Sides();
        cout << "White";
        Sides();
        cout << endl;

        for (int i = count; i >= 1; --i) {
            for (int j = 1; j <= count; ++j) {
                bool checkerFound = false;
                for (const Checker& checker : checkers) {
                    if (checker.getRow() == i && checker.getCol() == j) {
                        cout << checker.getColor() << checker.getFigure() << " ";
                        checkerFound = true;
                        break;
                    }
                }
                if (!checkerFound) {
                    cout << "[] ";
                }
            }
            cout << "| ";
            for (int j = 1; j <= count; ++j) {
                bool checkerFound = false;
                for (const Checker& checker : checkers) {
                    if (checker.getRow() == count+1-i && checker.getCol() == count+1-j) {
                        cout << checker.getColor() << checker.getFigure() << " ";
                        checkerFound = true;
                        break;
                    }
                }
                if (!checkerFound) {
                    cout << "[] ";
                }
            }
            cout << endl;
        }

        Sides();
        cout << "White";
        Sides();
        cout << " | ";
        Sides();
        cout << "Black";
        Sides();
        cout << endl;

        return 0;
    }

    //function to analyze Kings, If they are under attack
    int analyzeKing(int FlagShow) {
        for (const Checker& checker : checkers) {
            char color = checker.getColor();
            char figure = checker.getFigure();
            int row = checker.getRow();
            int col = checker.getCol();
            int attackCount = 0;

            if ((figure == 'K')&&(KingVulnerability(color, row, col, FlagShow) == 1)){
                return 1;
            }
        }
        return 0;
    }

    //function to analyze positions of figures and in which calls function for calculation of possible attacks
    void analyzePosition() {
        for (const Checker& checker : checkers) {
            char color = checker.getColor();
            char figure = checker.getFigure();
            int row = checker.getRow();
            int col = checker.getCol();
            int attackCount = 0;

            cout << endl;
            figureName(figure);
            cout << " at ";
            RowToLetter(col);
            cout << row << " (Color: ";

            if (color == 'B'){
                cout << "Black";
            }
            if (color == 'W'){
                cout << "White";
            }
            cout << ") can attack:" << endl;

            attackCount = NextPossibleAttack(color, figure, row, col, attackCount);

            cout << "In total " << attackCount << " opponents." << endl;
        }
    }

    //function to get a chech of color of figure at specific position
    char getCheckerAtPosition(int row, int col) {
        for (const Checker& checker : checkers) {
            if (checker.getRow() == row && checker.getCol() == col) {
                return checker.getColor();
            }
        }
        return '.';
    }

    //function to output name of figure at specific position with its coordinates
    void getFigureAtPosition(int row, int col) {
        for (const Checker& checker : checkers) {
            if (checker.getRow() == row && checker.getCol() == col) {
                char figure = checker.getFigure();
                figureName(figure);
                cout << " at ";
                RowToLetter(col);
                cout << row << "." << endl;
                break;
            }
        }
    }

    //function to get name of figure at specific position
    char getFigureNameAtPosition(int row, int col) {
        char figure;
        for (const Checker& checker : checkers) {
            if (checker.getRow() == row && checker.getCol() == col) {
                figure = checker.getFigure();
                break;
            }
        }
        return figure;
    }

    //function to call a function according to piece which is attacking
    int NextPossibleAttack(char color, char figure, int row, int col, int attackCount) {
        char opponentColor = (color == 'W') ? 'B' : 'W';
        // Check if the figure is a Pawn
        if (figure == 'P') {
            attackCount = PawnAttack(opponentColor, row, col, attackCount);
        }
        // Check if the figure is a Knight
        if (figure == 'H') {
            attackCount = KnightAttack(opponentColor, row, col, attackCount);
        }
        // Check if the figure is a Bishop
        if (figure == 'B') {
            attackCount = BishopAttack(color, opponentColor, row, col, attackCount);
        }
        // Check if the figure is a Rook
        if (figure == 'R') {
            attackCount = RookAttack(color, opponentColor, row, col, attackCount);
        }
        // Check if the figure is a Queen
        if (figure == 'Q') {
            attackCount = BishopAttack(color, opponentColor, row, col, attackCount);
            attackCount = RookAttack(color, opponentColor, row, col, attackCount);
        }
        // Check if the figure is a King
        if (figure == 'K') {
            attackCount = KingAttack(color, opponentColor, row, col, attackCount);
        }
        
        return attackCount;
    }

    //function to check if Pawn can attack anyone
    int PawnAttack(char opponentColor, int row, int col, int attackCount){
        if (row >= 1 && row <= count) {
            if (opponentColor == 'W') {
                // Check diagonal left-up
                if (col > 1 && getCheckerAtPosition(row - 1, col - 1) == opponentColor) {
                    attackCount++;
                    getFigureAtPosition(row - 1, col - 1);

                }
                // Check diagonal right-up
                if (col < count && getCheckerAtPosition(row - 1, col + 1) == opponentColor) {
                    attackCount++;
                    getFigureAtPosition(row - 1, col + 1);
                }
            }
            if (opponentColor == 'B') {
                // Check diagonal left-down
                if (col > 1 && getCheckerAtPosition(row + 1, col - 1) == opponentColor) {
                    attackCount++;
                    getFigureAtPosition(row + 1, col - 1);

                }
                // Check diagonal right-down
                if (col < count && getCheckerAtPosition(row + 1, col + 1) == opponentColor) {
                    attackCount++;
                    getFigureAtPosition(row + 1, col + 1);

                }
            }
        }
        return attackCount;
    }

    //function to check if Knight can attack anyone
    int KnightAttack(char opponentColor, int row, int col, int attackCount){
        if (row >= 1 && row <= count) {
            // Check up-left
            if (col > 1 && getCheckerAtPosition(row - 1, col - 2) == opponentColor) {
                attackCount++;
                getFigureAtPosition(row - 1, col - 2);
            }
            // Check down-left
            if (col > 1 && getCheckerAtPosition(row + 1, col - 2) == opponentColor) {
                attackCount++;
                getFigureAtPosition(row + 1, col - 2);
            }
            // Check up-right
            if (col < count && getCheckerAtPosition(row - 1, col + 2) == opponentColor) {
                attackCount++;
                getFigureAtPosition(row - 1, col + 2);
            }
            // Check down-right
            if (col < count && getCheckerAtPosition(row + 1, col + 2) == opponentColor) {
                attackCount++;
                getFigureAtPosition(row + 1, col + 2);
            }
            // Check left-down
            if (col > 1 && getCheckerAtPosition(row + 2, col - 1) == opponentColor) {
                attackCount++;
                getFigureAtPosition(row + 2, col - 1);
            }
            // Check right-down
            if (col > 1 && getCheckerAtPosition(row + 2, col + 1) == opponentColor) {
                attackCount++;
                getFigureAtPosition(row + 2, col + 1);
            }
            // Check left-up
            if (col < count && getCheckerAtPosition(row - 2, col - 1) == opponentColor) {
                attackCount++;
                getFigureAtPosition(row - 2, col - 1);
            }
            // Check left-up
            if (col < count && getCheckerAtPosition(row - 2, col + 1) == opponentColor) {
                attackCount++;
                getFigureAtPosition(row - 2, col + 1);
            }
        }
        return attackCount;
    }

    //function to check if Bishop can attack anyone
    int BishopAttack(char color, char opponentColor, int row, int col, int attackCount){
        int row_def = row;
        int col_def = col;
        if (row >= 1 && row <= count) {
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check diagonal left-up
                if (col > 1 && getCheckerAtPosition(row - 1, col - 1) == opponentColor) {
                    attackCount++;
                    getFigureAtPosition(row - 1, col - 1);
                    break;
                }
                //if meet blockage then stop
                if (col > 1 && getCheckerAtPosition(row - 1, col - 1) == color) {
                    break;
                }
                row = row - 1;
                col = col - 1;
            };
            row = row_def;
            col = col_def;
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check diagonal right-up
                if (col < count && getCheckerAtPosition(row - 1, col + 1) == opponentColor) {
                    attackCount++;
                    getFigureAtPosition(row - 1, col + 1);
                    break;
                }
                //if meet blockage then stop
                if (col > 1 && getCheckerAtPosition(row - 1, col + 1) == color) {
                    break;
                }
                row = row - 1;
                col = col + 1;
            }
            row = row_def;
            col = col_def;
        }
        if (row >= 1 && row <= count) {
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check diagonal left-down
                if (col > 1 && getCheckerAtPosition(row + 1, col - 1) == opponentColor) {
                    attackCount++;
                    getFigureAtPosition(row + 1, col - 1);
                    break;
                }
                //if meet blockage then stop
                if (col > 1 && getCheckerAtPosition(row + 1, col - 1) == color) {
                    break;
                }
                row = row + 1;
                col = col - 1;
            }
            row = row_def;
            col = col_def;
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check diagonal right-down
                if (col < count && getCheckerAtPosition(row + 1, col + 1) == opponentColor) {
                    attackCount++;
                    getFigureAtPosition(row + 1, col + 1);
                    break;
                }
                //if meet blockage then stop
                if (col > 1 && getCheckerAtPosition(row + 1, col + 1) == color) {
                    break;
                }
                row = row + 1;
                col = col + 1;
            }
            row = row_def;
            col = col_def;
        }
        return attackCount;
    }

    //function to check if Rook can attack anyone
    int RookAttack(char color, char opponentColor, int row, int col, int attackCount){
        int row_def = row;
        int col_def = col;
        if (row >= 1 && row <= count) {
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check horizontal left
                if (col > 1 && getCheckerAtPosition(row, col - 1) == opponentColor) {
                    attackCount++;
                    getFigureAtPosition(row, col - 1);
                    break;
                }
                //if meet blockage then stop
                if (col > 1 && getCheckerAtPosition(row, col - 1) == color) {
                    break;
                }
                col = col - 1;
            };
            col = col_def;
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check horizontal right
                if (col < count && getCheckerAtPosition(row, col + 1) == opponentColor) {
                    attackCount++;
                    getFigureAtPosition(row, col + 1);
                    break;
                }
                //if meet blockage then stop
                if (col > 1 && getCheckerAtPosition(row, col + 1) == color) {
                    break;
                }
                col = col + 1;
            }
            col = col_def;
        }
        if (row >= 1 && row <= count) {
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check vertical top
                if (col > 1 && getCheckerAtPosition(row - 1, col) == opponentColor) {
                    attackCount++;
                    getFigureAtPosition(row - 1, col);
                    break;
                }
                //if meet blockage then stop
                if (col > 1 && getCheckerAtPosition(row - 1, col) == color) {
                    break;
                }
                row = row - 1;
            }
            row = row_def;
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check vertical down
                if (col < count && getCheckerAtPosition(row + 1, col) == opponentColor) {
                    attackCount++;
                    getFigureAtPosition(row + 1, col);
                    break;
                }
                //if meet blockage then stop
                if (col > 1 && getCheckerAtPosition(row + 1, col) == color) {
                    break;
                }
                row = row + 1;
            }
            row = row_def;
        }
        return attackCount;
    }

    //function to check if King can attack anyone
    int KingAttack(char color, char opponentColor, int row, int col, int attackCount){
        if (row >= 1 && row <= count) {
            // Check diagonal left-top
            if (col > 1 && getCheckerAtPosition(row - 1, col - 1) == opponentColor && KingVulnerability(color, row - 1, col - 1, 0) != 1) {
                attackCount++;
                getFigureAtPosition(row - 1, col - 1);

            }
            // Check diagonal right-top
            if (col < count && getCheckerAtPosition(row - 1, col + 1) == opponentColor && KingVulnerability(color, row - 1, col + 1, 0) != 1) {
                attackCount++;
                getFigureAtPosition(row - 1, col + 1);

            }
            // Check diagonal left-down
            if (col > 1 && getCheckerAtPosition(row + 1, col - 1) == opponentColor && KingVulnerability(color, row + 1, col - 1, 0) != 1) {
                attackCount++;
                getFigureAtPosition(row + 1, col - 1);

            }
            // Check diagonal right-down
            if (col < count && getCheckerAtPosition(row + 1, col + 1) == opponentColor && KingVulnerability(color, row + 1, col + 1, 0) != 1) {
                attackCount++;
                getFigureAtPosition(row + 1, col + 1);
            }

            // Check horizontal left
            if (col > 1 && getCheckerAtPosition(row, col - 1) == opponentColor && KingVulnerability(color, row, col - 1, 0) != 1) {
                attackCount++;
                getFigureAtPosition(row, col - 1);

            }
            // Check horizontal right
            if (col < count && getCheckerAtPosition(row, col + 1) == opponentColor && KingVulnerability(color, row, col + 1, 0) != 1) {
                attackCount++;
                getFigureAtPosition(row, col + 1);

            }
            // Check vertical top
            if (col > 1 && getCheckerAtPosition(row - 1, col) == opponentColor && KingVulnerability(color, row - 1, col, 0) != 1) {
                attackCount++;
                getFigureAtPosition(row - 1, col);

            }
            // Check vertical down
            if (col < count && getCheckerAtPosition(row + 1, col) == opponentColor && KingVulnerability(color, row + 1, col, 0) != 1) {
                attackCount++;
                getFigureAtPosition(row + 1, col);
            }
        }
        return attackCount;
    }

    //function to check if any Pawn can attack a King
    int KingPawnVulnerability(int row, int col, char opponentColor){
        if (row >= 1 && row <= count) {
            if (opponentColor == 'W') {
                // Check diagonal left-up
                if ((col > 1 && getCheckerAtPosition(row - 1, col - 1) == opponentColor)&&(getFigureNameAtPosition(row - 1, col - 1) == 'P')) {
                    return 1;

                }
                // Check diagonal right-up
                if ((col < count && getCheckerAtPosition(row - 1, col + 1) == opponentColor)&&(getFigureNameAtPosition(row - 1, col + 1) == 'P')) {
                    return 1;
                }
            }
            if (opponentColor == 'B') {
                // Check diagonal left-down
                if ((col > 1 && getCheckerAtPosition(row + 1, col - 1) == opponentColor)&&(getFigureNameAtPosition(row + 1, col - 1) == 'P')) {
                    return 1;

                }
                // Check diagonal right-down
                if ((col < count && getCheckerAtPosition(row + 1, col + 1) == opponentColor)&&(getFigureNameAtPosition(row + 1, col + 1) == 'P')) {
                    return 1;

                }
            }
        }
        return 0;
    }

    //function to check if any Knight can attack a King
    int KingKnightVulnerability(int row, int col, char opponentColor){
        // Check top-left
        if ((col > 1 && getCheckerAtPosition(row - 1, col - 2) == opponentColor)&&(getFigureNameAtPosition(row - 1, col - 2) == 'H')) {
            return 1;
        }
        // Check down-left
        if ((col > 1 && getCheckerAtPosition(row + 1, col - 2) == opponentColor)&&(getFigureNameAtPosition(row + 1, col - 2) == 'H')) {
            return 1;
        }
        // Check top-right
        if ((col < count && getCheckerAtPosition(row - 1, col + 2) == opponentColor)&&(getFigureNameAtPosition(row - 1, col + 2) == 'H')) {
            return 1;
        }
        // Check down-right
        if ((col < count && getCheckerAtPosition(row + 1, col + 2) == opponentColor)&&(getFigureNameAtPosition(row + 1, col + 2) == 'H')) {
            return 1;
        }
        // Check left-down
        if ((col > 1 && getCheckerAtPosition(row + 2, col - 1) == opponentColor)&&(getFigureNameAtPosition(row + 2, col - 1) == 'H')) {
            return 1;
        }
        // Check right-down
        if ((col > 1 && getCheckerAtPosition(row + 2, col + 1) == opponentColor)&&(getFigureNameAtPosition(row + 2, col + 1) == 'H')) {
            return 1;
        }
        // Check left-top
        if ((col < count && getCheckerAtPosition(row - 2, col - 1) == opponentColor)&&(getFigureNameAtPosition(row - 2, col - 1) == 'H')) {
            return 1;
        }
        // Check right-top
        if ((col < count && getCheckerAtPosition(row - 2, col + 1) == opponentColor)&&(getFigureNameAtPosition(row - 2, col + 1) == 'H')) {
            return 1;
        }
        return 0;
    }

    //function to check if any Rook can attack a King
    int KingRookVulnerability(int row, int col, char opponentColor){
        int row_def = row;
        int col_def = col;
        if (row >= 1 && row <= count) {
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check horizontal left
                if ((col > 1 && getCheckerAtPosition(row, col - 1) == opponentColor)&&(getFigureNameAtPosition(row, col - 1) == 'R')) {
                    return 1;
                }
                if ((col > 1 && getCheckerAtPosition(row, col - 1) == opponentColor)&&(getFigureNameAtPosition(row, col - 1) == 'Q')) {
                    return 1;
                }
                col = col - 1;
                //if meet blockage then stop
                if (getFigureNameAtPosition(row, col - 1) == 'H'
                    ||getFigureNameAtPosition(row, col - 1) == 'B'
                    ||getFigureNameAtPosition(row, col - 1) == 'P'){
                    break;
                }
            };
            col = col_def;
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check horizontal right
                if ((col < count && getCheckerAtPosition(row, col + 1) == opponentColor)&&(getFigureNameAtPosition(row, col + 1) == 'R')) {
                    return 1;
                }
                if ((col < count && getCheckerAtPosition(row, col + 1) == opponentColor)&&(getFigureNameAtPosition(row, col + 1) == 'Q')) {
                    return 1;
                }
                col = col + 1;
                //if meet blockage then stop
                if (getFigureNameAtPosition(row, col + 1) == 'H'
                    ||getFigureNameAtPosition(row, col + 1) == 'B'
                    ||getFigureNameAtPosition(row, col + 1) == 'P'){
                    break;
                }
            }
            col = col_def;
        }
        if (row >= 1 && row <= count) {
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check vertical top
                if ((col > 1 && getCheckerAtPosition(row - 1, col) == opponentColor)&&(getFigureNameAtPosition(row - 1, col) == 'R')) {
                    return 1;
                }
                if ((col > 1 && getCheckerAtPosition(row - 1, col) == opponentColor)&&(getFigureNameAtPosition(row - 1, col) == 'Q')) {
                    return 1;
                }
                row = row - 1;
                //if meet blockage then stop
                if (getFigureNameAtPosition(row - 1, col) == 'H'
                    ||getFigureNameAtPosition(row - 1, col) == 'B'
                    ||getFigureNameAtPosition(row - 1, col) == 'P'){
                    break;
                }
            }
            row = row_def;
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check vertical down
                if ((col < count && getCheckerAtPosition(row + 1, col) == opponentColor)&&(getFigureNameAtPosition(row + 1, col) == 'R')) {
                    return 1;
                }
                if ((col < count && getCheckerAtPosition(row + 1, col) == opponentColor)&&(getFigureNameAtPosition(row + 1, col) == 'Q')) {
                    return 1;
                }
                row = row + 1;
                //if meet blockage then stop
                if (getFigureNameAtPosition(row + 1, col) == 'H'
                    ||getFigureNameAtPosition(row + 1, col) == 'B'
                    ||getFigureNameAtPosition(row + 1, col) == 'P'){
                    break;
                }
            }
            row = row_def;
        }
        return 0;
    }

    //function to check if any bishop can attack a King
    int KingBishopVulnerability(int row, int col, char opponentColor){
        int row_def = row;
        int col_def = col;
        if (row >= 1 && row <= count) {
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check diagonal left-top
                if ((col > 1 && getCheckerAtPosition(row - 1, col - 1) == opponentColor)&&(getFigureNameAtPosition(row - 1, col - 1) == 'B')) {
                    return 1;
                }
                if ((col > 1 && getCheckerAtPosition(row - 1, col - 1) == opponentColor)&&(getFigureNameAtPosition(row - 1, col - 1) == 'Q')) {
                    return 1;
                }
                row = row - 1;
                col = col - 1;
                //if meet blockage then stop
                if (getFigureNameAtPosition(row - 1, col - 1) == 'H'
                    ||getFigureNameAtPosition(row - 1, col - 1) == 'R'
                    ||getFigureNameAtPosition(row - 1, col - 1) == 'P'){
                    break;
                }
            };
            row = row_def;
            col = col_def;
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check diagonal right-top
                if ((col < count && getCheckerAtPosition(row - 1, col + 1) == opponentColor)&&(getFigureNameAtPosition(row - 1, col + 1) == 'B')) {
                    return 1;
                }
                if ((col < count && getCheckerAtPosition(row - 1, col + 1) == opponentColor)&&(getFigureNameAtPosition(row - 1, col + 1) == 'Q')) {
                    return 1;
                }
                row = row - 1;
                col = col + 1;
                //if meet blockage then stop
                if (getFigureNameAtPosition(row - 1, col + 1) == 'H'
                    ||getFigureNameAtPosition(row - 1, col + 1) == 'R'
                    ||getFigureNameAtPosition(row - 1, col + 1) == 'P'){
                    break;
                }
            }
            row = row_def;
            col = col_def;
        }
        if (row >= 1 && row <= count) {
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check diagonal left-down
                if ((col > 1 && getCheckerAtPosition(row + 1, col - 1) == opponentColor)&&(getFigureNameAtPosition(row + 1, col - 1) == 'B')) {
                    return 1;
                }
                if ((col > 1 && getCheckerAtPosition(row + 1, col - 1) == opponentColor)&&(getFigureNameAtPosition(row + 1, col - 1) == 'Q')) {
                    return 1;
                }
                row = row + 1;
                col = col - 1;
                //if meet blockage then stop
                if (getFigureNameAtPosition(row + 1, col - 1) == 'H'
                    ||getFigureNameAtPosition(row + 1, col - 1) == 'R'
                    ||getFigureNameAtPosition(row + 1, col - 1) == 'P'){
                    break;
                }
            }
            row = row_def;
            col = col_def;
            while ((row > 0 && row <= count)&&(col > 0 && col <= count)) {
                // Check diagonal right-down
                if ((col < count && getCheckerAtPosition(row + 1, col + 1) == opponentColor)&&(getFigureNameAtPosition(row + 1, col + 1) == 'B')) {
                    return 1;
                }
                if ((col < count && getCheckerAtPosition(row + 1, col + 1) == opponentColor)&&(getFigureNameAtPosition(row + 1, col + 1) == 'Q')) {
                    return 1;
                }
                row = row + 1;
                col = col + 1;
                //if meet blockage then stop
                if (getFigureNameAtPosition(row + 1, col + 1) == 'H'
                    ||getFigureNameAtPosition(row + 1, col + 1) == 'R'
                    ||getFigureNameAtPosition(row + 1, col + 1) == 'P'){
                    break;
                }
            }
            row = row_def;
            col = col_def;
        }
        return 0;
    }

    //function to check if any King is under attack
    int KingVulnerability(char color, int row, int col, int FlagShow){
        char opponentColor = (color == 'W') ? 'B' : 'W';
        if (row >= 1 && row <= count) {
            if((KingKnightVulnerability(row, col, opponentColor) == 1)
                ||(KingRookVulnerability(row, col, opponentColor) == 1)
                ||(KingBishopVulnerability(row, col, opponentColor) == 1)
                ||(KingPawnVulnerability(row, col, opponentColor) == 1)){
                if (FlagShow == 1){
                    if (color == 'B'){
                        cout << "Black";
                    }
                    if (color == 'W'){
                        cout << "White";
                    }
                    cout << " King is under attack" << endl;
                }
                return 1;
            }
        }
        return 0;
    }

    // function to write '---' in terminal for looks
    void Sides(){
        for (int i = 0; i < (count-2)/2; ++i){
            cout << "---";
        }
    }

private:
    vector<Checker> checkers;
    int count;
};

//Structur for Sort function
struct Entry {
    char letter;
    char symbol;
    char column;
    int num;
};

bool compareEntries(const Entry& entry1, const Entry& entry2) {
    // First, we compare the letters, and if they are the same, we leave them in the current order.
    if (entry1.letter == entry2.letter) {
        return false;
    }
    return (entry1.letter == 'W' && entry2.letter == 'B');
}

//Sort function, which sorts data in file for better looks
int SortFile() {
    ifstream inputFile("chessboard.txt"); // Replace "chessboard.txt" with the name of your file
    if (!inputFile.is_open()) {
        cerr << "File opening error." << endl;
        return 1;
    }

    vector<Entry> entries;
    Entry entry;
    while (inputFile >> entry.letter >> entry.symbol >> entry.column >> entry.num) {
        entries.push_back(entry);
    }

    inputFile.close();

    sort(entries.begin(), entries.end(), compareEntries);

    ofstream outputFile("chessboard.txt"); // Replace "chessboard.txt" with a file name to save the sorted list
    if (!outputFile.is_open()) {
        cerr << "Error opening a file for writing." << endl;
        return 1;
    }

    char letter;
    char symbol;
    char column;

    for (const Entry& e : entries) {
        letter = toupper(e.letter);
        symbol = toupper(e.symbol);
        column = toupper(e.column);
        outputFile << letter << " " << symbol << " " << column << e.num << endl;
    }

    outputFile.close();

    return 0;
}
//end of sort


// Structure for storing information about shapes
struct FigureInfo {
    char color;
    char piece;
    int row;
    int column;
};

// Function for checking repeated coordinates
bool checkDuplicateCoordinates(const vector<FigureInfo>& figures) {
    map<pair<int, int>, int> coordinateCount;
    
    for (const FigureInfo& figure : figures) {
        pair<int, int> coordinate = make_pair(figure.row, figure.column);
        
        if (coordinateCount.find(coordinate) != coordinateCount.end()) {
            return true; // Duplicate coordinates are found
        } else {
            coordinateCount[coordinate] = 1;
        }
    }
    
    return false; // No duplicate coordinates
}

// Function to check the number of shapes of each type for black and white
bool checkPieceCount(const vector<FigureInfo>& figures) {
    map<char, int> blackPieceCount;
    map<char, int> whitePieceCount;
    
    // loop for calculation of a count of white and black pieces
    for (const FigureInfo& figure : figures) {
        if (figure.color == 'B') {
            blackPieceCount[figure.piece]++;
        } else if (figure.color == 'W') {
            whitePieceCount[figure.piece]++;
        }
    }
    
    // Check the number of shapes of each type for black and white
    if (blackPieceCount['R'] > 10 || blackPieceCount['B'] > 10 || blackPieceCount['Q'] > 9 ||
        blackPieceCount['K'] > 1 || blackPieceCount['H'] > 10 || blackPieceCount['P'] > 8 ||
        whitePieceCount['R'] > 10 || whitePieceCount['B'] > 10 || whitePieceCount['Q'] > 9 ||
        whitePieceCount['K'] > 1 || whitePieceCount['H'] > 10 || whitePieceCount['P'] > 8
        ||((blackPieceCount['R'] + blackPieceCount['B'] + blackPieceCount['Q'] + blackPieceCount['K'] + blackPieceCount['H'] + blackPieceCount['P']) > 16)
        ||((whitePieceCount['R'] + whitePieceCount['B'] + whitePieceCount['Q'] + whitePieceCount['K'] + whitePieceCount['H'] + whitePieceCount['P']) > 16)) {
        return false; // Not the right number of pieces
    }

    return true; // The right number of pieces
}

// function to check if data in file is correctly written
int CheckIfCorrectData(){
    vector<FigureInfo> figures;
    ifstream inputFile("chessboard.txt"); // Open the file for reading

    if (inputFile.is_open()) {
        string line;

        while (getline(inputFile, line)) {
            char color, piece, column_char;
            int row, column;
            
            // Unpacking a string
            if (sscanf(line.c_str(), " %c %c %c %d", &color, &piece, &column_char, &row) == 4) {
                int FlagError = 0;
                color = toupper(color);
                piece = toupper(piece);
                column_char = toupper(column_char);
                column = LetterOfRow(column_char);
                // Check if color is valid
                if (color != 'W' && color != 'B'){
                    cerr << "An error in the string: " << line << endl << "Can't use other color except W(White) and B(Black)." << endl;
                    FlagError =  1;
                }
                // Check if piece is valid
                if (piece != 'P' && piece != 'H' && piece != 'R' && piece != 'B' && piece != 'Q' && piece != 'K'){
                    cerr << "An error in the string: " << line << endl << "Can't use other figures except:" << endl;
                    cerr << "P - Pawn" << endl << "H - Knight" << endl << "R - Rook" << endl << "B - Bishop" << endl << "Q - Queen" << endl << "K - King" << endl;
                    FlagError =  1;
                }
                // Check if column is valid
                if (column == 0){
                    cerr << "Error: The Pawn can't be at " << column_char << row << endl;
                    FlagError =  1;
                }
                // Check borders for rows and columns
                if (row >= 1 && row <= 8) {
                    if (piece == 'P' && (row == 8 || row == 1)){
                        cerr << "Error: The Pawn can't be at row " << row << endl;
                        FlagError =  1;
                    }
                    else{
                        // Adding shape information to a vector
                        figures.push_back({color, piece, row, column});
                    }
                } else {
                    cerr << "Error: The coordinates of the figures must be between 1 and 8." << endl;
                    FlagError =  1;
                }
                if (FlagError == 1){
                    return 1;
                }
            } else {
                cerr << "An error in the string: " << line << endl;
                return 1;
            }
        }

        inputFile.close(); // Close the file
    } else {
        cerr << "Error opening a file" << endl;
        return 1;
    }

    // Calling functions to check for repeating coordinates and the number of shapes of each type for black and white
    if (checkDuplicateCoordinates(figures)) {
        cout << "Figures with the same coordinates are found." << endl;
        return 1;
    }
    // Calling a function to check if the number of pieces of shapes is valid
    if (!checkPieceCount(figures)) {
        cout << "Incorrect number of pieces." << endl;
        return 1;
    }

    return 0;
}

int main() {
    // Call a function to Check if data in file is correct for working with it
    if (CheckIfCorrectData() == 1){
        return 1;
    }
    // Call a function a data in file for better readability
    if (SortFile() == 1){
        return 1;
    }

    Chessboard chessboard;

    chessboard.SetChessboardSize(8);

    // Loading data from a file
    if (chessboard.loadFromFile("chessboard.txt") == 1){
        return 1;
    }

    // Call a function to analyze if King is under attack
    if (chessboard.analyzeKing(1) == 1){
        return 1;
    }

    cout << endl << "Pieces:" << endl;

    // Analyse positions and display results on the screen
    chessboard.analyzePosition();

    getch();

    return 0;
}
