#include <iostream>
#include <vector>
#include "Initialize.h"
#include "Backend.h"
#include <iomanip>
#include <algorithm>
#include <cstdlib>
using namespace std;

clock_t start_time, end_time;// used for search time limit

vector <class move> moves; //each move = opp seq & current piece

vector <class move> get_legal_moves(int board[8][8], std::vector <piece> &team1, std::vector <piece> &team2, int team_turn){ //gets all legal moves for a team

    moves.clear(); //clear global moves vector

    if (team_turn == 1){
        for (auto piece: team1){ //iterate through each piece and collect moves
            piece_legal_moves(board, piece, team1, team2);
        }
    }
    else{ //team2's turn
        for (auto piece: team2){
            piece_legal_moves(board, piece, team1, team2);
        }
    }

    bool forced_move = false;

    for (auto move: moves){
        if (!(move.opp_IDs.empty())){ //if there is a forced move
            forced_move = true;
        }
    }

    if (forced_move){
        for (int i = moves.size()-1; i>=0; i--){
            if (moves[i].opp_IDs.empty()){ //open move
                moves.erase(moves.begin()+i); // erase open move
            }
        }
    }

    return moves;

}

void piece_legal_moves(int board[8][8], piece &piece, vector <class piece> &team1,vector <class piece> &team2) { //starts search in initial directions based on piece type and position

    class move move;
    int board1[8][8];
    int board2[8][8];
    int board3[8][8];
    int board4[8][8];
    copy_board(board1, board);
    copy_board(board2, board);
    copy_board(board3, board);
    copy_board(board4, board);

    if (piece.type == 3 || piece.type == 4) { //same movements allowed for all kings
        if (piece.currentPos[0] == 0) { //top row
            if (0 < piece.currentPos[1] && piece.currentPos[1] < 7) { //middle columns for top (2 max legal moves)
                find_moves(move, board1, 3, piece, true, team1, team2); //lower left
                find_moves(move, board2, 4, piece, true, team1, team2); //lower right
            }
            else if (piece.currentPos[1]==7){ //upper right corner (1 max legal move)
                find_moves(move, board1, 3, piece, true, team1, team2); //lower left
            }
        }
        else if (0<piece.currentPos[0] && piece.currentPos[0]<7){ //middle rows
            if (piece.currentPos[1]==0){ //leftmost column (2 max legal moves)
                find_moves(move, board1, 2, piece, true, team1, team2); //upper right
                find_moves(move, board2, 4, piece, true, team1, team2); //lower right
            }
            else if (piece.currentPos[1]==7){ //rightmost column (2 max legal moves)
                find_moves(move, board1, 1, piece, true, team1, team2); //upper left
                find_moves(move, board2, 3, piece, true, team1, team2); //lower left
            }
            else{ //middle columns (4 max legal moves)
                find_moves(move, board1, 1, piece, true, team1, team2); //upper left
                find_moves(move, board2, 2, piece, true, team1, team2); //upper right
                find_moves(move, board3, 3, piece, true, team1, team2); //lower left
                find_moves(move, board4, 4, piece, true, team1, team2); //lower right
            }
        }
        else{ //bottom row
            {
                if (0<piece.currentPos[1] && piece.currentPos[1]<7){ //middle columns for bottom (2 max legal moves)
                    find_moves(move, board1, 1, piece, true, team1, team2); //upper left
                    find_moves(move, board2, 2, piece, true, team1, team2); //upper right
                }
                else if (piece.currentPos[1] == 0){ //lower left corner (1 max legal move)
                    find_moves(move, board1, 2, piece, true, team1, team2); //upper right
                }
            }
        }
    }
    else if (piece.type == 1 && 0<piece.currentPos[0]){ //team 1 pawn
        if (piece.currentPos[1]==0){ //leftmost piece (1 max legal move)
            find_moves(move, board1, 2, piece, true, team1, team2); //upper right
        }
        else if (piece.currentPos[1]==7){ //rightmost piece (1 max legal move)
            find_moves(move, board1, 1, piece, true, team1, team2); //upper left
        }
        else{ //middle pieces (2 max legal moves)
            find_moves(move, board1, 1, piece, true, team1, team2); //upper left
            find_moves(move, board2, 2, piece, true, team1, team2); //upper right
        }
    }
    else if (piece.type == 2 && piece.currentPos[0]<7){ //team 2 pawn
        if (piece.currentPos[1]==0){ //leftmost piece (1 max legal move)
            find_moves(move, board1, 4, piece, true, team1, team2); //lower right
        }
        else if (piece.currentPos[1]==7){ //rightmost piece (1 max legal move)
            find_moves(move, board1, 3, piece, true, team1, team2); //lower left
        }
        else{ //middle pieces (2 max legal moves)
            find_moves(move, board1, 3, piece, true, team1, team2); //lower left
            find_moves(move, board2, 4, piece, true, team1, team2); //lower right
        }
    }
}


