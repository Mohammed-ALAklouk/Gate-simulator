#include "Circuit.h"

void Circuit::evaluate()
{
	for (auto& component : m_components)
	{
		evaluateComponent(component.m_component);
	}

	for (auto& wire : m_wires)
	{
		wire.value = m_components[wire.input.ComponentIndex].m_component.m_output_pin.value;
	}
}

void Circuit::evaluateComponent(Component& component)
{
	std::vector<LogicLevel> input_values;
	for (int i = 0; i < component.m_input_wires.size(); ++i)
	{
		int wire_index = component.m_input_wires[i];
		if (wire_index == -1)
		{
			input_values.push_back(LogicLevel::UNDEFINED);
			continue;
		}


		input_values.push_back(m_wires[wire_index].value);
	}

	component.evaluate(input_values);
}