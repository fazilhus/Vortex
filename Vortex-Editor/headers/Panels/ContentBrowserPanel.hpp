#pragma once

#include <filesystem>
#include "Vortex/Core/Core.hpp"
#include "Vortex/Renderer/Texture.hpp"

namespace Vortex {

	class ContentBrowserPanel {
	private:
		std::filesystem::path m_currentDir;

		Ref<Texture2D> m_fileIcon;
		Ref<Texture2D> m_dirIcon;

	public:
		ContentBrowserPanel();
		~ContentBrowserPanel() = default;

		void OnImGuiRender();
	};

}
