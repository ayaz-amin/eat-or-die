#pragma once

#include <iostream>
#include "raylib35/include/raylib.h"

// For animations
struct AnimatedSprite
{
    Texture2D sprite;
    Rectangle frame_rect;
    int num_frames;
    int frame_speed;

    int frame_counter;
    int current_frame;

    AnimatedSprite(std::string path, int frames_size, int speed);
    ~AnimatedSprite();
    void RenderSprite(Vector2 transform);
};

struct Button
{
    Texture2D button_sprite;
    Rectangle source_frame;
    const int num_frames = 3;

    int state; // three states: normal, hover and clicked
    bool is_pressed;

    Button(std::string path);
    ~Button();
    
    bool RenderButton(Vector2 button_pos, Vector2 mouse_pos);
};