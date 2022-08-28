#pragma once

#include "Jerboa/Core/Events/WindowResizeEvent.h"
#include "Jerboa/Core/EventObserver.h"

#include "Jerboa/Rendering/Renderer.h"

namespace Jerboa {
	class Mesh;
	class GL_RenderState;

	class GL_Renderer : public Renderer
	{
	public:
		GL_Renderer(GL_RenderState* renderStateGL);

		void Clear() override;
		void Draw(Mesh& mesh) override;
	private:
		void OnWindowResize(const WindowResizeEvent& evnt);

		GL_RenderState* m_RenderStateGL;

		EventObserver m_EventObserver;
	};
}