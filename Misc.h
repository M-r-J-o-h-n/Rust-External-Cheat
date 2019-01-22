#pragma once
#include <cstdint>
#include <unordered_map>

namespace Rust {
	struct WeaponRecoilInfo {
		float recoilYawMin;
		float recoilYawMax;
		float recoilPitchMin;
		float recoilPitchMax;
	};

	class Misc
	{
	public:
		Misc();
		~Misc();

		void exec();
		
		struct setting {
			struct ModificationInfo {
				bool enable;
				bool changed;
				bool multiplier;
			};

			ModificationInfo RecoilSpread;
			ModificationInfo VelocityScale;
			ModificationInfo FastGather;
		};
	private:
		void ModifyItems();
		std::unordered_map<uint64_t, WeaponRecoilInfo> m_ModdedWeaponList; // pItem -> uint64_t
	};

}

