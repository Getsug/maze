#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

//implements a stack to store coordinates of path
typedef struct Node
{
    int row;
    int col;

    struct Node *next;

}Node;

//stores the allowed moves
typedef struct
{
    int vert;
    int horiz;
}Moves;

//function prototypes
Node *createNode();
void insert(int row, int col);
void deletetop();
void mvforward(int row, int col);
void backtrack();
Moves direction(int num);
void printmaze(int **maze);
void printpath(int **maze, int **markedfield);
void freememory(int **maze, int **markedfield);

//global variables
Node *top; // top of stack
Node currpos; // current position
int startr, startcol, finrow, fincol; // start and finish coordinates
int boundrow = 0;
int boundcol  = 0;


int main(void)
{

    Moves move; //allowed moves
    int row, col;
    int flag = 0; // true


    top = NULL; //empty list


    FILE *infile = fopen("maze1.txt", "r");

    if(infile == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    //read first line
    fscanf(infile, " %d %d", &row, &col);


    //size of field with boundary
    boundrow = row + 2;
    boundcol = col + 2;

    // matrix representation of the maze field with outer boundary
    int **field = (int**) malloc(boundrow * sizeof(int*));

    //copy of field to represent marked areas
    int **markedfield = (int**) malloc(boundrow * sizeof(int*));

    for(int i = 0; i < boundrow; i++)
    {
        field[i] = (int*) malloc(boundcol * sizeof(int));
        markedfield[i] = (int*) malloc(boundcol * sizeof(int));
    }



    //create field with boundary
    for(int i = 0; i < boundrow; i++)
    {
        for(int j = 0; j < boundcol; j++)
        {
            if(i == 0 || i == (boundrow - 1) || j == 0 || j == (boundcol - 1))
            {
                field[i][j] = 1;
                markedfield[i][j] = 1;
            }
            else
            {
                fscanf(infile, " %d", &field[i][j]);

                //copy element to maxfield
                markedfield[i][j] = field[i][j];
            }
        }
    }

    //read the start and finish coordinates
    fscanf(infile, " %d %d %d %d", &startr, &startcol, &finrow, &fincol);

    //display maze with boundary
    printf("Maze with 1 boundary\n");
    printmaze(field);

    //set start position
    currpos.row = startr + 1;
    currpos.col = startcol + 1;

    // loop until a path is found
    //when no path is found, exit loop
    do
    {
        int nextrow, nextcol; // row and column next path/nove
        int possiblepath = 1;  // false
        int pathmarked = 1; // false


        //check cells clockwise
        for(int cellschecked = 0; cellschecked < 8 ; cellschecked++)
        {
            move = direction(cellschecked);

            nextrow = currpos.row + move.vert;
            nextcol = currpos.col + move.horiz;

            //is likely a path and is not marked
            if(field[nextrow][nextcol] == 0 && markedfield[nextrow][nextcol] == 0)
            {
                possiblepath = 0; // true

                markedfield[nextrow][nextcol] = 2;  //set a mark
                break;
            }
            // path is already marked
            else if(field[nextrow][nextcol] == 0 && markedfield[nextrow][nextcol] == 2)
            {
                pathmarked = 0;
                continue; // check next direction
            }

        }


        if(possiblepath == 0 && pathmarked == 1)
        {

            mvforward(nextrow, nextcol);

            //check whether it's the destination
            if(currpos.row == (finrow + 1) && currpos.col == (fincol + 1)) //finrow/col + 1  to math the current coordinates of the field with boundary
            {
                printf("Path to destination is found\n");
                printf("The path is\n");

                //display path
                printpath(field, markedfield);
                flag = 1; // to stop do while loop
            }
        }
        else if(possiblepath == 0 && pathmarked == 0) // if there is a marked cell and an unmarked cell, choose the unmarked cell
        {
            mvforward(nextrow, nextcol); // move to the unmarked cell

            //check whether it's the destination
            if(currpos.row == (finrow + 1) && currpos.col == (fincol + 1))
            {
                printf("Path to destination is found\n");
                printf("The path is\n");

                //display path
                printpath(field, markedfield);;
                flag = 1; // to stop do while loop
            }
        }
        else if(possiblepath == 1 && pathmarked == 0)
        {
            field[currpos.row][currpos.col] = 9;  //permanently disable going back
            backtrack();
        }
        else if (possiblepath == 1 && pathmarked == 1)
        {
            //printf("Path to destination not found!\n");
            printf("Maze does not have any paths!\n");
            printmaze(field);
            printf("\n\n\n");
            flag = 1; // to stop the do while loop
        }


    } while(flag == 0);


    //free memory
    freememory(field, markedfield);

    return 0;
}

//functions

Node *createNode()
{
    Node *temp = (Node*)malloc(sizeof(Node));

    if(temp == NULL)
    {
        printf("Can't allocate memory\n");
        exit(1);
    }

    return temp;
}

//push coordinates to top of stack
void insert(int row, int col)
{
    Node *temp = createNode();

    temp->row = row;
    temp->col = col;
    temp->next = NULL;

    if(top != NULL)
    {
        temp->next = top; // point the current top
    }

    top = temp;  //create a new top;
}

//pop the stack
void deletetop()
{
    if(top == NULL)
    {
        printf("\nStack is empty!\n");
        exit(1);
    }
    else
    {
        Node *temp = createNode();

        temp = top;

        // new top
        top = temp->next;

        free(temp); //delete old
    }

}

//push coordinates to stack and update current position
void mvforward(int row, int col)
{
    insert(row, col);

    currpos.row = row;
    currpos.col = col;

}

//pop stack then update current position
void backtrack()
{
    deletetop();

    currpos.row = top->row;
    currpos.col = top->col;
}

// set direction of next move
Moves direction(int num)
{
    Moves move;

    switch (num)
    {
        case 0:
            move.vert = -1;
            move.horiz = 0;
            break;
        case 1:
            move.vert = -1;
            move.horiz = 1;
            break;
        case 2:
            move.vert = 0;
            move.horiz = 1;
            break;
        case 3:
            move.vert = 1;
            move.horiz = 1;
            break;
        case 4:
            move.vert = 1;
            move.horiz = 0;
            break;
        case 5:
            move.vert = 1;
            move.horiz = -1;
            break;
        case 6:
            move.vert = 0;
            move.horiz = -1;
            break;
        case 7:
            move.vert = -1;
            move.horiz = -1;
            break;

    }

    return move;
}

void printmaze(int **maze)
{
    int zero = 0;

    for(int i = 0; i < boundrow; i++)
    {
        for(int j = 0; j < boundcol; j++)
        {
            if(maze[i][j] == 9)
            {
                printf("%3d", zero);
            }
            else
            {
                printf("%3d", maze[i][j]);
            }

        }
        printf("\n");
    }
    printf("\n");
}

void printpath(int **maze, int **markedfield)
{

    char s = 'S';
    char f = 'F';
    char x = 'X';
    int zero = 0;
    //Node *temp = top;

    /*
    while(temp != NULL)
    {
        printf("(%d, %d)<-", temp->row, temp->col);
        temp = temp->next;

    }
    printf("start\n\n");
    */

    for(int i = 0; i < boundrow; i++)
    {
        for(int j = 0; j < boundcol; j++)
        {
            if(i == startr + 1 && j == startcol + 1)
            {
                printf("%3c", s);
            }
            else if(maze[i][j] == 0 && markedfield[i][j] == 2)
            {
                if(i == (finrow + 1) && j == (fincol + 1 ))
                {
                    printf("%3c", f);
                }
                else
                {
                    printf("%3c", x);
                }

            }
            else if(maze[i][j] == 9)
            {
                printf("%3d", zero);
            }
            else
            {
                printf("%3d", maze[i][j]);
            }

        }
        printf("\n");
    }

}

void freememory(int **maze, int **markedfield)
{
    //free array
    for(int i = 0; i < boundrow; i++)
    {
        free(maze[i]);
        free(markedfield[i]);
    }
    free(maze);
    free(markedfield);

    //free stack
    while(top != NULL)
    {
        Node *temp = top;
        top = temp->next;
        free(temp);
    }
}
