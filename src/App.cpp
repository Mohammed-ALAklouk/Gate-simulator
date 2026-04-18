#include "App.h"

App::App()
{
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(window_width, window_height, "GateSimulator");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    panning_context = { 0, 0 };
    
    camera = { 0 };
    camera.offset = Vector2{ window_width / 2.0f, window_height / 2.0f };
    camera.zoom = 1;
}

App::~App()
{
    rlImGuiShutdown();
    CloseWindow();
}

void App::HandleInput()
{
}

void App::Update(float deltaTime)
{
    switch (current_mouse_state) {
    case Idle:
        if (IsMouseButtonDown(MouseButton::MOUSE_BUTTON_LEFT))
        {
            current_mouse_state = Panning;
            panning_context.initial_pos = GetMousePosition();
            panning_context.initial_target = camera.target;                     
        }   
        break;
    case Panning:
        {
            if (IsMouseButtonUp(MouseButton::MOUSE_BUTTON_LEFT)) {
                current_mouse_state = Idle;
                break;
            }

            auto mouse_pos = GetMousePosition();
        
            Vector2 delta = {
                panning_context.initial_pos.x - mouse_pos.x,
                panning_context.initial_pos.y - mouse_pos.y
            };

            camera.target = Vector2{
                panning_context.initial_target.x + (delta.x / camera.zoom),
                panning_context.initial_target.y + (delta.y / camera.zoom)
            };
        }

        break;

        default:
            break;
    }

    auto zoom_change = GetMouseWheelMove();

    if (zoom_change)
    {
        int index = current_zoom_index + zoom_change;
        int max_zoom_index = sizeof(zoom_levels) / sizeof(zoom_levels[0]);
        if (index >= 0 && index < max_zoom_index) {
            current_zoom_index = index;
            camera.zoom = zoom_levels[current_zoom_index];
        }
    }
}

void App::UI() const
{
    rlImGuiBegin();
    rlImGuiEnd();
}

void App::Draw() const
{ 
    BeginDrawing();

    BeginMode2D(camera);
    ClearBackground(darkTheme.background);

    DrawGrid();

    EndMode2D();

    UI();
    EndDrawing();
}

void App::DrawGrid() const     
{
    Vector2 min = GetScreenToWorld2D({ 0, 0 }, camera);
    Vector2 max = GetScreenToWorld2D({ (float)GetScreenWidth(), (float)GetScreenHeight() }, camera);

    float startX = floor(min.x / cell_size) * cell_size;
    float startY = floor(min.y / cell_size) * cell_size;
    int major_step = 5;

    for (float x = startX; x <= max.x; x += cell_size) {
        int lineIndex = (int)round(x / cell_size);
        bool isMajor = (lineIndex % major_step == 0);

        Color color = isMajor ? darkTheme.gridMajor : darkTheme.gridMinor;

        float baseThickness = isMajor ? 1.5f : 0.8f;
        float adjustedThickness = (baseThickness / camera.zoom);
        if (adjustedThickness < 1.0f / camera.zoom) adjustedThickness = 1.0f / camera.zoom;

        DrawLineEx({ x, min.y }, { x, max.y }, adjustedThickness, color);
    }
    
    for (float y = startY; y <= max.y; y += cell_size) {
        int lineIndex = (int)round(y / cell_size);
        bool isMajor = (lineIndex % major_step == 0);

        Color color = isMajor ? darkTheme.gridMajor : darkTheme.gridMinor;

        float baseThickness = isMajor ? 1.5f : 0.8f;
        float adjustedThickness = (baseThickness / camera.zoom);
        if (adjustedThickness < 1.0f / camera.zoom) adjustedThickness = 1.0f / camera.zoom;

        DrawLineEx({ min.x, y }, { max.x, y }, adjustedThickness, color);
    }
}

void App::Run()
{
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        HandleInput();
        Update(deltaTime);
        Draw();
    }
}
