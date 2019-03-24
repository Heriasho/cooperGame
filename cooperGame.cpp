#include <iostream>
#include <stdlib.h>
#include <string> //For strings
#include <map> //For hashmaps
#include <sstream>
#include <cstring>
using namespace std;

/*Cooper Game:
Sneak into a facility while a guard is hunting for you.
The board is various sizes and the spawn point of the player and the guard are random.
The computer guard tries to hunt the player while the player tries to reach the end of the level.
Both player and computer generate their possible moves and displays them. Their only valid moves are ones on the generated possible moves.
*/
const int ROW_SIZE = 5;
const int COL_SIZE = 5;
const int depth = 5;
const string LEFT = "LEFT";
const string RIGHT = "RIGHT";
const string TOP = "TOP";
const string DOWN = "DOWN";
char emptyCharacter = ' ';
char board[5][5];
char storedPiece[depth][2];
bool debug = false;

map<char,int> numbers;
map<string,string> movement;
string moveList[depth][200];
string boardString[5];
int moveSize[depth];
int playerPosition[2];
int playerNewPosition[200][2];

void setUp();
void displayBoard();
void initBoardState();
void humanMove();
void computerMove();
void hashMapThePieces();
void moveGenerator(bool isHuman);
void addToLegalMoveArray(int &move, string position);
void printMoves();

bool isInBoard(int row, int col);
bool isEmpty(char currentPiece);
bool validateHumanMove(string move);
bool escaped();
bool capturedPlayer();

