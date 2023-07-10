#pragma once
#include "Core/Vector2.h"
#include <SDL2-2.28.0/include/SDL.h>
#include <vector>
#include <array>

namespace kiko
{
	class InputSystem
	{
	public:
		enum class InputState
		{
			Idle,
			Pressed,
			Held,
			Released
		};

	public:
		InputSystem() = default;
		~InputSystem() = default;

		bool Initialize();
		void Shutdown();

		void Update();

		// keyboard
		InputState GetKeyState(uint32_t key);
		bool GetKeyDown(uint32_t key) const { return m_keyboardState[key]; }
		bool GetPreviousKeyDown(uint32_t key) const { return m_prevKeyboardState[key]; }

		// mouse
		const Vector2& GetMousePosition() const { return m_mousePosition; }

		InputState GetMouseButtonState(uint32_t button);
		bool GetMouseButtonDown(uint32_t button) { return m_mouseButtonState[button]; }
		bool GetPreviousMouseButtonDown(uint32_t button) { return m_prevMouseButtonState[button]; }


	private:
		std::vector<uint8_t> m_keyboardState;
		std::vector<uint8_t> m_prevKeyboardState;

		Vector2 m_mousePosition;
		std::array<uint8_t, 3> m_mouseButtonState;
		std::array<uint8_t, 3> m_prevMouseButtonState;
	};
}