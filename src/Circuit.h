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

	void set_component_input_wire(int component_index, int input_index, int wire_index) {
		m_components[component_index].m_component.m_input_wires[input_index] = wire_index;
	}

	void addWire(PinRef input, PinRef output) {
		m_wires.push_back({ input, output, m_components[input.ComponentIndex].m_component.m_output_pin.value });
	}

	const LogicNode& getComponent(int index) const {
		return m_components[index];
	}

	const Wire& getWire(int index) const {
		return m_wires[index];
	}

	void draw() const {
		for (const auto& component : m_components) {
			std::vector<LogicLevel> inputs;
			for (int i = 0; i < component.m_component.m_input_wires.size(); ++i) {
				int wire_index = component.m_component.m_input_wires[i];
				if (wire_index == -1) {
					inputs.push_back(LogicLevel::UNDEFINED);
				}
				else {
					inputs.push_back(m_wires[wire_index].value);
				}
			}


			component.draw(inputs);
		}

		for (const auto& wire : m_wires) {
			auto inputComponent = m_components[wire.input.ComponentIndex];
			auto outputComponent = m_components[wire.output.ComponentIndex];
			auto start = inputComponent.getOutputPosition();
			auto end = outputComponent.getInputPositions()[wire.output.PinIndex];
			DrawLineEx(start, end, 3, LogicLevelColors[wire.value]);
		}
	}

	std::vector<LogicNode> m_components;
	std::vector<Wire>m_wires;
};