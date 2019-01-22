#pragma once
#include "Vector.h"
#include <cstdint>


namespace Rust {
	class MainCam {
	public:
		static bool WorldToScreen(const DirectX::XMFLOAT4X4& ViewProjMatrix, const Cheat::Vector3& worldpos, Cheat::Vector2& screenpos, int width, int height);
		static Cheat::Vector3 GetPosition(uint64_t pTransform);
		static DirectX::XMFLOAT4X4 GetViweMatrix();
		static DirectX::XMFLOAT4X4 GetViewProjClipMatrix();
		static float GetFov();

	private:

		struct TransformAccessReadOnly
		{
			uint64_t pTransformData;
		};

		struct TransformData
		{
			uint64_t pTransformArray;
			uint64_t pTransformIndices;
		};

		struct Matrix34
		{
			Cheat::Vector4 vec0;
			Cheat::Vector4 vec1;
			Cheat::Vector4 vec2;
		};
	};
}
