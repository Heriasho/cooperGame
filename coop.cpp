/*Ryan Bartoli
CSC 180 - 01 
3/22/19
*/
#include <iostream>
#include <stdlib.h>
#include <array>
#include <bits/stdc++.h> 
#include <map>
#include <cassert>
#include <iterator> 
#include <sstream>
#include <string>
#include <time.h>
#define RESET           0
#define BRIGHT          1
#define DIM             2
#define UNDERLINE       3
#define BLINK           4
#define REVERSE         7
#define HIDDEN          8
#define BLACK           0
#define RED             1
#define GREEN           2
#define YELLOW          3
#define BLUE            4
#define MAGENTA         5
#define CYAN            6
#define WHITE           7
using namespace std;

const int BOARD_COL_SIZE = 8;
const int BOARD_ROW_SIZE = 6;
const int LOWER_COL_BOARDER = -1;
const int UPPER_COL_BOARDER = 8;
const int LOWER_ROW_BOARDER = 0;
const int UPPER_ROW_BOARDER = 7;
const int LEGAL_MOVE_SIZE = 200;
int depth = 0;
int maxDepthInit = 5;
int maxDepth = maxDepthInit;
bool humanTurn, humanFirst;
bool debug = false;
bool debugValidation = false;
bool debugScoring = false;
bool debugTimer = true;
const int depthLimit = 50;
clock_t t;
void computerMove(int s[depthLimit]);
int min();
int max();
int evaluate();

char emptyCharacter = ' ';

bool check4Winner(bool print);
bool checkGameOver(bool human);
void humanMove();
void setup();
void printboard();
void makeHashMap();
void printMovesAtCurrentDepth();
void moveGenerator(bool isHuman);
void partySwap(int newCol, int newRow);
void initBoardState(char b[BOARD_COL_SIZE][BOARD_ROW_SIZE+1]);
map<char, int> letters,numbers,numbersOpponent;	
map<char, string> names, computer,human;
char b[BOARD_COL_SIZE][BOARD_ROW_SIZE+1];
string m[depthLimit][200];
int s[depthLimit];
char storePiece[depthLimit][2];
int ab[depthLimit][2];
int main(){ 
	setup();
	makeHashMap();
	//printboard();
	for (;;){
		if(humanFirst){
			humanMove();
			if(checkGameOver(false)){
				break;
			}
			computerMove(s);
			if(checkGameOver(true)){
				break;
			}
		}
		else if(!humanFirst){
			computerMove(s);
			if(checkGameOver(true)){
				break;
			}
			humanMove();
			if(checkGameOver(false)){
				break;
			}
		}  
	}
	cout << "The game is over." << endl;
}
/*void textcolor(int attr, int fg, int bg){ 
  char command[13];
  sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
  printf("%s", command);
}*/
/*8x6 board*/
void printboard(){ 
  cout << "\n  +---+---+---+---+---+---+---+---+ " << " Computer" << endl;
  cout << "6 | " << b[0][6] << " | " << b[1][6] << " | " << b[2][6] << " | " << b[3][6] << " | " << b[4][6] << " | " << b[5][6] << " | " << b[6][6] << " | " << b[7][6] << " | " << endl;
  cout << "  +---+---+---+---+---+---+---+---+ " << endl;
  cout << "5 | " << b[0][5] << " | " << b[1][5] << " | " << b[2][5] << " | " << b[3][5] << " | " << b[4][5] << " | " << b[5][5] << " | " << b[6][5] << " | " << b[7][5] << " | " << endl;
  cout << "  +---+---+---+---+---+---+---+---+ " << endl;
  cout << "4 | " << b[0][4] << " | " << b[1][4] << " | " << b[2][4] << " | " << b[3][4] << " | " << b[4][4] << " | " << b[5][4] << " | " << b[6][4] << " | " << b[7][4] << " | " << endl;
  cout << "  +---+---+---+---+---+---+---+---+ " << endl;
  cout << "3 | " << b[0][3] << " | " << b[1][3] << " | " << b[2][3] << " | " << b[3][3] << " | " << b[4][3] << " | " << b[5][3] << " | " << b[6][3] << " | " << b[7][3] << " | " << endl;
  cout << "  +---+---+---+---+---+---+---+---+ " << endl;
  cout << "2 | " << b[0][2] << " | " << b[1][2] << " | " << b[2][2] << " | " << b[3][2] << " | " << b[4][2] << " | " << b[5][2] << " | " << b[6][2] << " | " << b[7][2] << " | " << endl;
  cout << "  +---+---+---+---+---+---+---+---+ " << endl;
  cout << "1 | " << b[0][1] << " | " << b[1][1] << " | " << b[2][1] << " | " << b[3][1] << " | " << b[4][1] << " | " << b[5][1] << " | " << b[6][1] << " | " << b[7][1] << " | " << endl;
  cout << "  +---+---+---+---+---+---+---+---+ " << " Human" << endl;
  cout << "    " << "A 	" << "B   " << "C   " << "D   " << "E   " << "F   " << "G   " << "H   " << endl;
}
void initBoardState(char b[BOARD_COL_SIZE][BOARD_ROW_SIZE+1]){
  b[3][1] = 'k'; b[4][1] = 'k'; b[3][6] = 'K'; b[4][6] = 'K';
  b[0][1] = 'h'; b[1][1] = 'h'; b[0][6] = 'H'; b[1][6] = 'H';
  b[6][1] = 'b'; b[7][1] = 'b'; b[6][6] = 'B'; b[7][6] = 'B';
  for(int i=1; i<BOARD_ROW_SIZE+1;i++){
    b[i][2] = 'p';
    b[i][5] = 'P';
  }
}
void isGameOverBoardState(char b[BOARD_COL_SIZE][BOARD_ROW_SIZE+1]){
	cout << "This is a debug board state. Expected that the player make a move and then the computer loses because they have no moves." << endl;
	b[2][5] = 'p'; b[0][6] = 'K';
	b[2][1] = 'P'; b[0][1] = 'k';
  }
void horseGotSeniority(char b[BOARD_COL_SIZE][BOARD_ROW_SIZE+1]){
	cout << "This is a debug board state. Expected that the player can attack backwards on horse." << endl;
	b[1][6] = 'h'; b[5][6] = 'K';
	b[3][5] = 'P'; b[5][1] = 'k';
  }
