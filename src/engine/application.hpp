#pragma once

#include <iostream>
#include "raylib35/include/raylib.h"

class Application
{
    public:
        Application(int width, int height, std::string title);
        ~Application();

        void Run(int FPS);

    protected:
        virtual void OnEvent();
        virtual void Update(float delta_time);
};