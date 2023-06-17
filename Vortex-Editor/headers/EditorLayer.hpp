#pragma once

#include "Vortex.hpp"

//Panels
#include "Panels/SceneHierarchyPanel.hpp"
#include "Panels/ContentBrowserPanel.hpp"

#include "Vortex/Renderer/Cameras/EditorCamera.hpp"

namespace Vortex {

	class EditorLayer : public Layer {
	private:
		Ref<FrameBuffer> m_frameBuffer;
		Ref<FrameBuffer> m_freezeFrame;

		glm::vec2 m_viewportPanelSize;
		glm::vec2 m_viewportBounds[2];

		bool m_viewportFocused, m_viewportHovered;

		Vector<Ref<Scene>> m_scenes;
		Ref<Scene> m_currentScene;
		Ref<Scene> m_editorScene;
		std::filesystem::path m_editorScenePath;

		Entity m_cameraEntity;

		EditorCamera m_editorCamera;

		Entity m_hoveredEntity;

		// Panels
		SceneHierarchyPanel m_sceneHierarchyPanel;
		ContentBrowserPanel m_contentBrowserPanel;
		Ref<Texture2D> m_playIcon, m_stopIcon, m_pauseIcon;

		Timestep m_frametime;

		int m_gizmoType;

		enum class SceneState {
			Edit = 0,
			Play = 1
		};

		SceneState m_sceneState;

		bool isPaused;


	public:
		EditorLayer();
		virtual ~EditorLayer() override = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void OnScenePlay();
		void OnSceneResume();
		void OnScenePause();
		void OnSceneStop();

		void OnDuplicateEntity();

		// Toolbar panel
		void UIToolbar();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);
	};

}
