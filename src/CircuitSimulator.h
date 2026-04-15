#pragma once
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

class CircuitSimulator
{
public:

	CircuitSimulator();
	~CircuitSimulator();
	void HandleInput();
	void Update(float deltaTime);
	void UI();
	void Draw();

	void Run();
};
