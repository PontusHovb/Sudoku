#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "functions.h"

// Check if solution is correct
int CorrectSolution(int (*sudoku)[SIZE][SIZE], int (*solution)[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if ((*sudoku)[row][col] != (*solution)[row][col]) {
                return 0;
            }
        }
    }
    return 1;
}

// Help functions
int GetAllUnsolved(int (*sudoku)[SIZE][SIZE], int (*unsolvedCells)[SIZE*SIZE][2]) {
    int noUnsolvedCells = 0;  // Initialize the count of unsolved cells
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if ((*sudoku)[row][col] == 0) {
                (*unsolvedCells)[noUnsolvedCells][0] = row;
                (*unsolvedCells)[noUnsolvedCells][1] = col;
                noUnsolvedCells++;
            }
        }
    }
    return noUnsolvedCells;
}

int* GetCandidates(int (*sudoku)[SIZE][SIZE], int row, int col) {
    int *candidates = (int *)malloc(SIZE * sizeof(int));
    
    // No candidates if cell is already filled
    if ((*sudoku)[row][col] != 0) {
        for (int i = 0; i < SIZE; i++) {
            candidates[i] = 0;
        }
        return candidates;
    }

    // Initialize all numbers as possible
    for (int i = 0; i < SIZE; i++) {
        candidates[i] = 1;
    }

    // Eliminate candidates in the same row
    for (int c = 0; c < SIZE; c++) {
        int value = (*sudoku)[row][c];
        if (value != 0) {
            candidates[value - 1] = 0;
        }
    }

    // Eliminate candidates in the same col
    for (int r = 0; r < SIZE; r++) {
        int value = (*sudoku)[r][col];
        if (value != 0) {
            candidates[value - 1] = 0;
        }
    }

    // Eliminate candidates in the same block
    int blockRow = (row / 3) * 3;
    int blockCol = (col / 3) * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int value = (*sudoku)[blockRow + i][blockCol + j];
            if (value != 0) {
                candidates[value - 1] = 0;
            }
        }
    }

    return candidates;
}

int IsOnlyPossibleLocation(int (*sudoku)[SIZE][SIZE], int row, int col, int guess, char checkType[3]) {
    if (strcmp(checkType, "row") == 0) {
        for (int c = 1; c <= SIZE; c++) {
            if ((*sudoku)[row][c] == 0 && c != col && ValidGuess(row, c, guess, sudoku)) {
                return 0;
            }
        }
    }
    else if (strcmp(checkType, "col") == 0) {
        for (int r = 1; r <= SIZE; r++) {
            if ((*sudoku)[r][col] == 0 && r != row && ValidGuess(r, col, guess, sudoku)) {
                return 0;
            }
        }
    }
    else if (strcmp(checkType, "box") == 0) {
        int startRow = 3 * (row / 3);
        int startCol = 3 * (col / 3);
        for (int r = startRow; r < startRow + 3; r++) {
            for (int c = startCol; c < startCol + 3; c++) {
                if ((*sudoku)[r][c] == 0 && !(r == row && c == col) && ValidGuess(r, c, guess, sudoku)) {
                    return 0;
                }
            }
        }
    }
    else {
        return 0;
    }
    return 1;
}

int ValidGuess(int row, int col, int guess, int (*sudoku)[SIZE][SIZE]) {
    // Check if guess is in row
    for (int i = 0; i < SIZE; i++) {
        if ((*sudoku)[row][i] == guess && i != col) {
            return 0;
        }
    }

    // Check if guess is in column
    for (int i = 0; i < SIZE; i++) {
        if ((*sudoku)[i][col] == guess && i != row) {
            return 0;
        }
    }

    // Check if guess is in the 3x3 square
    int startRow = 3 * (row / 3);
    int startCol = 3 * (col / 3);
    for (int r = startRow; r < startRow + 3; r++) {
        for (int c = startCol; c < startCol + 3; c++) {
            if ((*sudoku)[r][c] == guess && r != row && c != col) {
                return 0;
            }
        }
    }

    return 1;
}

int IdenticArrays(int array1[SIZE], int array2[SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        if (array1[i] != array2[i]) {
            return 0;
        }
    }
    return 1;
}

