#include "Circuit.h"

void Circuit::evaluate()
{
	for (auto& wire : m_wires)
	{
		wire.value = getComponent(wire.input.ComponentID).m_component.m_output_pin.value;
	}
	
	for (auto& component : m_components)
	{
		evaluateComponent(component.m_component);
	}
}

void Circuit::evaluateComponent(Component& component)
{
	std::vector<LogicLevel> input_values;
	for (int i = 0; i < component.m_input_wires.size(); ++i)
	{
		int wire_id = component.m_input_wires[i];
		if (wire_id == -1)
		{
			input_values.push_back(LogicLevel::UNDEFINED);
			continue;
		}


		input_values.push_back(getWire(wire_id).value);
	}

	component.evaluate(input_values);
}

void Circuit::draw() 
{
	{
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
}
