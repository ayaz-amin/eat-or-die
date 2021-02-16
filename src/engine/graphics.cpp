#include "graphics.hpp"

// AnimatedSprite
AnimatedSprite::AnimatedSprite(std::string path, int frames_size, int speed=4)
{
    sprite = LoadTexture(path.c_str());
    num_frames = frames_size;
    frame_rect = {0.0f, 0.0f, (float)sprite.width / num_frames, (float)sprite.height};
    frame_speed = speed;
}

AnimatedSprite::~AnimatedSprite()
{
    UnloadTexture(sprite);
}

void AnimatedSprite::RenderSprite(Vector2 transform)
{
    frame_counter++;
    if(frame_counter >= (60 / num_frames))
    {
        current_frame++;
        frame_counter = 0;
        if(current_frame > (num_frames - 1)) {current_frame=0;}
        frame_rect.x = (float)current_frame * (float)sprite.width / num_frames;
    }
    DrawTextureRec(sprite, frame_rect, transform, WHITE);
}

// Button
Button::Button(std::string path, int button_size)
{
    button_sprite = LoadTexture(path.c_str());
    num_frames = button_size;
    source_frame = {0, 0, (float)button_sprite.width, (float)(button_sprite.height / num_frames)};
}

Button::~Button()
{
    UnloadTexture(button_sprite);
}

bool Button::RenderButton(Vector2 button_pos, Vector2 mouse_point)
{
    is_pressed = false;
    float frame_height = (float)(button_sprite.height / num_frames);
    Rectangle btn_bounds = {button_pos.x, button_pos.y, (float)button_sprite.width, frame_height};

    if(CheckCollisionPointRec(mouse_point, btn_bounds))
    {
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            state = 2;
        } else {
            state = 1;
        }
        
        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            is_pressed = true;
        }
    } else {
        state = 0;
    }

    source_frame.y = state * frame_height;

    DrawTextureRec(button_sprite, source_frame, button_pos, WHITE);
    return is_pressed;
}