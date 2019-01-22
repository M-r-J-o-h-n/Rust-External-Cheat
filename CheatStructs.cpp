#include "CheatStructs.h"
#include "GameStructs.h"
#include "Globals.h"
#include "Helpers.h"

using namespace Rust;
using namespace Rust::CheatStruct;
using namespace DirectX;

DirectX::XMFLOAT4X4 Rust::CheatStruct::TaggedObject::ms_ViewProjClipMatrix;

bool Rust::CheatStruct::Player::IsAddable(uint64_t gameobject)
{
	auto BasePlayer = Rust::Globals::hack_data.RustMemory->ReadFromChain<uint64_t>(gameobject, { 0x30, 0x18, 0x28 });

	if (Rust::CheatStruct::Player::GetPlayerLifeState(BasePlayer) == (uint32_t)Rust::LifeState::Dead)
		return false;


	return TaggedObject::IsAddable(gameobject);
}

bool Rust::CheatStruct::Player::IsUpdatable()
{
	return TaggedObject::IsUpdatable();
}

bool Rust::CheatStruct::Player::IsDeletable()
{
	if (Rust::CheatStruct::Player::GetPlayerLifeState(pOwnClassObject) == (uint32_t)Rust::LifeState::Dead)
		return true;

	return TaggedObject::IsDeletable();
}

uint64_t Rust::CheatStruct::Player::GetPlayerFlag(uint64_t BasePlayer)
{
	return Globals::hack_data.RustMemory->Read<uint64_t>(BasePlayer + 0x0520);
}

uint32_t Rust::CheatStruct::Player::GetPlayerLifeState(uint64_t BasePlayer)
{
	return Globals::hack_data.RustMemory->Read<uint32_t>(BasePlayer + 0x01C0);
}

unsigned int Rust::CheatStruct::Player::GetPlayerHealth(uint64_t BasePlayer)
{
	return Globals::hack_data.RustMemory->Read<float>(BasePlayer + 0x01C8);
}

bool Rust::CheatStruct::Player::isVisible(uint64_t BasePlayer)
{
	return Globals::hack_data.RustMemory->Read<float>(BasePlayer + 0x011D);
}

bool Rust::CheatStruct::Player::isAlive(uint64_t BasePlayer)
{
	return Globals::hack_data.RustMemory->Read<float>(BasePlayer + 0x01C0) == (uint32_t)Rust::LifeState::Alive;
}

uint64_t Rust::CheatStruct::Player::GetUserID(uint64_t BasePlayer)
{
	return Globals::hack_data.RustMemory->Read<float>(BasePlayer + 0x0528);
}

bool Rust::CheatStruct::Player::hasThisFlag(Rust::PlayerFlags flag)
{
	return ((this->Playerflag && (uint64_t)flag) == (uint64_t)flag);
}

void Rust::CheatStruct::Player::UpdateData()
{
	TaggedObject::UpdateData();

	//BasePlayer stuff
	Rust::BasePlayer BasePlayer(pOwnClassObject);
	HeadPos = Rust::MainCam::GetPosition(Rust::Globals::hack_data.RustMemory->ReadFromChain<uint64_t>(BasePlayer.m_data->Model, {0x28, 0x10 }));
	Health = BasePlayer.m_data->Health;
	Playerflag = BasePlayer.m_data->PlayerFlags;
	UserID = BasePlayer.m_data->UserID;
	visible = BasePlayer.m_data->isVisible;
	pInput = BasePlayer.m_data->pInput;
	pInventory = BasePlayer.m_data->pInventory;

	//update screen datas
	Rust::MainCam::WorldToScreen(ms_ViewProjClipMatrix, HeadPos, ScreenHeadPos, Rust::Globals::system_data.width, Rust::Globals::system_data.height);
}

void Rust::CheatStruct::Player::UpdateOnlyOnce(uint64_t gameobject)
{
	UpdateEntityAddress(gameobject);

	Tag = Rust::ObjectTag::PLAYER;

	//Get name only once
	unsigned nameLength = Rust::Globals::hack_data.RustMemory->ReadFromChain<unsigned>(pOwnClassObject, { 0x4A8, 0x10 });	ZeroMemory(Name, sizeof(Name));
	Rust::Globals::hack_data.RustMemory->ReadFromChainRaw(Name, nameLength * sizeof(wchar_t), pOwnClassObject, { 0x4A8, 0x14 });
}

