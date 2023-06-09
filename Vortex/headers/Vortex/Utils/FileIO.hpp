#pragma once

#include <string>

namespace Vortex {

	class FileIO {
	public:
		static std::optional<std::string> OpenFile(const char* filter);
		static std::optional<std::string> SaveFile(const char* filter);
	};

}