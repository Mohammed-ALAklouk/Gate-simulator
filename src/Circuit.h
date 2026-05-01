#pragma once
#include <vector>
#include "Component.h"
#include "Wire.h"
#include "LogicNode.h"
#include "IdManager.h"


class Circuit
{
public:
	void evaluate();
	void evaluateComponent(Component& component);

	void addComponent(Component::Type type, Vector2 position) {
		int id = m_id_manager.getNextId();
		m_components.push_back(LogicNode(type, position, id));
		m_id_manager.setIndex(id, m_components.size() - 1);
	}

	void set_component_input_wire(int componentID, int input_index, int wire_ID) {
		int component_index = m_id_manager.getIndex(componentID);
		m_components[component_index].m_component.m_input_wires[input_index] = wire_ID;
	}

	void addWire(PinRef input, PinRef output) {
		int id = m_id_manager.getNextId();
		int input_component_index = m_id_manager.getIndex(input.ComponentID);
		m_wires.push_back({ input, output, m_components[input_component_index].m_component.m_output_pin.value, id });
		m_id_manager.setIndex(id, m_wires.size() - 1);
	}

	const LogicNode& getComponent(int id) const {
		int index = m_id_manager.getIndex(id);	
		return m_components[index];
	}

	const Wire& getWire(int id) const {
		int index = m_id_manager.getIndex(id);
		return m_wires[index];
	}

	void draw() const {
		for (const auto& component : m_components) {
			std::vector<LogicLevel> inputs;
			for (int i = 0; i < component.m_component.m_input_wires.size(); ++i) {
				int wire_id = component.m_component.m_input_wires[i];
				if (wire_id == -1) {
					inputs.push_back(LogicLevel::UNDEFINED);
				}
				else {
					inputs.push_back(getWire(wire_id).value);
				}
			}

			component.draw(inputs);
		}

		for (const auto& wire : m_wires) {
			auto start = getComponent(wire.input.ComponentID).getOutputPosition();
			auto end = getComponent(wire.output.ComponentID).getInputPositions()[wire.output.PinIndex];
			DrawLineEx(start, end, 3, LogicLevelColors[wire.value]);
		}
	}

	std::vector<LogicNode> m_components;
	std::vector<Wire>m_wires;
	IdManager m_id_manager;

private:
	LogicNode& getNodeById(int id) {
		int index = m_id_manager.getIndex(id);
		return m_components[index];
	}

	Wire& getWireById(int id) {
		int index = m_id_manager.getIndex(id);
		return m_wires[index];
	}
};