#include <SDL2/SDL.h>
#include <iostream>

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