#pragma once
#include <SDL_keyboard.h>

#include "Vector3.h"

using namespace dae;

struct Camera
{
	Camera() = default;
	Camera(const Vector3 _origin) { origin = _origin; }

	Vector3 origin;
	float scale = 1.f;

	void Update(float deltaTime)
	{
		constexpr float moveSpeed = 500.f;
		constexpr float scaleSpeed = 1.f;

		const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);
		if (pKeyboardState[SDL_SCANCODE_Q])
		{
			MoveForward(deltaTime, scaleSpeed);
		}
		if (pKeyboardState[SDL_SCANCODE_E])
		{
			MoveBackward(deltaTime, scaleSpeed);
		}
		if (pKeyboardState[SDL_SCANCODE_D])
		{
			origin.x -= moveSpeed * deltaTime;
		}
		if (pKeyboardState[SDL_SCANCODE_A])
		{
			origin.x += moveSpeed * deltaTime;
		}
		if (pKeyboardState[SDL_SCANCODE_W])
		{
			MoveUp(deltaTime, moveSpeed);
		}
		if (pKeyboardState[SDL_SCANCODE_S])
		{
			MoveDown(deltaTime, moveSpeed);
		}
	}

	void MoveForward(float deltaTime, float scaleSpeed) { scale += deltaTime * scaleSpeed; }
	void MoveBackward(float deltaTime, float scaleSpeed) { scale -= deltaTime * scaleSpeed; }
	void MoveUp(float deltaTime, float moveSpeed) { origin.y -= deltaTime * moveSpeed; }
	void MoveDown(float deltaTime, float moveSpeed) { origin.y += deltaTime * moveSpeed; }
};