#pragma once
#include <vector>
#include "Component.h"
#include "Wire.h"
#include "LogicNode.h"


class Circuit
{
public:
	void evaluate();
	void evaluateComponent(Component& component);

	void addComponent(Component::Type type, Vector2 position) {
		m_components.emplace_back(LogicNode(type, position));
	}

	void addWire(PinRef input, PinRef output) {
		m_wires.push_back({ input, output, LogicLevel::UNDEFINED });
	}

	const LogicNode& getComponent(int index) const {
		return m_components[index];
	}

	const Wire& getWire(int index) const {
		return m_wires[index];
	}

	void draw() const {
		for (const auto& component : m_components) {
			component.draw({ UNDEFINED, UNDEFINED });
		}

		for (const auto& wire : m_wires) {
			auto inputComponent = m_components[wire.input.ComponentIndex];
			auto outputComponent = m_components[wire.output.ComponentIndex];
			auto start = inputComponent.getOutputPosition();
			auto end = outputComponent.getInputPositions()[wire.output.PinIndex];
			DrawLineEx(start, end, 3, wire.value == LogicLevel::HIGH ? GREEN : RED);
		}
	}

	std::vector<LogicNode> m_components;
	std::vector<Wire>m_wires;
};