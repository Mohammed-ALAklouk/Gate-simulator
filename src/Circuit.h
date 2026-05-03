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
	void draw() ;

	void addComponent(Component::Type type, Vector2 position) {
		int id = m_component_ids.getNextId();
		m_components.push_back(LogicNode(type, position, id));
		m_component_ids.setIndex(id, m_components.size() - 1);
	}

	void set_component_input_wire(int componentID, int input_index, int wire_ID) {
		int component_index = m_component_ids.getIndex(componentID);
		m_components[component_index].m_component.m_input_wires[input_index] = wire_ID;
	}

	void addWire(PinRef input, PinRef output) {
		int id = m_wire_ids.getNextId();
		int input_component_index = m_component_ids.getIndex(input.ComponentID);
		auto& input_component = m_components[input_component_index];
		input_component.m_component.m_output_wires.push_back(id);
		m_wires.push_back({ input, output, input_component.m_component.m_output_pin.value, id });
		m_wire_ids.setIndex(id, m_wires.size() - 1);
	}

	LogicNode& getComponent(int id) {
		int index = m_component_ids.getIndex(id);	
		return m_components[index];
	}

	Wire& getWire(int id) {
		int index = m_wire_ids.getIndex(id);
		return m_wires[index];
	}

	void removeComponent(int id) {
		int index = m_component_ids.getIndex(id);
		if (index != -1) {
			auto& component = m_components[index];
			for (int input_wire_id : component.m_component.m_input_wires) {
				removeWire(input_wire_id);
			}

			for (int output_wire_id : component.m_component.m_output_wires) {
				removeWire(output_wire_id);
			}

			int lastIndex = (int)m_components.size() - 1;
			if (index != lastIndex) {
				std::swap(m_components[index], m_components[lastIndex]);
				m_component_ids.setIndex(m_components[index].id, index);
			}

			m_components.pop_back();
			m_component_ids.releaseId(id);
		}
	}

	void removeWire(int id) {
		int index = m_wire_ids.getIndex(id);
		if (index != -1) {
			auto& wire = m_wires[index];
			auto& input_component = m_components[m_component_ids.getIndex(wire.input.ComponentID)];
			input_component.m_component.m_output_wires.erase(
				std::remove( input_component.m_component.m_output_wires.begin(), input_component.m_component.m_output_wires.end(), id), 
				input_component.m_component.m_output_wires.end());

			auto& output_component = m_components[m_component_ids.getIndex(wire.output.ComponentID)];
			for (auto& input_wire_id : output_component.m_component.m_input_wires) {
				if (input_wire_id == id) {
					input_wire_id = -1;
				}
			}


			int lastIndex = (int)m_wires.size() - 1;
			if (index != lastIndex) {
				std::swap(m_wires[index], m_wires[lastIndex]);
				m_wire_ids.setIndex(m_wires[index].id, index);
			}
			
			m_wires.pop_back();
			m_wire_ids.releaseId(id);
		}
	}

	void selectComponentsInArea(Rectangle selectionRect, std::vector<int>& selectedComponentIDs) const {
		for (const auto& component : m_components) {
			if (CheckCollisionRecs(component.m_draggable.rect, selectionRect)) {
				selectedComponentIDs.push_back(component.id);
			}
		}
	}


	std::vector<LogicNode> m_components;
	std::vector<Wire>m_wires;
	IdManager m_component_ids;
	IdManager m_wire_ids;

private:
	LogicNode& getNodeById(int id) {
		int index = m_component_ids.getIndex(id);
		return m_components[index];
	}

	Wire& getWireById(int id) {
		int index = m_wire_ids.getIndex(id);
		return m_wires[index];
	}
};