#pragma once

enum LogicLevel
{
	LOW,
	HIGH,
	UNDEFINED
};

struct Pin
{
	Pin(LogicLevel initialLevel = LogicLevel::UNDEFINED): value(initialLevel)
	{
	}

	LogicLevel value;
};

struct PinRef
{
	int ComponentIndex;
	int PinIndex;
};