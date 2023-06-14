#pragma once

#include <filesystem>

namespace Vortex {

	class ContentBrowserPanel {
	private:
		std::filesystem::path m_currentDir;

	public:
		ContentBrowserPanel();
		~ContentBrowserPanel() = default;

		void OnImGuiRender();
	};

}
