#pragma once

#include "Component.h"
#include "Draggable.h"

class LogicNode 
{

	public:
	LogicNode(Component::Type ComponentType, int x, int y)
		: m_component(ComponentType), m_draggable({ (float)(x / 20 * 20), (float)(y / 20 * 20), 100, 100 }, { 0, 0, 255, 255 })
	{
	}

	LogicNode(Component::Type ComponentType, Vector2 position)
		: LogicNode(ComponentType, position.x, position.y)
	{
	}

	void draw(std::vector <LogicLevel> inputs) const {
		m_draggable.render(inputs, m_component.m_output_pin.value, m_component.m_name);
	}

	bool containsPoint(Vector2 point) const {
		return m_draggable.containsPoint(point);
	}

	std::vector<Vector2> getInputPositions() const {
		std::vector<Vector2> positions;
		for (int i = 0; i < m_component.m_input_wires.size(); ++i) {
			float y = m_draggable.getPosition().y + m_draggable.rect.height / (m_component.m_input_wires.size() + 1) * (i + 1);
			positions.push_back({ m_draggable.getPosition().x, y });
		}
		return positions;
	}

	Vector2 getOutputPosition() const {
		return { m_draggable.getPosition().x + m_draggable.rect.width, m_draggable.getPosition().y + m_draggable.rect.height / 2 };
	}

	int inputPinsContainPoint(Vector2 point) const {
		auto inputPositions = getInputPositions();
		for (int i = 0; i < m_component.m_input_wires.size(); ++i) {
			float y = inputPositions[i].y;
			float x = inputPositions[i].x;

			Vector2 translation = { point.x - x, point.y - y };
			float distanceSquared = translation.x * translation.x + translation.y * translation.y;

			if (distanceSquared < 25) {
				return i;
			}
		}
		return -1;
	}

	int get_wire_index_for_input_pin(int pinIndex) const {
		if (pinIndex < 0 || pinIndex >= m_component.m_input_wires.size()) {
			return -1;
		}

		return m_component.m_input_wires[pinIndex];
	}

	bool outputPinContainsPoint(Vector2 point) const {
		auto outputPosition = getOutputPosition();
		Vector2 translation = { point.x - outputPosition.x, point.y - outputPosition.y };
		float distanceSquared = translation.x * translation.x + translation.y * translation.y;

		return distanceSquared < 25;
	}

	Component m_component;
	Draggable m_draggable;
};