void Rust::CheatStruct::Player::UpdateEntityAddress(uint64_t gameobject)
{
	EntityAddresses::UpdateEntityAddress(gameobject);
	pOwnClassObject = Rust::Globals::hack_data.RustMemory->Read<uint64_t>(pOwnClassObject + 0x28);
}

void Rust::CheatStruct::EntityAddresses::UpdateEntityAddress(uint64_t gameobject)
{
	pGameObject = gameobject;
	pCoreObject = Rust::Globals::hack_data.RustMemory->ReadFromChain<uint64_t>(pGameObject, { 0x30 });
	pVisuaState = Rust::Globals::hack_data.RustMemory->ReadFromChain<uint64_t>(pCoreObject, { 0x8, 0x38 });
	pOwnClassObject = Rust::Globals::hack_data.RustMemory->ReadFromChain<uint64_t>(pCoreObject, { 0x18 });
}

bool Rust::CheatStruct::TaggedObject::IsAddable(uint64_t gameobject)
{
	auto VisuaState = Rust::Globals::hack_data.RustMemory->ReadFromChain<uint64_t>(gameobject, { 0x30, 0x8, 0x38 });

	if (Rust::CheatStruct::Player::GetDistance(VisuaState) > Rust::Globals::hack_setting.Visual.TAGGED_OBJECT_CATCH_DISTANCE)
		return false;

	return true;
}

void Rust::CheatStruct::TaggedObject::UpdateMatrix()
{
	try
	{
		Globals::hack_data.RustMemory->ReadRaw(&ms_ViewProjClipMatrix, (void*)(Globals::hack_data.MainCam.pOwnClassObject + 0xC0), sizeof(ms_ViewProjClipMatrix));
		XMStoreFloat4x4(&ms_ViewProjClipMatrix, XMMatrixTranspose(XMLoadFloat4x4(&ms_ViewProjClipMatrix)));
	}
	catch (Cheat::ExternalMemoryManager::MemException& ex) {
		return;
	}
}

bool Rust::CheatStruct::TaggedObject::IsUpdatable()
{
	Usable = false;

	float distance = Rust::CheatStruct::ActiveObject::GetDistance(pVisuaState);

	auto object = Rust::Globals::hack_setting.Visual.TaggedObjectSettingInfo.find(Tag);

	if (object != Rust::Globals::hack_setting.Visual.TaggedObjectSettingInfo.end()) {
		if (object->second.enable) {
			if (distance > object->second.distance)
				return false;
		}
		else
			return false;
	}
	else
		return false;

	if (Rust::CheatStruct::TaggedObject::isBehind(pVisuaState))
		return false;

	Usable = true;
	return true;
}

bool Rust::CheatStruct::TaggedObject::IsDeletable()
{
	if (Rust::CheatStruct::TaggedObject::GetDistance(pVisuaState) > Rust::Globals::hack_setting.Visual.TAGGED_OBJECT_CATCH_DISTANCE)
		return true;
	else
		return false;
}

void Rust::CheatStruct::TaggedObject::UpdateData()
{
	//distance
	Cheat::Vector3 localpos = Globals::hack_data.RustMemory->Read<Cheat::Vector3>(Globals::hack_data.LocalPlayer.pVisuaState + 0xB0);
	Distance = Pos.Distance(localpos);
	//pos and screen position
	Pos = Rust::Globals::hack_data.RustMemory->Read<Cheat::Vector3>(pVisuaState + 0xb0);
	Rust::MainCam::WorldToScreen(ms_ViewProjClipMatrix, Pos, ScreenPos, Rust::Globals::system_data.width, Rust::Globals::system_data.height);
}

