#pragma once
#include "Aimbot.h"
#include "Visual.h"
#include "Misc.h"

namespace Rust {
	struct HackSetting {
		Rust::Visual::setting Visual;
		Rust::Aimbot::setting Aimbot;
		Rust::Misc::setting Misc;
	};
}