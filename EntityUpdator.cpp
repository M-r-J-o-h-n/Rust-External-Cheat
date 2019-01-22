#include "EntityUpdator.h"
#include "CheatStructs.h"
#include "GameStructs.h"
#include "cexception.h"
#include "Globals.h"
#include "Timer.h"
#include "GOM.h"
#include <vector>
#include <unordered_set>

using namespace DirectX;
using namespace Rust;

void Rust::EntityUpdator::UpdateTaggedObject()
{
	try
	{
		Rust::Globals::hack_data.TaggedObject.mutex.lock();

		auto it = Rust::Globals::hack_data.TaggedObject.map.begin();
		while (it != Rust::Globals::hack_data.TaggedObject.map.end()) {
			try {
				if (it->second->IsDeletable()) {
					it = Rust::Globals::hack_data.TaggedObject.map.erase(it);
					continue;
				}
				else {
					if (it->second->IsUpdatable())
						it->second->UpdateData();
				}
				++it;
			}
			catch (Cheat::ExternalMemoryManager::MemException& ex) {
				it = Rust::Globals::hack_data.TaggedObject.map.erase(it);
			}
		}

		Rust::Globals::hack_data.TaggedObject.mutex.unlock();
	}
	catch (Cheat::MemoryManager::MemException& ex) {
		Rust::Globals::hack_data.TaggedObject.mutex.unlock();
	}
}

void Rust::EntityUpdator::UpdateActiveObject()
{
	try
	{
		Rust::Globals::hack_data.ActiveObjects.mutex.lock();

		auto it = Rust::Globals::hack_data.ActiveObjects.map.begin();

		while (it != Rust::Globals::hack_data.ActiveObjects.map.end()) {
			try {
				if (it->second->IsDeletable()) {
					it = Rust::Globals::hack_data.ActiveObjects.map.erase(it);
					continue;
				}
				else {
					if (it->second->IsUpdatable())
						it->second->UpdateData();
				}
				++it;
			}
			catch (Cheat::ExternalMemoryManager::MemException& ex) {
				it = Rust::Globals::hack_data.ActiveObjects.map.erase(it);
			}
		}

		Rust::Globals::hack_data.ActiveObjects.mutex.unlock();
	}
	catch (Cheat::MemoryManager::MemException& ex) {
		Rust::Globals::hack_data.ActiveObjects.mutex.unlock();
	}
}

void Rust::EntityUpdator::UpdateThread()
{
	while (Rust::Globals::game_state.inGame) { // Update Additional object in every 500ms
		if (!Rust::Globals::CheatRunning)
			break;

		CheatStruct::TaggedObject::UpdateMatrix();

		UpdateTaggedObject();
		UpdateActiveObject();
	}
}

void Rust::EntityUpdator::AddNewTaggedObjects()
{
	while (Rust::Globals::game_state.inGame) { // Update Additional object in every 500ms

		if (!Rust::Globals::CheatRunning)
			break;

		try {
			std::vector<std::pair<uint64_t, Rust::ObjectTag>> NewTaggedObject;
			std::vector<std::shared_ptr<CheatStruct::TaggedObject>> NewTaggedObjectPtrs;
			std::unordered_set<uint64_t> KeyList;

			NewTaggedObject.reserve(1000);
			NewTaggedObjectPtrs.reserve(1000);
			KeyList.reserve(1000);

			bool LocalPlayerPassed = false;

			//Tagged Obeject
			for (uint64_t CurrentTaggedObject = Rust::GOM::GetTaggedObject();
				CurrentTaggedObject != Rust::GOM::GetLastTaggedObject();
				CurrentTaggedObject = Rust::GOM::GetNextTaggedObject(CurrentTaggedObject)) {

				auto gameobject = Rust::Globals::hack_data.RustMemory->Read<uint64_t>(CurrentTaggedObject + 0x10);
				auto tag = (Rust::ObjectTag)Rust::Globals::hack_data.RustMemory->Read<uint16_t>(gameobject + 0x5C);

				//exclude local player
				if (!LocalPlayerPassed && tag == Rust::ObjectTag::PLAYER) {
					char tagname[100] = { 0 };
					Rust::Globals::hack_data.RustMemory->ReadFromChainRaw(tagname, sizeof(tagname), CurrentTaggedObject, { 0x10, 0x68, 0x0 });

					if (!strcmp(tagname, "LocalPlayer")) {
						Rust::Globals::hack_data.LocalPlayer.UpdateEntityAddress(gameobject);
						LocalPlayerPassed = true;
						continue;
					}
				}

				switch (tag) {
				case Rust::ObjectTag::PLAYER:
				case Rust::ObjectTag::CORPSE:
				case Rust::ObjectTag::ANIMAL:
				case Rust::ObjectTag::SKYDOME:
				case Rust::ObjectTag::MONUMENT:
				//case Rust::ObjectTag::MISC:
				//case Rust::ObjectTag::TERRAIN:
				//case Rust::ObjectTag::RIVERMESH:

					if (tag == Rust::ObjectTag::PLAYER) {
						if (CheatStruct::Player::IsAddable(gameobject))
							NewTaggedObject.push_back(std::make_pair(gameobject, tag));
					}
					else {
						if (CheatStruct::TaggedObject::IsAddable(gameobject))
							NewTaggedObject.push_back(std::make_pair(gameobject, tag));
					}

					break;
				default:
					break;
				}
			}

			Rust::Globals::hack_data.TaggedObject.mutex.lock(); // add object;

			for (const auto& map : Globals::hack_data.TaggedObject.map)
				KeyList.insert(map.first);

			Rust::Globals::hack_data.TaggedObject.mutex.unlock(); // add object;

			for (const auto& Object : NewTaggedObject) {
				auto result = KeyList.find(Object.first);

				if (result == KeyList.end()) {
					std::shared_ptr<CheatStruct::TaggedObject> TaggedObject;

					if (Object.second == Rust::ObjectTag::PLAYER)
						TaggedObject = std::make_shared<CheatStruct::Player>();
					else
						TaggedObject = std::make_shared<CheatStruct::TaggedObject>();

					TaggedObject->Tag = Object.second;
					try {TaggedObject->UpdateOnlyOnce(Object.first);} 
					catch (Cheat::ExternalMemoryManager::MemException& ex) { continue;}
					NewTaggedObjectPtrs.push_back(TaggedObject);
				}
			}

			Rust::Globals::hack_data.TaggedObject.mutex.lock(); // add object;

			for(const auto& ptr : NewTaggedObjectPtrs)
				Globals::hack_data.TaggedObject.map.insert({ ptr->pGameObject, ptr });

			Rust::Globals::hack_data.TaggedObject.mutex.unlock(); // add object;

		}
		catch (Cheat::MemoryManager::MemException& ex) {
			continue;
		}
		catch (Cheat::cexception& ex) {
			MessageBoxA(NULL, ex.what(), "Exception", 0);
			return;
		}
	}
}