void find_moves(class move move, int board[8][8], int direction, class piece piece, bool first_check, vector<class piece> &team1, vector<class piece> &team2) {

    int opp_ID;
    bool drxn1 = false;
    bool drxn2 = false;
    bool drxn3 = false;
    bool drxn4 = false;
    bool recursion = false;

    if (direction == 1) { //check upper left
        if (first_check && board[piece.currentPos[0] - 1][piece.currentPos[1] - 1] == 0) { //empty space
            piece.currentPos[0]-=1;
            piece.currentPos[1]-=1;
            check_is_king(piece);
            moves.push_back({piece});
        }
        else {
            if (1 < piece.currentPos[0] && 1 < piece.currentPos[1]) { //index limitations for recursion
                if ((board[piece.currentPos[0] - 1][piece.currentPos[1] - 1] != 0 && board[piece.currentPos[0] - 1][piece.currentPos[1] - 1] % 2 != piece.type % 2) && (board[piece.currentPos[0] - 2][piece.currentPos[1] - 2] == 0)) { //checking if opponent piece, then space behind opp.
                    opp_ID = find_opp_ID(piece.currentPos[0] - 1, piece.currentPos[1] - 1, piece.team, team1,team2); //save opp piece
                    move.opp_IDs.push_back(opp_ID); //push opp ID
                    board[piece.currentPos[0]][piece.currentPos[1]]=0; //set piece position to zero
                    board[piece.currentPos[0] - 1][piece.currentPos[1] - 1] = 0; //set opp position to zero
                    piece = {piece.currentPos[0] - 2,piece.currentPos[1] - 2, piece.team, piece.type, piece.ID}; //update piece position
                    check_is_king(piece);
                    move.curPiece = piece;
                    if (piece.type == 3 || piece.type == 4) { //king
                        drxn1=true;
                        drxn2=true;
                        drxn3=true;
                        check_valid_drxns(board, piece, drxn1, drxn2, drxn3, drxn4, recursion);
                        if (recursion){
                            if (drxn1){
                                find_moves(move, board, 1, piece, false, team1, team2); //UL
                            }
                            if (drxn2){
                                find_moves(move, board, 2, piece, false, team1, team2); //UR
                            }
                            if (drxn3){
                                find_moves(move, board, 3, piece, false, team1, team2); //LL
                            }
                        }
                        else{
                            moves.push_back(move); //no more opps to take
                        }
                    }
                    else { //team1 pawn
                        drxn1=true;
                        drxn2=true;
                        check_valid_drxns(board, piece, drxn1, drxn2, drxn3, drxn4, recursion);
                        if (recursion){
                            if (drxn1){
                                find_moves(move, board, 1, piece, false, team1, team2); //UL
                            }
                            if (drxn2){
                                find_moves(move, board, 2, piece, false, team1, team2); //UR
                            }
                        }
                        else{
                            moves.push_back(move); //no more opps to take
                        }
                    }
                }
            }
        }
    }
    else if (direction==2){ //check upper right
        if (first_check && board[piece.currentPos[0] - 1][piece.currentPos[1] + 1] == 0) { //empty space
            piece.currentPos[0]-=1;
            piece.currentPos[1]+=1;
            check_is_king(piece);
            moves.push_back({piece});
        }
        else {
            if (1 < piece.currentPos[0] && piece.currentPos[1] < 6) { //index limitations for recursion
                if ((board[piece.currentPos[0] - 1][piece.currentPos[1] + 1] != 0 && board[piece.currentPos[0] - 1][piece.currentPos[1] + 1] % 2 != piece.type % 2) && (board[piece.currentPos[0] - 2][piece.currentPos[1] + 2] == 0)) { //checking if opponent piece, then space behind opp.
                    opp_ID = find_opp_ID(piece.currentPos[0] - 1, piece.currentPos[1] + 1, piece.team, team1,team2); //save opp piece
                    move.opp_IDs.push_back(opp_ID); //push opp ID
                    board[piece.currentPos[0]][piece.currentPos[1]]=0; //set piece position to zero
                    board[piece.currentPos[0] - 1][piece.currentPos[1] + 1] = 0; //set opp position to zero
                    piece = {piece.currentPos[0] - 2,piece.currentPos[1] + 2, piece.team, piece.type, piece.ID}; //update piece position
                    check_is_king(piece);
                    move.curPiece = piece;
                    if (piece.type == 3 || piece.type == 4) { //king
                        drxn1=true;
                        drxn2=true;
                        drxn4=true;
                        check_valid_drxns(board, piece, drxn1, drxn2, drxn3, drxn4, recursion);
                        if (recursion){
                            if (drxn1){
                                find_moves(move, board, 1, piece, false, team1, team2); //UL
                            }
                            if (drxn2){
                                find_moves(move, board, 2, piece, false, team1, team2); //UR
                            }
                            if (drxn4){
                                find_moves(move, board, 4, piece, false, team1, team2); //LR
                            }
                        }
                        else{
                            moves.push_back(move); //no more opps to take
                        }
                    }
                    else { //team1 pawn
                        drxn1=true;
                        drxn2=true;
                        check_valid_drxns(board, piece, drxn1, drxn2, drxn3, drxn4, recursion);
                        if (recursion){
                            if (drxn1){
                                find_moves(move, board, 1, piece, false, team1, team2); //UL
                            }
                            if (drxn2){
                                find_moves(move, board, 2, piece, false, team1, team2); //UR
                            }
                        }
                        else{
                            moves.push_back(move); //no more opps to take
                        }
                    }
                }
            }
        }
    }
    else if (direction==3){ //check lower left
        if (first_check && board[piece.currentPos[0] + 1][piece.currentPos[1] - 1] == 0) { //empty space
            piece.currentPos[0]+=1;
            piece.currentPos[1]-=1;
            check_is_king(piece);
            moves.push_back({piece});
        }
        else {
            if (piece.currentPos[0] < 6 && 1 < piece.currentPos[1]) { //index limitations for recursion
                if ((board[piece.currentPos[0] + 1][piece.currentPos[1] - 1] != 0 && board[piece.currentPos[0] + 1][piece.currentPos[1] - 1] % 2 != piece.type % 2) && (board[piece.currentPos[0] + 2][piece.currentPos[1] - 2] == 0)) { //checking if opponent piece, then space behind opp.
                    opp_ID = find_opp_ID(piece.currentPos[0] + 1, piece.currentPos[1] - 1, piece.team, team1,team2); //save opp piece
                    move.opp_IDs.push_back(opp_ID); //push opp ID
                    board[piece.currentPos[0]][piece.currentPos[1]]=0; //set piece position to zero
                    board[piece.currentPos[0] + 1][piece.currentPos[1] - 1] = 0; //set opp position to zero
                    piece = {piece.currentPos[0] + 2,piece.currentPos[1] - 2, piece.team, piece.type, piece.ID}; //update piece position
                    check_is_king(piece);
                    move.curPiece = piece;
                    if (piece.type == 3 || piece.type == 4) { //king
                        drxn1=true;
                        drxn3=true;
                        drxn4=true;
                        check_valid_drxns(board, piece, drxn1, drxn2, drxn3, drxn4, recursion);
                        if (recursion){
                            if (drxn1){
                                find_moves(move, board, 1, piece, false, team1, team2); //UL
                            }
                            if (drxn3){
                                find_moves(move, board, 3, piece, false, team1, team2); //LL
                            }
                            if (drxn4){
                                find_moves(move, board, 4, piece, false, team1, team2); //LR
                            }
                        }
                        else{
                            moves.push_back(move); //no more opps to take
                        }
                    }
                    else { //team2 pawn
                        drxn3=true;
                        drxn4=true;
                        check_valid_drxns(board, piece, drxn1, drxn2, drxn3, drxn4, recursion);
                        if (recursion){
                            if (drxn3){
                                find_moves(move, board, 3, piece, false, team1, team2); //UL
                            }
                            if (drxn4){
                                find_moves(move, board, 4, piece, false, team1, team2); //UR
                            }
                        }
                        else{
                            moves.push_back(move); //no more opps to take
                        }
                    }
                }
            }
        }
    }
    else if (direction==4){ //check lower right
        if (first_check && board[piece.currentPos[0] + 1][piece.currentPos[1] + 1] == 0) { //empty space
            piece.currentPos[0]+=1;
            piece.currentPos[1]+=1;
            check_is_king(piece);
            moves.push_back({piece});
        }
        else {
            if (piece.currentPos[0] < 6 && piece.currentPos[1] < 6) { //index limitations for recursion
                if ((board[piece.currentPos[0] + 1][piece.currentPos[1] + 1] != 0 && board[piece.currentPos[0] + 1][piece.currentPos[1] + 1] % 2 != piece.type % 2) && (board[piece.currentPos[0] + 2][piece.currentPos[1] + 2] == 0)) { //checking if opponent piece, then space behind opp.
                    opp_ID = find_opp_ID(piece.currentPos[0] + 1, piece.currentPos[1] + 1, piece.team, team1,team2); //save opp piece
                    move.opp_IDs.push_back(opp_ID); //push opp ID
                    board[piece.currentPos[0]][piece.currentPos[1]]=0; //set piece position to zero
                    board[piece.currentPos[0] + 1][piece.currentPos[1] + 1] = 0; //set opp position to zero
                    piece = {piece.currentPos[0] + 2,piece.currentPos[1] + 2, piece.team, piece.type, piece.ID}; //update piece position
                    check_is_king(piece);
                    move.curPiece = piece;
                        if (piece.type == 3 || piece.type == 4) { //king
                            drxn2=true;
                            drxn3=true;
                            drxn4=true;
                            check_valid_drxns(board, piece, drxn1, drxn2, drxn3, drxn4, recursion);
                            if (recursion){
                                if (drxn2){
                                    find_moves(move, board, 2, piece, false, team1, team2); //UL
                                }
                                if (drxn3){
                                    find_moves(move, board, 3, piece, false, team1, team2); //LL
                                }
                                if (drxn4){
                                    find_moves(move, board, 4, piece, false, team1, team2); //LR
                                }
                            }
                            else{
                                moves.push_back(move); //no more opps to take
                            }
                        }
                        else { //team2 pawn
                            drxn3=true;
                            drxn4=true;
                            check_valid_drxns(board, piece, drxn1, drxn2, drxn3, drxn4, recursion);
                            if (recursion){
                                if (drxn3){
                                    find_moves(move, board, 3, piece, false, team1, team2); //LL
                                }
                                if (drxn4){
                                    find_moves(move, board, 4, piece, false, team1, team2); //LR
                                }
                            }
                            else{
                                moves.push_back(move); //no more opps to take
                            }
                        }
                    }
                }
            }
        }
}


