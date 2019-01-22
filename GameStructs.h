#pragma once
#include <cstdint>
#include "Vector.h"

namespace Rust {
	struct LastObjectBase;
	struct BaseObject;
	struct GameObject;
	struct CoreObject;
	struct Transform;
	struct VisualState;

	class GameObjectManager {
	public:
		LastObjectBase* lastTaggedObject; //0x0000 
		BaseObject* taggedObjects; //0x0008
		LastObjectBase* lastActiveObject; //0x0010 
		BaseObject* activeObjects; //0x0018
	};

	class LastObjectBase
	{
	public:
		char pad_0x0000[0x10]; //0x0000
		GameObject* lastObject; //0x0010 
	};

	class BaseObject
	{
	public:
		char pad_0x0000[0x8]; //0x0000
		BaseObject* nextObjectLink; //0x0008 
		GameObject* object; //0x0010 
	};

	class GameObject
	{
	public:
		struct Internal_GameObject;

		GameObject(uint64_t GameObject);
		~GameObject();

		Internal_GameObject* m_data;

		struct Internal_GameObject {
			char pad_0000[24]; //0x0000
			uint32_t InstanceID; //0x0018
			class N000000EF* link; //0x001C
			char pad_0024[12]; //0x0024
			class N00000095* pCoreObject; //0x0030
			char pad_0038[8]; //0x0038
			uint32_t label; //0x0040
			char pad_0044[4]; //0x0044
			uint32_t size; //0x0048
			char pad_004C[4]; //0x004C
			uint32_t capacity; //0x0050
			char pad_0054[4]; //0x0054
			uint32_t layer; //0x0058
			int16_t tag; //0x005C
			bool active; //0x005E
			bool activeCached; //0x005F
			bool destroying; //0x0060
			bool activating; //0x0061
			uint32_t N000000BA; //0x0062
			uint16_t N000000C3; //0x0066
			char* objectname; //0x0068
			char pad_0070[1072]; //0x0070
		}; //Size: 0x04A0

	};

	class CoreObject // GameObject + 0x30
	{
	public:
		char pad0[0x8];
		uint64_t Transform;  // 0x8
		char pad1[0x8];
		uint64_t OwnClass; // 0x18  
	};

	class Transform {
	public:
		char pad0[0x38];
		VisualState* VisuaState; // 0x38
	};

	class VisualState {
	public:
		char pad_0x0000[0xB0]; //0x0000 // 0x90 head
		Cheat::Vector3 position; //0x00B0
		Cheat::Vector4 rotation; //0x00BC //qeuaternino
	};

	class Model {
	public:
		char pad[0x18];
		uint64_t collision; //UnityEngine.SphereCollider
		uint64_t rootBone; //unityengine.transform  -> this is penis not feet
		uint64_t headBone; //unityengine.transform -> this actually is CoreObject pointer so to get transform you have to deref, headbone+0x10] = transform
		uint64_t eyeBone; //unityengine.transform 
		uint64_t animator; //unityengine.animator
		uint64_t boneTransform; // unityengine.transform
		uint64_t boneNames; //system.string
	};

	class BasePlayer   // ObjectsCore + 0x18 -> 0x28 = BasePlayer  :::: BasePlayer->BaseComabatEntity->BaseEntity
	{
	public:
		struct Internal_BasePlayer;

		BasePlayer(uint64_t BasePlayer);
		~BasePlayer();

		Internal_BasePlayer* m_data;

