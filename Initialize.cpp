//Includes:
//  - piece class: each piece object belongs has attributes -> team, type (pawn vs. king), current position
//  - initialize_board function: initializes board as 2D array of zeros
//  - standard_board function: turns initial board into conventional board
//  - piece_to_object function: iterates through each element of a board (standard or loaded) -> transforms board piece to piece object and loads team vector
//  - load_file function: load input text files and convert board to 2D array
//  - game_type function: choose between player vs. AI and AI vs. AI
//  - print_board: prints ASCII interface

#include <iostream>
#include<fstream>
#include "Initialize.h"
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

int game_prompt(int (&board)[8][8], std::vector <piece> &team1, std::vector <piece> &team2){

    char game_type='\0';
    char game_setup='\0';
    char first_turn='\0';
    initialize_board(board); //matrix of zeroes

    cout<<"Welcome to Checkers AI."<<endl;
    cout<<"Would you like to play player vs. AI (Enter 'a') or AI vs AI (Enter 'b'): ";

    while (true) {
        cin>>game_type;

        if (game_type == 'a') { //player vs AI
            cout << "Game Mode: Player vs. AI" << endl;
            break;
        }
        else if (game_type == 'b') { //AI vs AI
            cout << "Game Mode: AI vs. AI" << endl;
            break;
        }
        cout<<"Please enter 'a' or 'b': ";
    }
    cout<<endl;

    if (game_type == 'a'){
        cout<<"Would you like to go first (Enter 'a') or second (Enter 'b'): ";

        while (true){
            cin>>first_turn;

            if (first_turn == 'a') {
                cout << "Turn: Player First" << endl;
                break;
            }
            else if (first_turn == 'b') { //AI vs AI
                cout << "Turn: AI First" << endl;
                break;
            }
            cout<<"Please enter 'a' or 'b': ";

        }
    }

    cout<<endl;

    cout<<"Would you like to load a game from file? (Enter 'a' for yes or Enter 'b' for no): ";

    while (true) {
        cin>>game_setup;

        if (game_setup == 'a') { //load from file
            cout<<endl;
            cout<<"Game Setup: Load File"<<endl;
            load_file(board);
            break;
        }
        else if (game_setup == 'b') { //standard config
            cout<<endl;
            cout<<"Game Setup: Standard Board"<<endl;
            standard_board(board);
            break;
        }
        cout<<"Please enter 'a' or 'b': ";
    }

    piece_to_object(board, team1, team2); //convert digit pieces to objects and
    print_board(board);

    if (game_type == 'a'){ //player vs. AI
        if (first_turn == 'a'){ //player first
            return 1;
        }
        else{
            return 2;
        }
    }
    else
    {
        return 3;
    }
}


void initialize_board(int (&board)[8][8]){ //sets up board of zeros

    for (auto & i : board){
        for (int & j : i){
            j = 0;
        }
    }
}

void standard_board(int (&board)[8][8]){

    for (int i=0; i<3; i++){ //create team 1 pieces; top 3 rows
        if (i%2 == 0 ){//even row
            for (int j=0; j<8; j++){
                if (j%2 != 0){ //odd column
                    board[i][j] = 2; //team 2 pawn
                }
            }
        }
        else { // odd row
            for (int j=0; j<8; j++){
                if (j%2 == 0){ // even column
                    board[i][j] =2;
            }
            }
        }
    }

    for (int i=5; i<8; i++){ //create team 2 pieces; bottom 3 rows
        if (i%2 != 0 ){//odd row
            for (int j=0; j<8; j++){
                if (j%2 == 0){ //even column
                    board[i][j] = 1; //team 1 pawn
                }
            }
        }
        else{ //even row
            for (int j=0; j<8; j++){
                if (j%2 != 0){ //odd column
                    board[i][j] =1;
                }
            }
        }
    }
}

void piece_to_object(int (&board)[8][8], std::vector <piece> &team1, std::vector <piece> &team2){ //look through all possible locations

    int team1_index = 0;
    int team2_index = 0;

    for (int i=0; i<8; i++){ //all rows
        for (int j=0; j<8; j++){
            if ((i%2 == 0 && j%2 != 0) || (i%2 != 0 && j%2 == 0)){ // even & odd combinations
                if (board[i][j] == 1){ //team 1 pawn
                    team1.push_back(piece{{i,j},1,1, team1_index});
                    team1_index++;
                }
                else if (board[i][j] == 2){ //team 2 pawn
                    team2.push_back(piece{{i,j},2,2, team2_index});
                    team2_index++;
                }
                else if (board[i][j] == 3){ //team 1 king
                    team1.push_back(piece{{i,j},1,3, team1_index});
                    team1_index++;
                }
                else if (board[i][j] == 4){ //team 2 king
                    team2.push_back(piece{{i,j},2,4, team2_index});
                    team2_index++;
                }
            }
        }
    }
}