int find_opp_ID(int row, int column, int current_team, vector<class piece> &team1, vector<class piece> &team2) {
    if (current_team == 1) {
        for (auto piece: team2) {
            if (piece.currentPos[0] == row && piece.currentPos[1] == column) {
                return piece.ID;
            }
        }
    } else {
        for (auto piece: team1) {
            if (piece.currentPos[0] == row && piece.currentPos[1] == column) {
                return piece.ID;
            }
        }
    }
}

void check_valid_drxns (int board[8][8], class piece &piece, bool &drxn1, bool &drxn2, bool &drxn3, bool &drxn4, bool &recursion) {

    if (drxn1) { //valid drxn to look in - only stays true if valid move
        if (1 < piece.currentPos[0] && 1 < piece.currentPos[1]) { //index limitations for recursion
            if ((board[piece.currentPos[0] - 1][piece.currentPos[1] - 1] != 0 &&
                 board[piece.currentPos[0] - 1][piece.currentPos[1] - 1] % 2 != piece.type % 2) &&
                (board[piece.currentPos[0] - 2][piece.currentPos[1] - 2] == 0)) {
                drxn1 = true;
            }
            else{
                drxn1 = false;
            }
        }
        else{
            drxn1 = false;
        }
    }

    if (drxn2) {
        if (1 < piece.currentPos[0] && piece.currentPos[1] < 6) { //index limitations for recursion
            if ((board[piece.currentPos[0] - 1][piece.currentPos[1] + 1] != 0 &&
                 board[piece.currentPos[0] - 1][piece.currentPos[1] + 1] % 2 != piece.type % 2) &&
                (board[piece.currentPos[0] - 2][piece.currentPos[1] + 2] ==
                 0)) { //checking if opponent piece, then space behind opp.
                drxn2 = true;
            }
            else{
                drxn2 = false;
            }
        }
        else{
            drxn2 = false;
        }
    }

    if (drxn3){
        if (piece.currentPos[0] < 6 && 1 < piece.currentPos[1]) { //index limitations for recursion
            if ((board[piece.currentPos[0] + 1][piece.currentPos[1] - 1] != 0 &&
                 board[piece.currentPos[0] + 1][piece.currentPos[1] - 1] % 2 != piece.type % 2) &&
                (board[piece.currentPos[0] + 2][piece.currentPos[1] - 2] == 0)) { //checking if opponent piece, then space behind opp.
                drxn3 = true;
            }
            else{
                drxn3 = false;
            }
        }
        else{
            drxn3 = false;
        }
    }

    if (drxn4){
        if (piece.currentPos[0] < 6 && piece.currentPos[1] < 6) { //index limitations for recursion
            if ((board[piece.currentPos[0] + 1][piece.currentPos[1] + 1] != 0 &&
                 board[piece.currentPos[0] + 1][piece.currentPos[1] + 1] % 2 != piece.type % 2) &&
                (board[piece.currentPos[0] + 2][piece.currentPos[1] + 2] == 0)) { //checking if opponent piece, then space behind opp.
            drxn4=true;
            }
            else{
                drxn4 = false;
            }
        }
        else{
            drxn4 = false;
        }
    }

    if (drxn1 || drxn2 || drxn3 || drxn4){ //recursion necc.
        recursion = true;
    }
}

