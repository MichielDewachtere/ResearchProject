#include "pch.h"
#include "Game.h"

#include <iostream>
#include <chrono>
#include <queue>

#include "utils.h"
#include "Vector3.h"
#include "Camera.h"
#include "GameStructs.h"
#include "MathUtils.h"


using namespace utils;

Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_pCamera(new Camera{ dae::Vector3{0,0,0} })
{
	Initialize();
}

Game::~Game()
{
	Cleanup();
}

void Game::Initialize()
{

	// Rooms init
	int amountOfFloors{};
	int amountOfApartments{};
	int amountOfOffices{};
	int amountOfCafes{};
	int amountOfAgents{};

	float bottom = 20;

	while (amountOfApartments < 1000)
	{
		bool floorIsFilled = false;
		int totalLength = 40;

		while (!floorIsFilled)
		{
			constexpr float maxLength = 1140;

			auto* newApartment = new Room();

			newApartment->bounds.left = (float)totalLength;
			newApartment->bounds.bottom = bottom;

			newApartment->bounds.width = 100.f + (rand() % 5) * 50.f;

			totalLength += (int)newApartment->bounds.width;

			int roomType = rand() % 9;
			if (roomType > 3 || roomType == 0)
				roomType = 1;

			switch (roomType) {
			case 1:
				newApartment->type = RoomType::apartment;
				newApartment->fillColor = Color4f(210.f / 255, 180.f / 255, 140.f / 255, 1);
				++amountOfApartments;
				break;
			case 2:
				newApartment->type = RoomType::office;
				newApartment->fillColor = Color4f(234.f / 255, 77.f / 255, 97.f / 255, 1);
				++amountOfOffices;
				break;
			case 3:
				newApartment->type = RoomType::cafe;
				newApartment->fillColor = Color4f(79.f / 255, 232.f / 255, 83.f / 255, 1);
				++amountOfCafes;
				break;
			}

			if (totalLength >= maxLength)
			{
				auto* newElevator = new Elevator(Point2f{ maxLength,bottom });
				m_ElevatorPtrs.push_back(newElevator);

				const float extra = (float)totalLength - maxLength;

				newApartment->bounds.width -= extra;

				floorIsFilled = true;

				bottom += m_ApartmentPtrs[0]->bounds.height;

				++amountOfFloors;
			}

			m_ApartmentPtrs.push_back(newApartment);
		}
	}

	// agents init
	for (const auto& apartment : m_ApartmentPtrs)
	{
		if (apartment->type == RoomType::apartment)
		{
			m_AgentPtrs.push_back(new Agent(*apartment));
			++amountOfAgents;
		}
	}

	for (const auto& agent : m_AgentPtrs)
	{
		Room* pWorkstation = m_ApartmentPtrs[rand() % m_ApartmentPtrs.size()];
		while(pWorkstation->type == RoomType::apartment)
		{
			pWorkstation = m_ApartmentPtrs[rand() % m_ApartmentPtrs.size()];
		}

		agent->pWorkStation = new Room(*pWorkstation);

		const int time = 0/*rand() % 3*/;
		// 0 -> night (= +0u)
		// 1 -> day (= +8u)
		// 2 -> evening (= + 16u)

		agent->schedule.push(std::make_pair(GoWork(*agent), (0 + 8 * time) % 25));
		agent->schedule.push(std::make_pair(GoToCafe(*agent, 0.5f), (0 + 8 * time) % 25));
		agent->schedule.push(std::make_pair(GoToCafe(*agent,1), (4 + 8 * time )% 25));
		agent->schedule.push(std::make_pair(GoToCafe(*agent, 0.25f), (7 + 8 * time )% 25));
		agent->schedule.push(std::make_pair(GoHome(*agent),	(8 + 8 * time) % 25));
		agent->schedule.push(std::make_pair(GoVisitRetail(*agent, 0.25f), (8 + 8 * time) % 25));
		agent->schedule.push(std::make_pair(GoToCafe(*agent, 0.5f), (12 + 8 * time) % 25));
		agent->schedule.push(std::make_pair(GoToCafe(*agent, 0.25f), (12 + 8 * time) % 25));
	}

	std::cout << "amount of floors: " << amountOfFloors << '\n';
	std::cout << "amount of apartments: " << amountOfApartments << '\n';
	std::cout << "amount of offices: " << amountOfOffices << '\n';
	std::cout << "amount of cafes: " << amountOfCafes << '\n';
	std::cout << "amount of agents: " << amountOfAgents << '\n';
}

void Game::Cleanup()
{
	for (const auto& apartment : m_ApartmentPtrs)
		delete apartment;

	for (const auto& elevator : m_ElevatorPtrs)
		delete elevator;

	for (const auto& agent : m_AgentPtrs)
		delete agent;

	delete m_pCamera;
}

