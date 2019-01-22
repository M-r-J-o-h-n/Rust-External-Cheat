#include "Visual.h"
#include "Globals.h"
#include "Vector.h"
#include <string>

using DirectX::XMFLOAT4X4;
using Cheat::Vector2;
using Cheat::Vector4;

Rust::Visual::Visual()
	:m_renderer(Globals::system_data.hOverlay, Globals::system_data.width, Globals::system_data.height)
{

	Rust::Globals::hack_setting.Visual.Radar = true;
	Rust::Globals::hack_setting.Visual.PlayerWeaponInfo = true;

	setting::ObjectSetting player;
	player.color = D2D1::ColorF::Enum::Green;
	player.distance = 250.f;
	player.enable = true;

	setting::ObjectSetting corpse;
	corpse.color = D2D1::ColorF::Enum::LightSkyBlue;
	corpse.distance = 50.f;
	corpse.enable = true;

	Rust::Globals::hack_setting.Visual.TaggedObjectSettingInfo[Rust::ObjectTag::PLAYER] = player;
	Rust::Globals::hack_setting.Visual.TaggedObjectSettingInfo[Rust::ObjectTag::CORPSE] = corpse;

	setting::ObjectSetting collectables;
	collectables.color = D2D1::ColorF::Enum::LightCoral;
	collectables.distance = 150.f;
	collectables.enable = true;

	setting::ObjectSetting ores;
	ores.color = D2D1::ColorF::Enum::LightCyan;
	ores.distance = 150.f;
	ores.enable = true;

	setting::ObjectSetting foods;
	foods.color = D2D1::ColorF::Enum::LightYellow;
	foods.distance = 50.f;
	foods.enable = true;

	setting::ObjectSetting constructions;
	constructions.color = D2D1::ColorF::Enum::LightGray;
	constructions.distance = 50.f;
	constructions.enable = true;

	setting::ObjectSetting loot;
	loot.color = D2D1::ColorF::Enum::LightSkyBlue;
	loot.distance = 150.f;
	loot.enable = true;

	setting::ObjectSetting animals;
	animals.color = D2D1::ColorF::Enum::Lime;
	animals.distance = 100.f;
	animals.enable = true;

	Rust::Globals::hack_setting.Visual.ActiveObjectSettingInfo[Rust::CheatStruct::ActiveObject::Info::Type::animals] = animals;
	Rust::Globals::hack_setting.Visual.ActiveObjectSettingInfo[Rust::CheatStruct::ActiveObject::Info::Type::collectables] = collectables;
	Rust::Globals::hack_setting.Visual.ActiveObjectSettingInfo[Rust::CheatStruct::ActiveObject::Info::Type::construction] = constructions;
	Rust::Globals::hack_setting.Visual.ActiveObjectSettingInfo[Rust::CheatStruct::ActiveObject::Info::Type::loot] = loot;
	Rust::Globals::hack_setting.Visual.ActiveObjectSettingInfo[Rust::CheatStruct::ActiveObject::Info::Type::food] = foods;
	Rust::Globals::hack_setting.Visual.ActiveObjectSettingInfo[Rust::CheatStruct::ActiveObject::Info::Type::ores] = ores;


	Rust::Globals::hack_setting.Visual.TAGGED_OBJECT_CATCH_DISTANCE = 300.f;
	Rust::Globals::hack_setting.Visual.ACTIVE_OBJECT_CATCH_DISTANCE = 250.f;
}


Rust::Visual::~Visual()
{

}

void Rust::Visual::BeginDraw()
{
	m_renderer.Begin();
}

void Rust::Visual::EndDraw()
{
	m_renderer.End();
}

void Rust::Visual::DrawOtherVisuals()
{
	DrawCorsshair(D2D1::ColorF::Enum::AntiqueWhite);
}

