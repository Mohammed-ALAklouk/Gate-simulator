#pragma once
#include <raylib.h>

static Color LogicLevelColors[] = {
	GRAY, // LOW
	GREEN, // HIGH
	RED // UNDEFINED
};

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