void bishopGotSeniority(char b[BOARD_COL_SIZE][BOARD_ROW_SIZE+1]){
	cout << "This is a debug board state. Expected that the player can attack backwards on bishop." << endl;
	b[6][6] = 'b'; b[5][6] = 'K';
	b[4][4] = 'P'; b[5][1] = 'k';
  }
void makeHashMap(){	
	letters.insert(pair<const char,int>('A',0));
	letters.insert(pair<const char,int>('B',1));
	letters.insert(pair<const char,int>('C',2));
	letters.insert(pair<const char,int>('D',3));
	letters.insert(pair<const char,int>('E',4));
	letters.insert(pair<const char,int>('F',5));
	letters.insert(pair<const char,int>('G',6));
	letters.insert(pair<const char,int>('H',7));

	//m.insert(pair<const char,int>(emptyCharacter,0));
	numbers.insert(pair<const char,int>('1',1));
	numbers.insert(pair<const char,int>('2',2));
	numbers.insert(pair<const char,int>('3',3));
	numbers.insert(pair<const char,int>('4',4));
	numbers.insert(pair<const char,int>('5',5));
	numbers.insert(pair<const char,int>('6',6));

	numbersOpponent.insert(pair<const char,int>('1',6));
	numbersOpponent.insert(pair<const char,int>('2',5));
	numbersOpponent.insert(pair<const char,int>('3',4));
	numbersOpponent.insert(pair<const char,int>('4',3));
	numbersOpponent.insert(pair<const char,int>('5',2));
	numbersOpponent.insert(pair<const char,int>('6',1));
  
	names.insert(pair<const char,string>('K',"King"));
	names.insert(pair<const char,string>('k',"King"));
	names.insert(pair<const char,string>('H',"Horse"));
	names.insert(pair<const char,string>('h',"Horse"));
	names.insert(pair<const char,string>('B',"Bishop"));
	names.insert(pair<const char,string>('b',"Bishop"));
	names.insert(pair<const char,string>('P',"Pawn"));
	names.insert(pair<const char,string>('p',"Pawn"));

	computer.insert(pair<const char,string>('K',"King"));
	computer.insert(pair<const char,string>('H',"Horse"));
	computer.insert(pair<const char,string>('B',"Bishop"));
	computer.insert(pair<const char,string>('P',"Pawn"));

	human.insert(pair<const char,string>('k',"King"));
	human.insert(pair<const char,string>('h',"Horse"));
	human.insert(pair<const char,string>('b',"Bishop"));
	human.insert(pair<const char,string>('p',"Pawn"));
}
void setup(){ 
  //textcolor(RESET,WHITE,BLACK);    // resets to normal white on black
  //textcolor(BRIGHT,RED,BLACK);     // enables bright red on black
  //textcolor(RESET,YELLOW,BLACK);   // enables normal yellow on black
  char turn;
  for(;;){
    cout << " Would you like to play first (1) or second (2)?" << endl;
    cin >> turn;
    if(turn == '1') {
      //human move first
      cout << " Human goes first. " << endl;
      humanFirst = true;
      break;
    }
    else if(turn == '2'){
      //computer first
      cout << " Computer goes first. " << endl;
      humanFirst = false;
      break;
    }
    else{
      cout << "Invalid input: " << turn << endl;
      continue;
    } 
  }
  humanTurn = humanFirst;

  /*Board creation*/
  for (int i=0; i<8; i++) {
    for (int j=0; j<7; j++) {
      b[i][j]=emptyCharacter;
    }
  }
  initBoardState(b);
  //isGameOverBoardState(b);
  //horseGotSeniority(b);
  //bishopGotSeniority(b);
}
/*COMPLETE*/ /*COMPLETE FOR COMPUTER*/ 
void movePiece(string selectedMove, bool output){
	char char_array[4];
	strcpy(char_array, selectedMove.c_str()); //Convert selectedMove string to char
	int col = letters.find(char_array[0])->second; //First has to be letter
	int row = numbers.find(char_array[1])->second; //Second has to be number
	int newCol = letters.find(char_array[2])->second; //Third has to be letter
	int newRow = numbers.find(char_array[3])->second; //Fourth has to be number

	storePiece[depth][0] = b[col][row];
	storePiece[depth][1] = b[newCol][newRow];
	//cout << "MOVING PIECE: " << b[col][row] << " to " <<  b[newCol][newRow] << endl; 
	b[newCol][newRow] = b[col][row];	
	b[col][row] = emptyCharacter;

	partySwap(newCol, newRow);

	if(output){
		cout << "Did move " << selectedMove << endl;
	}
}
string convertNotation(string selectedMove){
	char char_array[4];
	strcpy(char_array, selectedMove.c_str()); //Convert selectedMove string to char
	int col = letters.find(char_array[0])->second; //First has to be letter
	int row = numbers.find(char_array[1])->second; //Second has to be number
	int newCol = letters.find(char_array[2])->second; //Third has to be letter
	int newRow = numbers.find(char_array[3])->second; //Fourth has to be number

	string s = "";
    s += ('A'+col);
    s += ('7'-row);
    s += ('A'+newCol);
	s += ('7'-newRow);
	
	return s;
}
void undoMove(string selectedMove){
	char char_array[4];
	strcpy(char_array, selectedMove.c_str()); //Convert selectedMove string to char
	int col = letters.find(char_array[0])->second; //First has to be letter
	int row = numbers.find(char_array[1])->second; //Second has to be number
	int newCol = letters.find(char_array[2])->second; //Third has to be letter
	int newRow = numbers.find(char_array[3])->second; //Fourth has to be number	
	b[col][row] = storePiece[depth][0]; //UNDOS MOVE
	b[newCol][newRow] = storePiece[depth][1]; //UNDOS MOVE
}
/*COMPLETE*/ /*COMPLETE FOR COMPUTER*/
bool inBoardMove(int col, int row){ //Lower_row = -1, Upper_row = 7, Lower_col = -1, Upper_col = 8
	if(debug) cout << "Row: " << row << " is less than " << LOWER_ROW_BOARDER << " | row " << " is less than " << UPPER_ROW_BOARDER << endl;
	return (row > LOWER_ROW_BOARDER && row < UPPER_ROW_BOARDER && col > LOWER_COL_BOARDER && col <= UPPER_COL_BOARDER);
}
/*COMPLETE*/ /*COMPLETE FOR COMPUTER*/
bool isEmpty(char store){
	return (store == emptyCharacter);
}
/*COMPLETE*/ /*COMPLETE FOR COMPUTER*/
bool samePosition(int initialI, int initialJ, int newCol, int newRow){
	return (b[initialI][initialJ] == b[newCol][newRow]);
}
/*COMPLETE*/ /*COMPLETE FOR COMPUTER*/
bool isPiece(int col, int row){
	char store = b[col][row];
	return (store != emptyCharacter);
}
/*COMPLETE*/ /*COMPLETE FOR COMPUTER*/
bool isSenior(int row, char storedInit){
	if(human.count(storedInit)>0){
		//Is human piece
		if(row > 3){
			return true;
		} else{
			return false;
		}
	} else if(computer.count(storedInit)>0){
		//Is computer piece
		if(row < 4){
			return true;
		} else{
			return false;
		}
	} else{
		cout << "How the hell did you get here." <<endl;
		return false;
	}
}
/*COMPLETE*/ /*COMPLETE FOR COMPUTER*/
void partySwap(int newCol, int newRow){
	if(newCol > 3 && b[newCol][newRow] == 'h'){ //horse to bishop
		b[newCol][newRow] = 'b';
	} 
	if(newCol < 4 && b[newCol][newRow] == 'b'){ //bishop to horse
		b[newCol][newRow] = 'h';
	} 
	if(newCol > 3 && b[newCol][newRow] == 'H'){ //Horse to Bishop
		b[newCol][newRow] = 'B';
	} 
	if(newCol < 4 && b[newCol][newRow] == 'B'){ //Bishop to Horse
		b[newCol][newRow] = 'H';
	}
}
/*LIKELY NEEDED FOR COMPUTER, MAYBE NOT*/
int mapAlphaToInt(char c){
	return (c-64);
}
/*LIKELY NEEDED FOR COMPUTER, MAYBE NOT*/
int mapCharIntToInt(char c){
	return (c-emptyCharacter);
}
/*COMPLETE*/ /*COMPLETE FOR COMPUTER*/
bool isPositionOrPieceInvalid(int col, int row, int newCol, int newRow){
	if(!isPiece(col,row) || !inBoardMove(newCol,newRow) || samePosition(col,row,newCol,newRow)){ 
		return true;
	} else{
		if(debug) if(isPiece(col,row)) cout << "Is a piece. " << endl;
		if(debug) if(inBoardMove(newCol,newRow)) cout << "Target position is in the board. " << endl;
		if(debug) if(!samePosition(col,row,newCol,newRow)) cout << "Target is not the same position. " << endl;
		return false;
	}
}

