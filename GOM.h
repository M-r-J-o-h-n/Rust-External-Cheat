#pragma once
#include <cstdint>

namespace Rust {
	class GOM {
	public:
		static uint64_t GetTaggedObject();
		static uint64_t GetLastTaggedObject();
		static uint64_t GetNextTaggedObject(uint64_t CurrentTaggedObejct);
		static uint64_t GetActiveObject();
		static uint64_t GetLastActiveObject();
		static uint64_t GetNextActiveObject(uint64_t CurrentActiveObject);
	};
};
