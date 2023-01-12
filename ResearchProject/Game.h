#pragma once
#include <vector>

struct Camera;
struct Elevator;
struct Room;
struct Agent;

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;

	Camera* m_pCamera;

	std::vector<Room*> m_ApartmentPtrs;
	std::vector<Elevator*> m_ElevatorPtrs;
	std::vector<Agent*> m_AgentPtrs;

	int m_Time{};
	float m_FPSTimer{};

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;

	Point2f GoWork(const Agent& agent);
	Point2f GoHome(const Agent& agent);

	Point2f GoToCafe(const Agent& agent, const float probability);
	Point2f GoVisitRetail(const Agent& agent, const float probability);

	Room* GetClosestCafe(const Agent& agent);
};