#pragma once
#include "Renderer.h"
#include "Vector.h"
#include "CheatStructs.h"
#include <unordered_map>

namespace Rust {
	class Visual
	{
	public:
		Visual();
		~Visual();

		void BeginDraw();
		void EndDraw();
		void DrawOtherVisuals();
		void DrawTaggedObject();
		void DrawActiveObject();

		struct setting {
			struct ObjectSetting {
				bool enable;
				D2D1::ColorF::Enum color;
				float distance;
			};

			std::unordered_map<Rust::ObjectTag, ObjectSetting> TaggedObjectSettingInfo;
			std::unordered_map<Rust::CheatStruct::ActiveObject::Info::Type, ObjectSetting> ActiveObjectSettingInfo;

			bool Radar;
			bool PlayerWeaponInfo;

			float TAGGED_OBJECT_CATCH_DISTANCE;
			float ACTIVE_OBJECT_CATCH_DISTANCE;
		};
	private:
		Cheat::Renderer m_renderer;

		void Draw2DBox(const Cheat::Vector4& Border, D2D1::ColorF::Enum color = D2D1::ColorF::Enum::Red);
		void DrawNameAndDistance(const wchar_t * name, const wchar_t * distance, const Cheat::Vector2 & pos, D2D1::ColorF::Enum color = D2D1::ColorF::Enum::White);
		void DrawHealthBar(int health, const Cheat::Vector4& Border);
		void DrawCorsshair(D2D1::ColorF::Enum color = D2D1::ColorF::Enum::Gray);
		void DrawPlayerWeapon();
		void DrawRadar();
	};
}
