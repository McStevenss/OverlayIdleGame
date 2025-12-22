#include <SDL2/SDL.h>
#include <iostream>
#include "Engine.h"

//Compilation (Linux): c++ main.cpp -o overlay -lSDL2main -lSDL2

// c++ src/*.cpp -I lib/include -lSDL2 -ldl -o overlay
int main(int argc, char* argv[])
{   
    //######### Entry Point #########
    Engine engine;
    engine.Start();
    //###############################
}