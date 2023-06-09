#pragma once

#include <string>

namespace Vortex {

	class FileIO {
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

}