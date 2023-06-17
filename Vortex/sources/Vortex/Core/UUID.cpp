#include "vtpch.hpp"

#include "Vortex/Core/UUID.hpp"

#include <random>

namespace Vortex {

	static std::random_device s_randomDevice;
	static std::mt19937_64 s_engine(s_randomDevice());
	static std::uniform_int_distribution<uint64> s_uniformDist;

	UUID::UUID() : m_uuid(s_uniformDist(s_engine)) {

	}

	UUID::UUID(uint64 uuid) : m_uuid(uuid) {

	}

}