void Rust::CheatStruct::TaggedObject::UpdateOnlyOnce(uint64_t gameobject)
{
	EntityAddresses::UpdateEntityAddress(gameobject);

	ZeroMemory(tagname, sizeof(pGameObject));
	Rust::Globals::hack_data.RustMemory->ReadFromChainRaw(tagname, sizeof(tagname), pGameObject, {0x68, 0x0});

	Layer = Rust::Globals::hack_data.RustMemory->Read<Rust::Layers>(pGameObject + 0x58);
	Tag = (Rust::ObjectTag)Rust::Globals::hack_data.RustMemory->Read<uint16_t>(gameobject + 0x5C);
}


uint32_t Rust::CheatStruct::TaggedObject::GetDistance(uint64_t VisuaState)
{
	auto pos = Globals::hack_data.RustMemory->Read<Cheat::Vector3>(VisuaState + 0xB0);
	auto localpos = Globals::hack_data.RustMemory->Read<Cheat::Vector3>(Globals::hack_data.LocalPlayer.pVisuaState + 0xB0);
	return pos.Distance(localpos);
}

bool Rust::CheatStruct::TaggedObject::isBehind(uint64_t visualState)
{
	auto pos = Globals::hack_data.RustMemory->Read<Cheat::Vector3>(visualState + 0xB0);
	Cheat::Vector2 ScreenPos;
	if (Rust::MainCam::WorldToScreen(ms_ViewProjClipMatrix, pos, ScreenPos, Rust::Globals::system_data.width, Rust::Globals::system_data.height))
		return false;
	else
		return true;
}

Rust::Layers Rust::CheatStruct::TaggedObject::GetLayer(uint64_t gameobject)
{
	return Rust::Globals::hack_data.RustMemory->Read<Rust::Layers>(gameobject + 0x58);
}

bool Rust::CheatStruct::TaggedObject::IsActive(uint64_t gameobject)
{
	return Globals::hack_data.RustMemory->Read<bool>(gameobject + 0x5E);
}

bool Rust::CheatStruct::ActiveObject::IsAddable(uint64_t gameobject)
{
	auto layer = Rust::CheatStruct::TaggedObject::GetLayer(gameobject);

	switch (layer) {
	case Rust::Layers::kConstruction:
	case Rust::Layers::kUILayer:
	case Rust::Layers::kNoFXLayer:
	case Rust::Layers::kTrigger:
	case Rust::Layers::kTree:
	case Rust::Layers::kInvisible:
		return false;

	default:
		break;
	}

	auto VisuaState = Rust::Globals::hack_data.RustMemory->ReadFromChain<uint64_t>(gameobject, { 0x30, 0x8, 0x38 });

	if (Rust::CheatStruct::ActiveObject::GetDistance(VisuaState) > Rust::Globals::hack_setting.Visual.ACTIVE_OBJECT_CATCH_DISTANCE)
		return false;

	return true;
}

void Rust::CheatStruct::ActiveObject::UpdateData()
{
	TaggedObject::UpdateData();
}

void Rust::CheatStruct::ActiveObject::UpdateOnlyOnce(uint64_t gameobject)
{
	EntityAddresses::UpdateEntityAddress(gameobject);
	Layer = Rust::Globals::hack_data.RustMemory->Read<Rust::Layers>(pGameObject + 0x58);
}

bool Rust::CheatStruct::ActiveObject::FilterObjectName(uint64_t gameobject, size_t & strindex)
{
	char tag_name[100] = { 0 };
	Rust::Globals::hack_data.RustMemory->ReadFromChainRaw(tag_name, sizeof(tag_name), gameobject, { 0x68, 0x0 });

	size_t index = 0;
	for (auto str : Rust::CheatStruct::GameNames::ActiveObjectName) {
		if (strstr(tag_name, str.IDName)) {
			strindex = index;
			return true;
		}
		++index;
	}
	
	return false;
}

bool Rust::CheatStruct::ActiveObject::IsUpdatable()
{
	Usable = false;

	float distance = Rust::CheatStruct::ActiveObject::GetDistance(pVisuaState);

	auto type = Rust::CheatStruct::GameNames::ActiveObjectName[InfoIndex].type;
	auto object = Rust::Globals::hack_setting.Visual.ActiveObjectSettingInfo.find(type);

	if (object != Rust::Globals::hack_setting.Visual.ActiveObjectSettingInfo.end()) {
		if (object->second.enable) {
			if (distance > object->second.distance)
				return false;
		}
		else
			return false;
	}
	else
		return false;

	if (Rust::CheatStruct::ActiveObject::isBehind(pVisuaState))
		return false;


	Usable = true;
	return true;
}