void copy_board(int board_copy[8][8], int board[8][8]){
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            board_copy[i][j] = board[i][j];
        }
    }
}

void apply_move(int board[8][8], class move move, std::vector <piece> &team1, std::vector <piece> &team2){
    if (move.curPiece.team == 1){ //team1 move
        int piece_index = move.curPiece.ID;
        board[team1[piece_index].currentPos[0]][team1[piece_index].currentPos[1]] = 0; //make prev piece position = 0
        team1[piece_index] = move.curPiece;//update piece info
        board[team1[piece_index].currentPos[0]][team1[piece_index].currentPos[1]] = team1[piece_index].type; //update with new pos

        if (!move.opp_IDs.empty()){
            sort(move.opp_IDs.begin(), move.opp_IDs.end()); //sort so that deleting pieces doesn't affect indexing
            for (int i=move.opp_IDs.size()-1; i>=0; i--){ //delete largest to smallest
                int opp_index = move.opp_IDs[i];
                board[team2[opp_index].currentPos[0]][team2[opp_index].currentPos[1]] = 0;//make opp position = 0
                team2.erase(team2.begin()+opp_index); // erase opp piece from team vector
            }
            for (int i=0; i<team2.size();i++){
                team2[i].ID=i; //re-ID opp team
            }
        }
    }
    else{ //team2 move
        int piece_index = move.curPiece.ID;
        board[team2[piece_index].currentPos[0]][team2[piece_index].currentPos[1]] = 0; //make prev piece position = 0
        team2[piece_index] = move.curPiece;//update piece info
        board[team2[piece_index].currentPos[0]][team2[piece_index].currentPos[1]] = team2[piece_index].type;

        if (!move.opp_IDs.empty()) {
            sort(move.opp_IDs.begin(), move.opp_IDs.end()); //sort so that deleting pieces doesn't affect indexing
            for (int i = move.opp_IDs.size() - 1; i >= 0; i--) { //delete largest to smallest
                int opp_index = move.opp_IDs[i];
                board[team1[opp_index].currentPos[0]][team1[opp_index].currentPos[1]] = 0;//make opp position = 0
                team1.erase(team1.begin() + opp_index); // erase opp piece from team vector
            }
            for (int i=0; i<team1.size();i++){
                team1[i].ID=i; //re-ID opp team
            }
        }
    }
}