int FindPreemptiveSets(int row, int col, int (*sudokuMarkup)[SIZE][SIZE][SIZE]) {
    int updates = 0;
    int noCandidates = 0;
    int noIdenticArrays;

    for (int k = 0; k < SIZE; k++) {
        noCandidates += (*sudokuMarkup)[row][col][k];
    }

    // Row
    noIdenticArrays = 0;
    for (int c = 0; c < SIZE; c++) {
        if(IdenticArrays((*sudokuMarkup)[row][c], (*sudokuMarkup)[row][col]) == 1) {
            noIdenticArrays++;
        }
    }
    if (noIdenticArrays == noCandidates) {
        for (int k = 0; k < SIZE; k++) {
            if ((*sudokuMarkup)[row][col][k] == 1) {
                for (int c = 0; c < SIZE; c++) {
                    if (IdenticArrays((*sudokuMarkup)[row][c], (*sudokuMarkup)[row][col]) == 0) {
                        (*sudokuMarkup)[row][c][k] = 0;
                        updates++;
                    }
                }
            }
        }
    }

    // Col
    noIdenticArrays = 0;
    for (int r = 0; r < SIZE; r++) {
        if(IdenticArrays((*sudokuMarkup)[r][col], (*sudokuMarkup)[row][col]) == 1) {
            noIdenticArrays++;
        }
    }
    if (noIdenticArrays == noCandidates) {
        for (int k = 0; k < SIZE; k++) {
            if ((*sudokuMarkup)[row][col][k] == 1) {
                for (int r = 0; r < SIZE; r++) {
                    if (IdenticArrays((*sudokuMarkup)[r][col], (*sudokuMarkup)[row][col]) == 0) {
                        (*sudokuMarkup)[r][col][k] = 0;
                        updates++;
                    }
                }
            }
        }
    }

    // Box
    noIdenticArrays = 0;
    int startRow = 3 * (row / 3);
    int startCol = 3 * (col / 3);
    for (int r = startRow; r < startRow + 3; r++) {
        for (int c = startCol; c < startCol + 3; c++) {
            if(IdenticArrays((*sudokuMarkup)[r][c], (*sudokuMarkup)[row][col]) == 1) {
                noIdenticArrays++;
            }
        }
    }
    if (noIdenticArrays == noCandidates) {
        for (int k = 0; k < SIZE; k++) {
            if ((*sudokuMarkup)[row][col][k] == 1) {
                for (int r = startRow; r < startRow + 3; r++) {
                    for (int c = startCol; c < startCol + 3; c++) {
                        if (IdenticArrays((*sudokuMarkup)[r][c], (*sudokuMarkup)[row][col]) == 0) {
                            (*sudokuMarkup)[r][c][k] = 0;
                            updates++;
                        }
                    }
                }
            }
        }
    }

    return updates;
}

// Choose which algorithm to use
int SolveSudoku(int (*sudoku)[SIZE][SIZE], int (*solution)[SIZE][SIZE]) {
    int unsolvedCells[SIZE*SIZE][2] = {{0}};
    int unsolvedIndex = 0;
    int noUnsolvedCells = GetAllUnsolved(sudoku, &unsolvedCells);

    if (strcmp(ALGORITHM, "Bruteforce") == 0) {
        return Bruteforce(sudoku, solution, &unsolvedCells, unsolvedIndex, noUnsolvedCells);
    }
    else if (strcmp(ALGORITHM, "BruteforceLookAhead") == 0) {
        return BruteforceLookAhead(sudoku, solution, &unsolvedCells, unsolvedIndex, noUnsolvedCells);
    }
    else if (strcmp(ALGORITHM, "CandidateChecking") == 0) {
        return CandidateChecking(sudoku, solution, &unsolvedCells, noUnsolvedCells);
    }
    else if (strcmp(ALGORITHM, "PlaceFinding") == 0) {
        return PlaceFinding(sudoku, solution, &unsolvedCells, noUnsolvedCells);
    }
    else if (strcmp(ALGORITHM, "CrooksAlgorithm") == 0) {
        return CrooksAlgorithm(sudoku, solution, &unsolvedCells, noUnsolvedCells);
    }
    else {
        printf("Choose valid algorithm\n");
        return 0;
    }
}

// Algorithms
int Bruteforce(int (*sudoku)[SIZE][SIZE], int (*solution)[SIZE][SIZE], int (*unsolvedCells)[SIZE*SIZE][2], int unsolvedIndex, int noUnsolvedCells) {
    if (unsolvedIndex == noUnsolvedCells) {
        return CorrectSolution(sudoku, solution);
    }

    for (int num = 1; num <= 9; num++) {                                                                    // If empty cells left, make a guess for first empty cell
        (*sudoku)[(*unsolvedCells)[unsolvedIndex][0]][(*unsolvedCells)[unsolvedIndex][1]] = num;

        if (Bruteforce(sudoku, solution, unsolvedCells, unsolvedIndex+1, noUnsolvedCells) == 1) {       // Recursive solving
            return 1;
        }
        else {
            (*sudoku)[(*unsolvedCells)[unsolvedIndex][0]][(*unsolvedCells)[unsolvedIndex][1]] = 0;      // Backtrack if guess don't yield solution
        }
    }

    return 0;
}

int BruteforceLookAhead(int (*sudoku)[SIZE][SIZE], int (*solution)[SIZE][SIZE], int (*unsolvedCells)[SIZE*SIZE][2], int unsolvedIndex, int noUnsolvedCells) {
    if (unsolvedIndex == noUnsolvedCells) {
        return CorrectSolution(sudoku, solution);
    }

    for (int guess = 1; guess <= 9; guess++) {                                                                    // If empty cells left, make a guess for first empty cell
        if (ValidGuess((*unsolvedCells)[unsolvedIndex][0], (*unsolvedCells)[unsolvedIndex][1], guess, sudoku)) {
            (*sudoku)[(*unsolvedCells)[unsolvedIndex][0]][(*unsolvedCells)[unsolvedIndex][1]] = guess;

            if (BruteforceLookAhead(sudoku, solution, unsolvedCells, unsolvedIndex+1, noUnsolvedCells) == 1) {       // Recursive solving
                return 1;
            }
            else {
                (*sudoku)[(*unsolvedCells)[unsolvedIndex][0]][(*unsolvedCells)[unsolvedIndex][1]] = 0;          // Backtrack if guess don't yield solution
            }
        }
    }

    return 0;
}

