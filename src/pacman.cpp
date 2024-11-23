#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <thread>
#include <chrono>
#include <windows.h>

using namespace std;

void DisplayMap();
void ShowScoreboard();
void ClearScreen();
void MovePacman(char input);
void DisableCursorBlinking();
void MainMenu();
void MapSetting();

// 팩맨 게임 맵의 가로와 세로 길이
const int WIDTH = 27;
const int HEIGHT = 30;

// 게임 종료 후, 점수를 저장할 score_board
vector<int> score_board;

// 게임 진행 중 점수를 나타낼 score (게임 종료시 해당 점수를 저장함)
int score;

// 팩맨과 고스트의 좌표를 나타내는 int 변수들 (아직 팩맨과 고스트의 움직임을 미구현하여 좌표 표현도 임의로 설정)
int pac_x = 1;
int pac_y = 1;
int ghost_x = 10;
int ghost_y = 14;

// 팩맨과 고스트의 위치를 시각적으로 표현
char PACMAN = 'P';
char GHOST = 'G';

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
            else if (i == ghost_y && j == ghost_x)
                cout << GHOST;
            else
                cout << map[i][j];
        }
        cout << "                    ";
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

        map[pac_y][pac_x] = ' ';

        pac_x = dx;
        pac_y = dy;

        map[pac_y][pac_x] = PACMAN;
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
                pac_x = 1;
                pac_y = 1;
                ghost_x = 10;
                ghost_y = 14;
                ClearScreen();
                DisplayMap();
                MapSetting();
                score = 0;
                char input;
                while(true) {
                    if(_kbhit()) {
                        input = _getch();
                    }
                    MovePacman(input);
                    ClearScreen();
                    DisplayMap();
                    this_thread::sleep_for(chrono::milliseconds(100));
                    if (pac_x == ghost_x && pac_y == ghost_y) {
                        score_board.push_back(score);
                        cout << "게임 종료!! 점수 = " << score << endl;
                        break;
                    }
                }
                break;
            }
            case 2: {
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
