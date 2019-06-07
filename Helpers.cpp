#include "Helpers.h"
#include "Globals.h"

using namespace DirectX;

bool Rust::MainCam::WorldToScreen(const XMFLOAT4X4 & ViewProjMatrix, const Cheat::Vector3 & worldpos, Cheat::Vector2 & screenpos, int width, int height)
{
	const XMFLOAT4X4& matrix = ViewProjMatrix;

	XMVECTOR trans = XMLoadFloat4(&XMFLOAT4(matrix._41, matrix._42, matrix._43, 0.0f));
	XMVECTOR up = XMLoadFloat4(&XMFLOAT4(matrix._21, matrix._22, matrix._23, 0.0f));
	XMVECTOR right = XMLoadFloat4(&XMFLOAT4(matrix._11, matrix._12, matrix._13, 0.0f));

	XMVECTOR pos = XMLoadFloat4(&(XMFLOAT4)worldpos);
	float depth = XMVectorGetX(XMVector4Dot(trans, pos)) + matrix._44;

	if (depth < 0.098f)
		return false;

	float XXXX = XMVectorGetX(XMVector4Dot(right, pos)) + matrix._14;
	float YYYY = XMVectorGetX(XMVector4Dot(up, pos)) + matrix._24;

	screenpos.x = (int)(width / 2) * (1f + XXXX / depth);
	screenpos.y = (int)(height / 2) * (1f - YYYY / depth);

	return true;
}
/*
bool Rust::Helpers::WorldToScreen(const XMFLOAT4X4 & ViewProjMatrix, const Cheat::Vector3 & worldpos, Cheat::Vector2 & screenpos, int width, int height, float fovX)
{
	//XMMATRIX VPM = XMMatrixTranspose(XMLoadFloat4x4(&ViewProjMatrix));
	//XMFLOAT4X4 matrix; XMStoreFloat4x4(&matrix, VPM);
	const XMFLOAT4X4& matrix = ViewProjMatrix;

	XMVECTOR trans = XMLoadFloat4(&XMFLOAT4(matrix._41, matrix._42, matrix._43, 0.0f));
	XMVECTOR up = XMLoadFloat4(&XMFLOAT4(matrix._21, matrix._22, matrix._23, 0.0f));
	XMVECTOR right = XMLoadFloat4(&XMFLOAT4(matrix._11, matrix._12, matrix._13, 0.0f));

	XMVECTOR pos = XMLoadFloat4(&(XMFLOAT4)worldpos);
	float depth = XMVectorGetX(XMVector4Dot(trans, pos)) + matrix._44;

	if (depth < 0.098f)
		return false;

	float XXXX = XMVectorGetX(XMVector4Dot(right, pos)) + matrix._14;
	float YYYY = XMVectorGetX(XMVector4Dot(up, pos)) + matrix._24;

	float fovXrad = fovX * 3.141592f / 180f;
	float fovYrad = fovXrad * height / width;

	screenpos.x = (int)(width / 2) * (1f + XXXX  / depth);
	screenpos.y = (int)(height / 2) * (1f - YYYY / depth);

	return true;
}
bool Rust::Helpers::WorldToScreenViewMatrix(const DirectX::XMFLOAT4X4 & ViewMatrix, const Cheat::Vector3 & worldpos, Cheat::Vector2 & screenpos, int width, int height, float fovX)
{
	//XMMATRIX VPM = XMMatrixTranspose(XMLoadFloat4x4(&ViewProjMatrix));
	//XMFLOAT4X4 matrix; XMStoreFloat4x4(&matrix, VPM);
	const XMFLOAT4X4& matrix = ViewMatrix;

	XMVECTOR trans = XMLoadFloat4(&XMFLOAT4(matrix._41, matrix._42, matrix._43, matrix._44));
	XMVECTOR up = XMLoadFloat4(&XMFLOAT4(matrix._21, matrix._22, matrix._23, matrix._24));
	XMVECTOR right = XMLoadFloat4(&XMFLOAT4(matrix._11, matrix._12, matrix._13, matrix._14));

	XMVECTOR pos = XMLoadFloat4(&(XMFLOAT4)worldpos);
	float depth = XMVectorGetX(XMVector4Dot(trans, pos));

	if (depth < 0.098f)
		return false;

	float XXXX = XMVectorGetX(XMVector4Dot(right, pos));
	float YYYY = XMVectorGetX(XMVector4Dot(up, pos));

	float fovXrad = fovX * 3.141592f / 180f;
	float fovYrad = fovXrad * height / width;

	screenpos.x = (int)(width / 2) * (1f + XXXX / fovXrad / depth);
	screenpos.y = (int)(height / 2) * (1f - YYYY / fovYrad / depth);

	return true;



	return false;
}
bool Rust::Helpers::WorldToScreenTest(const DirectX::XMFLOAT4X4 & worldtoClipSpace, const Cheat::Vector3 & worldpos, Cheat::Vector2 & screenpos, int width, int height)
{
	XMVECTOR worldPOS = XMLoadFloat4(&(XMFLOAT4)worldpos);
	XMMATRIX viewproj = XMLoadFloat4x4(&viewProj);
	XMVECTOR screenVec = DirectX::XMVector3Project(worldPOS, 0, 0, width, height,
		0.1f, 3000f, DirectX::XMMatrixIdentity(), viewproj, DirectX::XMMatrixIdentity());

	if (DirectX::XMVectorGetZ(screenVec) < 1f)
		return false;

	DirectX::XMStoreFloat2((XMFLOAT2*)(&screenpos), screenVec);

	return true;
}

*/
Cheat::Vector3 Rust::MainCam::GetPosition(uint64_t pTransform)
{
	__m128 result;

	const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
	const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
	const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

	TransformAccessReadOnly pTransformAccessReadOnly = Rust::Globals::hack_data.RustMemory->Read<TransformAccessReadOnly>(pTransform + 0x38);
	unsigned int index = Rust::Globals::hack_data.RustMemory->Read<unsigned int>(pTransform + 0x40);
	TransformData transformData = Rust::Globals::hack_data.RustMemory->Read<TransformData>(pTransformAccessReadOnly.pTransformData + 0x18);

	SIZE_T sizeMatriciesBuf = sizeof(Matrix34) * index + sizeof(Matrix34);
	SIZE_T sizeIndicesBuf = sizeof(int) * index + sizeof(int);

	// Allocate memory for storing large amounts of data (matricies and indicies)
	PVOID pMatriciesBuf = malloc(sizeMatriciesBuf);
	PVOID pIndicesBuf = malloc(sizeIndicesBuf);

	if (pMatriciesBuf && pIndicesBuf)
	{
		// Read Matricies array into the buffer
		Rust::Globals::hack_data.RustMemory->ReadRaw(pMatriciesBuf, (void*)transformData.pTransformArray, sizeMatriciesBuf);
		// Read Indices array into the buffer
		Rust::Globals::hack_data.RustMemory->ReadRaw(pIndicesBuf, (void*)transformData.pTransformIndices, sizeIndicesBuf);

		result = *(__m128*)((ULONGLONG)pMatriciesBuf + 0x30 * index);
		int transformIndex = *(int*)((ULONGLONG)pIndicesBuf + 0x4 * index);

		while (transformIndex >= 0)
		{
			Matrix34 matrix34 = *(Matrix34*)((ULONGLONG)pMatriciesBuf + 0x30 * transformIndex);

			__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x00));	// xxxx
			__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x55));	// yyyy
			__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x8E));	// zwxy
			__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xDB));	// wzyw
			__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xAA));	// zzzz
			__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x71));	// yxwy
			__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

			result = _mm_add_ps(
				_mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
						tmp7)), *(__m128*)(&matrix34.vec0));

			transformIndex = *(int*)((ULONGLONG)pIndicesBuf + 0x4 * transformIndex);
		}

		free(pMatriciesBuf);
		free(pIndicesBuf);
	}

	return Cheat::Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
}

