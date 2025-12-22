#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

struct cube {
    float x = 0.0f;
    float y = 0.0f;

    float vx = 0.0f;
    float vy = 0.0f;

    int r = 100;
    int g = 100;
    int b = 100;
};

class Engine{
    public:
        Engine();
        void Start();
        void HandleInput(SDL_Event event);

    private:
        int width = 300;
        int height = 300;
        bool dragging = false;
        int dragOffsetX = 0;
        int dragOffsetY = 0;
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_GLContext OpenGLContext;
        bool running = true;
};