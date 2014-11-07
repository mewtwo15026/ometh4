// Declarations and functions for project #4

#include <iostream>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>

using namespace std;

typedef int ValueType; // The type of the value in a cell
const int Blank = -1;  // Indicates that a cell is blank
 
const int SquareSize = 3;  //  The number of cells in a small square
                           //  (usually 3).  The board has
                           //  SquareSize^2 rows and SquareSize^2
                           //  columns.

const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

int numSolutions = 0;

class board
// Stores the entire Sudoku board
{
public:
  board(int);
  board(string);
  void clear();
  void initialize(ifstream &fin);
  void printBoard();
  void printConflicts();
  void setCell(int, int, int);
  void print();
  void initConflicts();
  bool isBlank(int, int);
  ValueType getCell(int, int);
  void clearCell(int row, int col);
  bool isDone();
private:
  void updateConflicts(int, int, bool);
  // The following matrices go from 1 to BoardSize in each
  // dimension, i.e., they are each (BoardSize+1) * (BoardSize+1)
  vector<bool> rowConflicts;
  vector<bool> columnConflicts;
  vector<bool> squareConflicts;
  matrix<ValueType> value;
};



ostream &operator<<(ostream &ostr, vector<int> &v)
// Overloaded output operator for vector class.
{
  for (int i = 0; i < v.size(); i++)
    ostr << v[i] << " ";
  ostr << endl;
}

ostream &operator<<(ostream &ostr, vector<bool> &v)
// Overloaded output operator for vector class.
{
  for (int i = 0; i < v.size(); i++)
    ostr << v[i] << " ";
  ostr << endl;
}

int squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
  // Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
  // coordinates of the square that i,j is in.  

  return SquareSize * ((i-1)/SquareSize) + (j-1)/SquareSize + 1;
}

bool board::isDone() {
  for (int i = MinValue; i <= MaxValue; i++) {
    for (int j = MinValue; j <= MaxValue; j++) {
      if (value[i][j] == 0) return false;
    }
  }
  return true;
}

void board::updateConflicts(int row, int col, bool b=true) {
  squareConflicts.at(squareNumber(row, col)) = b;
  columnConflicts.at(col) = b;
  rowConflicts.at(row) = b;
} 


board::board(string fName)
  :value(BoardSize+1,BoardSize+1)
{
  initConflicts();
  ifstream inFile;
  inFile.open(fName.c_str());
  initialize(inFile);
}


void board::printConflicts() {
  cout << "Row Conflicts: " << rowConflicts;
  cout << "Column Conflicts: " << columnConflicts;
  cout << "Square Conflicts: " << squareConflicts;
}

void board::print() {
  printBoard();
  printConflicts();
}



void board::initConflicts() {
  rowConflicts = vector<bool> (BoardSize+1, false);
  columnConflicts = vector<bool> (BoardSize+1, false);
  squareConflicts =  vector<bool> (BoardSize+1, false);

}


board::board(int sqSize)
  : value(BoardSize+1,BoardSize+1)
    // Board constructor
{
  clear();
}

void board::setCell(int row, int col, int val) {
  value[row][col] = val;
  updateConflicts(row, col);
}

void board::clearCell(int row, int col) {
  value[row][col] = 0;
  updateConflicts(row, col, false);
}

void board::clear()
// Mark all possible values as legal for each board entry
{
  for (int i = 1; i <= BoardSize; i++)
    for (int j = 1; j <= BoardSize; j++)
      {
        value[i][j] = Blank;
      }
}

void board::initialize(ifstream &fin)
// Read a Sudoku board from the input file.
{
  char ch;

  clear();
   
  for (int i = 1; i <= BoardSize; i++)
    for (int j = 1; j <= BoardSize; j++)
      {
        fin >> ch;

        // If the read char is not Blank
        if (ch != '.') {
          setCell(i,j,ch-'0');   // Convert char to int
          updateConflicts(i, j);
        }
        else setCell(i,j,0);
	
      }
}



ValueType board::getCell(int i, int j)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
  if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
    return value[i][j];
  else
    throw rangeError("bad value in getCell");
}

bool board::isBlank(int i, int j)
// Returns true if cell i,j is blank, and false otherwise.
{
  if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
    throw rangeError("bad value in setCell");

  return (getCell(i,j) == Blank);
}

void board::printBoard()
// Prints the current board.
{
  for (int i = 1; i <= BoardSize; i++)
    {
      if ((i-1) % SquareSize == 0)
        {
          cout << " -";
          for (int j = 1; j <= BoardSize; j++)
            cout << "---";
          cout << "-";
          cout << endl;
        }
      for (int j = 1; j <= BoardSize; j++)
        {
          if ((j-1) % SquareSize == 0)
            cout << "|";
          if (!isBlank(i,j))
            cout << " " << getCell(i,j) << " ";
          else
            cout << "   ";
        }
      cout << "|";
      cout << endl;
    }

  cout << " -";
  for (int j = 1; j <= BoardSize; j++)
    cout << "---";
  cout << "-";
  cout << endl;
}

int main()
{
  ifstream fin;
  // Read the sample grid from the file.
  string fileName = "sudoku1.txt";

  fin.open(fileName.c_str());
  if (!fin)
    {
      cerr << "Cannot open " << fileName << endl;
    }

  try
    {
      board b1(fileName);

      while (fin && fin.peek() != 'Z')
        {
          b1.initialize(fin);
          b1.print();
        }
    }
  catch  (indexRangeError &ex)
    {
      cout << ex.what() << endl;
    }
}
