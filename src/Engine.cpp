#include "Engine.h"


struct cube {
    float x = 0;
    float y;
    int r = 100;
    int g = 100;
    int b = 100;
    float velocity = 2;
};

Engine::Engine()
{
    std::cout << "Initializing Engine..." << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
    }

    window = SDL_CreateWindow(
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
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );


    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

void Engine::Start()
{
    int square_size = 8;
    
    cube cube1; 
    cube1.g = 255;
    cube1.velocity = 1.0f;

    cube cube2; 
    cube2.r = 255;
    cube2.velocity = 1.25f;
    
    cube cube3;
    cube3.b = 255;
    cube3.velocity = 1.5f;

    cube cubes[3] = {cube1,cube2,cube3};

    SDL_Event event;

    while (running)
    {
        
        HandleInput(event);

        // Update
        for(int i = 0; i < 3; i++)
        {
            cubes[i].x += cubes[i].velocity;
            cubes[i].y = height / 2 - (i*square_size);

            if (cubes[i].x < 0 || cubes[i].x > width - square_size)
                cubes[i].velocity = cubes[i].velocity * -1;
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        for(int i = 0; i < 3; i++)
        {
            SDL_Rect rect{(int)cubes[i].x,(int)cubes[i].y, square_size, square_size};
            SDL_SetRenderDrawColor(renderer, cubes[i].r, cubes[i].g, cubes[i].b, 255);
            SDL_RenderFillRect(renderer, &rect);
        }

        
        SDL_RenderPresent(renderer);
        SDL_Delay(10); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return;

}

void Engine::HandleInput(SDL_Event event)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]){
        running = false;
    }
    else if (state[SDL_SCANCODE_O]){
        width = width + 60;
        height = height + 60;
        SDL_SetWindowSize(window, width, height);
    }
    
    else if (state[SDL_SCANCODE_P]){
        width = width - 60;
        height = height - 60;

        SDL_SetWindowSize(window, width, height);
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
}
