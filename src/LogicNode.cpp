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

int LogicNode::inputPinsContainPoint(Vector2 point) const
{
	for (int i = 0; i < m_component.m_input_wires.size(); ++i) {
		Vector2 inputPosition = getInputPosition(i);
		if (CheckCollisionCircles(point, 5, inputPosition, 5)) {
			return i;
		}
	}
	return -1;
}
