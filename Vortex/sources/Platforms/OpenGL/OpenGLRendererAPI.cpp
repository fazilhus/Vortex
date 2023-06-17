#include <vtpch.hpp>
#include "Platforms/OpenGL/OpenGLRendererAPI.hpp"

#include <glad/glad.h>

namespace Vortex {
	void OpenGLMessageCallback(uint4 source, uint4 type, uint4 id, uint4 severity, int len, const char* msg, const void* params) {
		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH: VT_CORE_CRITICAL(msg); return;
			case GL_DEBUG_SEVERITY_MEDIUM: VT_CORE_ERROR(msg); return;
			case GL_DEBUG_SEVERITY_LOW: VT_CORE_WARN(msg); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: VT_CORE_TRACE(msg); return;
			default: VT_CORE_ASSERT(false, "Unknown severity level"); return;
		}
	}

	void OpenGLRendererAPI::Init(const RendererConfig& cfg) {

#ifdef VT_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		if (cfg.enableBlending) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendEquation(GL_FUNC_ADD);
		}
		else {
			glDisable(GL_BLEND);
		}

		if (cfg.enableDepthTest) {
			glEnable(GL_DEPTH_TEST);
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}

		if (cfg.enableLineSmooth) {
			glEnable(GL_LINE_SMOOTH);
		}
		else {
			glDisable(GL_LINE_SMOOTH);
		}
	}

	void OpenGLRendererAPI::SetViewport(uint4 x, uint4 y, uint4 width, uint4 height) {
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
		VT_PROFILE_FUNC();
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear() {
		VT_PROFILE_FUNC();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& va, uint32 count) {
		VT_PROFILE_FUNC();
		va->Bind();
		auto c = count ? count : va->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, c, GL_UNSIGNED_INT, nullptr);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& va, uint32 vertexCount) {
		va->Bind();
		glDrawArrays(GL_LINE, 0, vertexCount);
	}

	void OpenGLRendererAPI::SetLineWidth(float width) {
		glLineWidth(width);
	}

}