DirectX::XMFLOAT4X4 Rust::MainCam::GetViweMatrix()
{
	XMFLOAT4X4 ViewMatrix;
	Globals::hack_data.RustMemory->ReadRaw(&ViewMatrix, (void*)(Globals::hack_data.MainCam.pOwnClassObject + 0xC0), sizeof(ViewMatrix));
	XMStoreFloat4x4(&ViewMatrix, XMMatrixTranspose(XMLoadFloat4x4(&ViewMatrix)));
	return ViewMatrix;
}

DirectX::XMFLOAT4X4 Rust::MainCam::GetViewProjClipMatrix()
{
	XMFLOAT4X4 ViewProjClipMatrix;
	Globals::hack_data.RustMemory->ReadRaw(&ViewProjClipMatrix, (void*)(Globals::hack_data.MainCam.pOwnClassObject + 0xC0), sizeof(ViewProjClipMatrix));
	XMStoreFloat4x4(&ViewProjClipMatrix, XMMatrixTranspose(XMLoadFloat4x4(&ViewProjClipMatrix)));
	return ViewProjClipMatrix;
}

float Rust::MainCam::GetFov()
{
	return Globals::hack_data.RustMemory->Read<float>(Globals::hack_data.MainCam.pOwnClassObject + 0x140);
}


template <typename T>
T SafeRead(T* Data)
{
	if (Data != nullptr)
		return *Data;
}

template <typename T>
bool SafeReadToBuffer(T* Data, T* Buffer, size_t Size)
{
	if (Data != nullptr && Buffer != nullptr && Size != 0)
	{
		memcpy(Buffer, Data, Size);
		return true;
	}

	return false;
}
/*
D3DXVECTOR3 GetBoneByID(BaseEntity* Entity, int Bone)
{
	auto BoneInfo = Entity->ModelState->SkinnedMultiMesh->BoneDictionary->BoneInfo;

	auto BoneValue = *(CBoneValue**)((uintptr_t)BoneInfo + 0x30 + ((Bone - 1) * 0x18));

	return (GetPosition(BoneValue->Transform));
}
*/