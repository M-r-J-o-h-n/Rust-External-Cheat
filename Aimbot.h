#pragma once
#include <cstdint>
#include "Vector.h"
#include "CheatStructs.h"

namespace Rust {
	class Aimbot
	{
	public:
		Aimbot();
		~Aimbot();

		void exec();

		struct setting {
			bool enable;
			bool prediction;
			float fov;
		};

	private:
		bool m_TargetExist;
		CheatStruct::EntityAddresses m_TargetData;
		void Apply_Predicition(Cheat::Vector3& position);
		Cheat::Vector2 CalcAngle(const Cheat::Vector3& LocalPos, const Cheat::Vector3& EnemyPos);
		bool FindTarget();


		float to_radian(float degree)
		{
			return degree * 3.141592f / 180f;
		}

		float to_degree(float radian)
		{
			return radian * 180f / 3.141592f;
		}
		
		void Normalize(float& Yaw, float& Pitch) { // OnlyDegree // �� �϶� pitch �� ������ �Ʒ��϶� pitch + �� yaw �� -360~360
			if (Pitch < -89)
				Pitch = -89;

			else if (Pitch > 89)
				Pitch = 89;

			if (Yaw < -360)
				Yaw += 360;

			else if (Yaw > 360)
				Yaw -= 360;
		}

		void SmoothAim(Cheat::Vector2& Angle, float smooth) {
			Angle /= smooth;
		}
	};
}


