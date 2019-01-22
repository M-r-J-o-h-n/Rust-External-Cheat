#include "GameStructs.h"
#include "Globals.h"

Rust::GameObject::GameObject(uint64_t GameObject)
{
	m_data = new Internal_GameObject;
	Rust::Globals::hack_data.RustMemory->ReadRaw(m_data, (void*)GameObject, sizeof(Internal_GameObject));
}

Rust::GameObject::~GameObject()
{
	delete m_data;
}

Rust::BasePlayer::BasePlayer(uint64_t BasePlayer)
{
	m_data = new Internal_BasePlayer;
	Rust::Globals::hack_data.RustMemory->ReadRaw(m_data, (void*)BasePlayer, sizeof(Internal_BasePlayer));
}

Rust::BasePlayer::~BasePlayer()
{
	delete m_data;
}
