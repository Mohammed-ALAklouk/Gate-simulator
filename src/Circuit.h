#pragma once
#include <vector>
#include "Component.h"
#include "Wire.h"


class Circuit
{
	void evaluate();
	void evaluateComponent(Component& component);

	std::vector<Component> m_components;
	std::vector<Wire>m_wires;
};