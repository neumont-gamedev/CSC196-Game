#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Renderer/ModelManager.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Framework/Scene.h"
#include "Renderer/Text.h"
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
	kiko::MemoryTracker::Initialize();
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets");

	kiko::g_renderer.Initialize();
	kiko::g_renderer.CreateWindow("CSC196", 800, 600);

	kiko::g_inputSystem.Initialize();

	kiko::g_audioSystem.Initialize();
	kiko::g_audioSystem.AddAudio("hit", "hit.wav");

	// create font / text objects
	std::shared_ptr<kiko::Font> font = std::make_shared<kiko::Font>("arcadeclassic.ttf", 24);
	std::unique_ptr<kiko::Text> text = std::make_unique<kiko::Text>(font);
	text->Create(kiko::g_renderer, "SCORE 0000", kiko::Color{ 1, 0, 1, 1 });


	vector<Star> stars;
	for (int i = 0; i < 1000; i++)
	{
		kiko::Vector2 pos(kiko::Vector2(kiko::random(kiko::g_renderer.GetWidth()), kiko::random(kiko::g_renderer.GetHeight())));
		kiko::Vector2 vel(kiko::randomf(100, 200), 0.0f);

		stars.push_back(Star(pos, vel));
	}

	kiko::Scene scene;
	unique_ptr<Player> player = make_unique<Player>(200.0f, kiko::Pi, kiko::Transform{ { 400, 300 }, 0, 6 }, kiko::g_manager.Get("ship.txt"));
	player->m_tag = "Player";
	scene.Add(std::move(player));

	for (int i = 0; i < 5; i++)
	{
		unique_ptr<Enemy> enemy = make_unique<Enemy>(kiko::randomf(75.0f, 150.0f), kiko::Pi, kiko::Transform{ { kiko::random(800), kiko::random(600) }, kiko::randomf(kiko::TwoPi), 3}, kiko::g_manager.Get("enemy.txt"));
		enemy->m_tag = "Enemy";
		scene.Add(std::move(enemy));
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
		scene.Update(kiko::g_time.GetDeltaTime());

		// draw game
		kiko::g_renderer.SetColor(0, 0, 0, 0);
		kiko::g_renderer.BeginFrame();
		for (auto& star : stars)
		{
			star.Update(kiko::g_renderer.GetWidth(), kiko::g_renderer.GetHeight());
			kiko::g_renderer.SetColor(kiko::random(256), kiko::random(256), kiko::random(256), 255);
			star.Draw(kiko::g_renderer);
		}

		kiko::g_renderer.SetColor(255, 255, 255, 255);
		scene.Draw(kiko::g_renderer);
		text->Draw(kiko::g_renderer, 40, 30);
		
		kiko::g_renderer.EndFrame();
	}

	stars.clear();
	scene.RemoveAll();

	return 0;
}
