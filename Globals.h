#pragma once
#include <Windows.h>
#include "MemoryManager.h"
#include "CheatStructs.h"
#include "HackSetting.h"
#include <utility>

namespace Rust {
	struct GameState {
		bool inGame;
	};

	struct SystemData {
		HWND hOverlay;
		RECT GameWindowRect;
		int width;
		int height;
	};

	struct HackData {
		Cheat::ExternalMemoryManager* RustMemory;
		SyncData<std::shared_ptr<Rust::CheatStruct::TaggedObject>> TaggedObject;
		SyncData<std::shared_ptr<Rust::CheatStruct::ActiveObject>> ActiveObjects;
		Rust::CheatStruct::Player LocalPlayer;
		Rust::CheatStruct::EntityAddresses MainCam;
	};

	struct Globals {
		static bool CheatRunning;
		static void Init();
		static HackSetting hack_setting;
		static GameState game_state;
		static SystemData system_data;
		static HackData hack_data;
	};
}

