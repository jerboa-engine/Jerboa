#pragma once

#include "Jerboa/Debug.h"
#include "Jerboa/Core/Layer.h"
 
#include "Jerboa/Core/Events/WindowResizeEvent.h"

namespace JerboaClient {
	class EditorLayer : public Jerboa::Layer
	{
	public:
		EditorLayer();

		virtual void OnUpdate() override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
	private:
		void OnWindowResize(const Jerboa::WindowResizeEvent& evnt);

		Jerboa::EventObserver mWindowResizeObserver;
	};
}

