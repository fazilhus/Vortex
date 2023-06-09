#pragma once

#include "Vortex.hpp"
#include "Panels/SceneHierarchyPanel.hpp"
#include "Vortex/Renderer/Cameras/EditorCamera.hpp"

namespace Vortex {

	class EditorLayer : public Layer {
	private:
		Ref<FrameBuffer> m_frameBuffer;

		glm::vec2 m_viewportPanelSize;

		bool m_viewportFocused, m_viewportHovered;

		Vector<Ref<Scene>> m_scenes;
		Ref<Scene> m_currentScene;

		EditorCamera m_editorCamera;

		// Panels
		SceneHierarchyPanel m_sceneHierarchyPanel;
		Timestep m_frametime;

		int m_gizmoType;

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

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	};

}