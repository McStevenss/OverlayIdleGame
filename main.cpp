#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    const int width = 300;
    // const int height = 200;
    const int height = 300;
    bool dragging = false;
    int dragOffsetX = 0;
    int dragOffsetY = 0;

    SDL_Window* window = SDL_CreateWindow(
        "Overlay",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP
    );

    if (!window)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    int x = 0;
    int velocity = 2;
    int square_size = 8;

    while (running)
    {
        // while (SDL_PollEvent(&event))
        // {
        //     if (event.type == SDL_QUIT)
        //         running = false;
        // }
        const Uint8 *state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_ESCAPE]){
            running = false;
        }
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
            
            else if (event.type == SDL_KEYDOWN &&
                    event.button.button == SDLK_ESCAPE)
            {
                running = false;
            }

            else if (event.type == SDL_MOUSEBUTTONDOWN &&
                    event.button.button == SDL_BUTTON_LEFT && state[SDL_SCANCODE_LCTRL])
            {
                dragging = true;
                dragOffsetX = event.button.x;
                dragOffsetY = event.button.y;
            }
            else if (event.type == SDL_MOUSEBUTTONUP &&
                    event.button.button == SDL_BUTTON_LEFT)
            {
                dragging = false;
            }
            else if (event.type == SDL_MOUSEMOTION && dragging)
            {
                int mouseX, mouseY;
                SDL_GetGlobalMouseState(&mouseX, &mouseY);
                SDL_SetWindowPosition(
                    window,
                    mouseX - dragOffsetX,
                    mouseY - dragOffsetY
                );
            }
        }

        // Update
        x += velocity;
        if (x < 0 || x > width - square_size)
            velocity = -velocity;

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect rect{ x, height / 2, square_size, square_size};
        SDL_SetRenderDrawColor(renderer, 255, 100, 250, 255);
        SDL_RenderFillRect(renderer, &rect);
    
        SDL_Rect rect2{ x, height / 2 - square_size, square_size, square_size};
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
        SDL_RenderFillRect(renderer, &rect2);

        
        SDL_Rect rect3{ x, height / 2 - (2*square_size), square_size, square_size};
        SDL_SetRenderDrawColor(renderer, 100, 250, 100, 255);
        SDL_RenderFillRect(renderer, &rect3);

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}