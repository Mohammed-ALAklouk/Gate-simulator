#pragma once
#include "raylib.h"
#include <vector>
#include <string>
#include "Pin.h"


class Draggable {
public:

	Draggable(Rectangle rect = {0, 0, 100, 100}, Color color = {0, 0, 0, 255}, std::string name = "")
		: rect(rect), color(color){}

	bool containsPoint(Vector2 point) const {
		return CheckCollisionPointRec(point, rect);
	}

	Vector2 getPosition() const {
		return { rect.x, rect.y };
	}

	void setPosition(Vector2 pos) {
		rect.x = pos.x;
		rect.y = pos.y;
	}

	void render(std::vector<LogicLevel> inputs, LogicLevel output, std::string name) const {
		DrawRectangleRec(rect, color);
		DrawText(name.c_str(), rect.x + 5, rect.y + 5, 18, WHITE);

		DrawCircle(rect.x + rect.width, rect.y + rect.height / 2, 5, LogicLevelColors[output]);
		float midY = rect.y + rect.height / 2;
		float spacing = 20; 

		for(int i = 0; i < inputs.size(); i++) {
			float y = rect.y + rect.height / (inputs.size() + 1) * (i + 1);
			DrawCircle(rect.x, y, 5, LogicLevelColors[inputs[i]]);
		}
	}
	Rectangle rect;
private:
	Color color;
};