		struct Internal_BasePlayer {
			char pad_0000[0x80]; //0x0000
			uint64_t Model; // 0x0080
			char pad_0001[0x78];
			uint64_t ParentBone; //0x0100
			uint64_t SkinID; //0x0108
			char pad_0110[8]; //0x0110
			uint32_t broadcastProtocol; //0x0118
			bool linkedToNeighbours; //0x011C
			bool isVisible; //0x011D
			bool isAnimatorVisible; //0x011E
			bool isShadowVisible; //0x011F
			char pad_0120[152]; //0x0120
			uint32_t StartHealth; //0x01B8
			uint32_t ShowHealthInfo; //0x01BC
			uint32_t LifeState; //0x01C0
			char pad_01C4[4]; //0x01C4
			float Health; //0x01C8
			uint32_t MaxHealth; //0x01CC
			char pad_01D0[640]; //0x01D0
			uint64_t pEyes; //0x0450
			uint64_t pInventory; //0x0458
			char pad_0460[0x10]; //0x0460
			uint64_t pInput; //0x470
			uint64_t pMovement; //0x0478
			uint64_t pCollision; //0x0480
			char pad_0488[24]; //0x0488
			uint64_t pUserIDString; //0x04A0
			uint64_t pDisplayName; //0x04A8
			char pad_04B0[32]; //0x04B0
			uint64_t pLookingAt; //0x04D0
			uint64_t pLookingAtEntity; //0x04D8
			uint64_t pLookingAtCollider; //0x04E0
			uint64_t pLookingAtTest; //0x04E8
			uint64_t pClientTeam; //0x04F0
			char pad_04F8[40]; //0x04F8
			uint64_t PlayerFlags; //0x0520
			uint64_t UserID; //0x0528
			char pad_0530[24]; //0x0530
			Cheat::Vector3 pLookingAtPoint; //0x0548
			char pad_0554[104]; //0x0554
			uint64_t DebugPrevVisible; //0x05BC
			char pad_05C4[664]; //0x05C4
		};//Size: 0x085C
	}; 

	class Camera  //OwnClass /
	{
	public:
		char pad_0x0000[0x40]; //0x0000
		Cheat::Vector4 view_matrix; //0x00C0 
		float height_scale; //0x0080 
		char pad_0x0084[0x10]; //0x0084
		float width_scale; //0x0094 
		char pad_0x0098[0x10]; //0x0098
		__int16 unkn1; //0x00A8 
		char pad_0x00AA[0xE]; //0x00AA
		float unkn2; //0x00B8 
		char pad_0x00BC[0x4]; //0x00BC
		Cheat::Vector4 ViewProj_matrix; //0x00C0 
		char pad_0x0100[0x40]; //0x0100
		float FOV; //0x0140 
	};

	/* playerinput
				20 : state (type: InputState)
				30 : hadInputBuffer (type: System.Boolean)
				34 : bodyRotation (type: UnityEngine.Quaternion)
				44 : bodyAngles (type: UnityEngine.Vector3)
				50 : headRotation (type: UnityEngine.Quaternion)
				60 : headAngles (type: UnityEngine.Vector3)
				6c : recoilAngles (type: UnityEngine.Vector3)
				78 : viewDelta (type: UnityEngine.Vector2)
				80 : headDelta (type: UnityEngine.Vector3)
				8c : mouseWheelUp (type: System.Int32)
				90 : mouseWheelDn (type: System.Int32)
				94 : mouseX (type: System.Single)
				98 : mouseY (type: System.Single)
				28 : trackir (type: TrackIR)
				9c : offsetAngles (type: UnityEngine.Vector3)
				a8 : ignoredButtons (type: System.Int32)
				ac : hasKeyFocus (type: System.Boolean)
	*/

	enum class TraitFlag
	{
		None = 0,
		Alive = 1,
		Animal = 2,
		Human = 4,
		Interesting = 8,
		Food = 16, // 0x00000010
		Meat = 32, // 0x00000020
		Water = Meat, // 0x00000020
	};

	enum class ItemCategory
	{
		Weapon = 0,
		Construction,
		Items,
		Resources,
		Attire,
		Tool, // µ¹ Æ÷ÇÔ
		Medical,
		Food,
		Ammunition,
		Traps,
		Misc,
		All,
		Common,
		Component,
		Search,
	};

