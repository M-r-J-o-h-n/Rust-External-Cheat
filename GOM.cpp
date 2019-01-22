#include "GOM.h"
#include "Globals.h"

using Rust::Globals;

uint64_t Rust::GOM::GetTaggedObject()
{
	uint64_t GameObjectManager = Globals::hack_data.RustMemory->Read<DWORD64>(Globals::hack_data.RustMemory->GetBaseAddress() + (uintptr_t)0x144FE40);
	return Globals::hack_data.RustMemory->Read<uint64_t>(GameObjectManager + 0x8);
}

uint64_t Rust::GOM::GetLastTaggedObject()
{
	uint64_t GameObjectManager = Globals::hack_data.RustMemory->Read<DWORD64>(Globals::hack_data.RustMemory->GetBaseAddress() + (uintptr_t)0x144FE40);
	return Globals::hack_data.RustMemory->Read<uint64_t>(GameObjectManager);
}

uint64_t Rust::GOM::GetNextTaggedObject(uint64_t CurrentTaggedObejct)
{
	return Globals::hack_data.RustMemory->Read<uint64_t>(CurrentTaggedObejct + 0x8);
}

uint64_t Rust::GOM::GetActiveObject()
{
	uint64_t GameObjectManager = Globals::hack_data.RustMemory->Read<DWORD64>(Globals::hack_data.RustMemory->GetBaseAddress() + (uintptr_t)0x144FE40);
	return Globals::hack_data.RustMemory->Read<uint64_t>(GameObjectManager + 0x18);
}

uint64_t Rust::GOM::GetLastActiveObject()
{
	uint64_t GameObjectManager = Globals::hack_data.RustMemory->Read<DWORD64>(Globals::hack_data.RustMemory->GetBaseAddress() + (uintptr_t)0x144FE40);
	return Globals::hack_data.RustMemory->Read<uint64_t>(GameObjectManager + 0x10);
}

uint64_t Rust::GOM::GetNextActiveObject(uint64_t CurrentActiveObject)
{
	return Globals::hack_data.RustMemory->Read<uint64_t>(CurrentActiveObject + 0x8);
}