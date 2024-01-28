#include <stdio.h>

#include <malloc.h>

#include "board.h"

#define POINTS_EMPTY 0.2 

#define POINTS_ALLY 1

#define POINTS_ENEMY 1

#define EMPTY_ID 0

int allyID;

int enemyID = -1; 

int const neiG[8][2] = { 

    {-1, -1}, 

    {-1,  0}, 

    {-1,  1}, 

    { 0,  1},

    { 1,  1},

    { 1,  0},

    { 1, -1},

    { 0, -1}

};

int isInBoard(const Board* b, int row, int column){

    int isInside = 1; 

    isInside = isInside && (row >= 0) && (column >= 0);

    isInside = isInside && (row < b->rows) && (column < b->columns);

    return isInside;

}


int findDropRow(const Board* b, int column){ 

    int rows = b->rows; 

    int row;

    

    for(row = 0; row < rows; row++){ 

        if(b->board[row][column] != EMPTY_ID){

            return row-1;

        }

    }

    return rows-1; 

}

float getScore(float count, int type){

    if(type == allyID){ 

        return count*count*POINTS_ALLY; 

    }else if(type == enemyID){

        return count*count*POINTS_ENEMY;

    }else if(type == EMPTY_ID){

        return count*POINTS_EMPTY;

    }

}

float findNeighborScore(const Board* b, int row, int column, int neigbor){ 

    int rows = b->rows;

    int columns = b->columns;

    float score = 0; 

    int neigborType; 

    int i = 1;

    float weight = 0;

    int rNeigbor = row + i * neiG[neigbor][0]; 

    int cNeighor = column + i * neiG[neigbor][1]; 
    if(isInBoard(b, rNeigbor, cNeighor)){ 

        neigborType = b->board[rNeigbor][cNeighor]; 

        while(isInBoard(b, rNeigbor, cNeighor) && b->board[rNeigbor][cNeighor] == neigborType){ 

            i++;

            weight += 1.0f;

            rNeigbor = row + i * neiG[neigbor][0]; 

            cNeighor = column + i * neiG[neigbor][1]; 
        }

        score = getScore(weight, neigborType);

    }

    return score;

}

int findIndexOfMaxPoints(int* validColumn, float* scorePerColumn, int columns){

    float max = scorePerColumn[0];

    int index = 0;

    int i;

    for (i = 1; i < columns; i++){ 

        if(validColumn[i] && scorePerColumn[i] > max){ 

            max = scorePerColumn[i];

            index = i;

        }

    }

    return index; 

}

int findAllyID(const Board* b, int const rows, int const columns){

    int x, y;

    for(x = 0; x < rows; x++){

        for(y = 0; y < columns; y++){

            int num = b->board[x][y]; 

            if(num != EMPTY_ID){

                return 2; 

            }

        }

    }

    return 1;

}

int findEnemyID(int allyID){

    return 3 - allyID;	

}

extern int pro_player(const Board* b){

    int const rows = b->rows;

    int const columns = b->columns;

    if(enemyID == -1){

        allyID = findAllyID(b, rows, columns);

        enemyID = findEnemyID(allyID);

    }

    int *validColumn = (int*)malloc(columns * sizeof(int)); 

    float *scorePerColumn = (float*)malloc(columns * sizeof(float)); 

    int column;

    for(column = 0; column < columns; column++){  paiksei

        validColumn[column] = 1; 

        scorePerColumn[column] = 0;

        int dropRow = findDropRow(b, column); 

        if(dropRow == -1){ 

            validColumn[column] = 0; 

            continue;

        }

        

        int i; 

        for(i = 0; i < 8; i++){ 

            scorePerColumn[column] += findNeighborScore(b, dropRow, column, i);
        }

    }
					
    int indexOfMaxPoints = findIndexOfMaxPoints(validColumn, scorePerColumn, columns); 

    free(validColumn);

    free(scorePerColumn);

    return indexOfMaxPoints; 

}
