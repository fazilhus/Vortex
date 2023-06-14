#include "Panels/ContentBrowserPanel.hpp"

#include <imgui/imgui.h>

namespace Vortex {

	static const std::filesystem::path s_assetPath = "res";

	ContentBrowserPanel::ContentBrowserPanel() : m_currentDir(s_assetPath) {

	}

	void ContentBrowserPanel::OnImGuiRender() {
		ImGui::Begin("Content Browser");

		if (m_currentDir != std::filesystem::path(s_assetPath)) {
			if (ImGui::Button("<-")) {
				m_currentDir = m_currentDir.parent_path();
			}
		}

		for (auto& dirEntry : std::filesystem::directory_iterator(m_currentDir)) {
			const auto& path = dirEntry.path();
			auto relPath = std::filesystem::relative(path, s_assetPath);
			std::string filename = relPath.filename().string();

			if (dirEntry.is_directory()) {
				if (ImGui::Button(filename.c_str())) {
					m_currentDir /= path.filename();
				}
			}
			else {
				if (ImGui::Button(filename.c_str())) {
					
				}
			}
		}

		ImGui::End();
	}

}