void Game::Update(float elapsedSec)
{
	m_pCamera->Update(elapsedSec);

	// FPS output
	m_FPSTimer += elapsedSec;
	if (m_FPSTimer >= 1)
	{
		const int fps = (int)(1 / elapsedSec);
		std::cout << "FPS: " << fps << '\n';
		m_FPSTimer = 0;
	}

	++m_Time;

	if (m_Time > 1'440)
	{
		std::cout << "dag verstreken\n";
		
		const int time = 0/*rand() % 3*/;
		// 0 -> night (= +0u)
		// 1 -> day (= +8u)
		// 2 -> evening (= + 16u)
  
		for (const auto& agent : m_AgentPtrs)
		{
			while (agent->schedule.empty() == false)
				agent->schedule.pop();

			agent->schedule.push(std::make_pair(GoWork(*agent), (0 + 8 * time) % 25));
			agent->schedule.push(std::make_pair(GoToCafe(*agent, 0.5f), (1 + 8 * time) % 25));
			agent->schedule.push(std::make_pair(GoToCafe(*agent, 1), (4 + 8 * time) % 25));
			agent->schedule.push(std::make_pair(GoToCafe(*agent, 0.25f), (7 + 8 * time) % 25));
			agent->schedule.push(std::make_pair(GoHome(*agent), (8 + 8 * time) % 25));
			agent->schedule.push(std::make_pair(GoVisitRetail(*agent, 0.25f), (9 + 8 * time) % 25));
			agent->schedule.push(std::make_pair(GoToCafe(*agent, 0.5f), (12 + 8 * time) % 25));
			agent->schedule.push(std::make_pair(GoToCafe(*agent, 0.25f), (12 + 8 * time) % 25));

			agent->relocate = false;
		}
		m_Time = 0;
	}

	// get update movement and get positions from queue

	for (const auto& agent : m_AgentPtrs)
	{
		if (agent->schedule.front().second * 60 == m_Time)
		{
			agent->relocate = true;
		}

		if (agent->relocate)
		{
			const Point2f newPosition = agent->schedule.front().first;
			//std::cout << "x: " << newPosition.x << ", y: " << newPosition.y << '\n';

			// if is in pos
			agent->schedule.pop();
			agent->relocate = false;
		}
	}
}

void Game::Draw() const
{
	ClearBackground();

	glPushMatrix();
	{
		glTranslatef(m_pCamera->origin.x, m_pCamera->origin.y, 0);

		glTranslatef(m_Window.width / 2, m_Window.height / 2, 0);
		glScalef(m_pCamera->scale, m_pCamera->scale, 1);
		glTranslatef(-m_Window.width / 2, -m_Window.height / 2, 0);

		const Rectf groundPlane(0, 0, m_Window.width, 50);
		SetColor(Color4f{ 36.f / 255, 36.f / 255, 36.f / 255,1 });
		FillRect(groundPlane);

		for (const auto& apartment : m_ApartmentPtrs)
		{
			apartment->Draw();
		}
		for (const auto& elevator : m_ElevatorPtrs)
		{
			elevator->Draw();
		}
		for (const auto& agent : m_AgentPtrs)
		{
			agent->Draw();
		}
	}
	glPopMatrix();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.53f, 0.81f, 0.92f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

Point2f Game::GoWork(const Agent& agent)
{
	const Room* pWorkstation = agent.pWorkStation;

	Point2f position;
	position.x = pWorkstation->bounds.left + rand() % (int)(pWorkstation->bounds.width - agent.body.width);
	position.y = pWorkstation->bounds.bottom;

	return position;
}

Point2f Game::GoHome(const Agent& agent)
{
	const Room* pApartment = agent.pApartment;

	Point2f position;
	position.x = pApartment->bounds.left + rand() % (int)(pApartment->bounds.width - agent.body.width);
	position.y = pApartment->bounds.bottom;

	return position;
}

Point2f Game::GoToCafe(const Agent& agent, const float probability)
{
	const int chance = rand() % int(1 / probability) + 1;

	if (chance == (int)(1 / probability))
	{
		const Room* cafe = GetClosestCafe(agent);
		Point2f position;
		position.x = cafe->bounds.left + rand() % (int)(cafe->bounds.width - agent.body.width);
		position.y = cafe->bounds.bottom;

		return position;
	}

	return Point2f{ agent.body.left, agent.body.bottom };
}

Point2f Game::GoVisitRetail(const Agent& agent, const float probability)
{
	Room* pRoom = m_ApartmentPtrs[rand() % m_ApartmentPtrs.size()];
	while (pRoom->type != RoomType::apartment)
	{
		pRoom = m_ApartmentPtrs[rand() % m_ApartmentPtrs.size()];
	}

	Point2f position;
	position.x = pRoom->bounds.left + rand() % (int)(pRoom->bounds.width - agent.body.width);
	position.y = pRoom->bounds.bottom;

	return position;
}
Room* Game::GetClosestCafe(const Agent& agent)
{
	float closestDistance = FLT_MAX;
	Room* closestRoom{};

	for (const auto& room : m_ApartmentPtrs)
	{
		if (room->type != RoomType::cafe)
			continue;

		const float distance = DistanceSquared(Point2f(agent.body.left, agent.body.bottom), Point2f(room->bounds.left, room->bounds.bottom));

		if (distance < closestDistance)
		{
			closestDistance = distance;
			closestRoom = room;
		}
	}

	return closestRoom;
}
