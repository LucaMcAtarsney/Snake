#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;


bool gameOver;
int width = 40, height = 20;
int x, y, fruitX, fruitY;
int score;
vector<pair<int,int> > tail;
enum Direction { STOP=0, LEFT, RIGHT, UP, DOWN };
Direction dir;

void Setup(){

    initscr(); // initialises ncurses for terminal usage
    clear(); 
    noecho(); // nothing printed to terminal
    cbreak(); // doesn't wait for enter to be pressed to accept input
    curs_set(0); // hides blinking cursor in terminal
    keypad(stdscr, TRUE); //enables use of arrow keys
    nodelay(stdscr, TRUE); //getch -> nonblocking

    // set variables
    gameOver = false;
    dir = STOP;
    x = width/2;
    y = height/2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void Draw(){

    clear();

    // top border
    for (int i=0; i < width+2; i++) mvprintw(0,i,"#");

    // mid section

    for (int i=0; i < height; i++)
    {
        for (int j=0; j < width; j++)
        {
            // left border
            if (j==0) mvprintw(i+1,0,"#");

            // draw snake head
            if (i == y & j == x) mvprintw(i+1, j+1, "O"); // +1 due to offset of l + t border
            // draw fruit
            else if (i == fruitY && j == fruitX) mvprintw(i+1, j+1, "*");
            else {
                bool print = false;

                for (auto seg : tail){
                    if (seg.first == j && seg.second == i) {
                        mvprintw(i+1, j+1, "o");
                        print = true;
                    }
                }

                if (!print) mvprintw(i+1, j+1, " ");
            }

            // left border
            if (j==width-1) mvprintw(i+1,j+2,"#");

            
        }
    }
    
    // bottom border
    for (int i=0; i < width+2; i++) mvprintw(height+1,i,"#");

    // score
    mvprintw(height+3,0,"Score = %d", score);

    refresh();
}

void Input(){

    int ch = getch();

    switch(ch){
        case KEY_LEFT: dir = LEFT;break;
        case KEY_RIGHT: dir = RIGHT;break;
        case KEY_UP: dir = UP;break;
        case KEY_DOWN: dir = DOWN;break;
        case 'x': gameOver = true;break;
    }

}

void Logic(){

    pair<int, int> prev(x,y);
    pair<int, int> prev2;

    // shift each tail segment up
    if (!tail.empty()){
        for (size_t i = 0; i < tail.size(); i++)
        {
            prev2 = tail[i];
            tail[i] = prev;
            prev = prev2;
        }
    }

    // move snake based on previous input
    switch(dir)
    {
        case UP: y--;break;
        case DOWN: y++;break;
        case RIGHT: x++;break;
        case LEFT: x--;break;

    }

    // collision
    for (auto seg: tail){
        if(seg.first == x && seg.second == y){
            gameOver = true;
        }
    }

    // fruit collision
    if (x == fruitX && y == fruitY){
        score +=1;
        fruitX = rand() % width;
        fruitY = rand() % height;
        tail.push_back({x,y});
    }

    // border collision
    if (x < 0 || x > width || y < 0 || y > height){
        gameOver = true;
    }
    
}

void GameOver(){
    clear(); // clear screen
    mvprintw(height/2, (width-9)/2, "GAME OVER"); // centered horizontally
    mvprintw(height/2 + 1, (width-18)/2, "Your score: %d\n", score);
    refresh(); // show the message
    napms(5000); // pause 2 seconds so player can see it
    endwin();
    clear();

}
int main() {

    srand(time(0)); // seeds randoms with current time

    // SETUP
    Setup();

    while(!gameOver){
        //DRAW
        Draw();

        //INPUT
        Input();

        //LOGIC
        Logic();

        napms(100); //sleep 100ms, handles game speed
    }

    GameOver();

    return 0;
}

