#include "Panels/ContentBrowserPanel.hpp"

#include <imgui/imgui.h>

namespace Vortex {

	extern const std::filesystem::path g_assetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel() : m_currentDir(g_assetPath) {
		m_fileIcon = Texture2D::Create("res/icons/ContentBrowser/file-icon.png");
		m_dirIcon = Texture2D::Create("res/icons/ContentBrowser/folder-icon.png");
	}

	void ContentBrowserPanel::OnImGuiRender() {
		ImGui::Begin("Content Browser");

		if (m_currentDir != std::filesystem::path(g_assetPath)) {
			if (ImGui::Button("<-")) {
				m_currentDir = m_currentDir.parent_path();
			}
		}

		static float padding = 12.0f;
		static float thumbnailSize = 48.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1) columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& dirEntry : std::filesystem::directory_iterator(m_currentDir)) {
			const auto& path = dirEntry.path();
			auto relPath = std::filesystem::relative(path, g_assetPath);
			std::string filename = relPath.filename().string();

			ImGui::PushID(filename.c_str());
			Ref<Texture2D> icon = dirEntry.is_directory() ? m_dirIcon : m_fileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource()) {
				const wchar_t* itemPath = relPath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (dirEntry.is_directory()) {
					m_currentDir /= path.filename();
				}
			}

			ImGui::TextWrapped(filename.c_str());
			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}

}
