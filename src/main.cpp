#include <iostream>
#include <string>

#include "engine/application.hpp"
#include "engine/graphics.hpp"

struct Player
{
    Color player_color;
    Vector2 position;
    float body_radius;
    const float max_radius;

    Player(Color _player_color, Vector2 _position, float _body_radius) : max_radius{_body_radius}
    {
        player_color = _player_color;
        position = _position;
        body_radius = _body_radius;
    }

    float Update(float dt)
    {
        if(IsKeyDown(KEY_LEFT))
        {
            position.x -= 500 * dt;
        }
        else if(IsKeyDown(KEY_RIGHT))
        {
            position.x += 500 * dt;
        }
        else if(IsKeyDown(KEY_UP))
        {
            position.y -= 500 * dt;
        }
        else if(IsKeyDown(KEY_DOWN))
        {
            position.y += 500 * dt;
        }
        DrawCircle(position.x, position.y, body_radius, player_color);
        body_radius -= 15 * dt;
        return body_radius;
    }
};

struct ConsumableBlobs
{
    Color color{GetRandomValue(200, 255), GetRandomValue(200, 255), GetRandomValue(200, 255), 255};
    float radius = GetRandomValue(10, 50);
    Vector2 position;

    ConsumableBlobs() {position.x = 100; position.y = 100;}
    ConsumableBlobs(int width, int height)
    {
        position.x = GetRandomValue(-width, width);
        position.y = GetRandomValue(-height, height);
    }

    void Update(float dt)
    {
        DrawCircle(position.x, position.y, radius, color);
    }
};

class EatOrDie : public Application
{
    const int MAX_MAP_WIDTH = 500;
    const int MAX_MAP_HEIGHT = 500;
    static const int MAX_BLOBS = 10;

    Vector2 start_pos = {(float)(GetScreenWidth() / 2), (float)(GetScreenHeight() / 2)};
    Camera2D camera{start_pos, (Vector2){0, 0}, 0.0f, 1.0f};
    Player player = {Color{0, 0, 255, 255}, start_pos, 50};

    ConsumableBlobs consumables[MAX_BLOBS];

    public:
        EatOrDie(int width, int height, std::string title) : Application(width, height, title)
        {
            InitWindow(width, height, title.c_str());
            for(int i=0; i < MAX_BLOBS; i++)
            {
                consumables[i] = ConsumableBlobs(MAX_MAP_WIDTH, MAX_MAP_HEIGHT);
            }
        }

    void Update(float dt) override
    {
        if(player.body_radius > 0)
        {
            BeginMode2D(camera);
            
            float radius = player.Update(dt);

            for(int i=0; i < MAX_BLOBS; i++)
            {
                ConsumableBlobs blob = consumables[i];
                blob.Update(dt);
                if(CheckCollisionCircles(blob.position, blob.radius, player.position, radius))
                {
                    consumables[i] = ConsumableBlobs(MAX_MAP_WIDTH, MAX_MAP_HEIGHT);
                    if(player.body_radius < player.max_radius)
                    {
                        player.body_radius += blob.radius / 2;
                    }
                }
            }

            camera.target = player.position;

            EndMode2D();
        } 
        else 
        {
            DrawText("Game Over", (int)(GetScreenWidth() / 2), (int)(GetScreenHeight() / 2), 10, BLACK);
        }
    }
};

int main()
{
    EatOrDie app{800, 600, "Sample"};
    app.Run(60);
    return 0;
}