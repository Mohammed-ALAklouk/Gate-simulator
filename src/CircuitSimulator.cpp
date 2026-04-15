#include "CircuitSimulator.h"

CircuitSimulator::CircuitSimulator()
{
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(800, 450, "GateSimulator");
    SetTargetFPS(60);
    rlImGuiSetup(true);
}

CircuitSimulator::~CircuitSimulator()
{
    rlImGuiShutdown();
    CloseWindow();
}

void CircuitSimulator::HandleInput()
{
}

void CircuitSimulator::Update(float deltaTime)
{
   
}

void CircuitSimulator::UI()
{
    rlImGuiBegin();
    rlImGuiEnd();
}

void CircuitSimulator::Draw()
{ 
    BeginDrawing();
    ClearBackground(BLACK);

    
    UI();
    EndDrawing();
}

void CircuitSimulator::Run()
{
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        HandleInput();
        Update(deltaTime);
        UI();
        Draw();
    }
}
