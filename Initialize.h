//Includes:
//  - piece class: each piece object has attributes -> current position, team, type (pawn vs. king), ID
//  - initialize_board function: initializes board of 2D array of zeros
//  - standard_board function: turns initial board into conventional board
//  - piece_to_object function: iterates through each element of a board (standard or loaded) -> transforms board piece to piece object and loads team vector
//  - load_file function: load input text files and input piece digits into board array
//  - game_prompt function: Sets user up for game and implements "initialize" functions
//  - print_board: prints ASCII board

class piece{

public:
    int currentPos[2]; //[0]=row, ,[1]=column
    int team; //1= team1, 2=team2
    int type; //1=team1 pawn, 2=team2 pawn, 3=team1 king, 4=team2 king
    int ID; //ID associated with team array index for constant lookup

    piece() = default;
};

void initialize_board(int (&board)[8][8]);
void standard_board(int (&board)[8][8]);
void piece_to_object(int (&board)[8][8], std::vector <piece> &team1, std::vector <piece> &team2);
int load_file(int (&board)[8][8]);
int game_prompt(int (&board)[8][8], std::vector <piece> &team1, std::vector <piece> &team2);
void print_board(int board[8][8]);
int search_time ();
void game_is_over(std::vector <piece> &team1, std:: vector <piece> &team2, int game_type);