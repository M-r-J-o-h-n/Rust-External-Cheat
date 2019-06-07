#include "Misc.h"
#include "Globals.h"
#include "GameStructs.h"
Rust::Misc::Misc()
{
	Rust::Globals::hack_setting.Misc.RecoilSpread.enable = true;
	Rust::Globals::hack_setting.Misc.RecoilSpread.changed = true;
	Rust::Globals::hack_setting.Misc.RecoilSpread.multiplier = 0.2f;

	Rust::Globals::hack_setting.Misc.VelocityScale.enable = true;
	Rust::Globals::hack_setting.Misc.VelocityScale.changed = true;
	Rust::Globals::hack_setting.Misc.VelocityScale.multiplier = 100f;

	Rust::Globals::hack_setting.Misc.FastGather.enable = true;
	Rust::Globals::hack_setting.Misc.FastGather.changed = true;
	Rust::Globals::hack_setting.Misc.FastGather.multiplier = 0.85f;
}


Rust::Misc::~Misc()
{
}

void Rust::Misc::exec()
{
	ModifyItems();
}

void Rust::Misc::ModifyItems()
{
	auto ItemList = Rust::Globals::hack_data.RustMemory->ReadFromChain<uint64_t>(
		Rust::Globals::hack_data.LocalPlayer.pOwnClassObject, 
		{0x0458, 0x28, 0x20});

	unsigned int itemCount = Rust::Globals::hack_data.RustMemory->Read<uint32_t>(ItemList + 0x18);
	uint64_t listArray = Rust::Globals::hack_data.RustMemory->Read<uint64_t>(ItemList + 0x10) + 0x20; // array in system.list object

	//Recoil and Spread Reduction 

	//Category »Æ¿Œ weapon! 
	for (unsigned i = 0; i < itemCount; ++i) {
		uint64_t item = Rust::Globals::hack_data.RustMemory->Read<uint64_t>(listArray + sizeof(void*)*i);

		//Set deploy speed  ; patched
		//Rust::Globals::hack_data.RustMemory->WriteFromChain<float>(0.5f, item, { 0x58, 0x1A0 });

		//Set repeat delay  ; patched
		//Rust::Globals::hack_data.RustMemory->WriteFromChain<float>(0.5f, item, { 0x58, 0x1A0 });

		//check if already modded
		auto result = m_ModdedWeaponList.find(item);
		if (result != m_ModdedWeaponList.end())
			continue;

		//////////////////////Get active item num
		Rust::ItemCategory ItemCategory = Rust::Globals::hack_data.RustMemory->ReadFromChain<Rust::ItemCategory>(item,
			{ 0x10, 0xBC });

		//only change recoil of specific weapons
		if (ItemCategory == Rust::ItemCategory::Weapon) {
			/*
			bool found = false;
			for (const wchar_t* name : Rust::CheatStruct::GameNames::WeaponName) {
				if (!wcscmp(name, weaponName)) {
					found = true;
					break;
				}
			}

			if (!found)
				continue;
			*/

			uint64_t RecoilProperties = Rust::Globals::hack_data.RustMemory->ReadFromChain<uint64_t>(item,
				{ 0x58, 0x240 });

			float RecoilSpread[6] = { 0 };
			Rust::Globals::hack_data.RustMemory->ReadRaw(RecoilSpread, (void*)(RecoilProperties + 0x28), sizeof(RecoilSpread));

			WeaponRecoilInfo recoil;
			recoil.recoilYawMin = RecoilSpread[0];
			recoil.recoilYawMax = RecoilSpread[1];
			recoil.recoilPitchMin = RecoilSpread[2];
			recoil.recoilPitchMax = RecoilSpread[3];

			m_ModdedWeaponList[item] = recoil;

			//no spread
			RecoilSpread[0] = 0f;
			RecoilSpread[1] = 0f;
			//x0.2 recoil
			RecoilSpread[2] *= 0.2f;
			RecoilSpread[3] *= 0.2f;
			//movement penalty;
			RecoilSpread[5] = 0f;

			Rust::Globals::hack_data.RustMemory->WriteRaw(RecoilSpread, (void*)(RecoilProperties + 0x28), sizeof(RecoilSpread));
		}

	}

}