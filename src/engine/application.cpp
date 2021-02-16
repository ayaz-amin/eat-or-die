#include "application.hpp"

Application::Application(int width, int height, std::string title)
{
    InitWindow(width, height, title.c_str());
}

Application::~Application()
{
    CloseWindow();
}

void Application::OnEvent() {}
void Application::Update(float delta_time) {}

void Application::Run(int FPS)
{
    SetTargetFPS(FPS);

    while(!WindowShouldClose())
    {
        float dt = GetFrameTime();
        OnEvent();
        BeginDrawing();
        ClearBackground(WHITE);
        Update(dt);
        EndDrawing();
    }

    CloseWindow();
}