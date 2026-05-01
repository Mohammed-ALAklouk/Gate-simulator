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

    if (IsMouseButtonDown(MouseButton::MOUSE_BUTTON_RIGHT) && !gate_placed)
    {
        auto mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);
		circuit.addComponent(selected_component_type, mouse_pos);
        gate_placed = true;
    }

    if (IsMouseButtonUp(MouseButton::MOUSE_BUTTON_RIGHT))
        gate_placed = false;

    if(IsKeyPressed(KEY_SPACE))
		circuit.evaluate();
}

void App::Update(float deltaTime)
{
	auto mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);
    switch (current_mouse_state) {
    case Idle:
    {
		bool is_button_down = IsMouseButtonDown(MouseButton::MOUSE_BUTTON_LEFT);
        if (is_button_down)
        {
			bool clicked_on_input_pin = false;
			bool clicked_on_component = false;

            for (auto& Component : circuit.m_components) {
                if (Component.outputPinContainsPoint(mouse_pos))
                {
					connecting_context.sourceComponentIndex = &Component - &circuit.m_components[0];
                    connecting_context.targetPin = {0, 0};
					current_mouse_state = Connecting;
					clicked_on_input_pin = true;
					break;
                }
            
            }
            
			if (clicked_on_input_pin) break;

            for (auto& component : circuit.m_components) {
				if (component.containsPoint(mouse_pos)) {
                    current_mouse_state = Dragging;
                    dragging_context.draggable = &component.m_draggable;
                    dragging_context.initial_mouse_pos = mouse_pos;
                    dragging_context.initial_draggable_pos = component.m_draggable.getPosition();
					clicked_on_component = true;
                    break;
                }
            }

            if (clicked_on_component) break;
            
            current_mouse_state = Panning;
            panning_context.initial_pos = GetMousePosition();
            panning_context.initial_target = camera.target;                     
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

        dragging_context.draggable->setPosition({
            dragging_context.initial_draggable_pos.x + delta.x,
            dragging_context.initial_draggable_pos.y + delta.y
        });
	}
        break;

    case Connecting:
    {
        if (IsMouseButtonUp(MouseButton::MOUSE_BUTTON_LEFT)) {
            if (connecting_context.targetPin.ComponentIndex != -1) {
				circuit.addWire({ connecting_context.sourceComponentIndex, 0 }, connecting_context.targetPin);
				circuit.set_component_input_wire(connecting_context.targetPin.ComponentIndex, connecting_context.targetPin.PinIndex, circuit.m_wires.size() - 1);
            }
            current_mouse_state = Idle;
            break;
		}

		bool found_target = false;
        for (auto& Component : circuit.m_components) {
			auto input_pin_index = Component.inputPinsContainPoint(mouse_pos);
            
            if (input_pin_index != -1 && Component.get_wire_index_for_input_pin(input_pin_index) == -1) {
                connecting_context.targetPin = { int(&Component - &circuit.m_components[0]), input_pin_index };
				found_target = true;
                break;
			}
        }

		if (!found_target) 
			connecting_context.targetPin = { -1, -1 };
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

void App::UI() 
{
    rlImGuiBegin();
    ImGui::Begin("Components");

    if (ImGui::Button("AND"))
        selected_component_type = Component::Type::AND;
    if (ImGui::Button("OR"))
        selected_component_type = Component::Type::OR;
    if (ImGui::Button("NOT"))
        selected_component_type = Component::Type::NOT;
    if (ImGui::Button("HIGH"))
        selected_component_type = Component::Type::HIGH;
    if (ImGui::Button("LOW"))
        selected_component_type = Component::Type::LOW;

    ImGui::End();
    rlImGuiEnd();
}

void App::Draw() 
{ 
    BeginDrawing();

    BeginMode2D(camera);
    ClearBackground(darkTheme.background);

    DrawGrid();

	circuit.draw();

    if (current_mouse_state == Connecting)
    {
		auto& inputComponent = circuit.getComponent(connecting_context.sourceComponentIndex);
        Vector2 start = inputComponent.getOutputPosition();
        Vector2 end = GetScreenToWorld2D(GetMousePosition(), camera);
        
        if (connecting_context.targetPin.ComponentIndex != -1) {
			 end = circuit.getComponent(connecting_context.targetPin.ComponentIndex).getInputPositions()[connecting_context.targetPin.PinIndex];
		}

        DrawLineEx(start, end, 3, LogicLevelColors[inputComponent.m_component.m_output_pin.value]);
    }

	auto top_left = GetScreenToWorld2D({ 0, 0 }, camera);
    std::string state_text;
    switch (current_mouse_state) {
        case Idle: state_text = "Idle"; break;
        case Panning: state_text = "Panning"; break;
        case Dragging: state_text = "Dragging"; break;
        case Connecting: state_text = "Connecting"; break;
	}
    
    DrawText(("State: " + state_text).c_str(), top_left.x, top_left.y, 20, RED);
    
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
