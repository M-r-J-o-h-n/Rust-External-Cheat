#pragma once
#include <cstdint>
#include "Vector.h"
#include "GameStructs.h"
#include <unordered_map>
#include <vector>
#include <mutex>

namespace Rust {
	template<typename type>
	struct SyncData {
		std::mutex mutex;
		std::unordered_map<uint64_t, type> map; // uint64_t is gameobject address;
	};

	namespace CheatStruct {

		class EntityAddresses {
		public:
			virtual void UpdateEntityAddress(uint64_t gameobject);
			uint64_t pGameObject;
			uint64_t pCoreObject;
			uint64_t pVisuaState;
			uint64_t pOwnClassObject;
		};

		class TaggedObject : public EntityAddresses {
		public:
			static bool IsAddable(uint64_t gameobject);

			static void UpdateMatrix();
			virtual bool IsUpdatable();
			virtual bool IsDeletable();
			virtual void UpdateData();
			virtual void UpdateOnlyOnce(uint64_t gameobject);

			bool Usable;
			char tagname[50];
			Rust::Layers Layer;
			Rust::ObjectTag Tag;
			Cheat::Vector3 Pos;
			Cheat::Vector2 ScreenPos;
			unsigned int Distance;
		protected:
			static uint32_t GetDistance(uint64_t VisuaState);
			static bool isBehind(uint64_t visualState);
			static Rust::Layers GetLayer(uint64_t gameobject);
			static bool IsActive(uint64_t gameobject);
			static DirectX::XMFLOAT4X4 ms_ViewProjClipMatrix;
		};

		class ActiveObject : public TaggedObject {
		public:
			static bool IsAddable(uint64_t gameobject);

			static bool FilterObjectName(uint64_t gameobject, size_t& strindex);

			virtual bool IsUpdatable();
			virtual bool IsDeletable();

			virtual void UpdateData();
			virtual void UpdateOnlyOnce(uint64_t gameobject);

			size_t InfoIndex;

			struct Info {
				enum class Type {
					collectables = 0,
					ores,
					food,
					construction,
					loot,
					animals,
				};

				Info(Type type = Type::loot, const char* idname = NULL, const char* displayname = NULL)
				{
					if(idname)
						strcpy_s(IDName, idname);

					if(displayname)
						strcpy_s(DispalyName, displayname);

					this->type = type;
				}

				Type type;
				char IDName[50] = { 0 };
				char DispalyName[50] = { 0 };
			};
		};

		class Player : public TaggedObject {
		public:
			static bool IsAddable(uint64_t gameobject);

			virtual bool IsUpdatable();
			virtual bool IsDeletable();

			static uint64_t GetPlayerFlag(uint64_t BasePlayer);
			static uint32_t GetPlayerLifeState(uint64_t BasePlayer);
			static uint32_t GetPlayerHealth(uint64_t BasePlayer);
			static bool isVisible(uint64_t BasePlayer);
			static bool isAlive(uint64_t BasePlayer);
			static uint64_t GetUserID(uint64_t BasePlayer);

			virtual void UpdateData();
			virtual void UpdateOnlyOnce(uint64_t gameobject); // call this only after UpdateEntityAddress is called
			virtual void UpdateEntityAddress(uint64_t gameobject);

			bool hasThisFlag(Rust::PlayerFlags flag);

			unsigned int Health;
			wchar_t Name[50];
			uint64_t Playerflag;
			uint64_t UserID;
			uint64_t pInput;
			uint64_t pInventory;
			bool visible;
			Cheat::Vector3 HeadPos;
			Cheat::Vector2 ScreenHeadPos;
		};

		namespace GameNames {
			extern std::vector<ActiveObject::Info> ActiveObjectName;
			extern std::vector<const wchar_t*> WeaponName;
		}
	}
}
