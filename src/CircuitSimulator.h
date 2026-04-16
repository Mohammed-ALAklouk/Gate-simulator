#pragma once
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include <iostream>

struct PanningContext {
	Vector2 initial_pos;
	Vector2 initial_target;
};

struct UITheme {
	Color background;
	Color gridMinor;
	Color gridMajor;
	Color logicLow;    // Logic 0
	Color logicHigh;   // Logic 1
	Color logicX;      // Undefined
};



class CircuitSimulator
{
public:
	CircuitSimulator();
	~CircuitSimulator();
	void Run();
	
private:
	void HandleInput();
	void Update(float deltaTime);
	void UI();
	void Draw();

	void DrawGrid();
	int cell_size = 20;
	int window_width = 800;
	int window_height = 450;

	typedef enum { Idle, Panning } MouseState;

	Camera2D camera;
	const float zoom_levels[6] = { 0.33f, 0.45f, 0.60f, 0.75f, 0.90f, 1.00f };
	int current_zoom_index = 5; // Start at 1.0 (index 5)

	MouseState current_mouse_state = Idle;
	PanningContext panning_context;


	UITheme darkTheme = {
	{ 18, 18, 18, 255 },
	{ 30, 30, 30, 255 },
	{ 50, 50, 50, 255 },
	{ 40, 60, 40, 255 },
	{ 0, 255, 0, 255 },
	{ 255, 165, 0, 255 }
	};
};
