#pragma once
#include "Pin.h"

struct Wire 
{
	PinRef input;
	PinRef output;
	LogicLevel value;
	int id;
};
