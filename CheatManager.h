#pragma once
#include <vector>
#include <cstdint>
#include "Visual.h"
#include "Aimbot.h"
#include "Misc.h"

namespace Rust {
	class CheatManager
	{
	public:
		CheatManager();
		~CheatManager();

		void exec();

	private:
		Rust::Visual m_visual;
		Rust::Aimbot m_aimbot;
		Rust::Misc m_misc;

		bool IsinGame();

		bool m_previousInGame;
	};

}

