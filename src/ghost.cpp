#include "ghost.h"
#include <queue>
#include <iostream>
#include <utility>

using namespace std;
// 유령이 이동하는 함수 (BFS 알고리즘 사용)
void Ghost::MoveGhost(int pac_x, int pac_y, char map[30][28]) {
    BFS(pac_x, pac_y, map); 
}

// BFS 알고리즘을 사용하여 팩맨의 위치로 최단 경로를 찾아 이동하는 함수
void Ghost::BFS(int pac_x, int pac_y, char map[30][28]) {

    // 방향 벡터 (상, 하, 좌, 우)
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    // BFS를 위한 큐와 방문을 체크하는 visited 배열
    queue<pair<int, int>> q;
    bool visited[30][28] = {false};

    // 시작 위치를 큐에 삽입하고 방문 처리
    q.push({ghost_y, ghost_x});
    visited[ghost_y][ghost_x] = true;

    // 최단 경로를 저장하는 배열    
    pair<int, int> prev[30][28]; // 배열 선언

    // 배열 초기화
    for (int i = 0; i < 30; ++i) {
        for (int j = 0; j < 28; ++j) {
            prev[i][j] = {-1, -1}; // 초기값으로 -1, -1을 설정
        }
    }

    while (!q.empty()) {
        pair<int, int> current = q.front(); // 맨 앞 요소 가져오기
        int cy = current.first;             // y 좌표 저장
        int cx = current.second;            // x 좌표 저장

        q.pop(); // 맨 앞 요소 제거

        // 팩맨의 위치에 도달하면 BFS 종료
        if (cy == pac_y && cx == pac_x) break;

        // 상, 하, 좌, 우로 이동
        for (int i = 0; i < 4; i++) {
            int ny = cy + dy[i];
            int nx = cx + dx[i];

            // 유효한 위치인지 확인
            if (ny >= 0 && ny < 30 && nx >= 0 && nx < 28 &&
                !visited[ny][nx] && map[ny][nx] != '#') {
                q.push({ny, nx});
                visited[ny][nx] = true;
                prev[ny][nx] = {cy, cx};
            }
        }
    }


    if (prev[pac_y][pac_x] == make_pair(-1, -1)) {
        for (int i = 0; i < 4; i++) {
            int ny = ghost_y + dy[i];
            int nx = ghost_x + dx[i];
            if (ny >= 0 && ny < 30 && nx >= 0 && nx < 28 && map[ny][nx] != '#') {
                ghost_y = ny;
                ghost_x = nx;
                return;
            }
        }
        return;
    }

    // 경로 역추적하여 유령이 이동할 위치 찾기
    pair<int, int> nextMove = {pac_y, pac_x};
    while (prev[nextMove.first][nextMove.second] != make_pair(ghost_y, ghost_x)) {
        nextMove = prev[nextMove.first][nextMove.second];
    }

    ghost_y = nextMove.first;  // 유령의 y 좌표 업데이트
    ghost_x = nextMove.second; // 유령의 x 좌표 업데이트
}
