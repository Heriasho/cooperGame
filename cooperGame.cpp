/*Ryan Bartoli
3/23/19
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
using namespace std;

/*Cooper Game:
Sneak into a facility while a guard is hunting for you.
The board is various sizes and the spawn point of the player and the guard are random.
The computer guard tries to hunt the player while the player tries to reach the end of the level.
Both player and computer generate their possible moves and displays them. Their only valid moves are ones on the generated possible moves.
*/
const int ROW_SIZE = 5;
const int COL_SIZE = 5;
const int depthLimit = 50;
int depth = 0;
int maxDepthInit = 5;
int maxDepth = maxDepthInit;
const string LEFT = "LEFT";
const string RIGHT = "RIGHT";
const string TOP = "TOP";
const string DOWN = "DOWN";
char emptyCharacter = '0';
char wallCharacter = '+';
char board[5][5];
char storedPiece[depthLimit][2][2];
bool debug = false;
bool debugTimer = false;
clock_t t;

map<char,int> numbers;
map<string,string> movement;
string moveList[depthLimit][200];
string boardString[5];
int moveSize[depthLimit];
int playerPosition[2];
int playerNewPosition[200][2];
int computerPosition[2];
int storedLocation[2];
int alphabetaPruning[depthLimit][2];

void setUp();
void displayBoard();
void initBoardState();
void humanMove();
void computerMove();
void hashMapThePieces();
void movePiece(string selectedMove, bool isHuman);
void undoMove(string selectedMove, bool isHuman);
void moveGenerator(bool isHuman);
void addToLegalMoveArray(int &move, string position);
void printMoves();
void minMax();
int min();
int max();
int evaluate();

bool isInBoard(int row, int col);
bool isEmpty(char currentPiece);
bool validateHumanMove(string move);
bool escaped();
bool capturedPlayer();
bool endOfLevel();

