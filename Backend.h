class move{

public:
    piece curPiece; //piece that move applies to
    std::vector <int> opp_IDs; //sequence of opp pieces

    move() = default;
};

std::vector <class move> get_legal_moves(int board[8][8], std::vector <piece> &team1, std::vector <piece> &team2, int team_turn);
void piece_legal_moves(int board[8][8], piece &piece, std::vector <class piece> &team1,std::vector <class piece> &team2);
void find_moves(class move move, int board[8][8], int direction, class piece piece, bool first_check, std::vector<class piece> &team1, std::vector<class piece> &team2);
int find_opp_ID(int row, int column, int current_team, std::vector<class piece> &team1, std::vector<class piece> &team2);
void check_valid_drxns (int board[8][8], class piece &piece, bool &drxn1, bool &drxn2, bool &drxn3, bool &drxn4, bool &recursion);
void copy_board(int board_copy[8][8], int board[8][8]);
void apply_move(int board[8][8], class move move, std::vector <piece> &team1, std::vector <piece> &team2);
void check_is_king(class piece &piece);
bool game_not_over(std::vector <piece> &team1, std::vector <piece> &team2);
void user_move_choice(int (&board)[8][8],std::vector <piece> &team1, std::vector <piece> &team2, int turn);
int eval(std::vector <piece> &team1, std::vector <piece> &team2,int board[8][8]);
int minimax (int board[8][8], int depth, std::vector <piece> team1, std::vector <piece> team2, int turn, int alpha, int beta, class move &best_move, bool &time_over, double time_limit);
void copy_vector(std::vector <class piece> &team_copy, std::vector <class piece> &team);
void iterative_deepening(int board[8][8], std::vector <piece> &team1, std::vector <piece> &team2, int turn, double time_limit);
void check_none_move(int board[8][8], std::vector <piece> &team1, std::vector <piece> &team2, int turn, class move &chosen_move, bool &not_one_move);
void AI_error_handling (int board[8][8], std::vector <piece> &team1, std::vector <piece> &team2, int turn, class move &best_move, bool &time_over, double time_limit);
int only_int_move();