void Rust::EntityUpdator::AddNewActiveObjects()
{
	while (Rust::Globals::game_state.inGame) { // Update Additional object in every 500ms

		if (!Rust::Globals::CheatRunning)
			break;
		
		std::vector<std::pair<uint64_t, size_t>> NewActiveObjects;
		std::vector<std::shared_ptr<CheatStruct::ActiveObject>> NewActiveObjectPtrs;
		std::unordered_set<uint64_t> KeyList;

		NewActiveObjects.reserve(1000);
		NewActiveObjectPtrs.reserve(1000);
		KeyList.reserve(1000);

		try {
			for (auto CurrentActiveObject = Rust::GOM::GetActiveObject();
				CurrentActiveObject != Rust::GOM::GetLastActiveObject();
				CurrentActiveObject = Rust::GOM::GetNextActiveObject(CurrentActiveObject)) {

				auto gameobject = Rust::Globals::hack_data.RustMemory->Read<uint64_t>(CurrentActiveObject + 0x10);

				if (CheatStruct::ActiveObject::IsAddable(gameobject)) {
					size_t index = 0;
					if (CheatStruct::ActiveObject::FilterObjectName(gameobject, index))
						NewActiveObjects.push_back(std::make_pair(gameobject, index));
				}
			}

			Rust::Globals::hack_data.ActiveObjects.mutex.lock(); // add object;

			for (const auto& map : Globals::hack_data.ActiveObjects.map)
				KeyList.insert(map.first);

			Rust::Globals::hack_data.ActiveObjects.mutex.unlock(); // add object;

			for (const auto& Object : NewActiveObjects) {

				auto result = KeyList.find(Object.first);

				if (result == KeyList.end()) {
					std::shared_ptr<CheatStruct::ActiveObject> ActiveObject(new CheatStruct::ActiveObject);

					ActiveObject->InfoIndex = Object.second;
					try { ActiveObject->UpdateOnlyOnce(Object.first); }
					catch (Cheat::ExternalMemoryManager::MemException& ex) { continue; }

					NewActiveObjectPtrs.push_back(ActiveObject);
				}
			}

			Rust::Globals::hack_data.ActiveObjects.mutex.lock(); // add object;

			for (const auto& ptr : NewActiveObjectPtrs)
				Globals::hack_data.ActiveObjects.map.insert({ ptr->pGameObject, ptr });

			Rust::Globals::hack_data.ActiveObjects.mutex.unlock(); // add object;

		}
		catch (Cheat::MemoryManager::MemException& ex) {
			continue;
		}
		catch (Cheat::cexception& ex) {
			MessageBoxA(NULL, ex.what(), "Exception", 0);
			return;
		}
	}
}

void Rust::EntityUpdator::UpdateLocalPlayerAndCameraData()
{
	bool CamUpdated = false;
	bool LocalPlayerUpdated = false;

	//uint64_t LastTaggedObject = Rust::GOM::GetLastTaggedObject(); 이렇게 하면 안 된다 왜냐하면 게임에서 lastTaggedObject가 list 에서 빠지게 된 다면 아래 코드는 무한푸를 돌게 된다. 그러므로 GetLastTaggedObject를 매번 새로 가져와서 비교해야한다.
	for (uint64_t CurrentTaggedObject = Rust::GOM::GetTaggedObject(); CurrentTaggedObject != Rust::GOM::GetLastTaggedObject(); CurrentTaggedObject = Rust::GOM::GetNextTaggedObject(CurrentTaggedObject)) {
		auto gameobject = Rust::Globals::hack_data.RustMemory->Read<uint64_t>(CurrentTaggedObject + 0x10);
		auto tag = (Rust::ObjectTag)Rust::Globals::hack_data.RustMemory->Read<unsigned short>(gameobject + 0x5C);

		if (tag == Rust::ObjectTag::MAINCAMERA) {
			Rust::Globals::hack_data.MainCam.UpdateEntityAddress(gameobject);
			CamUpdated = true;
		}
			
		else if (tag == Rust::ObjectTag::PLAYER) {
			char tagname[100] = { 0 };
			Rust::Globals::hack_data.RustMemory->ReadFromChainRaw(tagname, sizeof(tagname), gameobject, { 0x68, 0x0 });

			if (!strcmp(tagname, "LocalPlayer")) {
				Rust::Globals::hack_data.LocalPlayer.UpdateEntityAddress(gameobject);
				LocalPlayerUpdated = true;
			}
		}

		if (CamUpdated && LocalPlayerUpdated)
			break;
	}
}