void check_is_king(class piece &piece){
    if (piece.type==1 && piece.currentPos[0]==0){ // can become king
        piece.type = 3; //update type
    }
    else if (piece.type==2 && piece.currentPos[0]==7){
        piece.type = 4;
    }
}

void user_move_choice(int (&board)[8][8],vector <piece> &team1, vector <piece> &team2, int turn){
    vector <class move> move_list;
    move_list = get_legal_moves(board, team1, team2, turn);
    int count=0;
   int move_choice;

    if (move_list.empty() && turn ==1){ //checking if ran out of moves
        cout<<"Team 1 has ran out of moves!"<<endl;
        cout<<"Team 2 has won!"<<endl;
        exit(0);
    }
    else if (move_list.empty() && turn ==2){
        cout<<"Team 2 has ran out of moves!"<<endl;
        cout<<"Team 1 has won!"<<endl;
        exit(0);
    }

    if (turn ==1){
        for (auto move: move_list){
            cout<<"Move "<<count<<": ";
            count++;
            if (!move.opp_IDs.empty()){
                cout<<"Piece: "<<"{"<<team1[move.curPiece.ID].currentPos[0]<<","<<team1[move.curPiece.ID].currentPos[1]<<"}"<<"->";
                for (int ID: move.opp_IDs){
                    cout<<"Opponent: "<<"{"<<team2[ID].currentPos[0]<<","<<team2[ID].currentPos[1]<<"}"<<"->";
                }
                cout<<"Future Loc: " <<"{"<< move.curPiece.currentPos[0] <<","<< move.curPiece.currentPos[1]<<"}"<<endl;
            }
            else{
                cout<<"Piece: "<<"{"<<team1[move.curPiece.ID].currentPos[0]<<","<<team1[move.curPiece.ID].currentPos[1]<<"}"<<"->"<<"Future Loc: "<<"{"<<move.curPiece.currentPos[0]<<","<<move.curPiece.currentPos[1]<<"}"<<endl;
            }
        }
    }
    else{//team 2 turn
        for (auto move: move_list){
            cout<<"Move "<<count<<": ";
            count++;
            if (!move.opp_IDs.empty()){
                cout<<"Piece: "<<"{"<<team2[move.curPiece.ID].currentPos[0]<<","<<team2[move.curPiece.ID].currentPos[1]<<"}"<<"->";
                for (int ID: move.opp_IDs){
                    cout<<"Opponent: "<<"{"<<team1[ID].currentPos[0]<<","<<team1[ID].currentPos[1]<<"}"<<"->";
                }
                cout<<"Future Loc: " <<"{"<< move.curPiece.currentPos[0] <<","<< move.curPiece.currentPos[1]<<"}"<<endl;
            }
            else{
                cout<<"Piece: "<<"{"<<team2[move.curPiece.ID].currentPos[0]<<","<<team2[move.curPiece.ID].currentPos[1]<<"}"<<"->"<<"Future Loc: "<<"{"<<move.curPiece.currentPos[0]<<","<<move.curPiece.currentPos[1]<<"}"<<endl;
            }
        }
    }

    cout<<"Please choose a listed move (Enter Move #): ";

    while (true){
        cin>>move_choice;

        if (move_choice>=0 && move_choice<count){
            break;
        }
        cout<<"Please enter a valid move number: ";
    }

    apply_move(board, move_list[move_choice],team1,team2);
    print_board(board);
}

