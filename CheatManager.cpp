#include "CheatManager.h"
#include "cexception.h"
#include "EntityUpdator.h"
#include "GOM.h"
#include "Globals.h"
#include <thread>

using DirectX::XMFLOAT4X4;
using Cheat::Vector2;
using Cheat::Vector3;
using Cheat::Vector4;

Rust::CheatManager::CheatManager()
	: m_previousInGame(false)
{
}


Rust::CheatManager::~CheatManager()
{
}

void Rust::CheatManager::exec()
{
	//if in game 
	try {
		if (!m_previousInGame && IsinGame()) {
			Rust::EntityUpdator::UpdateLocalPlayerAndCameraData();
			m_previousInGame = true;
		}
		else if (m_previousInGame && !IsinGame()) {
			m_previousInGame = false;
		}

		m_visual.BeginDraw();
		m_visual.DrawOtherVisuals();
		
		//do something with tagged object
		try {
			Rust::Globals::hack_data.TaggedObject.mutex.lock();

			m_visual.DrawTaggedObject();
			m_aimbot.exec();
			m_misc.exec();

			Rust::Globals::hack_data.TaggedObject.mutex.unlock();
		}
		catch (Cheat::MemoryManager::MemException& ex) {
			Rust::Globals::hack_data.TaggedObject.mutex.unlock();
		}

		//do something with active object when you can
		try {
			Rust::Globals::hack_data.ActiveObjects.mutex.lock();

			m_visual.DrawActiveObject();

			Rust::Globals::hack_data.ActiveObjects.mutex.unlock();

		}
		catch (Cheat::MemoryManager::MemException& ex) {
			Rust::Globals::hack_data.TaggedObject.mutex.unlock();
		}
		
		m_visual.EndDraw();
	}
	catch (Cheat::cexception& ex) {
		throw ex;
	}
}

bool Rust::CheatManager::IsinGame()
{
	return true;
}