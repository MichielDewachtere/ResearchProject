#include "pch.h"
#include "Game.h"

#include <iostream>

#include "utils.h"
#include "Vector3.h"
#include "Camera.h"
#include "GameStructs.h"


using namespace utils;

Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_pCamera(new Camera{ dae::Vector3{0,0,0} })
{
	Initialize();

	m_AgentPtrs.push_back(new Agent(Point2f{ 10,10 }));
}

Game::~Game()
{
	Cleanup();
}

void Game::Initialize()
{
	int amountOfApartments{};
	int amountOfOffices{};
	int amountOfCafes{};
	float bottom = 20;
	int amountOfFloors{};

	while (amountOfApartments < 1000)
	{
		bool floorIsFilled = false;
		int totalLength = 40;

		while (!floorIsFilled)
		{
			auto* newApartment = new Room();

			newApartment->bounds.left = (float)totalLength;
			newApartment->bounds.bottom = bottom;

			newApartment->bounds.width = 100.f + (float)((rand() % 5) * 50);

			totalLength += (int)newApartment->bounds.width;

			newApartment->type = static_cast<RoomType>(rand() % 9);
				++amountOfApartments;

			switch (newApartment->type) {
			case RoomType::apartment:
				newApartment->fillColor = Color4f(210.f / 255, 180.f / 255, 140.f / 255, 1);
				break;
			case RoomType::office:
				newApartment->fillColor = Color4f(234.f / 255, 77.f / 255, 97.f / 255, 1);
				++amountOfOffices;
				break;
			case RoomType::cafe:
				newApartment->fillColor = Color4f(79.f / 255, 232.f / 255, 83.f / 255, 1);
				++amountOfCafes;
				break;
			}

			if (totalLength >= 1220)
			{
				auto* newElevator = new Elevator(Point2f{ 1220,bottom });
				m_ElevatorPtrs.push_back(newElevator);

				const float extra = (float)totalLength - 1240.f;

				newApartment->bounds.width -= extra;

				floorIsFilled = true;

				bottom += m_ApartmentPtrs[0]->bounds.height;

				++amountOfFloors;
			}

			m_ApartmentPtrs.push_back(newApartment);
		}
	}

	std::cout << "amount of floors: " << amountOfFloors << '\n';
	std::cout << "amount of apartments: " << amountOfApartments << '\n';
	std::cout << "amount of offices: " << amountOfOffices << '\n';
	std::cout << "amount of cafes: " << amountOfCafes << '\n';
}

void Game::Cleanup()
{
	for (const auto& apartment : m_ApartmentPtrs)
		delete apartment;

	for (const auto& elevator : m_ElevatorPtrs)
		delete elevator;

	delete m_pCamera;
}

void Game::Update( float elapsedSec )
{
	m_pCamera->Update(elapsedSec);
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
	//std::cout << "MOUSEBUTTONUP event: ";
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

void Game::ClearBackground( ) const
{
	glClearColor( 0.53f, 0.81f, 0.92f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}