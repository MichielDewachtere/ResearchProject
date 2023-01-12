#pragma once

#include "structs.h"
#include "utils.h"

using namespace utils;

enum class RoomType
{
	apartment = 0,
	office = 1,
	cafe = 2
};

struct Elevator
{
	Elevator(const Point2f position)
	{
		elevator.left = position.x;
		elevator.bottom = position.y;
	}

	void Draw() const
	{
		SetColor(Color4f{ outlineColor });
		DrawRect(elevator, 4);
		SetColor(Color4f{ fillColor });
		FillRect(elevator);
	}

	Rectf elevator{ 0,0,30,75 };
	const Color4f outlineColor{ 0,0,0,1 };
	const Color4f fillColor{ 144.f / 255, 144.f / 255, 144.f / 255, 1 };
};

struct Room
{
	Room() = default;
	Room(const Point2f position)
	{
		bounds.left = position.x;
		bounds.bottom = position.y;
	}

	void Draw() const
	{
		SetColor(outlineColor);
		DrawRect(bounds, 4);
		SetColor(fillColor);
		FillRect(bounds);
	}

	Rectf bounds{ 0,0,150,75 };
	const Color4f outlineColor{ 0,0,0,1 };
	Color4f fillColor{ 210.f / 255,180.f / 255,140.f / 255,1 };
	RoomType type;
};

struct Agent
{
	Agent() = default;
	~Agent()
	{
		delete pWorkStation;
		delete pApartment;
	}
	Agent(const Room& house)
	{
		constexpr float offSet = 10;
		Point2f position;

		position.x = house.bounds.left + rand() % (int)(house.bounds.width - offSet);
		position.y = house.bounds.bottom;


		body.left = position.x;
		body.bottom = position.y;
		head = Rectf(body.left, body.bottom + body.height, 13, 13);

		pApartment = new Room(house);
	}

	void Draw() const
	{
		SetColor(bodyColor);
		FillRect(body);
		SetColor(headColor);
		FillRect(head);
	}

	Rectf body{ 0,0,13,30 };
	Color4f bodyColor{ (float)(rand() % 2),(float)(rand() % 2),(float)(rand() % 2),1 };

	Rectf head{  };
	Color4f headColor{ 255 / 255.f,206 / 255.f,180 / 255.f,1 };

	Room* pApartment{};
	Room* pWorkStation{};
	std::queue<std::pair<Point2f, int>> schedule{};

	bool relocate;
};