bool Rust::CheatStruct::ActiveObject::IsDeletable()
{
	if (!Rust::CheatStruct::ActiveObject::IsActive(pGameObject))
		return true;

	if (Rust::CheatStruct::TaggedObject::GetDistance(pVisuaState) > Rust::Globals::hack_setting.Visual.ACTIVE_OBJECT_CATCH_DISTANCE)
		return true;

	return false;
}

std::vector<ActiveObject::Info> GameNames::ActiveObjectName {
	ActiveObject::Info(ActiveObject::Info::Type::collectables, "tree_stump", "tree_stump"),
	ActiveObject::Info(ActiveObject::Info::Type::collectables, "wood-collectable", "wood"),
	ActiveObject::Info(ActiveObject::Info::Type::collectables, "metal-collectable", "metal"),
	ActiveObject::Info(ActiveObject::Info::Type::collectables, "stone-collectable", "stone"),
	ActiveObject::Info(ActiveObject::Info::Type::collectables, "sulfer-collectable", "sulfer"),
	ActiveObject::Info(ActiveObject::Info::Type::collectables, "hemp-collectable", "hemp"),
	ActiveObject::Info(ActiveObject::Info::Type::collectables, "hemp_mature", "hemp"),

	ActiveObject::Info(ActiveObject::Info::Type::food, "corn.entity", "corn"),
	ActiveObject::Info(ActiveObject::Info::Type::food, "pumpkin.entity", "pumpkin"),
	ActiveObject::Info(ActiveObject::Info::Type::food, "mushroom.entity", "mushroom"),

	ActiveObject::Info(ActiveObject::Info::Type::ores, "metal-ore", "metal_ore"),
	ActiveObject::Info(ActiveObject::Info::Type::ores, "sulfur-ore", "sulfur_ore"),
	ActiveObject::Info(ActiveObject::Info::Type::ores, "stone-ore", "stone_ore"),

	ActiveObject::Info(ActiveObject::Info::Type::construction, "woodbox", "woodbox"),
	ActiveObject::Info(ActiveObject::Info::Type::construction, "cupboard.tool.deployed", "cupboard"),
	ActiveObject::Info(ActiveObject::Info::Type::construction, "box.wooden.large", "large_woodbox"),
	ActiveObject::Info(ActiveObject::Info::Type::construction, "sleeping", "sleeping_bag"),

	ActiveObject::Info(ActiveObject::Info::Type::animals, "chicken", "woodbox"),
	ActiveObject::Info(ActiveObject::Info::Type::animals, "horse", "cupboard"),
	ActiveObject::Info(ActiveObject::Info::Type::animals, "boar", "large_woodbox"),
	ActiveObject::Info(ActiveObject::Info::Type::animals, "wolf", "sleeping_bag"),

	ActiveObject::Info(ActiveObject::Info::Type::loot, "supply_drop", "supply_drop"),
	ActiveObject::Info(ActiveObject::Info::Type::loot, "trash-pile", "trash_pile"),
	ActiveObject::Info(ActiveObject::Info::Type::loot, "crate_normal", "crate"),
	ActiveObject::Info(ActiveObject::Info::Type::loot, "loot_barrel", "barrel"),
};

std::vector<const wchar_t*> GameNames::WeaponName {
	L"pistol.python", L"bow.hunting",L"crossbow", L"pistol.m92", L"smg.mp5", L"smg.2", L"rifle.ak", L"lmg.m249",
	L"pistol.revolver", L"shotgun.pump", L"shotgun.waterpipe", L"smg.thompson", L"rifle.semiauto", L"pistol.semiauto",
	L"crossbow", L"rifle.bolt", L"rifle.lr300", L"shotgun.double", L"rocket.launcher", L"smg.thompson"
};