int main(){
    setUp();
    for(;;){
        humanMove();
        if(escaped()){
            break;
        }
        computerMove();
        if(capturedPlayer()){
            break;
        }
    }
    cout << "The game is over." << endl;   
}
void setUp(){
    for (int row=0; row<ROW_SIZE; row++) {
        for (int col=0; col<COL_SIZE; col++) {
            board[row][col] = emptyCharacter;
        }
    }
    hashMapThePieces();
    initBoardState();
}
void initBoardState(){
    board[4][4] = '$'; //Teleport
    board[4][0] = '!'; //Guard
    board[3][2] = '+'; //Wall
    board[3][1] = '+'; //Wall
    board[2][1] = '+'; //Wall
    board[1][1] = '+'; //Wall
    board[0][4] = '&'; //Player
}
void displayBoard(){
    cout << "\nRow  +---+---+---+---+---+ " << " Computer" << endl;
    cout << "   4 | " << board[4][0] << " | " << board[4][1] << " | " << board[4][2] << " | " << board[4][3] << " | " << board[4][4] << " | "  << endl;
    cout << "     +---+---+---+---+---+ " << endl;
    cout << "   3 | " << board[3][0] << " | " << board[3][1] << " | " << board[3][2] << " | " << board[3][3] << " | " << board[3][4] << " | "  << endl;
    cout << "     +---+---+---+---+---+ " << endl;
    cout << "   2 | " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << " | " << board[2][3] << " | " << board[2][4] << " | "  << endl;
    cout << "     +---+---+---+---+---+ " << endl;
    cout << "   1 | " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << " | " << board[1][3] << " | " << board[1][4] << " | "  << endl;
    cout << "     +---+---+---+---+---+ " << endl;
    cout << "   0 | " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << " | " << board[0][3] << " | " << board[0][4] << " | "  << endl;
    cout << "     +---+---+---+---+---+ " << " Human" << endl;
    cout << "Col    " << "0   " << "1   " << "2   " << "3   " << "4   " << endl;
}
void hashMapThePieces(){
    numbers.insert(pair<const char,int>('0',0));
    numbers.insert(pair<const char,int>('1',1));
    numbers.insert(pair<const char,int>('2',2));
    numbers.insert(pair<const char,int>('3',3));
    numbers.insert(pair<const char,int>('4',4));

    movement.insert(pair<const string,string>("LEFT","left"));
    movement.insert(pair<const string,string>("TOP","top"));
    movement.insert(pair<const string,string>("RIGHT","right"));
    movement.insert(pair<const string,string>("DOWN","down"));

}
void movePiece(string selectedMove){
    int row,col;
    row = playerPosition[1];
    col = playerPosition[2];
    storedPiece[depth][1] = board[row][col]; //Stored for later UNDO
    if(selectedMove == LEFT){
        cout << "MOVE LEFT" << endl;
        board[row][col-1] = board[row][col];	
        board[row][col] = emptyCharacter;
        storedPiece[depth][2] = board[row][col-1]; //Stored for later UNDO
        cout << "MOVING PIECE: " << row << col << " to " << row << col-1 << endl; 
    }
    if(selectedMove == TOP){
        cout << "MOVE TOP" << endl;
        board[row+1][col] = board[row][col];
        board[row][col] = emptyCharacter;
        storedPiece[depth][2] = board[row+1][col]; //Stored for later UNDO
        cout << "MOVING PIECE: " << row << col << " to " << row+1 << col << endl; 
    }
    if(selectedMove == RIGHT){
        cout << "MOVE RIGHT" << endl;
        board[row][col+1] = board[row][col];	
        board[row][col] = emptyCharacter;
        storedPiece[depth][2] = board[row][col+1]; //Stored for later UNDO
        cout << "MOVING PIECE: " << row << col << " to " << row << col+1 << endl; 
    }
    if(selectedMove == DOWN){
        cout << "MOVE DOWN" << endl;
        board[row-1][col] = board[row][col];	
        board[row][col] = emptyCharacter;
        storedPiece[depth][2] = board[row-1][col]; //Stored for later UNDO
        cout << "MOVING PIECE: " << row << col << " to " << row-1 << col << endl; 
    }
	//board[newCol][newRow] = board[col][row];	
	//board[col][row] = emptyCharacter;
}
void humanMove(){
    displayBoard();
    moveGenerator(true);
    printMoves();
    string input;
    int row,col;
    row = playerPosition[1];
    col = playerPosition[2];
    if(debug) cout << "Player's current position: " << row << col << endl;
    for(;;){
        cout << "Please make a move, (Q)uit or (P)rint board: " << endl;
        cin >> input;
        int n = input.length();
		char char_array[n+1];
        strcpy(char_array,input.c_str());  
		if(char_array[0] == 'Q' || char_array[0] == 'q'){
			cout << "Quitting... " << endl;
			exit(0);
		}
		if(char_array[0] == 'P' || char_array[0] == 'p'){
			displayBoard();
			continue;
        }

        if(validateHumanMove(input)){
			movePiece(input);
			break;
		}
    }
}
void computerMove(){
    cout << "Computer move" << endl;
    moveGenerator(false);
}
void moveGenerator(bool isHuman){
    int move = 0;
    char currentPiece;
    for(int row=0;row<ROW_SIZE;row++){
        for(int col=0;col<COL_SIZE;col++){
            int LEFT_POS = board[row][col-1];
            int TOP_POS = board[row+1][col];
            int RIGHT_POS = board[row][col+1];
            int DOWN_POS = board[row-1][col];
            currentPiece = board[row][col];
            if(isEmpty(currentPiece)){
                continue;
            } else if(isHuman && currentPiece == '&'){
                playerPosition[1] = row;
                playerPosition[2] = col;
                if(debug) cout << "Player is located at " << row << col << endl;
                if(isEmpty(LEFT_POS) && isInBoard(row,col-1)){
                    playerNewPosition[move][1] = row;
                    playerNewPosition[move][2] = col-1;
                    addToLegalMoveArray(move, LEFT);
                }
                if(isEmpty(TOP_POS) && isInBoard(row+1,col)){
                    playerNewPosition[move][1] = row+1;
                    playerNewPosition[move][2] = col;
                    addToLegalMoveArray(move, TOP);
                }
                if(isEmpty(RIGHT_POS) && isInBoard(row,col+1)){
                    playerNewPosition[move][1] = row;
                    playerNewPosition[move][2] = col+1;
                    addToLegalMoveArray(move, RIGHT);
                }
                if(isEmpty(DOWN_POS) && isInBoard(row-1,col)){
                    playerNewPosition[move][1] = row-1;
                    playerNewPosition[move][2] = col;
                    addToLegalMoveArray(move, DOWN);
                }
                break;
            } else if(!isHuman && currentPiece == '!'){
                cout << "Computer is located at " << row << col << endl;
                if(isEmpty(LEFT_POS) && isInBoard(row,col-1)){
                    addToLegalMoveArray(move, LEFT);
                }
                if(isEmpty(TOP_POS) && isInBoard(row+1,col)){
                    addToLegalMoveArray(move, TOP);
                }
                if(isEmpty(RIGHT_POS) && isInBoard(row,col+1)){
                    addToLegalMoveArray(move, RIGHT);
                }
                if(isEmpty(DOWN_POS) && isInBoard(row-1,col)){
                    addToLegalMoveArray(move, DOWN);
                }
                break;
            } else {
                //cout << "" << endl;
            }
        }
    }
    moveSize[depth] = move;
}
void addToLegalMoveArray(int &move, string position){
    moveList[depth][move] = position; 
	move++;
	//cout << row << col << newRow << newCol << endl;
}
void printMoves(){
	if(depth%2==0 && debug) cout << "This is a Computer depth" << endl;
	if(depth%2==1 && debug) cout << "This is a Human depth" << endl; 
	cout << "We have " << moveSize[depth] << " valid moves" << endl;
	string moves = "";
	for(int i=0; i<moveSize[depth]; i++){
		moves += moveList[depth][i] + " ";
	}
	cout << moves << endl;
}
bool validateHumanMove(string move){
	for(int i = 0; i < moveSize[depth]; i++){
		//if(depth != 0) cout << "Something is terribly wrong." << endl;
		if(move == moveList[depth][i]){
            //cout << "Valid Move" << endl;
			return true;
		}
    }
    //cout << "Invalid Move" << endl;
	return false;
}
bool isEmpty(char currentPiece){
    return (currentPiece == emptyCharacter);
}
bool isInBoard(int row, int col){ //Lower_row = -1, Upper_row = 7, Lower_col = -1, Upper_col = 8
    //if(row > -1 && col > -1) cout << "Move " << row << col << " > -1" << endl;
    //if(row < 5 && col < 5) cout << "Move " << row << col << " < 5" << endl;
    return (row > -1 && row < 5  && col > -1 && col < 5);
}
bool samePosition(int col, int row, int newCol, int newRow){
	return (board[col][row] == board[newCol][newRow]);
}
bool escaped(){
    return false;
}
bool capturedPlayer(){
    return false;
}