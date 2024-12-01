#include <queue>
#include <utility>

class Ghost {
public:
    int ghost_x, ghost_y;  // 유령의 위치 (x, y 좌표)
    
    Ghost(int x, int y) : ghost_x(x), ghost_y(y) {}

    void MoveGhost(int pac_x, int pac_y, char map[30][28]);  // 유령 이동 함수
    
private:
    void BFS(int pac_x, int pac_y, char map[30][28]);  // BFS 알고리즘으로 최단 경로 찾기
};