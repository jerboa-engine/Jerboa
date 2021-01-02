#include "jerboa-pch.h"
#include "ImGuiApp.h"

namespace Jerboa::UI {
	void ImGuiApp::Initialize(Window* window) {
		static bool initialized = false;
		JERBOA_ASSERT(!initialized, "ImGuiApp::Initialize() should only be called once");
		if (initialized)
			return;
		initialized = true;

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // TODO: Enable at a later point
		
		auto glfwWindow = static_cast<GLFWwindow*>(window->GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void ImGuiApp::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiApp::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}