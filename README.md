## maze
This is a program designed to find a path in a provided maze using a backtracking algorithm.<br>
The program reads a text file containing a maze representation and outputs a path.

A Sample maze is represented by a 2D arrray as shown below.
 
    1 1 1 1 1
    1 1 0 0 0
    1 0 1 1 1
    1 1 0 0 1
    0 1 0 1 1
    
A Sample txt file that contains a maze representation is shown below.

    5 5
    1 1 1 1 1
    1 1 0 0 0
    1 0 1 1 1
    1 1 0 0 1
    0 1 0 1 1
    4 2 1 4

The first row indicates the the total rows and colums respectively.<br>
In the last row:

    -column 0 an 1 indicate the starting point's row and colunm respectively
    -column 2 and 3 indicate the end point's row and colunm respectively