int minimax (int board[8][8], int depth, vector <piece> team1, vector <piece> team2, int turn, int alpha, int beta, class move &best_move, bool &time_over, double time_limit){
    clock_t start, end;
    int value;
    int val;
    int random;
    vector <class move> move_list;
    vector <class piece> copy_team1;
    vector <class piece> copy_team2;
    move_list = get_legal_moves(board, team1, team2, turn);
    int board_copy[8][8];
    end_time = clock();

    if (!game_not_over(team1, team2) || depth==0 || double(end_time-start_time)/double(CLOCKS_PER_SEC) >= 0.99 * time_limit){
        if (team1.empty()){ //no black pieces
            return -1000000;
        }
        else if (team2.empty()){
            return 1000000;
        }
        else if (depth==0){
            return eval(team1, team2, board);
        }
        else{ //time ran out
            time_over = true;
            return eval(team1, team2, board);
        }
    }

    if (turn == 1){ //team 1/max turn
        value = -10000000;
        for (auto move: move_list){
            copy_board(board_copy, board);
            copy_vector(copy_team1, team1);
            copy_vector(copy_team2, team2);
            apply_move(board_copy, move, copy_team1, copy_team2);
            val = minimax(board_copy, depth-1, copy_team1, copy_team2, 2, alpha, beta, best_move, time_over, time_limit);
            if (val>value){
                value = val;
                best_move = move;
            }
            else if (val==value){ //if equal evaluation, choose randomly
                random = rand()%2;

                if (random==0){
                    best_move = move;
                }
            }

            alpha = max(alpha, value);
            if (alpha>=beta){
                break;
            }
        }
        return value;
    }
    else if (turn==2){ // team 2/min turn
        value = 10000000;
        for (auto move: move_list){
            copy_board(board_copy, board);
            copy_vector(copy_team1, team1);
            copy_vector(copy_team2, team2);
            apply_move(board_copy, move, copy_team1, copy_team2);
            val = minimax(board_copy, depth-1, copy_team1, copy_team2, 1, alpha, beta, best_move, time_over, time_limit);
            if (val<value){
                value = val;
                best_move = move;
            }
            else if (val==value){ //if equal evaluation, choose randomly
                random = rand()%2;

                if (random==0){
                    best_move = move;
                }
            }

            beta = min(beta, value);
            if (alpha>=beta){
                break;
            }
        }
        return value;
    }
}

