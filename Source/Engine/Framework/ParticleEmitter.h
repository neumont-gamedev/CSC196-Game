#pragma once
#include "Actor.h"

namespace kiko
{
	class ParticleEmitter
	{
	public:
		ParticleEmitter() = default;

		void Update(float dt);
		void Draw(kiko::Renderer& renderer);

	private:

	};
}