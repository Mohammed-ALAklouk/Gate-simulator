#include "LogicNode.h"

void LogicNode::draw(std::vector<LogicLevel> inputs) const
{
	DrawRectangleRec(rect, color);
	DrawText(m_component.m_name.c_str(), rect.x + 5, rect.y + 5, 18, WHITE);

	DrawCircle(rect.x + rect.width, rect.y + rect.height / 2, 5, LogicLevelColors[m_component.m_output_pin.value]);
	float midY = rect.y + rect.height / 2;
	float spacing = 20;

	for (int i = 0; i < inputs.size(); i++) {
		float y = rect.y + rect.height / (inputs.size() + 1) * (i + 1);
		DrawCircle(rect.x, y, 5, LogicLevelColors[inputs[i]]);
	}
}

std::vector<Vector2> LogicNode::getInputPositions() const
{
	std::vector<Vector2> positions;
	for (int i = 0; i < m_component.m_input_wires.size(); ++i) {
		float y = rect.y + rect.height / (m_component.m_input_wires.size() + 1) * (i + 1);
		positions.push_back({ rect.x, y });
	}
	return positions;
}

int LogicNode::inputPinsContainPoint(Vector2 point) const
{
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
