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
        body_radius -= 20 * dt;
        return body_radius;
    }

    void Reset() {body_radius = max_radius;}
};

struct ConsumableBlobs
{
    Color color{GetRandomValue(200, 255), GetRandomValue(200, 255), GetRandomValue(200, 255), 255};
    float radius = GetRandomValue(10, 50);
    Vector2 position{};

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
    Player player = {Color{255, 40, 0, 255}, start_pos, 50};

    ConsumableBlobs consumables[MAX_BLOBS];

    Texture2D eat_or_die = LoadTexture("assets/eat_or_die.png");
    Texture2D game_over = LoadTexture("assets/game_over.png");

    Button play_button{"assets/play_button.png"};
    Button exit_button{"assets/exit_button.png"};
    Button main_menu_button{"assets/main_menu.png"};
    Button try_again_button{"assets/try_again.png"};

    int score = 0;
    int high_score = 0;

    int app_state = 0; // 0: Menu screen, 1: Game, 2: Game over screen

    public:
        EatOrDie(int width, int height, std::string title) : Application(width, height, title)
        {
            _bg_color = {19, 42, 46, 255};
        }

    protected:
        void Update(float dt) override
        {
            if(app_state == 0)
            {
                MenuScreen(dt);
            }

            else if(app_state == 1)
            {
                MainGameLoop(dt);
            } 
            else if(app_state == 2)
            {
                GameOverScreen(dt);
            }
        }

    private:
        void MenuScreen(float dt)
        {
            int screen_width = GetScreenWidth();
            int screen_height = GetScreenHeight();

            int pb_width = play_button.button_sprite.width;
            int pb_height = play_button.button_sprite.height / play_button.num_frames;

            int eb_width = exit_button.button_sprite.width;
            int eb_height = exit_button.button_sprite.height / exit_button.num_frames;
            
            Vector2 pb_pos = {(float)(screen_width / 2) - (float)(pb_width / 2) - 200, (float)(screen_height / 2) - (float)(pb_height / 4) + 50};
            Vector2 eb_pos = {(float)(screen_width / 2) - (float)(eb_width / 2) + 200, (float)(screen_height / 2) - (float)(eb_height / 4) + 50};

            DrawTexture(eat_or_die, (float)(screen_width / 2) - (float)(eat_or_die.width / 2), 150, WHITE);
            DrawText("© 2021 Ahmad Ayaz Amin", screen_width - 250, screen_height - 25, 18, WHITE);

            bool is_pb_pressed = play_button.RenderButton(pb_pos, GetMousePosition());
            bool is_eb_pressed = exit_button.RenderButton(eb_pos, GetMousePosition());

            if(is_pb_pressed)
            {
                InitGameObjects();
                app_state = 1;    
            }

            if(is_eb_pressed) should_exit = true;
        }

        void MainGameLoop(float dt)
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
                        score++;
                        if(score > high_score) high_score = score;
                        consumables[i] = ConsumableBlobs(MAX_MAP_WIDTH, MAX_MAP_HEIGHT);
                        if(player.body_radius < player.max_radius)
                        {
                            player.body_radius += blob.radius / 2;
                        }
                    }
                }

                camera.target = player.position;

                EndMode2D();

                DrawText(("Score: " + std::to_string(score)).c_str(), 50, 100, 18, WHITE);
                DrawText(("High Score: " + std::to_string(high_score)).c_str(), 50, 50, 18, WHITE);
            }

            else app_state = 2;
        }

        void GameOverScreen(float dt)
        {
            int screen_width = GetScreenWidth();
            int screen_height = GetScreenHeight();

            int mm_width = main_menu_button.button_sprite.width;
            int mm_height = main_menu_button.button_sprite.height / play_button.num_frames;

            int ta_width = try_again_button.button_sprite.width;
            int ta_height = try_again_button.button_sprite.height / play_button.num_frames;

            Vector2 mm_pos = {(float)(screen_width / 2) - (float)(mm_width / 2), (float)(screen_height / 2) - (float)(mm_height / 4) + 150};
            Vector2 ta_pos = {(float)(screen_width / 2) - (float)(ta_width / 2), (float)(screen_height / 2) - (float)(ta_height / 4)};

            DrawTexture(game_over, (float)(screen_width / 2) - (float)(game_over.width / 2), 150, WHITE);

            bool is_mm_pressed = main_menu_button.RenderButton(mm_pos, GetMousePosition());
            bool is_ta_pressed = try_again_button.RenderButton(ta_pos, GetMousePosition());

            if(is_mm_pressed) app_state = 0;
            if(is_ta_pressed) 
            {
                InitGameObjects();
                app_state = 1;
            }
        }

        void InitGameObjects()
        {
            score = 0;
            player.Reset();
            for(int i=0; i < MAX_BLOBS; i++)
            {
                consumables[i] = ConsumableBlobs(MAX_MAP_WIDTH, MAX_MAP_HEIGHT);
            }
        }
};

int main()
{
    EatOrDie app{800, 600, "Eat or Die!"};
    app.Run(60);
    return 0;
}
