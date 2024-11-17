#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 팩맨 게임 맵의 가로와 세로 길이
const int WIDTH = 27;
const int HEIGHT = 30;

// 게임 종료 후, 점수를 저장할 score_board
vector<int> score_board;

// 게임 진행 중 점수를 나타낼 score (게임 종료시 해당 점수를 저장함)
int score = 0;

// 팩맨과 고스트의 좌표를 나타내는 int 변수들 (아직 팩맨과 고스트의 움직임을 미구현하여 좌표 표현도 임의로 설정)
int pacX = 1;
int pacY = 1;
int ghostX = 5;
int ghostY = 1;

// 팩맨과 고스트의 위치를 시각적으로 표현
char PACMAN = 'P';
char GHOST = 'G';

// 맵 구현 (# = 벽, . = 코인(점수), * = 아이템)
char map[HEIGHT][WIDTH + 1] = {
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
void DisplayMap(char map[][WIDTH+1]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH+1; j++) {
            if (i == pacY && j == pacX)
                cout << PACMAN;
            else if (i == ghostY && j == ghostX)
                cout << GHOST;
            else
                cout << map[i][j];
        }
        cout << endl;
    }
}

// 점수판 출력 함수 (MAIN 메뉴에서 2 입력 시 실행 가능, 지금까지 실행했던 게임의 점수 저장)
void showScoreboard() {
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

// 맵을 출력하기 전에 콘솔창을 깔끔하게 하기 위한 함수
void clearScreen() {
    for (int i = 0; i < 10; ++i) {
        cout << endl;
    }
}

// 메인메뉴를 구현하는 함수
void mainMenu() {
    bool game_flag = true;
	while(game_flag){
		cout << "====== MAIN MENU ======" << endl;
        cout << "1. 게임 시작" << endl;
        cout << "2. 점수판 보기" << endl;
        cout << "3. 게임 종료" << endl;
        cout << "선택: ";
        int input;
        cin >> input;
        switch (input) {
            case 1:
                clearScreen();
                DisplayMap(map);
                break;
            case 2:
                showScoreboard();
                break;
            case 3:
                game_flag = false;
                break;
            default:
                cout << "잘못된 입력입니다. (1~3 입력)" << endl;
        }
    }
}

int main() {
    mainMenu();
    // score_board.push_back(score); 게임 실행 도중 종료되면 다음 코드를 사용하여 점수 저장
}