int main(){
    setUp();
    for(;;){
        humanMove();
        displayBoard();
        if(endOfLevel()){
            break;
        }
        computerMove();
        displayBoard();
        cout << "End of computer turn" << endl;
        if(endOfLevel()){
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
void movePiece(string selectedMove,bool isHuman){
    int row,col;
    int human = 1;
    int computer = 2;
    if(isHuman){
        row = playerPosition[1];
        col = playerPosition[2];
        storedPiece[depth][1][human] = board[row][col]; //Stored for later UNDO
    } else {
        row = computerPosition[1];
        col = computerPosition[2];
        storedPiece[depth][1][computer] = board[row][col]; //Stored for later UNDO
    }
    if(selectedMove == LEFT){
        if(debug) cout << "MOVE LEFT" << endl;
        if(isHuman) {
            storedPiece[depth][2][human] = board[row][col-1]; //Stored for later UNDO
        } else{
            storedPiece[depth][2][computer] = board[row][col-1]; //Stored for later UNDO
        }
        board[row][col-1] = board[row][col];	
        board[row][col] = emptyCharacter;
        storedLocation[1] = row;
        storedLocation[2] = col-1;
        if(debug) cout << "MOVING PIECE: " << row << col << " to " << row << col-1 << endl; 
        if(debug) cout << "PIECES MOVED: " << storedPiece[depth][1] << " and " << storedPiece[depth][2] << endl;
    }
    if(selectedMove == TOP){
        if(debug) cout << "MOVE TOP" << endl;
        if(isHuman){
            storedPiece[depth][2][human] = board[row+1][col]; //Stored for later UNDO
        } else {
            storedPiece[depth][2][computer] = board[row+1][col]; //Stored for later UNDO
        }
        board[row+1][col] = board[row][col];
        board[row][col] = emptyCharacter;
        storedLocation[1] = row+1;
        storedLocation[2] = col;
        if(debug) cout << "MOVING PIECE: " << row << col << " to " << row+1 << col << endl; 
        if(debug) cout << "PIECES MOVED: " << storedPiece[depth][1] << " and " << storedPiece[depth][2] << endl;
    }
    if(selectedMove == RIGHT){
        if(debug) cout << "MOVE RIGHT" << endl;
        if(isHuman){
            storedPiece[depth][2][human] = board[row][col+1]; //Stored for later UNDO
        } else {
            storedPiece[depth][2][computer] = board[row][col+1]; //Stored for later UNDO
        }
        board[row][col+1] = board[row][col];	
        board[row][col] = emptyCharacter;
        storedLocation[1] = row;
        storedLocation[2] = col+1;
        if(debug) cout << "MOVING PIECE: " << row << col << " to " << row << col+1 << endl; 
        if(debug) cout << "PIECES MOVED: " << storedPiece[depth][1] << " and " << storedPiece[depth][2] << endl;
    }
    if(selectedMove == DOWN){
        if(debug) cout << "MOVE DOWN" << endl;
        if(isHuman){
            storedPiece[depth][2][human] = board[row-1][col]; //Stored for later UNDO
        } else {
            storedPiece[depth][2][computer] = board[row-1][col]; //Stored for later UNDO
        }
        board[row-1][col] = board[row][col];	
        board[row][col] = emptyCharacter;
        storedLocation[1] = row-1;
        storedLocation[2] = col;
        if(debug) cout << "MOVING PIECE: " << row << col << " to " << row-1 << col << endl; 
        if(debug) cout << "PIECES MOVED: " << storedPiece[depth][1] << " and " << storedPiece[depth][2] << endl;
    }
	//board[newCol][newRow] = board[col][row];	
	//board[col][row] = emptyCharacter;
}
void undoMove(string selectedMove, bool isHuman){
    int row,col,newRow,newCol,pivit;
    char oldPiece;
    newRow = storedLocation[1];
    newCol = storedLocation[2];
    if(isHuman){
        pivit = 1;
        row = playerPosition[1];
        col = playerPosition[2];
        oldPiece = storedPiece[depth][2][pivit];
    } else {
        pivit = 2;
        row = computerPosition[1];
        col = computerPosition[2];
        oldPiece = storedPiece[depth][2][pivit];
    }
    if(debug) cout << "UNDO PIECE: " << row << col << " back to " << newRow << newCol << endl; 
    board[row][col] = storedPiece[depth][1][pivit];
    board[newRow][newCol] = storedPiece[depth][2][pivit];
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
			movePiece(input, true);
			break;
		}
    }
}
void computerMove(){
    cout << "Computer move" << endl;
    moveGenerator(false);
    printMoves();
    minMax();
}
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
		alphabetaPruning[depth][0] = INT_MIN; //Alpha lower bound
		alphabetaPruning[depth][1] = INT_MAX; //Beta upper bound
		string bestThis;
		for(int i=0; i<moveSize[depth]; i++){ //LOOP THROUGH MOVES
			string selectedMove = moveList[depth][i]; //Get the iterated move within moves at depth.
			//cout << "Testing move " << selectedMove << endl;
			movePiece(selectedMove, false);

			alphabetaPruning[depth+1][0] = alphabetaPruning[depth][0];
			alphabetaPruning[depth+1][1] = alphabetaPruning[depth][1];
			int score = min();
			//cout << "Eval depth " << depth << " is " << score << " for move " << selectedMove << endl;
			//cout << "Score for move is " << score << endl;
			if(score > thisScore){ 
				//cout << "New best score " << score << endl;
				bestThis = selectedMove;
				thisScore=score;
			}
			if(alphabetaPruning[depth+1][0] > alphabetaPruning[depth][0]){
				alphabetaPruning[depth][0] = alphabetaPruning[depth+1][0];
			}
			if(alphabetaPruning[depth][0] >= alphabetaPruning[depth][1]){
				undoMove(selectedMove, false);
				break;
			}
			undoMove(selectedMove, false);
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
	movePiece(bestMove , true);
}
int min(){
    depth++;
    moveGenerator(true);
    if(endOfLevel()){
        depth--;
        return (INT_MAX-depthLimit+depth);
    }
    if(depth == maxDepth){
        int val = evaluate();
        depth--;
        return val;
    }
    int bestScore = INT_MAX;
    string selectedMove;
    for(int i=0;i<moveSize[depth];i++){
        string selectedMove = moveList[depth][i];
        movePiece(selectedMove, true);
        alphabetaPruning[depth+1][0] = alphabetaPruning[depth][0];
		alphabetaPruning[depth+1][1] = alphabetaPruning[depth][1];
        int score = max();
        if(score < bestScore){
            bestScore = score;
        }
        if(alphabetaPruning[depth+1][0] < alphabetaPruning[depth][1]){
			alphabetaPruning[depth][1] = alphabetaPruning[depth+1][0];
		}
		if(alphabetaPruning[depth][0] >= alphabetaPruning[depth][1]){
			undoMove(selectedMove, true);
			break;
		}
        undoMove(selectedMove, true);
    }
    depth--;
    return(bestScore);
}
int max(){
    depth++;
    moveGenerator(false);
    if(endOfLevel()){
        depth--;
        return (INT_MIN+depthLimit-depth);
    }
    if(depth == maxDepth){
        int val = evaluate();
        depth--;
        return val;
    }
    int bestScore = INT_MIN;
    string selectedMove;
    for(int i=0; i<moveSize[depth];i++){
        string selectedMove = moveList[depth][i];
        movePiece(selectedMove, false);
        alphabetaPruning[depth+1][0] = alphabetaPruning[depth][0];
		alphabetaPruning[depth+1][1] = alphabetaPruning[depth][1];
        int score = min();
        if(score > bestScore){
            bestScore = score;
        }
        if(alphabetaPruning[depth+1][1] > alphabetaPruning[depth][0]){
			alphabetaPruning[depth][0] = alphabetaPruning[depth+1][1];
		}
		if(alphabetaPruning[depth][0] >= alphabetaPruning[depth][1]){
			undoMove(selectedMove, false);
			break;
		}
        undoMove(selectedMove, false);
    }
    depth--;
    return(bestScore);
}
int evaluate(){
    int i = 0;
    if(capturedPlayer){
        i += 5000;
    }
    return i;
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
                if(debug) cout << "Computer is located at " << row << col << endl;
                computerPosition[1] = row;
                computerPosition[2] = col;
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
    return (currentPiece == emptyCharacter || currentPiece == wallCharacter);
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
	for(int i=0;i<ROW_SIZE;i++){
		for(int j=0;j<COL_SIZE;j++){
			if(board[i][j] == '&'){
                if(debug) cout << "Player still alive at depth " << depth << endl;
				return false;
            }
        }
    }
    return true;
}
bool endOfLevel(){
    if(capturedPlayer()){
        if(debug) if(depth) cout << "Caught player at depth " << depth << endl;
        return true;
    }
    return false;
}