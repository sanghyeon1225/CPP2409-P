#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <thread>
#include <chrono>
#include <windows.h>
#include <queue>
#include <utility>

#include "ghost.h"

using namespace std;

void DisplayMap();
void ShowScoreboard();
void ClearScreen();
void MovePacman(char input);
void DisableCursorBlinking();
void MainMenu();
void MapSetting();
void Initialize();

// 팩맨 게임 맵의 가로와 세로 길이
const int WIDTH = 27;
const int HEIGHT = 30;

// 게임 종료 후, 점수를 저장할 score_board
vector<int> score_board;

// 게임 진행 중 점수를 나타낼 score (게임 종료시 해당 점수를 저장함)
int score;

// 팩맨과 좌표를 나타내는 변수
int pac_x;
int pac_y;

// 유령 객체 4개를 관리할 벡터
vector<Ghost> ghosts;  
vector<Ghost> respawn;
// 팩맨과 고스트의 위치를 시각적으로 표현
char PACMAN = 'P';
char GHOST = 'G';

// 팩맨의 파워업 모드 상태
bool power_up = false;

// 파워업 모드가 됐을 때 시간을 측정하기 위한 time 변수들
std::chrono::steady_clock::time_point power_time;
std::chrono::steady_clock::time_point real_time;

// 맵 구현 (# = 벽, . = 코인(점수), * = 아이템)
char map[HEIGHT][WIDTH + 1];
char original_map[HEIGHT][WIDTH + 1] = {
		"###########################" ,
		"#............#............#" ,
		"#.####.#####.#.#####.####.#" ,
		"#*####.#####.#.#####.####*#" ,
		"#.####.#####.#.#####.####.#" ,
		"#.........................#" ,
		"#.####.##.#######.##.####.#" ,
		"#.####.##.#######.##.####.#" ,
		"#......##....#....##......#" ,
		"######.##### # #####.######" ,
		"     #.##### # #####.#     " ,
		"     #.##         ##.#     " ,
		"     #.## ##   ## ##.#     " ,
		"######.## #     # ##.######" ,
		"#     .             .     #" ,
		"######.## #     # ##.######" ,
		"     #.## ##   ## ##.#     " ,
		"     #.##         ##.#     " ,
		"     #.##.#######.##.#     " ,
		"######.##....#....##.######" ,
		"#......#####.#.#####......#" ,
		"#.####.#####.#.#####.####.#" , 
		"#.####.#####.#.#####.####.#" ,
		"#*..##...............##..*#" ,
		"###.##.##.#######.##.##.###" ,
		"###.##.##....#....##.##.###" ,
		"#......#####.#.#####......#" ,
		"#.##########.#.##########.#" ,
		"#.........................#" ,
		"###########################" ,
};

// 맵을 출력하는 함수
void DisplayMap() {
    for (int i = 0; i < HEIGHT; i++) {
        cout << "                    ";
        for (int j = 0; j < WIDTH+1; j++) {
            if (i == pac_y && j == pac_x)
                cout << PACMAN;
            else {
                bool is_ghost = false;
                for (int g = 0; g < ghosts.size(); g++) {
                    if (i == ghosts[g].ghost_y && j == ghosts[g].ghost_x && ghosts[g].is_alive == true) {   
                        cout << GHOST; // 유령 출력
                        is_ghost = true;
                        break;
                    }
                }
                if (!is_ghost)
                    cout << map[i][j];
            }
        }
        cout << endl;
    }
    cout << "현재 점수 : " << score << endl;
}

// 커서 깜빡임 제거 함수
void DisableCursorBlinking() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 콘솔 핸들 가져오기
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hConsole, &cursorInfo); // 현재 커서 정보 가져오기
    cursorInfo.bVisible = FALSE;                // 커서를 보이지 않게 설정
    SetConsoleCursorInfo(hConsole, &cursorInfo); // 커서 정보 갱신
}

// 점수판 출력 함수 (MAIN 메뉴에서 2 입력 시 실행 가능, 지금까지 실행했던 게임의 점수 저장)
void ShowScoreboard() {
    cout << "========= 점수판 =========" << endl;
    if (score_board.size() <= 0) {
        cout << "No Game Data. Play Game Please" << endl;
    }
    else {
        for(int i = 0; i < score_board.size(); i++) {
            cout << i+1 << ". 점수: " << score_board[i] << endl;
        }
    }
}

// 콘솔 커서 위치를 맨 위로 이동시키는 함수
void ClearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0}; // 커서를 (0, 0)으로 이동
    SetConsoleCursorPosition(hConsole, coord);
}

