#pragma once

#include "Vortex/Core/Core.hpp"

namespace Vortex {

	class UUID {
	private:
		uint64 m_uuid;

	public:
		UUID();
		UUID(uint64 uuid);
		UUID(const UUID&) = default;
		~UUID() = default;

		operator uint64() const { return m_uuid; }
	
	};
}

namespace std {

	template<typename T> struct hash;
		
	template<>
	struct hash<Vortex::UUID> {
		size_t operator()(const Vortex::UUID& other) {
			return static_cast<uint64>(other);
		}
	};

}