int CandidateChecking(int (*sudoku)[SIZE][SIZE], int (*solution)[SIZE][SIZE], int (*unsolvedCells)[SIZE*SIZE][2], int noUnsolvedCells) {
    // Loop through unsolved cells until no more cell can be solved
    int solvedIter = 1;             // Used for initialization
    int solvedPrevIter = 0;        

    while (solvedIter > solvedPrevIter) {
        solvedPrevIter = solvedIter;
        solvedIter = 0;

        // Test all unsolved cells
        for (int i = 0; i < noUnsolvedCells; i++) {
            int possibleValues = 0;
            int possibleGuess;
            for (int guess = 1; guess <= 9; guess++) {
                if (ValidGuess((*unsolvedCells)[i][0], (*unsolvedCells)[i][1], guess, sudoku)) {
                    possibleGuess = guess;
                    possibleValues++;
                }
            }

            if (possibleValues == 1) {
                (*sudoku)[(*unsolvedCells)[i][0]][(*unsolvedCells)[i][1]] = possibleGuess;
                solvedIter++;
            }
        }

        if (CorrectSolution(sudoku, solution)) {
            return 1;
        }
    }
    return 0;
}

int PlaceFinding(int (*sudoku)[SIZE][SIZE], int (*solution)[SIZE][SIZE], int (*unsolvedCells)[SIZE*SIZE][2], int noUnsolvedCells) {
    // Loop through unsolved cells until no more cell can be solved
    int solvedIter = 1;             // Used for initialization
    int solvedPrevIter = 0;   

    while (solvedIter > solvedPrevIter) {
        solvedPrevIter = solvedIter;
        solvedIter = 0;

        for (int guess = 1; guess <= 9; guess++) {
            // Test all unsolved cells
            for (int i = 0; i < noUnsolvedCells; i++) {
                if (ValidGuess((*unsolvedCells)[i][0], (*unsolvedCells)[i][1], guess, sudoku)) {
                    if (IsOnlyPossibleLocation(sudoku, (*unsolvedCells)[i][0], (*unsolvedCells)[i][1], guess, "row") == 1 || 
                        IsOnlyPossibleLocation(sudoku, (*unsolvedCells)[i][0], (*unsolvedCells)[i][1], guess, "col") == 1 || 
                        IsOnlyPossibleLocation(sudoku, (*unsolvedCells)[i][0], (*unsolvedCells)[i][1], guess, "box") == 1) {
                            (*sudoku)[(*unsolvedCells)[i][0]][(*unsolvedCells)[i][1]] = guess;
                            solvedIter++;
                    }
                }
            }
        }

        if (CorrectSolution(sudoku, solution)) {
            return 1;
        }
    }
    return 0;
}

int CrooksAlgorithm(int (*sudoku)[SIZE][SIZE], int (*solution)[SIZE][SIZE], int (*unsolvedCells)[SIZE*SIZE][2], int noUnsolvedCells) {
    int sudokuMarkup[SIZE][SIZE][SIZE] = {{{0}}};

    for (int i = 0; i < noUnsolvedCells; i++) {
        int *candidates = GetCandidates(sudoku, (*unsolvedCells)[i][0], (*unsolvedCells)[i][1]);
        for (int k = 0; k < SIZE; k++) {
            sudokuMarkup[(*unsolvedCells)[i][0]][(*unsolvedCells)[i][1]][k] = candidates[k];
        }
        free(candidates);
    }

    int changesMade = 1; 
    int noCandidates;
    int candidate;
    while (changesMade > 0) {
        changesMade = 0;
        for (int i = 0; i < noUnsolvedCells; i++) {
            int updates = FindPreemptiveSets((*unsolvedCells)[i][0], (*unsolvedCells)[i][1], &sudokuMarkup);
            changesMade += updates;

            // Check whether there are naked singles
            noCandidates = 0;
            candidate = 0;
            for (int k = 0; k < SIZE; k++) {
                if (sudokuMarkup[(*unsolvedCells)[i][0]][(*unsolvedCells)[i][1]][k] == 1) {
                    candidate = k + 1;
                    noCandidates++;
                }
            }

            if (noCandidates == 1) {
                (*sudoku)[(*unsolvedCells)[i][0]][(*unsolvedCells)[i][1]] = candidate;
                sudokuMarkup[(*unsolvedCells)[i][0]][(*unsolvedCells)[i][1]][candidate - 1] = 0;
                changesMade++;
            }
        }

        if (CorrectSolution(sudoku, solution)) {
            return 1;
        }
    }

    return 0;
}