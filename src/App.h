#pragma once
#include <math.h>
#include <string>

#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "Draggable.h"

struct PanningContext {
	Vector2 initial_pos;
	Vector2 initial_target;
};

struct DraggingContext {
	Draggable* draggable;
	Vector2 initial_mouse_pos;
	Vector2 initial_draggable_pos;
};

struct UITheme {
	Color background;
	Color gridMinor;
	Color gridMajor;
	Color logicLow;    // Logic 0
	Color logicHigh;   // Logic 1
	Color logicX;      // Undefined
};



class App
{
public:
	App();
	~App();
	void Run();
	
private:
	void HandleInput();
	void Update(float deltaTime);
	void UI() const;
	void Draw() const;

	void DrawGrid() const;
	int cell_size = 20;
	int window_width = 800;
	int window_height = 450;

	typedef enum { Idle, Panning, Dragging } MouseState;

	Camera2D camera;
	const float zoom_levels[6] = { 0.33f, 0.45f, 0.60f, 0.75f, 0.90f, 1.00f };
	int current_zoom_index = 5; // Start at 1.0 (index 5)

	MouseState current_mouse_state = Idle;
	PanningContext panning_context;
	DraggingContext dragging_context;

	Draggable draggable;

	int major_step = 5;
	float grid_line_minor_thinkness = 0.8f;
	float grid_line_major_thinkness = 1.5f;


	UITheme darkTheme = {
	{ 18, 18, 18, 255 },
	{ 30, 30, 30, 255 },
	{ 50, 50, 50, 255 },
	{ 40, 60, 40, 255 },
	{ 0, 255, 0, 255 },
	{ 255, 165, 0, 255 }
	};
};
