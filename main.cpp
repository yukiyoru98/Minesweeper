#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <ctime>

using namespace std;

//globals
vector< vector<int> > board;//first and last row&column is wall
//-2:mine; -1:wall; 0:blank; >0:number of mines nearby
vector< vector<int> > pressed;//0:not pressed; 1:pressed
bool gameOver = false;
int explored = 0;//num of positions explored
int w;
//function prototypes
bool isInteger();
bool legalPosition(int, int);
void showBoard();
void explore(int, int);

int main(){
    
    while(1){
        int length=0;
        int mines=0;
        //prompt for input:length, mines
        do{
            cout << "Please enter the length of the board:";
            cout << "(0 < length < 100)\n";
            cin >> length;
        }while((isInteger()==false) || (length <= 0) || (length >= 100));
        
        do{
            cout << "Please enter the num of mines:\n";
            cout << "(0 < mines < " << length*length << ")\n";
            cin >> mines;
        }while((isInteger()==false) || (mines >= length*length) || (mines <= 0));
        w = length * 3 / 2 + 8;//just for printing
        
        //create board
        for(int i=0; i<length+2; i++){
            vector<int> x(length+2);
            for(int j=0; j<length+2; j++){
                
                if((i == 0)||(i == length+1)){//for first and last row, all elements are walls
                    x[j] = -1;
                }
                else{//for other rows
                    x[j] = 0;
                    //first and last elements are walls
                    x[0] = -1;
                    x[length+1] = -1;
                }
            }
            board.push_back(x);
        }
        pressed = board;//pressed is all 0 except for wall at first
        
        //plant mines and numbers on board
        srand(unsigned(time(NULL)));
        for(int cnt=0; cnt < mines; cnt++){
            int r=0;
            int c=0;
            do{//random select position for mines between 1~length
                r = 1 + rand()%length;
                c = 1 + rand()%length;
            }while(board[r][c] == -2);//redo if it has been occupied by mine already
            
            board[r][c] = -2;//plant mine
            
            //nearby positions +1
            for(int dx=-1; dx<=1; dx++){
                for(int dy=-1; dy<=1; dy++){
                    if((dx == 0)&&(dy == 0)){//skip the mine itself
                        continue;
                    }
                    if((board[r+dy][c+dx] != -1)&&(board[r+dy][c+dx] != -2)){//not wall or mine
                        board[r+dy][c+dx]++;
                    }
                }
            }
            
        }
        
        //Game Start
        while(1){
            
            showBoard();
            
            //prompt for input:x,y
            int x=0, y=0;
            do{
                do{
                    cout << "Select a row(y):";
                    cin >> y;
                }while(isInteger()==false);
                do{
                    cout << "Select a column(x):";
                    cin >> x;
                }while(isInteger()==false);
            }while(legalPosition(x, y)==false);
            
            //explore the chosen position
            explore(x, y);
            
            //end game if game over or win
            if((gameOver == true) || (explored == length*length - mines)){
                break;
            }
        }//Game Ends
        
        if(gameOver == false){//win
            //press all in order to show the whole board
            for(int i=1; i<board.size()-1; i++){
                for(int j=1; j<board.size()-1; j++){
                    pressed[i][j] = 1;
                }
            }
            cout << "YOU WIN!\n";
        }
        showBoard();
        char ans;
        do{
            cout << "Play again?(Y/N):";
            cin >> ans;
        }while((ans!='Y') && (ans!='N'));
        if(ans == 'N'){
            cout << "Thank You For Playing.\n";
            break;
        }
        board.clear();
        pressed.clear();
        gameOver = false;
        explored = 0;
    }
    
    
    system("pause");
    return 0;
}


//functions
bool isInteger(){//avoid other characters
    
    if(cin.fail()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout << "Invalid input!\n";
        return false;
    }
    else return true;
}

bool legalPosition(int x, int y){
    
    if((!(x < board.size()-1)) || (!(x > 0)) || (!(y > 0)) || (!(y < board.size()-1))){//out of bound
        cout << "Out of bound! Please try again.\n";
        return false;
    }
    if(pressed[y][x] == 1){//pressed already
        cout << "The position has already been explored. Please try again.\n";
        return false;
    }
    return true;
}

void showBoard(){
    
    cout << setw(w) << "<MAP>\n";
    cout << "(* : mine)\n";
    cout << " (y)-";
    for(int i=1; i<board.size()-1; i++){
        cout << "---";
    }
    cout << endl;
    for(int i=1; i<board.size()-1; i++){
        cout << setw(3) << i;
        cout << "||";
        for(int j=1; j<board[i].size()-1; j++){
            if(pressed[i][j] == 1){
                if(board[i][j] == -2) cout << setw(3)<< "*|";
                else cout << setw(2) << board[i][j] << '|';
            }
            else{
                cout << setw(3) << " |";
            }
        }
        cout << "|\n";
    }
    cout << "    -";
    for(int i=1; i<board.size()-1; i++){
        cout << "---";
    }
    cout << "\n    ";
    for(int i=1; i<board.size()-1; i++){
        cout << setw(3) << i;
    }
    cout << "(x)\n";
}

void explore(int x, int y){
    //press on (x,y)
    pressed[y][x] = 1;
    explored++;
    
    //step on mine
    if(board[y][x] == -2){
        //press all mine
        for(int i=1; i<board.size()-1; i++){
            for(int j=1; j<board.size()-1; j++){
                if(board[i][j] == -2){
                    pressed[i][j] = 1;
                }
            }
        }
        cout << "GAME OVER!\n";
        gameOver = true;
    }
    
    //step on blank
    else if(board[y][x] == 0){
        for(int dx=-1; dx<=1; dx++){//explore nearby positions, start from up-left and go counterclockwise
            for(int dy=-1; dy<=1; dy++){
                if((dx == 0) && (dy == 0)){//itself
                    continue;
                }
                if((board[y+dy][x+dx] != -1) && (pressed[y+dy][x+dx] != 1)){
                    //not wall or pressed
                    explore(x+dx, y+dy);
                }
            }
        }
    }
}