	enum class Layers : int
	{
		kDefaultLayer = 0,
		kNoFXLayer = 1,
		kIgnoreRaycastLayer = 2,
		kIgnoreCollisionLayer = 3,
		kWaterLayer = 4,
		kUILayer = 5,
		kDeployed = 8,//repair bench/research table/tool cupboard/etc
		kRagdoll = 9,//supply signal/heli crates
		kInvisible = 10,
		kAI = 11,
		kPlayerMovement = 12,
		kSkyReflection = 13,
		kSky = 15,
		kWorld = 16,
		kServerPlayer = 17,
		kTrigger = 18,
		kViewModel = 20,
		kConstruction = 21,
		kTerrain = 23,
		kDebris = 26,//dropped items
		kDisplacement = 27,
		kTree = 30
	};

	enum class ObjectTag : uint16_t
	{
		MAINCAMERA = 5,
		PLAYER = 6,
		TERRAIN = 20001,
		CORPSE = 20009, //Animals & Players
		MISC = 20006, //Trash cans, Sleeping Bags, Storage Box, etc
		ANIMAL = 20008,
		SKYDOME = 20011,
		RIVERMESH = 20014,
		MONUMENT = 20015 //Airport, Powerplant, etc
	};

	enum class Bone_List : int {
		pelvis = 1,
		l_hip,
		l_knee,
		l_foot,
		l_toe,
		l_ankle_scale,
		penis,
		GenitalCensor,
		GenitalCensor_LOD0,
		Inner_LOD0,
		GenitalCensor_LOD1,
		GenitalCensor_LOD2,
		r_hip,
		r_knee,
		r_foot,
		r_toe,
		r_ankle_scale,
		spine1,
		spine1_scale,
		spine2,
		spine3,
		spine4,
		l_clavicle,
		l_upperarm,
		l_forearm,
		l_hand,
		l_index1,
		l_index2,
		l_index3,
		l_little1,
		l_little2,
		l_little3,
		l_middle1,
		l_middle2,
		l_middle3,
		l_prop,
		l_ring1,
		l_ring2,
		l_ring3,
		l_thumb1,
		l_thumb2,
		l_thumb3,
		IKtarget_righthand_min,
		IKtarget_righthand_max,
		l_ulna,
		neck,
		head,
		jaw,
		eyeTranform,
		l_eye,
		l_Eyelid,
		r_eye,
		r_Eyelid,
		r_clavicle,
		r_upperarm,
		r_forearm,
		r_hand,
		r_index1,
		r_index2,
		r_index3,
		r_little1,
		r_little2,
		r_little3,
		r_middle1,
		r_middle2,
		r_middle3,
		r_prop,
		r_ring1,
		r_ring2,
		r_ring3,
		r_thumb1,
		r_thumb2,
		r_thumb3,
		IKtarget_lefthand_min,
		IKtarget_lefthand_max,
		r_ulna,
		l_breast,
		r_breast,
		BoobCensor,
		BreastCensor_LOD0,
		BreastCensor_LOD1,
		BreastCensor_LOD2,
		collision,
		displacement,
	};

	enum class PlayerFlags
	{
		Unused1 = 1,
		Unused2 = 2,
		IsAdmin = 4,
		ReceivingSnapshot = 8,
		Sleeping = 16, // 0x00000010
		Spectating = 32, // 0x00000020
		Wounded = 64, // 0x00000040
		IsDeveloper = 128, // 0x00000080
		Connected = 256, // 0x00000100
		VoiceMuted = 512, // 0x00000200
		ThirdPersonViewmode = 1024, // 0x00000400
		EyesViewmode = 2048, // 0x00000800
		ChatMute = 4096, // 0x00001000
		NoSprint = 8192, // 0x00002000
		Aiming = 16384, // 0x00004000
		DisplaySash = 32768, // 0x00008000
		Relaxed = 65536, // 0x00010000
		SafeZone = 131072, // 0x00020000
		Workbench1 = 1048576, // 0x00100000
		Workbench2 = 2097152, // 0x00200000
		Workbench3 = 4194304, // 0x00400000
	};

	enum class LifeState
	{
		Alive,
		Dead,
	};

	class UnicodeString
	{
	public:
		char pad_0x0000[0x10]; //0x0000
		__int32 size; //0x0010 
		wchar_t str; //0x0014 
	};
}

