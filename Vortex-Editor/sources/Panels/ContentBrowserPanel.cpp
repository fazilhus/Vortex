#include "Panels/ContentBrowserPanel.hpp"

#include <imgui/imgui.h>

namespace Vortex {

	static const std::filesystem::path s_assetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel() : m_currentDir(s_assetPath) {
		m_fileIcon = Texture2D::Create("res/icons/ContentBrowser/file-icon.png");
		m_dirIcon = Texture2D::Create("res/icons/ContentBrowser/folder-icon.png");
	}

	void ContentBrowserPanel::OnImGuiRender() {
		ImGui::Begin("Content Browser");

		if (m_currentDir != std::filesystem::path(s_assetPath)) {
			if (ImGui::Button("<-")) {
				m_currentDir = m_currentDir.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1) columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& dirEntry : std::filesystem::directory_iterator(m_currentDir)) {
			const auto& path = dirEntry.path();
			auto relPath = std::filesystem::relative(path, s_assetPath);
			std::string filename = relPath.filename().string();

			Ref<Texture2D> icon = dirEntry.is_directory() ? m_dirIcon : m_fileIcon;
			ImGui::ImageButton((ImTextureID)icon->GetID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (dirEntry.is_directory()) {
					m_currentDir /= path.filename();
				}
			}

			ImGui::TextWrapped(filename.c_str());
			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}

}