void Initialize(string game_diff)
{
    pac_x = 12;
    pac_y = 14;
    power_up = false;
    
    ghosts.clear();
    if (game_diff == "easy") {    
        ghosts.push_back(Ghost(1, 1, 'A'));    // 유령 1
        ghosts.push_back(Ghost(25, 26, 'B'));  // 유령 2
    }
    else if (game_diff == "normal") {
        ghosts.push_back(Ghost(1, 1, 'A'));    // 유령 1
        ghosts.push_back(Ghost(25, 1, 'B'));   // 유령 2
        ghosts.push_back(Ghost(1, 26, 'A'));   // 유령 3
        ghosts.push_back(Ghost(25, 26, 'B'));  // 유령 4
    }
    else if (game_diff == "hard") {
        ghosts.push_back(Ghost(1, 1, 'A'));    // 유령 1
        ghosts.push_back(Ghost(12, 1, 'B'));    // 유령 2
        ghosts.push_back(Ghost(25, 1, 'A'));   // 유령 3
        ghosts.push_back(Ghost(1, 26, 'B'));   // 유령 4
        ghosts.push_back(Ghost(12, 26, 'A'));    // 유령 5
        ghosts.push_back(Ghost(25, 26, 'B'));  // 유령 6
    }
    score = 0;
    for (int i = 0; i < 40; i++)
    {
        cout << " " << endl;
    }
    ClearScreen();
}


// 팩맨 이동 함수
void MovePacman(char input) {
    int dx = pac_x, dy = pac_y;
    
    if (input == 'w') dy--; // 위
    if (input == 's') dy++; // 아래
    if (input == 'a') dx--; // 왼쪽
    if (input == 'd') dx++; // 오른쪽

    if (map[dy][dx] == ' ' || map[dy][dx] == '.' || map[dy][dx] == '*' || map[dy][dx] == 'G') {

        if (map[dy][dx] == '.') {
            score += 10;
        }

        if (map[dy][dx] == '*') {
            if (power_up == false) {
                power_time = chrono::steady_clock::now();
                power_up = true;
            } else {
                power_time = chrono::steady_clock::now();
            }
        }

        real_time = chrono::steady_clock::now();
        
        if (chrono::duration_cast<std::chrono::seconds>(real_time - power_time).count() >= 6) {
            power_up = false;
        }

        map[pac_y][pac_x] = ' ';

        pac_x = dx;
        pac_y = dy;
    }

}

void MapSetting() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH + 1; j++) {
            map[i][j] = original_map[i][j];
        }
    }
}


// 메인메뉴를 구현하는 함수
void MainMenu() {
    bool game_flag = true;
	while(game_flag){
		cout << "====== MAIN MENU ======" << endl;
        cout << "1. 게임 시작" << endl;
        cout << "2. 점수판 보기" << endl;
        cout << "3. 게임 종료" << endl;
        cout << "선택: ";
        int menu_number;
        cin >> menu_number;
        switch (menu_number) {
            case 1: {
                string game_diff;
                cout << "난이도를 선택해주세요. (easy, normal, hard)" << endl;
                cin >> game_diff;

                if (game_diff == "easy" || game_diff == "normal" || game_diff == "hard") {
                    Initialize(game_diff);
                } else {
                    game_flag = false;
                }

                MapSetting();
                DisplayMap();
                char input;
                int move_counter = 0; // 팩맨의 이동 횟수를 세는 카운터
                const int ghost_speed = 2; // 팩맨이 2번 움직일 때 고스트 1번 움직임
                bool game_running = true;
                
                while(game_running) {
                    if(_kbhit()) {
                        input = _getch();
                    }
                    MovePacman(input);
                    // 고스트의 이동 횟수를 조절하기 위함 (팩맨 2번 이동 마다 고스트 한번 이동)
                    if (move_counter % ghost_speed == 0) {
                        for (int i = 0; i < ghosts.size(); i++){
                            ghosts[i].MoveGhost(pac_x, pac_y, map); 
                        }
                        move_counter = 0;
                    }
                    move_counter++;
                    ClearScreen();
                    DisplayMap();
                    this_thread::sleep_for(chrono::milliseconds(100));

                    // 팩맨과 유령이 만났을 때 게임 종료 또는 유령 제거
                    for (int i = 0; i < ghosts.size(); i++){
                        // 유령이 죽어있는지 확인하여 죽은지 8초가 지났다면 부활시키는 기능
                        ghosts[i].RespawnGhost();

                        if (pac_x == ghosts[i].ghost_x && pac_y == ghosts[i].ghost_y && ghosts[i].is_alive == true) {
                            if (power_up) {
                                ghosts[i].KillGhost();
                            } else {
                                score_board.push_back(score); // 점수판에 현재 점수 저장
                                cout << "게임 종료!! 점수 = " << score << endl;
                                game_running = false;
                                break;
                            }
                        }
                    }

                }
                break;
            }
            case 2: {
                for (int i = 0; i < 40; i++) {
                    cout << "        " << endl;
                }
                ClearScreen();
                ShowScoreboard();
                break;
            }
            case 3: {
                game_flag = false;
                break;
            }
            default: {
                cout << "잘못된 입력입니다. (1~3 입력)" << endl;
                break;   
            }
        }
    }
}

int main() {
    DisableCursorBlinking(); // 커서 깜빡임 제거
    MainMenu();
}

