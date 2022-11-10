#include <iostream>
#include <vector>
#include "Initialize.h"
#include "Backend.h"
using namespace std;

vector <piece> team1; //vector of piece objects
vector <piece> team2;

int main() {
//declare variables
    int board[8][8];
    int game_type; //1=Player vs AI; 2 = AI vs. player; 3 = AI vs AI
    int turn=1;
    double time_limit;

    game_type = game_prompt(board, team1, team2);
    time_limit = search_time()*1.0;

    if (game_type == 1){ //Player (first = team1) vs. AI
        while (game_not_over(team1, team2)){
            if (turn == 1){
                user_move_choice(board, team1, team2, turn);
                turn++;
            }
            else{
                iterative_deepening(board, team1, team2, turn, time_limit);
                turn--;
            }
        }
        game_is_over(team1, team2, game_type);
    }
    else if (game_type == 2){ //AI (first = team1) vs. Player
        while (game_not_over(team1, team2)){
            if (turn == 1){
                iterative_deepening(board, team1, team2, turn, time_limit);
                turn++;
            }
            else{
                user_move_choice(board, team1, team2, turn);
                turn--;
            }
        }
        game_is_over(team1, team2, game_type);
    }
    else{ //AI vs. AI
        while (game_not_over(team1, team2)){
            if (turn == 1){
//                cout<<"turn :"<<turn<<endl;
                iterative_deepening(board, team1, team2, 1, time_limit);
                turn++;
            }
            else {
//                cout<<"turn :"<<turn<<endl;
                iterative_deepening(board, team1, team2, 2, time_limit);
                turn--;
            }
        }
        game_is_over(team1, team2, game_type);
    }

    return 0;
}

