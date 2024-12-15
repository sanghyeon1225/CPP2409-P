#include <queue>
#include <utility>
#include <thread>
#include <chrono>

class Ghost {
public:
    int ghost_x, ghost_y;  // 유령의 위치 (x, y 좌표)
    int spawn_x, spawn_y;
    char moving_mode;
    bool is_alive = true;
    std::chrono::steady_clock::time_point spawn_time;
    std::chrono::steady_clock::time_point now_time;

    Ghost(int x, int y, char mode) : ghost_x(x), ghost_y(y), spawn_x(x), spawn_y(y), moving_mode(mode) {}
    char temp_mode = moving_mode;

    void MoveGhost(int pac_x, int pac_y, char map[30][28]);  // 유령 이동 함수
    void KillGhost();
    void RespawnGhost();
private:
    void BFS(int pac_x, int pac_y, char map[30][28]);  // A모드 : BFS 알고리즘으로 최단 경로 찾기
    void BMode(int pac_x, int pac_y, char map[30][28]); // B모드 : 팩맨 주변 4칸까지 BFS로 가다가, 4칸부터는 5초간 Avoid 모드
    void Avoid(char map[30][28]); // C모드 : 유령들이 자신의 스폰 장소를 향해 BFS로 이동하는 모드
};