bool game_not_over(vector <piece> &team1, vector <piece> &team2){
    if (team1.empty() || team2.empty()){ //either has no more pieces
        return false;
    }
    else{
        return true;
    }
}

int eval(vector <piece> &team1, vector <piece> &team2, int board[8][8]){
    int team1_pts=0;
    int team2_pts=0;

    for (auto piece: team1){
        if (piece.type == 3){
            team1_pts += 15;
        }
        else{//pawn
            team1_pts += 9;

            if (piece.currentPos[0]==7){ // want pawns to stay and protect home end
                team1_pts += 2;
            }
        }

        if ((0<piece.currentPos[0]) && (0<piece.currentPos[1] && piece.currentPos[1]<7)){ //want pieces backing each other up
            if ((board[piece.currentPos[0]+1][piece.currentPos[1]-1]==1 || board[piece.currentPos[0]+1][piece.currentPos[1]-1]==3) && (board[piece.currentPos[0]+1][piece.currentPos[1]+1]==1 || board[piece.currentPos[0]+1][piece.currentPos[1]+1]==3)){
                team1_pts += 3; //both back support
            }
            else if ((board[piece.currentPos[0]+1][piece.currentPos[1]-1]==1 || board[piece.currentPos[0]+1][piece.currentPos[1]-1]==3) || (board[piece.currentPos[0]+1][piece.currentPos[1]+1]==1 || board[piece.currentPos[0]+1][piece.currentPos[1]+1]==3)){
                team1_pts += 2; //one side support
            }
        }
    }

    for (auto piece: team2){
        if (piece.type == 4){
            team2_pts += 15;
        }
        else{//pawn
            team2_pts += 9;

            if (piece.currentPos[0]==0){ // want pawns to stay and protect home end
                team2_pts += 2;
            }
        }

        if ((piece.currentPos[0]<7) && (0<piece.currentPos[1] && piece.currentPos[1]<7)){
            if ((board[piece.currentPos[0]-1][piece.currentPos[1]-1]==2 || board[piece.currentPos[0]-1][piece.currentPos[1]-1]==4) && (board[piece.currentPos[0]-1][piece.currentPos[1]+1]==2 || board[piece.currentPos[0]-1][piece.currentPos[1]+1]==4)){
                team2_pts += 3;
            }
            else if ((board[piece.currentPos[0]-1][piece.currentPos[1]-1]==2 || board[piece.currentPos[0]-1][piece.currentPos[1]-1]==4) || (board[piece.currentPos[0]-1][piece.currentPos[1]+1]==2 || board[piece.currentPos[0]-1][piece.currentPos[1]+1]==4)){
                team2_pts += 2;
            }
        }
    }

    return (team1_pts-team2_pts); //black = max; white = min
}

