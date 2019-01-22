#include "Aimbot.h"
#include "Globals.h"
#include "Helpers.h"


Rust::Aimbot::Aimbot()
	: m_TargetExist(false)
{
	Rust::Globals::hack_setting.Aimbot.enable = true;
	Rust::Globals::hack_setting.Aimbot.prediction = true;
	Rust::Globals::hack_setting.Aimbot.fov = 400.f;
}


Rust::Aimbot::~Aimbot()
{
}

void Rust::Aimbot::exec()
{
	bool buttonPressed = (GetAsyncKeyState(VK_XBUTTON2)) && 0x8000; //VK_XBUTTON1 -> mouse back button

	if (buttonPressed) {
		if(!m_TargetExist)
			if (!FindTarget())
				return;

		if (!Rust::CheatStruct::Player::isAlive(m_TargetData.pOwnClassObject)) {
			m_TargetExist = false;
			return;
		}

		auto TargetHeadPos = Rust::MainCam::GetPosition(Rust::Globals::hack_data.RustMemory->ReadFromChain<uint64_t>(m_TargetData.pOwnClassObject, {0x80, 0x28, 0x10 }));
		auto LocalHeadPos = Rust::MainCam::GetPosition(Rust::Globals::hack_data.RustMemory->ReadFromChain<uint64_t>(Rust::Globals::hack_data.LocalPlayer.pOwnClassObject, { 0x80, 0x28, 0x10 }));
		auto AngleAddress = Rust::Globals::hack_data.RustMemory->Read<uint64_t>(Rust::Globals::hack_data.LocalPlayer.pOwnClassObject + 0x470);
		auto OriginalAngle = Rust::Globals::hack_data.RustMemory->Read<Cheat::Vector2>(AngleAddress + 0x44);

		if (Rust::Globals::hack_setting.Aimbot.prediction)
			Apply_Predicition(TargetHeadPos);

		auto Offset = CalcAngle(LocalHeadPos, TargetHeadPos) - OriginalAngle;
		SmoothAim(Offset, 2);
		Normalize(Offset.y, Offset.x);

		auto AngleToAim = OriginalAngle + Offset;
		Rust::Globals::hack_data.RustMemory->Write(AngleToAim, AngleAddress + 0x44);
	}
	else {
		m_TargetExist = false;
	}
}

bool Rust::Aimbot::FindTarget()
{
	float CurrentNearDistance = 10000.f;
	Cheat::Vector2 ScreenMiddle = { Rust::Globals::system_data.width/2.f, Rust::Globals::system_data.height / 2.f }; // rect 좌표로 하지 말 것 왜냐하면 world2screen 값은 오프셋이기 때문

	Rust::CheatStruct::Player* pTarget = NULL;
	for (const auto& PlayerObject : Rust::Globals::hack_data.TaggedObject.map) {
		if (!PlayerObject.second->Usable)
			continue;

		auto player = (Rust::CheatStruct::Player*)(PlayerObject.second.get());

		float distance = player->ScreenHeadPos.distance(ScreenMiddle);
		if (distance < Rust::Globals::hack_setting.Aimbot.fov && distance < CurrentNearDistance) {
			CurrentNearDistance = distance;
			pTarget = player;
		}
	}

	if (pTarget) {
		m_TargetExist = true;
		m_TargetData.pCoreObject = pTarget->pCoreObject;
		m_TargetData.pGameObject = pTarget->pGameObject;
		m_TargetData.pVisuaState = pTarget->pVisuaState;
		m_TargetData.pOwnClassObject = pTarget->pOwnClassObject;
		return true;
	}

	return false;
}

void Rust::Aimbot::Apply_Predicition(Cheat::Vector3 & position)
{
}

Cheat::Vector2 Rust::Aimbot::CalcAngle(const Cheat::Vector3 & LocalPos, const Cheat::Vector3 & EnemyPos)
{
	Cheat::Vector3 dir = LocalPos - EnemyPos;

	float Yaw = to_degree(-atan2(dir.x, -dir.z));
	float Pitch = to_degree(asin(dir.y / dir.Length()));

	return { Pitch, Yaw };// 공식이 YAW ROLL PITCH 순임
}
