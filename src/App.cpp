#include "App.h"

App::App()
{
    SetTraceLogLevel(LOG_ERROR);

    SetConfigFlags(FLAG_WINDOW_HIDDEN | FLAG_WINDOW_RESIZABLE);
    InitWindow(window_width, window_height, "GateSimulator");
    MaximizeWindow();
    ClearWindowState(FLAG_WINDOW_HIDDEN);
    
    SetTargetFPS(60);
    rlImGuiSetup(true);



    panning_context = { 0, 0 };
    dragging_context = { nullptr, {0, 0}, {0, 0} };
    
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
    if (IsWindowResized())
    {
		window_height = GetScreenHeight();
		window_width = GetScreenWidth();
        camera.offset = Vector2{ window_width / 2.0f, window_height / 2.0f };
    }
}

void App::Update(float deltaTime)
{
    switch (current_mouse_state) {
    case Idle:
    {

		bool is_button_down = IsMouseButtonDown(MouseButton::MOUSE_BUTTON_LEFT);
        if (is_button_down)
        {
			auto mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);

            if (CheckCollisionPointRec(mouse_pos, draggable.rect))
            {
				current_mouse_state = Dragging; 
				dragging_context.draggable = &draggable;
				dragging_context.initial_mouse_pos = mouse_pos;
				dragging_context.initial_draggable_pos = { draggable.rect.x, draggable.rect.y };
            }
            else
            {
                current_mouse_state = Panning;
                panning_context.initial_pos = GetMousePosition();
                panning_context.initial_target = camera.target;                     
            }
        }   
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

    case Dragging:
    {
        if (IsMouseButtonUp(MouseButton::MOUSE_BUTTON_LEFT)) {
            current_mouse_state = Idle;
            dragging_context.draggable = nullptr;
            break;
        }
        auto mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);
        Vector2 delta = {
            mouse_pos.x - dragging_context.initial_mouse_pos.x,
            mouse_pos.y - dragging_context.initial_mouse_pos.y
        };

		delta.x = round(delta.x / cell_size) * cell_size;
		delta.y = round(delta.y / cell_size) * cell_size;

        dragging_context.draggable->rect.x = dragging_context.initial_draggable_pos.x + delta.x;
        dragging_context.draggable->rect.y = dragging_context.initial_draggable_pos.y + delta.y;
	}
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
	DrawRectangleRec(draggable.rect, darkTheme.logicHigh);
	auto top_left = GetScreenToWorld2D({ 0, 0 }, camera);
	DrawText(("State: " + std::string((current_mouse_state == Idle) ? "Idle" : (current_mouse_state == Panning) ? "Panning" : "Dragging")).c_str(), 
        top_left.x, top_left.y, 20, RED);
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

    for (float x = startX; x <= max.x; x += cell_size) {
        int lineIndex = (int)round(x / cell_size);
        bool isMajor = (lineIndex % major_step == 0);

        Color color = isMajor ? darkTheme.gridMajor : darkTheme.gridMinor;

        float baseThickness = isMajor ? grid_line_major_thinkness : grid_line_minor_thinkness;
        float adjustedThickness = (baseThickness / camera.zoom);
        if (adjustedThickness < 1.0f / camera.zoom) adjustedThickness = 1.0f / camera.zoom;

        DrawLineEx({ x, min.y }, { x, max.y }, adjustedThickness, color);
    }
    
    for (float y = startY; y <= max.y; y += cell_size) {
        int lineIndex = (int)round(y / cell_size);
        bool isMajor = (lineIndex % major_step == 0);

        Color color = isMajor ? darkTheme.gridMajor : darkTheme.gridMinor;

        float baseThickness = isMajor ? grid_line_major_thinkness : grid_line_minor_thinkness;
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
