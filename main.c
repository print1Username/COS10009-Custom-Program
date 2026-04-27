#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL 无法初始化! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow(
        "Swinburne SDL2 Test",            // 窗口标题
        SDL_WINDOWPOS_CENTERED,           // X 坐标
        SDL_WINDOWPOS_CENTERED,           // Y 坐标
        800, 600,                         // 宽，高
        SDL_WINDOW_SHOWN                  // 立即显示
    );

    if (window == NULL) {
        printf("窗口创建失败! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 关键：事件循环 (Event Loop)
    // 如果没有这段代码，程序运行完 SDL_CreateWindow 就会直接退出
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            // 用户点击了窗口右上角的关闭按钮
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    // 销毁并退出
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}