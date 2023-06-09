#pragma once

#include "Vortex.hpp"
#include "Panels/SceneHierarchyPanel.hpp"

namespace Vortex {

	class EditorLayer : public Layer {
	private:
		/*OrthoCameraController m_cameraController;

		Ref<VertexArray> m_vao;
		Ref<Shader> m_shader;
		Ref<Texture2D> m_texture1;
		Ref<Texture2D> m_texture2;*/
		Ref<FrameBuffer> m_frameBuffer;

		glm::vec2 m_viewportPanelSize;

		bool m_viewportFocused, m_viewportHovered;

		Vector<Ref<Scene>> m_scenes;
		Ref<Scene> m_currentScene;

		Entity m_primaryCamera;
		Entity m_secondaryCamera;
		bool m_isPrimaryCamera = true;

		Entity m_square;

		// Panels
		SceneHierarchyPanel m_sceneHierarchyPanel;
		Timestep m_frametime;

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