int load_file(int (&board)[8][8]){

    string input_file;
    vector <int> piece_list;

    printf("Input Game File: ");
    cin>>input_file;

    ifstream file; //declare stream object
    file.open(input_file); //associate input_file with file object

    if (file.is_open()){

        string line;
        int i=0;
        while (getline(file,line)){
            int digit;
            int j=0;

            if (i % 2 == 0){
                j=1; //needs to start odd column for even row
            }

            stringstream ss(line);
            while (ss >> digit){
                board[i][j]=digit;
                j+=2; //every other square
            }
            i++;
            }
    }
    else{ //error handling
        cout<<"The game file cannot be opened."<<endl;
        return EXIT_FAILURE;
    }
    file.close();
    return 0;
}

void print_board(int board[8][8]) {
    cout << "      0       1       2       3       4       5       6       7       " << endl;
    for (int i = 0; i < 8; i++) {
        cout << "  +-------+-------+-------+-------+-------+-------+-------+-------+" << endl;
        if (i % 2 != 0) { //odd row
            cout << "  |     " << "  |\033[100m       \033[m" << "|       " << "|\033[100m       \033[m" << "|       "<< "|\033[100m       \033[m" << "|       " << "|\033[100m       \033[m" << "|" << endl;
            cout << i << " ";
            for (int j = 0; j < 8; j++) {
                int num = board[i][j];
                if (j % 2 != 0) { //odd col - forbidden space
                    cout << "|\033[100m       \033[m";
                }
                else if (board[i][j] == 0) { //empty space
                    cout << "|       ";
                }
                else if (board[i][j] == 1 || board[i][j] == 3) {
                    cout<<"| ";
                    printf("\033[48;5;%dm%3d  \033[m", 0, num);
                    cout<<" ";
                }
                else if (board[i][j] == 2 || board[i][j] == 4) {
                    cout<<"| ";
                    printf("\033[48;5;%dm%3d  \033[m", 1, num);
                    cout<<" ";
                }
            }
            cout << "|" << endl;
            cout << "  |     " << "  |\033[100m       \033[m" << "|       " << "|\033[100m       \033[m" << "|       "<< "|\033[100m       \033[m" << "|       " << "|\033[100m       \033[m" << "|" << endl;
        }
        else { //even row
            cout << "  |\033[100m       \033[m" << "|       " << "|\033[100m       \033[m" << "|       "<< "|\033[100m       \033[m" << "|       " << "|\033[100m       \033[m" << "|       " << "|" << endl;
            cout << i << " ";
            for (int j = 0; j < 8; j++) {
                int num = board[i][j];
                if (j % 2 == 0) { //even col - forbidden space
                    cout << "|\033[100m       \033[m";
                }
                else if (board[i][j] == 0) { //empty space
                    cout << "|       ";
                }
                else if (board[i][j] == 1 || board[i][j] == 3) {
                    cout<<"| ";
                    printf("\033[48;5;%dm%3d  \033[m", 0, num);
                    cout<<" ";
                }
                else if (board[i][j] == 2 || board[i][j] == 4) {
                    cout<<"| ";
                    printf("\033[48;5;%dm%3d  \033[m", 1, num);
                    cout<<" ";
                }
            }
            cout << "|" << endl;
            cout << "  |\033[100m       \033[m" << "|       " << "|\033[100m       \033[m" << "|       "<< "|\033[100m       \033[m" << "|       " << "|\033[100m       \033[m" << "|       " << "|" << endl;
        }
    }
    cout << "  +-------+-------+-------+-------+-------+-------+-------+-------+" << endl;
}

int search_time (){
    unsigned char time='\0';

    cout<<"Please enter AI search time limit (seconds): ";

    while (true) {
        cin>>time;

        if (isdigit(time) && (int(time)-'0')>0){ //converted to ascii value -> need to subtract '0' for int value
            cout<<"Search Time Limit: "<<int(time)-'0'<<endl;
            return (int(time)-'0');
        }
        cout<<"Please enter a valid time limit: ";
    }
}

void game_is_over(std::vector <piece> &team1, std:: vector <piece> &team2, int game_type){

    cout<<endl<<"Game Over!"<<endl;

    if (team1.empty()){
        if (game_type==1){
            cout<<"AI has Won!"<<endl;
        }
        else if (game_type==2){
            cout<<"Player has Won!"<<endl;
        }
        else if (game_type==3){
            cout<<"Red AI has Won!"<<endl;
        }
    }
    else if (team2.empty()){
        if (game_type==1){
            cout<<"Player has Won!"<<endl;
        }
        else if (game_type==2){
            cout<<"AI has Won!"<<endl;
        }
        else if (game_type==3){
            cout<<"Black AI has Won!"<<endl;
        }
    }
}






