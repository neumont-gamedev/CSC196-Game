#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Renderer/Model.h"
#include "Input/InputSystem.h"
#include "Player.h"
#include "Enemy.h"

#include <iostream>
#include <vector>
#include <thread>

using namespace std;

class Star
{
public:
	Star(const kiko::vec2& pos, const kiko::vec2& vel) :
		m_pos{ pos },
		m_vel{ vel }
	{}

	void Update(int width, int height)
	{
		m_pos += m_vel * kiko::g_time.GetDeltaTime();
		if (m_pos.x >= width) m_pos.x = 0;
		if (m_pos.y >= height) m_pos.y = 0;
	}

	void Draw(kiko::Renderer& renderer)
	{
		renderer.DrawPoint(m_pos.x, m_pos.y);
	}

public:
	kiko::vec2 m_pos;
	kiko::vec2 m_vel;
};

int main(int argc, char* argv[])
{
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets");

	kiko::g_renderer.Initialize();
	kiko::g_renderer.CreateWindow("CSC196", 800, 600);

	kiko::g_inputSystem.Initialize();

	kiko::Model model;
	model.Load("ship.txt");

	vector<Star> stars;
	for (int i = 0; i < 1000; i++)
	{
		kiko::Vector2 pos(kiko::Vector2(kiko::random(kiko::g_renderer.GetWidth()), kiko::random(kiko::g_renderer.GetHeight())));
		kiko::Vector2 vel(kiko::randomf(100, 200), 0.0f);

		stars.push_back(Star(pos, vel));
	}

	kiko::vec2 position{ 400, 300 };
	kiko::Transform transform{ { 400, 300 }, 0, 3 };

	float speed = 200;
	constexpr float turnRate = kiko::DegreesToRadians(180);

	Player player{ 200, kiko::Pi, { { 400, 300 }, 0, 6 }, model };

	std::vector<Enemy> enemies;
	for (int i = 0; i < 100; i++)
	{
		Enemy enemy{ 300, kiko::Pi, { { kiko::random(800), kiko::random(600) }, kiko::randomf(kiko::TwoPi), 3}, model};
		enemies.push_back(enemy);
	}

	// main game loop
	bool quit = false;
	while (!quit)
	{
		// update engine
		kiko::g_time.Tick();
		kiko::g_inputSystem.Update();
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE))
		{
			quit = true;
		}

		// update game
		player.Update(kiko::g_time.GetDeltaTime());
		for (auto& enemy : enemies)	enemy.Update(kiko::g_time.GetDeltaTime());

		// draw game
		kiko::g_renderer.SetColor(0, 0, 0, 0);
		kiko::g_renderer.BeginFrame();
		for (auto& star : stars)
		{
			star.Update(kiko::g_renderer.GetWidth(), kiko::g_renderer.GetHeight());
			kiko::g_renderer.SetColor(kiko::random(256), kiko::random(256), kiko::random(256), 255);
			star.Draw(kiko::g_renderer);
		}
		player.Draw(kiko::g_renderer);
		for (auto& enemy : enemies)
		{
			kiko::g_renderer.SetColor(kiko::random(256), kiko::random(256), kiko::random(256), 255);
			enemy.Draw(kiko::g_renderer);
		}
		
		kiko::g_renderer.EndFrame();
	}

	return 0;
}