void copy_vector(vector <class piece> &team_copy, vector <class piece> &team){
    team_copy.clear();
    for (auto piece: team){
        team_copy.push_back(piece);

    }
}

void iterative_deepening(int board[8][8], vector <piece> &team1, vector <piece> &team2, int turn, double time_limit){

    class move best_move;
    class move chosen_move;
    int chosen_eval;
    int eval;
    int depth;
    bool time_over = false;
    bool not_one_move = true;

    start_time = clock();
    check_none_move(board, team1, team2, turn, chosen_move, not_one_move);


//    vector <class move> move_list;
//    move_list = get_legal_moves(board, team1, team2, turn);
//
//    for (auto move: move_list){
//        cout<<move.curPiece.team<<endl;
//    }

    if (not_one_move) {
        for (int i = 1; i <= 100; i++) {
            depth = i;
            eval = minimax(board, depth, team1, team2, turn, -1000000, 1000000, best_move, time_over, time_limit);

            if (time_over) {
                break;
            }
            else { //save best move and its evaluation from prev. completed depth
                chosen_move = best_move;
                chosen_eval = eval;
            }
        }
        AI_error_handling(board, team1, team2, turn, chosen_move, time_over, time_limit+0.5);
        cout<<"Completed Search Depth: " << depth-1 <<std::endl;
        cout<<"Reached Search Depth: " << depth <<std::endl;
        cout<< "Evaluation score: "<<chosen_eval<<endl;
    }
    else{
        cout<<"Only one move available. No minimax search occurred."<<endl;
    }

    end_time = clock();

    cout<<"Search Time: "<< std::fixed<<double(end_time-start_time)/double(CLOCKS_PER_SEC)<<std::setprecision(5)<<"seconds"<<std::endl;
//    cout<<"Move info: "<<"Piece team -> "<<chosen_move.curPiece.team<<" future position->"<<chosen_move.curPiece.currentPos[0]<<chosen_move.curPiece.currentPos[1]<<endl;
//    for (auto opp: chosen_move.opp_IDs){
//        cout<<opp<<endl;
//    }
    apply_move(board, chosen_move, team1, team2);
    print_board(board);
}

void check_none_move(int board[8][8], std::vector <piece> &team1, std::vector <piece> &team2, int turn, class move &chosen_move, bool &not_one_move) { //checks is there is no move or one move left
    vector <class move> move_list;
    move_list = get_legal_moves(board, team1, team2, turn);

    if (turn == 1){
        if (move_list.size()==1){ //only one move
            chosen_move = move_list[0];
            not_one_move = false;
        }
        else if (move_list.empty()){ // no moves
            cout<<"Team 1 has ran out of moves!"<<endl;
            cout<<"Team 2 has won!"<<endl;
            exit(0);
        }
    }
    else{
        if (move_list.size()==1){ //only one move
            chosen_move = move_list[0];
            not_one_move = false;
        }
        else if (move_list.empty()){ // no moves
            cout<<"Team 2 has ran out of moves!"<<endl;
            cout<<"Team 1 has won!"<<endl;
            exit(0);
        }
    }
}

void AI_error_handling (int board[8][8], vector <piece> &team1, vector <piece> &team2, int turn, class move &best_move, bool &time_over, double time_limit){ //in case turn and piece (team) doesn't match up
    if (turn != best_move.curPiece.team){

        minimax(board, 1, team1, team2, turn, -1000000, 1000000, best_move, time_over, time_limit);
    }
}





