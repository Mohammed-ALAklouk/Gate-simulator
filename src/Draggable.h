#pragma once
#include "raylib.h"


class Draggable {
	Rectangle rect;
	Color color;
	std::string name;
public:

	Draggable(Rectangle rect = {0, 0, 100, 100}, Color color = {0, 0, 0, 255}, std::string name = "")
		: rect(rect), color(color), name(name) {}

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

	void render() const {
		DrawRectangleRec(rect, color);
		DrawText(name.c_str(), rect.x + 5, rect.y + 5, 18, WHITE);
	}


};
