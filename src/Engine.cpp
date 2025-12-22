#include "Engine.h"


inline float clamp(float v, float min, float max)
{
    if (v < min) return min;
    if (v > max) return max;
    return v;
}

Engine::Engine()
{
    std::cout << "Initializing Engine..." << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
    }

    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    window = SDL_CreateWindow(
        "Overlay",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_OPENGL
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

    // OpenGLContext = SDL_GL_CreateContext(window);
    // if(OpenGLContext == nullptr){
    //     std::cout << "[!][Engine] Could not create OpenGL context" << std::endl;
    //     exit(1);
    // }
    // SDL_GL_MakeCurrent(window, OpenGLContext);

    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

void Engine::Start()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    int square_size = 8;
    int num_cubes = rand() % 10;
    if (num_cubes == 0)
    {
        num_cubes = 5;
    }

    std::cout << "Creating " << num_cubes << " cubes" << std::endl; 
    
    cube cubes[num_cubes];

    for (int i = 0; i < num_cubes; i++)
    {
        cube new_cube;
        // cubes[i] = temp_cube;

        new_cube.x = rand() % (width - square_size);
        new_cube.y = rand() % (height - square_size);

        new_cube.r = (rand() % 255);
        new_cube.g = (rand() % 255);
        new_cube.b = (rand() % 255);

        if(new_cube.r < 50)
            new_cube.r = 50;
        if(new_cube.g < 50)
            new_cube.g = 50;
        if(new_cube.b < 50)
            new_cube.b = 50;

        float speed = 1.0f + (rand() % 100) / 50.0f; // ~1.0–3.0
        float angle = (rand() % 360) * 3.1415926f / 180.0f;

        new_cube.vx = std::cos(angle) * speed;
        new_cube.vy = std::sin(angle) * speed;

        cubes[i] = new_cube;
    }

    SDL_Event event;

    while (running)
    {
        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        HandleInput(event);

        // Update cubes
        for (int i = 0; i < num_cubes; i++)
        {
            cube& c = cubes[i];

            c.x += c.vx;
            c.y += c.vy;

            if (c.x <= 0 || c.x >= width - square_size)
            {
                c.vx *= -1;
                c.x = clamp(c.x, 0.0f, (float)(width - square_size));
            }

            if (c.y <= 0 || c.y >= height - square_size)
            {
                c.vy *= -1;
                c.y = clamp(c.y, 0.0f, (float)(height - square_size));
            }

            // Render updated cube
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