/*COMPLETE*/ /*COMPLETE FOR COMPUTER*/
bool isValidKingMove(int col, int row, int newCol, int newRow,char storedInit, char storedNew, bool isHuman){
	if(!isPositionOrPieceInvalid(col,row,newCol,newRow)){
		if(((col<4 && col-1 == newCol && newCol < col) || (col>3 && col+1 == newCol && newCol > col)) && row == newRow) {
			if(isEmpty(storedNew)){
				if(debugValidation)cout << "Valid " << names.find(storedInit)->second << " move" << endl;
				return true; //Move
			} else if (computer.count(storedInit)>0 && !isHuman){
				if(human.count(storedNew)>0){
					if(debug) cout << "Valid " << names.find(storedInit)->second << " attack against " << human.find(storedNew)->second << endl;
					return true; //Attack
				} else {
						if(debug) cout << "Invalid " << names.find(storedInit)->second << " move" << endl; 
						return false;
				}
			} else if ((human.count(storedInit) > 0) & isHuman){
				if(computer.count(storedNew)>0){
					if(debug) cout << "Valid " << names.find(storedInit)->second << " attack against " << computer.find(storedNew)->second << endl;
					return true; //Attack
				} else {
					if(debug) cout << "Invalid " << names.find(storedInit)->second << " move" << endl; 
					return false;
				}			
			} else{
					if(debug) cout << "Invalid " << names.find(storedInit)->second << " move" << endl;
					return false;
				}
			}
	}
	if(debug) cout << "Invalid " << names.find(storedInit)->second << " move" << endl; 
	return false;
}
/*COMPLETE*/ /*COMPLETE FOR COMPUTER*/
bool isValidPawnMove(int col, int row, int newCol, int newRow,char storedInit, char storedNew, bool isHuman){
	if(!isPositionOrPieceInvalid(col, row, newCol, newRow)){
		if(human.count(storedInit)>0 && isHuman){ //HUMAN MOVE
			if(isEmpty(storedNew) && newRow > row && newRow == row+1 && newCol == col){
				if(debugValidation) cout << "Valid Human " << names.find(storedInit)->second << " move at " << col << " " << row << " to " << newCol << " " << newRow << endl;
				return true; //Move;
				//Computer version has, newRow == row-1 for both cases, human.count(storedNew)>0, newRow < row, newCol != col
			} else if (((newCol == col+1 && newRow == row+1) || (newCol == col-1 && newRow == row+1)) && computer.count(storedNew)>0 && newRow > row && newCol != col){
					if(debugValidation) if(debug) cout << "Valid Human " << names.find(storedInit)->second << " attack against " << computer.find(storedNew)->second << endl;
				return true; //Attack;
			} else{
				if(debug) cout << "Invalid " << names.find(storedInit)->second << " move" << endl;
				return false;
			}
		} else if (computer.count(storedInit)>0 && !isHuman){ //COMPUTER MOVE
			if(isEmpty(storedNew) && newRow < row && newRow == row-1 && newCol == col){
				if(debugValidation) cout << "Valid Computer " << names.find(storedInit)->second << " move at " << col << " " << row << " to " << newCol << " " << newRow << endl;
				return true; //Move;
				//Computer version has, newRow == row-1 for both cases, human.count(storedNew)>0, newRow < row, newCol != col
			} else if (((newCol == col+1 && newRow == row-1) || (newCol == col-1 && newRow == row-1)) && human.count(storedNew)>0 && newRow < row && newCol != col){
				if(debugValidation) cout << "Valid Computer " << names.find(storedInit)->second << " attack against " << human.find(storedNew)->second << endl;
				return true; //Attack;
			} else{
				if(debug) cout << "Invalid " << names.find(storedInit)->second << " move for " << col << " " << row << " to " << newCol << " " << newRow << endl;
				return false;
			}		
		} else {
			if(debug) cout << "Invalid " << names.find(storedInit)->second << " move" << endl;
			return false;
		}
	}
	if(debug) cout << "Invalid " << names.find(storedInit)->second << " move" << endl; 
	return false;
}
/*COMPLETE*/ /*COMPLETE FOR COMPUTER*/
bool isValidBishopMove(int col, int row, int newCol, int newRow, char storedInit, char storedNew, bool isHuman) {
   if(human.count(storedInit) && isHuman){
		if (col < newCol && row < newRow) { //going up and to the right <HUMAN>
			for (int i = 1; i<newRow; i++) { //loop from initial row to new row
				if(col+i == newCol && row+i == newRow) { //if reached target, is it a move or attack
					if(computer.count(storedNew) > 0) { //attack
						if(debugValidation) cout << "Valid Human " << names.find(storedInit)->second << " attack against " << computer.find(storedNew)->second << endl;
						return true;
					} else if (isEmpty(storedNew)) { //move
						if(debugValidation) cout << "Valid Human " << names.find(storedInit)->second << " move" << endl;
						return true;
					} else {
						if(debug) cout << "Invalid " << names.find(storedInit)->second << " move" << endl; 
						return false; //must have reached your piece, illegal move
					}   
				}
				if(!isEmpty(b[col+i][row+i])) { //obstruction
					if(debug) cout << "Invalid " << names.find(storedInit)->second << " move" << endl; 
					return false;
				}
			}
			return false;
		} else if(col > newCol && row < newRow) { //going up and to the left <HUMAN>
			for (int i = 1; i<newRow; i++) { //loop from colal row to new row
				if(col-i == newCol && row+i == newRow) { // if reached target, is it move or attack
					if(computer.count(storedNew) > 0) { //attack
						if(debugValidation) cout << "Valid " << names.find(storedInit)->second << " attack against " << computer.find(storedNew)->second << endl;
						return true;
					} else if (isEmpty(storedNew)) { //move
						if(debugValidation) cout << "Valid Human " << names.find(storedInit)->second << " move" << endl;
						return true;
					} else {
						if(debug) cout << "Invalid " << names.find(storedInit)->second << " move" << endl; 
						return false; //must have reached your piece, illegal move
					}     
				}
				if(!isEmpty(b[col-i][row+i])) { //obstruction
					if(debug) cout << "Invalid " << names.find(storedInit)->second << " move" << endl; 
					return false;
				}
			}
			if(debug) cout << "Invalid " << names.find(storedInit)->second << " move" << endl; 
			return false;
		} else if(isSenior(row,storedInit) && col < newCol && row > newRow){ //Backward down going right if Senior.
				for (int i = 1; i<row; i++) { //loop from initial row to new row
					if(col+i == newCol && row-i == newRow) { //if reached target, is it a move or attack
						if(computer.count(storedNew) > 0) { //attack
							if(debugValidation) cout << "Valid Senior " << names.find(storedInit)->second << " attack against " << computer.find(storedNew)->second << endl;
							return true;
						} else {
							if(debug) cout << "Invalid Senior " << names.find(storedInit)->second << " move" << endl; 
							return false; //must have reached your piece, illegal move
						}   
					}
					if(!isEmpty(b[col+i][row-i])) { //obstruction
						if(debug) cout << "Invalid Senior" << names.find(storedInit)->second << " move." << " Blockage!" << endl; 
						return false;
					}
				}
				return false;
		} else if(isSenior(row,storedInit) && col > newCol && row > newRow){ //Backward down going left if Senior.
				for (int i = 1; i<row; i++) { //loop from initial row to new row
					if(col-i == newCol && row-i == newRow) { //if reached target, is it a move or attack
						if(computer.count(storedNew) > 0) { //attack
							if(debug) cout << col << row << newCol << newRow << endl;
							if(debug) cout << "Valid Senior " << names.find(storedInit)->second << " attack against " << computer.find(storedNew)->second << endl;
							return true;
						} else {
							if(debug) cout << col << row << newCol << newRow << endl;
							if(debug) cout << "Invalid Senior " << names.find(storedInit)->second << " move" << endl; 
							return false; //must have reached your piece, illegal move
						}   
					}
					if(!isEmpty(b[col-i][row-i])) { //obstruction
						if(debug) cout << "Invalid Senior" << names.find(storedInit)->second << " move." << " Blockage at " << col-i << row-i << endl; 
						return false;
					}
				}
				return false;
   		} else {
	  			if(debug) cout << "Invalid " << names.find(storedInit)->second << " move" << endl; 
      			return false;
   				}
	   } else if (computer.count(storedInit) && !isHuman){ 
		if (col < newCol && row > newRow){ //going down and to the right <COMPUTER>
			for(int i = 1; i < row; i++){
				if(col+i == newCol && row-i == newRow){
					if(human.count(storedNew)>0){
						if(debugValidation) cout << "Valid " << names.find(storedInit)->second << " attack against " << computer.find(storedNew)->second << endl;
						return true;
					} else if (isEmpty(storedNew)) {
						if(debugValidation) cout << "Valid Computer " << names.find(storedInit)->second << " move" << endl;
						return true;
					} else {
						if(debug) cout << "Invalid 5 " << names.find(storedInit)->second << " move" << endl; 
						return false;
					}
				}
				if(!isEmpty(b[col+i][row-i])){
					if(debug) cout << "Invalid 4 " << names.find(storedInit)->second << " move" << endl; 
					return false;
				}
			}
			return false;
		} else if(col > newCol && row > newRow){ //going down and to the left <COMPUTER>
			for (int i = 1; i<row; i++){
				if(col-i == newCol && row-i == newRow) { // if reached target, is it move or attack
					if(human.count(storedNew) > 0) { //attack
						if(debugValidation) cout << "Valid " << names.find(storedInit)->second << " attack against " << computer.find(storedNew)->second << endl;
						return true;
					} else if (isEmpty(storedNew)) { //move
						if(debugValidation) cout << "Valid " << names.find(storedInit)->second << " move" << endl;
						return true;
					} else {
						if(debug) cout << "Invalid 2 " << names.find(storedInit)->second << " move" << endl; 
						return false; //must have reached your piece, illegal move
					}
				}
				if(!isEmpty(b[col-i][row-i])) { //obstruction
					if(debug) cout << "Invalid 1 " << names.find(storedInit)->second << " move" << endl; 
					return false;
				}
			}
			if(debug) cout << "Invalid 3 " << names.find(storedInit)->second << " move" << endl; 
			return false;     
		} else if(isSenior(row,storedInit) && col < newCol && row < newRow){ //Backup & right <COMPUTER> if senior
				for (int i = 1; i<newRow; i++) { //loop from initial row to new row
					if(col+i == newCol && row+i == newRow) { //if reached target, is it a move or attack
						if(human.count(storedNew) > 0) { //attack
							if(debugValidation) cout << "Valid Senior " << names.find(storedInit)->second << " attack against " << human.find(storedNew)->second << endl;
							return true;
						} else {
							if(debugValidation) cout << "Invalid Senior " << names.find(storedInit)->second << " move" << endl; 
							return false; //must have reached your piece, illegal move
						}   
					}
					if(!isEmpty(b[col+i][row+i])) { //obstruction
						if(debugValidation) cout << "Invalid Senior " << names.find(storedInit)->second << " move." << " Blockage!" << endl; 
						return false;
					}
				}
				return false;			
		} else if(isSenior(row,storedInit) && col > newCol && row < newRow){ //Backup & left <COMPUTER> if senior
				for (int i = 1; i<newRow; i++) { //loop from initial row to new row
					if(col-i == newCol && row+i == newRow) { //if reached target, is it a move or attack
						if(human.count(storedNew) > 0) { //attack
							if(debugValidation) cout << "Valid Senior " << names.find(storedInit)->second << " attack against " << human.find(storedNew)->second << endl;
							return true;
						} else {
							if(debug) cout << "Invalid Senior " << names.find(storedInit)->second << " move" << endl; 
							return false; //must have reached your piece, illegal move
						}   
					}
					if(!isEmpty(b[col-i][row+i])) { //obstruction
						if(debug) cout << "Invalid Senior" << names.find(storedInit)->second << " move." << " Blockage!" << endl; 
						return false;
					}
				}
				return false;
		} else {
			if(debug) cout << "Invalid 22" << names.find(storedInit)->second << " move" << endl; 
			return false;
		}
	} else {
		if(debug) cout << "Invalid 44" << names.find(storedInit)->second << " move" << endl; 
		return false;
	}
}
/*COMPLETE*/ /*COMPLETE FOR COMPUTER*/
bool isValidHorseMove(int col, int row, int newCol, int newRow,char storedInit, char storedNew, bool isHuman){
	if(!isPositionOrPieceInvalid(col,row,newCol,newRow)){
		if(human.count(storedInit)>0 && isHuman){
			if(isEmpty(storedNew)&&(newRow > row) &&
				(((newRow==row+2)&&(col-1 == newCol || col+1 == newCol)) || 
				((newRow==row+1)&&(col-2 == newCol || col+2 == newCol)))){
				if(debugValidation) cout << "Valid Human " << names.find(storedInit)->second << " move" << endl;
				return true; //Move
			} else if(computer.count(storedNew)>0 &&(newRow > row) &&
				(((newRow==row+2)&&(col-1 == newCol || col+1 == newCol)) || 
				((newRow==row+1)&&(col-2 == newCol || col+2 == newCol)))){
				if(debugValidation) cout << "Valid " << names.find(storedInit)->second << " attack against " << computer.find(storedNew)->second << endl;
				return true; //Generic attack
			} else if(isSenior(row, storedInit) && computer.count(storedNew)>0 &&
				(((newRow==row-2)&&(col-1 == newCol || col+1 == newCol)) || 
				((newRow==row-1)&&(col-2 == newCol || col+2 == newCol)))){
				if(debugValidation) cout << "Valid Senior " << names.find(storedInit)->second << " attack against " << computer.find(storedNew)->second << endl;
				return true; //Senior attack
			} else {
				if(debug) cout << "Invalid 55" << names.find(storedInit)->second << " move" << endl;
				return false;
			}
		} else if(computer.count(storedInit)>0 && !isHuman){
			if(isEmpty(storedNew)&&(newRow < row) &&
				(((newRow == row-2)&&(col-1 == newCol || col+1 == newCol)) ||
				((newRow == row-1)&&(col-2 == newCol || col+2 == newCol)))) {
				if(debug) cout << "stored new: " << storedNew << endl;
				if(debugValidation) cout << "Valid Computer " << names.find(storedInit) -> second << " move" << endl;
				return true;
			  } else if(human.count(storedNew)>0 && (newRow < row) &&
				(((newRow == row-2)&&(col-1 == newCol || col+1 == newCol)) ||
				((newRow == row-1)&&(col-2 == newCol || col+2 == newCol)))) {
				if(debugValidation) cout << "Valid " << names.find(storedInit) -> second << " attack against" << human.find(storedNew)->second << endl;
				return true;
			  } else if(isSenior(row, storedInit) && human.count(storedNew)>0 &&
				(((newRow == row+2)&&(col-1 == newCol || col+1 == newCol)) ||
				((newRow == row+1)&&(col-2 == newCol || col+2 == newCol)))) {
				if(debugValidation) cout << "Valid Senior " << names.find(storedInit) -> second << " attack against" << human.find(storedNew)->second << endl;
				return true;
			  } else {
				if(debug) cout << "Invalid 35" << names.find(storedInit) -> second << " move" << endl;
				return false;
		  }   
		} else{
			cout << "How did you get here?" << endl;
			return false;
		}
	}
	if(debug) cout << "Invalid " << names.find(storedInit)->second << " move" << endl; 
	return false;
}
bool validateHumanMove(string move){
	for(int i = 0; i < s[depth]; i++){
		if(depth != 0) cout << "Something is terribly wrong." << endl;
		if(move == m[depth][i]){
			return true;
		}
	}
	return false;
}
/*JUST ABOUT COMPLETE; CHECK TODO LIST*/
void humanMove(){ 
	printboard();
	moveGenerator(true);
	printMovesAtCurrentDepth();
	string input;
  	for(;;){
		cout << "Please make a move, (Q)uit or (P)rint board: " << endl;
		cin >> input;
		cout << endl;
		int n = input.length();
		char char_array[n+1];
		strcpy(char_array,input.c_str());
		if(char_array[0] == 'Q' || char_array[0] == 'q'){
			cout << "Quitting... " << endl;
			exit(0);
		}
		if(char_array[0] == 'P' || char_array[0] == 'p'){
			printboard();
			printMovesAtCurrentDepth();
			continue;
		}
		if(validateHumanMove(input)){
			movePiece(input, true);
			break;
		}
 	}
}
void addToLegalMoveArray(int &move, int col, int row, int newCol, int newRow){
    string s = "";
    s += ('A'+col);
    s += ('0'+row);
    s += ('A'+newCol);
    s += ('0'+newRow);
	m[depth][move] = s; 
	move++;
	//cout << col << row << newCol << newRow << endl;
}
void getValidPawnMoves(int &move, int col,int row, string m[depthLimit][200], bool human){
	char storedInit = b[col][row];
	if(!human){
		if(isValidPawnMove(col, row, col, row-1, storedInit, b[col][row-1],human)){
			addToLegalMoveArray(move,col,row,col,row-1);
		}
		if(isValidPawnMove(col, row, col-1, row-1, storedInit, b[col-1][row-1],human)){
			addToLegalMoveArray(move,col,row,col-1,row-1);
		}
		if(isValidPawnMove(col, row, col+1, row-1, storedInit, b[col+1][row-1],human)){
			addToLegalMoveArray(move,col,row,col+1,row-1);
		} else {
			if(debug) cout << "Invalid pawn move trying to set " << col << " " << row << " to " << col << " " << row << endl;
		}
	} else if(human) {
		if(isValidPawnMove(col, row, col, row+1, storedInit, b[col][row+1],human)){
			addToLegalMoveArray(move,col,row,col,row+1);
		}
		if(isValidPawnMove(col, row, col-1, row+1, storedInit, b[col-1][row+1],human)){
			addToLegalMoveArray(move,col,row,col-1,row+1);
		}
		if(isValidPawnMove(col, row, col+1, row+1, storedInit, b[col+1][row+1],human)){
			addToLegalMoveArray(move,col,row,col+1,row+1);
		} else {
			if(debug) cout << "Invalid pawn move trying to set " << col << " " << row << " to " << col << " " << row << endl;
		}		
	} else{
		if(debug) cout << "How did you get here?" << endl;
	}

	/*Get legal moves, convert into a string*/
}
void getValidBishopMove(int &move, int col,int row, string m[depthLimit][200], bool human){
	string s;
	char storedInit = b[col][row];
	for(int i=1;;i++){
		if(isPositionOrPieceInvalid(col,row,col+i,row-i)){ //DOWN RIGHT col+i row-i
			break;
		}
		else if(isValidBishopMove(col,row,col+i,row-i, storedInit, b[col+i][row-i],human)){
			addToLegalMoveArray(move,col,row,col+i,row-i);
			continue;
		}
	}
	for(int i=1;;i++){
		if(isPositionOrPieceInvalid(col,row,col-i,row+i)){ //UP LEFT col-1 row+1
			break;
		}
		else if(isValidBishopMove(col,row,col-i,row+i, storedInit, b[col-i][row+i],human)){
			addToLegalMoveArray(move,col,row,col-i,row+i);
			continue;
		}
	}	
	for(int i=1;;i++){
		if(isPositionOrPieceInvalid(col,row,col+i,row+i)){ //UP RIGHT col+1 row+1
			break;
		}
		if(isValidBishopMove(col,row,col+i,row+i, storedInit, b[col+i][row+i],human)){
			addToLegalMoveArray(move,col,row,col+i,row+i);
			continue;
		}
	}	
	for(int i=1;;i++){
		if(isPositionOrPieceInvalid(col,row,col-i,row-i)){ //DOWN LEFT col-1 row-1
			break;
		}
		if(isValidBishopMove(col,row,col-i,row-i, storedInit, b[col-i][row-i],human)){
			addToLegalMoveArray(move,col,row,col-i,row-i);
			continue;
		}
	}	
}
void getValidHorseMove(int &move, int col,int row, string m[depthLimit][200], bool human){
	string s;
	char storedInit = b[col][row];
	if(isValidHorseMove(col, row, col+1, row+2, storedInit, b[col+1][row+2],human)){ //COL+1 ROW+2 SENIOR MOVE
		addToLegalMoveArray(move,col,row,col+1,row+2);
	} 
	if(isValidHorseMove(col, row, col+2, row+1, storedInit, b[col+2][row+1],human)){ //COL+2 ROW+1 SENIOR MOVE	
		addToLegalMoveArray(move,col,row,col+2,row+1);
	}
	if(isValidHorseMove(col, row, col-1, row+2, storedInit, b[col-1][row+2],human)){ //COL-1 ROW+2 SENIOR MOVE 
		addToLegalMoveArray(move,col,row,col-1,row+2);	
	} 
	if(isValidHorseMove(col, row, col-2, row+1, storedInit, b[col-2][row+1],human)){ //COL-2 ROW+1 SENIOR MOVE 
		addToLegalMoveArray(move,col,row,col-2,row+1);	
	} 
	if(isValidHorseMove(col, row, col+2, row-1, storedInit, b[col+2][row-1],human)){ //COL+2 ROW-1  
		addToLegalMoveArray(move,col,row,col+2,row-1);
	} 
	if(isValidHorseMove(col, row, col+1, row-2, storedInit, b[col+1][row-2],human)){ //COL+1 ROW-2 
		addToLegalMoveArray(move,col,row,col+1,row-2);
	} 
	if(isValidHorseMove(col, row, col-1, row-2, storedInit, b[col-1][row-2],human)){ //COL-1 ROW-2 
		addToLegalMoveArray(move,col,row,col-1,row-2);
	} 
	if(isValidHorseMove(col, row, col-2, row-1, storedInit, b[col-2][row-1],human)){ //COL-2 ROW-1 
		addToLegalMoveArray(move,col,row,col-2,row-1);
	} 
}
void getValidKingMove(int &move, int col,int row, string m[depthLimit][200], bool human){
	string s;
	char storedInit = b[col][row];
	if(col<4 && isValidKingMove(col, row, col-1, row, storedInit, b[col-1][row],human)){ //LEFT KING
		addToLegalMoveArray(move,col,row,col-1,row);
	}
	if(col>3 && isValidKingMove(col, row, col+1, row, storedInit, b[col+1][row],human)){ //RIGHT KING
		addToLegalMoveArray(move,col,row,col+1,row);
	}	
}
/*COMPLETE*/
void printMovesAtCurrentDepth(){
	if(depth%2==0 && debug) cout << "This is a Computer depth" << endl;
	if(depth%2==1 && debug) cout << "This is a Human depth" << endl; 
	cout << "We have " << s[depth] << " valid moves" << endl;
	string moves = "";
	for(int i=0; i<s[depth]; i++){
		moves += m[depth][i] + " ";
	}
	cout << moves << endl;
}
/*COMPLETE*/
void moveGenerator(bool isHuman){
	int move = 0;
	char storedInit;
	for(int i=0;i<BOARD_COL_SIZE;i++){
		for(int j=0;j<BOARD_ROW_SIZE+1;j++){
			storedInit = b[i][j];
			if(debug) cout << "Stored piece at " << i << " " << j << " is " << names.find(storedInit) -> second << " also known as " << storedInit << endl;
			if(isEmpty(storedInit)){ 
				continue;
			} else if (isHuman && human.count(storedInit)>0){ // HUMAN MOVES
				if(storedInit == 'p'){
					getValidPawnMoves(move, i, j, m,isHuman);
				} else if (storedInit == 'b'){
					getValidBishopMove(move, i, j, m,isHuman);
				} else if (storedInit == 'h'){
					getValidHorseMove(move,i,j,m,isHuman);
				} else if (storedInit == 'k'){
					getValidKingMove(move,i,j,m,isHuman);
				} else {
					cout << "Not a piece" << endl;
				}
			} else if (!isHuman && computer.count(storedInit)>0){ //COMPUTER MOVES
				if(storedInit == 'P'){
					getValidPawnMoves(move, i, j, m,isHuman);
				} else if (storedInit == 'B'){
					//cout << "Got here" << endl;
					getValidBishopMove(move, i, j, m,isHuman);
				} else if (storedInit == 'H'){
					getValidHorseMove(move,i,j,m,isHuman);
				} else if (storedInit == 'K'){
					getValidKingMove(move,i,j,m,isHuman);
				} else {
					cout << "Not a piece" << endl;
				}
			} else {
				if(debug) cout << "Found no moves" << endl;
			}
		}
	}
	s[depth] = move;
}
/*COMPLETE*/
void minMax(){
	long timeRemaining = (long)CLOCKS_PER_SEC * 5.1;
	if(debugTimer) cout << "Clicks allowed " << timeRemaining << endl;
	long timeTaken = 0;
	depth = 0;
	string bestMove;
	int bestScore;
	int cnt = 1;
	while(timeRemaining > timeTaken * 2 && maxDepth <= depthLimit){
	//for(int cnt = 0; cnt < 1; cnt++){
		t = clock();
		if(debugTimer) cout << "Attempting depth " << maxDepth << " plys." << endl;
		int thisScore = INT_MIN;
		ab[depth][0] = INT_MIN; //Alpha lower bound
		ab[depth][1] = INT_MAX; //Beta upper bound
		string bestThis;
		for(int i=0; i<s[depth]; i++){ //LOOP THROUGH MOVES
			string selectedMove = m[depth][i]; //Get the iterated move within moves at depth.
			//cout << "Testing move " << selectedMove << endl;
			movePiece(selectedMove, false);

			ab[depth+1][0] = ab[depth][0];
			ab[depth+1][1] = ab[depth][1];
			int score = min();
			//cout << "Eval depth " << depth << " is " << score << " for move " << selectedMove << endl;
			//cout << "Score for move is " << score << endl;
			if(score > thisScore){ 
				//cout << "New best score " << score << endl;
				bestThis = selectedMove;
				thisScore=score;
			}
			if(ab[depth+1][0] > ab[depth][0]){
				ab[depth][0] = ab[depth+1][0];
			}
			if(ab[depth][0] >= ab[depth][1]){
				undoMove(selectedMove);
				break;
			}
			undoMove(selectedMove);
		}
		t = clock() - t;
		timeTaken = (long)t;
		if(debugTimer) cout << "Clicks used " << timeTaken << ". Clicks remaining " << timeRemaining - timeTaken << endl;
		timeRemaining -= timeTaken;
		if(timeRemaining >= 0){
			bestMove = bestThis;
			bestScore = thisScore;
		}
		if(timeRemaining < 0){
			cout << "Time ran out. Using result from " << maxDepth-1 << " plys." << endl;
		}
		maxDepth++;
	}
	maxDepth = maxDepthInit;
	cout << "My move is " << bestMove << "  with a score of " << bestScore << endl;
	cout << "COMPUTER MOVES " << bestMove << " (" << convertNotation(bestMove) << ")" << endl;
	movePiece(bestMove , true);
}
/*COMPLETE XD*/
void computerMove(int s[depthLimit]){ 
	printboard();
	moveGenerator(false);
	printMovesAtCurrentDepth();
	minMax();
}
/*COMPLETE*/
int min(){
	depth++;
	moveGenerator(true);
	if (check4Winner(false)){
		depth--;
		return (INT_MAX-depthLimit+depth); //TODO: fix
	}
	if (depth == maxDepth){
		int val = evaluate();
		depth--;
		return val;
	}
	int bestScore = INT_MAX;
	string selectedMove;
	//printMovesAtCurrentDepth();
	for(int i=0; i<s[depth]; i++){ //LOOP THROUGH MOVES
		string selectedMove = m[depth][i]; //Get the iterated move within moves at depth.
		movePiece(selectedMove, false);

		ab[depth+1][0] = ab[depth][0];
		ab[depth+1][1] = ab[depth][1];
		int score = max();
		//cout << "Eval depth " << depth << " is " << score << " for move " << selectedMove << endl;

		if(score < bestScore){ 
			//cout << "New best score " << bestScore << endl;
			bestScore=score;
		}
		if(ab[depth+1][0] < ab[depth][1]){
			ab[depth][1] = ab[depth+1][0];
		}
		if(ab[depth][0] >= ab[depth][1]){
			undoMove(selectedMove);
			break;
		}

		undoMove(selectedMove);
	}
	depth--;
  	return(bestScore);
}
/*COMPLETE*/
int max(){
	depth++;
	moveGenerator(false);
	if (check4Winner(false)){
		depth--;
		return (INT_MIN+depthLimit-depth); //TODO: fix
	}
	if (depth == maxDepth){
		int val = evaluate();
		depth--;
		return val;
	}
	int bestScore = INT_MIN;
	string selectedMove;
	//printMovesAtCurrentDepth();
	for(int i=0; i<s[depth]; i++){ //LOOP THROUGH MOVES
		string selectedMove = m[depth][i]; //Get the iterated move within moves at depth.
		movePiece(selectedMove, false);

		ab[depth+1][0] = ab[depth][0];
		ab[depth+1][1] = ab[depth][1];
		int score = min();
		//cout << "Eval depth " << depth+1 << " is " << score << " for move " << selectedMove << endl;

		if(score > bestScore){ 
			//cout << "New best score " << bestScore << endl;
			bestScore=score;
		}
		if(ab[depth+1][1] > ab[depth][0]){
			ab[depth][0] = ab[depth+1][1];
		}
		if(ab[depth][0] >= ab[depth][1]){
			undoMove(selectedMove);
			break;
		}

		undoMove(selectedMove);
	}
	depth--;
	return(bestScore);
}
int moreMoves(int depth){
	if(depth%2==0){ //If current depth is Computer
		if(debugScoring)cout << "Current depth is Computer" << endl;
		if(s[depth-2] > s[depth-1]){ //If you have more moves than your opponent
			if(debugScoring)cout << "Computer has more moves than Human at: " << s[depth-1] << " to " << s[depth-2] << endl;
			return -20;
		}  else if(s[depth-2] == s[depth-1]){
			if(debugScoring)cout << "Computer have the same amount of moves as the Human at: " << s[depth-1] << " to " << s[depth-2] << endl;
			return 0;
		} else {
			if(debugScoring)cout << "Human has more moves than Computer at: " << s[depth-1] << " to " << s[depth-2] << endl;
			return 20;
		}
	} else {
		if(debugScoring)cout << "Current depth is Human" << endl;
		if(s[depth-2] > s[depth-1]){ //If you have more moves than your opponent
			if(debugScoring)cout << "Human has more moves than Computer at: " << s[depth-1] << " to " << s[depth-2] << endl;
			return 20;
		}  else if(s[depth-2] == s[depth-1]){
			if(debugScoring)cout << "Human have the same amount of moves as the Computer at: " << s[depth-1] << " to " << s[depth-2] << endl;
			return 0;
		} else {
			if(debugScoring)cout << "Computer has more moves than Human at: " << s[depth-1] << " to " << s[depth-2] << endl;
			return -20;
		}		
	}
}
bool bothKingsDead(char king){
	if(king == 'K') return ((b[0][6]!='K')&&(b[1][6]!='K')&&(b[2][6]!='K')&&(b[3][6]!='K')&&(b[4][6]!='K')&&(b[5][6]!='K')&&(b[6][6]!='K')&&(b[7][6]!='K'));
	if(king == 'k') return ((b[0][1]!='k')&&(b[1][1]!='k')&&(b[2][1]!='k')&&(b[3][1]!='k')&&(b[4][1]!='k')&&(b[5][1]!='k')&&(b[6][1]!='k')&&(b[7][1]!='k'));
	if(debugScoring)cout << "Err: Can't determine king status for winner" << endl;	
	return false;
}
bool neitherKingDead(){
	if (!bothKingsDead('K')&&!bothKingsDead('k')) {
		if(debugScoring)cout << "Neither side's kings are dead." << endl;
		return true;
	}
	return false;
}
int kingsKilled(char king){
	int kingCounter = 0;
	int row,score;
	if(king == 'K') {
		row = 6; score = 5000;
	}
	if(king == 'k') {
		row = 1; score = 5000;
	}
	for(int i=0;i<BOARD_COL_SIZE;i++){
		if(b[i][row] == king) kingCounter++;
	}

	if (kingCounter == 1){
		if(debugScoring)cout << "There are " << kingCounter << " " << king << "(s) remaining" << endl;
		return score;	
	} else {
		if(debugScoring)cout << "There are " << kingCounter << " " << king << "(s) remaining" << endl;
		return 0;
	}
}
int politicansKilled(char politican){
	int politicanCounter = 0;
	int score = 0;
	if(politican == 'H' || politican == 'B'){
		score = 500;
	}
	if(politican == 'h' || politican == 'b'){
		score = -500;
	}
	for(int i=0;i<BOARD_COL_SIZE;i++){
		for(int j=0;j<BOARD_ROW_SIZE+1;j++){
			if(b[i][j] == politican){
				politicanCounter++;
			}
		}
	}
	if(debugScoring)cout << "There are " << politicanCounter << " " << politican << "(s) left" << endl;
	return politicanCounter*score;
}
int pawnsKilled(char pawn){
	int pawnCounter = 0;
	int score = 0;
	if(pawn == 'P'){
		score = 50;
	}
	if(pawn == 'p'){
		score = -50;
	}
	for(int i=0;i<BOARD_COL_SIZE;i++){
		for(int j=0;j<BOARD_ROW_SIZE+1;j++){
			if(b[i][j] == pawn){
				pawnCounter++;
			}
		}
	}
	if(debugScoring)cout << "There are " << pawnCounter << " " << pawn << "(s) left" << endl;
	return pawnCounter*score;
}
/*INCOMPLETE*/
int evaluate(){
	int i = 0;		
	//cout << "Max Depth reached at " << depth << endl;
	//i += moreMoves(depth);
	i -= kingsKilled('K');
	i += kingsKilled('k');
	//i -= politicansKilled('H');
	//i += politicansKilled('h');
	//i -= politicansKilled('B');
	//i += politicansKilled('b');
	//i -= pawnsKilled('P');
	//i += pawnsKilled('p');

	ab[depth][0] = i;
	ab[depth][1] = i;
	//cout << "Eval depth " << depth << " is " << i << endl;
	return i; 
}  // stub
bool check4Winner(bool print){
	if(debugScoring)cout << "Checking for winner at depth " << depth << endl;
	if(s[depth] == 0){
		if(print)cout << "There are no moves to make." << endl;
		return true;
	}
  	if(bothKingsDead('K')){
		if(print)cout << "All my kings dead." << endl; //NO COMPUTER KINGS ON THEIR ROW
		return true;  // HUMAN wins
	}
  	if(bothKingsDead('k')){
		if(print)cout << "All opponent kings dead." << endl; //NO HUMAN KINGS ON THEIR ROW
      	return true;  // COMPUTER wins
	}
	if(neitherKingDead()){
		return false;
	}
  	return false; //Game is not over
}
/*INCOMPLETE*/
bool checkGameOver(bool human){
	moveGenerator(human);
	if(check4Winner(true)){
		if(human) cout << "My opponent has lost." << endl;
		else cout << "I have lost." << endl;
		return true;
  	}
  	return false;
}