void Rust::Visual::DrawTaggedObject()
{
	for (const auto& Object : Rust::Globals::hack_data.TaggedObject.map) {
		if (!Object.second->Usable)
			continue;

		if (Object.second->Tag == Rust::ObjectTag::PLAYER) {
			auto player = (Rust::CheatStruct::Player*)(Object.second.get());

			float BoxHeight = player->ScreenPos.y - player->ScreenHeadPos.y;
			float BoxWidth = BoxHeight / 1.5f;

			if (BoxHeight < 5.f) {
				BoxWidth = 5.f;
				BoxHeight = 10.f;
			}

			Vector4 Border;
			Border.x = player->ScreenPos.x - (BoxWidth / 2);
			Border.y = player->ScreenHeadPos.y;
			Border.z = Border.x + BoxWidth;
			Border.w = Border.y + BoxHeight;

			if (player->visible)
				Draw2DBox(Border, D2D1::ColorF::Enum::Red);
			else
				Draw2DBox(Border, Rust::Globals::hack_setting.Visual.TaggedObjectSettingInfo[Rust::ObjectTag::PLAYER].color);

			DrawHealthBar(player->Health, Border);
			DrawNameAndDistance(player->Name, std::to_wstring(player->Distance).c_str(), { player->ScreenPos.x, player->ScreenPos.y + 15.f });
			DrawPlayerWeapon();
			DrawRadar();
		}
		else {
			auto object = (Rust::CheatStruct::TaggedObject*)(Object.second.get());
			std::string aStr;

			if (object->Tag == Rust::ObjectTag::CORPSE)
				std::string aStr = "corpse";
			else
				std::string aStr = object->tagname;
			

			std::wstring wtagname(aStr.begin(), aStr.end());
			DrawNameAndDistance(wtagname.c_str(), std::to_wstring(object->Distance).c_str(), { object->ScreenPos.x, object->ScreenPos.y },
				Rust::Globals::hack_setting.Visual.TaggedObjectSettingInfo[object->Tag].color);
		}
	}
}

void Rust::Visual::DrawActiveObject()
{
	for (const auto& Object : Rust::Globals::hack_data.ActiveObjects.map) {
		if (!Object.second->Usable)
			continue;

		auto object = (Rust::CheatStruct::ActiveObject*)(Object.second.get());
		auto type = Rust::CheatStruct::GameNames::ActiveObjectName[object->InfoIndex].type;

		std::string aStr = Rust::CheatStruct::GameNames::ActiveObjectName[object->InfoIndex].DispalyName;
		std::wstring wtagname(aStr.begin(), aStr.end());

		DrawNameAndDistance(wtagname.c_str(), std::to_wstring(object->Distance).c_str(), { object->ScreenPos.x, object->ScreenPos.y }, 
			Rust::Globals::hack_setting.Visual.ActiveObjectSettingInfo[type].color);
	}
}

void Rust::Visual::Draw2DBox(const Cheat::Vector4 & Border, D2D1::ColorF::Enum color)
{
	m_renderer.DrawRoundedRect(Border, 3.0f, color, 1.0f, true, D2D1::ColorF::Enum::Black, 0.3f);
}

void Rust::Visual::DrawNameAndDistance(const wchar_t * name, const wchar_t * distance, const Cheat::Vector2 & pos, D2D1::ColorF::Enum color)
{
	std::wstring NameAndDistance = name;
	std::wstring Distance = distance;
	NameAndDistance += L"[" + Distance + L"m]";
	m_renderer.DrawString(NameAndDistance.c_str(), pos, color);
}

void Rust::Visual::DrawHealthBar(int health, const Cheat::Vector4 & Border)
{
	float BoxHeight = Border.w - Border.y;
	float BoxWIidth = Border.z - Border.x;

	float HealthBarHeight = 0.f;

	HealthBarHeight = (BoxHeight / 100.f) * health;

	Vector4 BoxRect;
	Vector4 BoxBorder;
	float thickness = 5.0f;

	BoxRect.x = Border.x - thickness - 3.f;
	BoxRect.y = Border.w - HealthBarHeight;
	BoxRect.z = Border.x - 3.f;
	BoxRect.w = Border.w;

	BoxBorder.x = Border.x - thickness - 3.f;
	BoxBorder.y = Border.y;
	BoxBorder.z = Border.x - 3.f;
	BoxBorder.w = Border.w;

	m_renderer.DrawRect(BoxBorder, 0.0000001f, D2D1::ColorF::Enum::Black, 1.0f, true, D2D1::ColorF::Enum::Black, 1.0f);
	m_renderer.DrawRect(BoxRect, 0.0000001f, D2D1::ColorF::Enum::Green, 1.0f, true, D2D1::ColorF::Enum::Green, 1.0f);
}

void Rust::Visual::DrawCorsshair(D2D1::ColorF::Enum color)
{
	float thickness = 1.2f;
	float length = 13.f;
	float width = Rust::Globals::system_data.width/2.f;
	float height = Rust::Globals::system_data.height/2.f;

	Cheat::Vector4 H = {
		width - length,
		height - thickness,
		width + length,
		height + thickness
	};
	Cheat::Vector4 V = {
		width - thickness,
		height - length,
		width + thickness,
		height + length
	};

	m_renderer.DrawRect(H, 0.0000001f, color, 1.0f, true, color, 1.0f);
	m_renderer.DrawRect(V, 0.0000001f, color, 1.0f, true, color, 1.0f);
}

void Rust::Visual::DrawPlayerWeapon()
{
}

void Rust::Visual::DrawRadar()
{
}