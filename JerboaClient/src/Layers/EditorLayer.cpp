#include "EditorLayer.h"
#include "imgui.h"

#include "Jerboa/UI/ImGui/ImGuiApp.h"

#include "Jerboa/Core/Input.h"

#include "Jerboa/Rendering/Renderer.h"

namespace JerboaClient {
	EditorLayer::EditorLayer()
		: mWindowResizeObserver(Jerboa::EventObserver::Create(GetSharedEventBus(), this, &EditorLayer::OnWindowResize)),
        mCamera(Jerboa::Camera(glm::vec3(-1, 0, 5), Jerboa::CameraType::Perspective, glm::radians(60.0)))
	{
    }

	void EditorLayer::OnImGuiRender()
	{
		ImGui::ShowDemoWindow();

		//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		ImGui::Begin("Window 1");
		ImGui::Button("Hello");
		ImGui::End();

		ImGui::Begin("Window 2");
		ImGui::Button("Hello");
		ImGui::End();
	}

	void EditorLayer::OnUpdate()
	{
        // TODO: Move timestamp calculations into engine
        static double lastTimeStamp = 0;
        auto timeStamp = glfwGetTime(); // TODO: Don't use glfw directly
        float deltaTime = timeStamp - lastTimeStamp;
        lastTimeStamp = timeStamp;

        auto& trans = mCamera.GetTransform();

        auto mouseMovement = Jerboa::Input::GetMouseMovement();
        auto rotation = mouseMovement * deltaTime * 100.0f;
        auto ori = glm::quat(-Jerboa::Transform::GetWorldUp() * rotation.x) * trans.GetOrientation();
        ori = ori * glm::quat(-Jerboa::Transform::GetWorldRight() * rotation.y);
        trans.SetOrientation(ori);

        float moveSpeed = 0.1;
        if (Jerboa::Input::IsKeyHeldDown(Jerboa::KeyCode::W)) 
            trans.SetPosition(trans.GetPosition() + trans.GetForward() * moveSpeed);
        else if (Jerboa::Input::IsKeyHeldDown(Jerboa::KeyCode::S))
            trans.SetPosition(trans.GetPosition() - trans.GetForward() * moveSpeed);

        if (Jerboa::Input::IsKeyHeldDown(Jerboa::KeyCode::D))
            trans.SetPosition(trans.GetPosition() + trans.GetRight() * moveSpeed);
        else if (Jerboa::Input::IsKeyHeldDown(Jerboa::KeyCode::A))
            trans.SetPosition(trans.GetPosition() - trans.GetRight() * moveSpeed);

        if (Jerboa::Input::IsKeyHeldDown(Jerboa::KeyCode::E))
            trans.SetPosition(trans.GetPosition() + trans.GetUp() * moveSpeed);
        else if (Jerboa::Input::IsKeyHeldDown(Jerboa::KeyCode::Q))
            trans.SetPosition(trans.GetPosition() - trans.GetUp() * moveSpeed);

        if (Jerboa::Input::IsKeyHeldDown(Jerboa::KeyCode::_1))
            Jerboa::Window::Get()->SetCursorMode(Jerboa::CursorMode::Disabled);
        else if (Jerboa::Input::IsKeyHeldDown(Jerboa::KeyCode::_2))
            Jerboa::Window::Get()->SetCursorMode(Jerboa::CursorMode::Normal);

        mTestShader->Bind();
        mTestShader->SetMat4("mat_V", mCamera.GetViewMatrix());
        mTestShader->SetMat4("mat_P", mCamera.GetProjectionMatrix());
        mTestShader->SetInt("texture_diffuse", 0);
        mTestTexture->Bind(0);

        glBindVertexArray(mVao);
        
        Jerboa::Renderer::Draw(mIndexBuffer->GetCount());
	}

	void EditorLayer::OnAttach() {
		JERBOA_LOG_INFO("EditorLayer attached");

        Jerboa::Window::Get()->SetCursorMode(Jerboa::CursorMode::Disabled);
        
        // TODO: Remove explicit OpenGL calls
        glEnable(GL_DEPTH_TEST);
        glBindVertexArray(mVao);

        float vertices[] = {
            // pos                  // tex coords

                // front
            -0.5f, -0.5f, 0.5f,     0.0f, 0.0f,     // 0 left bottom  
             0.5f, -0.5f, 0.5f,     1.0f, 0.0f,     // 1 right bottom
             0.5f,  0.5f, 0.5f,     1.0f, 1.0f,     // 2 right top
            -0.5f,  0.5f, 0.5f,     0.0f, 1.0f,     // 3 left top

                // back
            -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,     // 4 left bottom  
             0.5f, -0.5f, -0.5f,     1.0f, 0.0f,     // 5 right bottom
             0.5f,  0.5f, -0.5f,     1.0f, 1.0f,     // 6 right top
            -0.5f,  0.5f, -0.5f,     0.0f, 1.0f,     // 7 left top

                // right
            0.5f, -0.5f, -0.5f,     0.0f, 0.0f,     // 8 left bottom  
            0.5f, -0.5f,  0.5f,     1.0f, 0.0f,     // 9 right bottom
            0.5f,  0.5f,  0.5f,     1.0f, 1.0f,     // 10 right top
            0.5f,  0.5f, -0.5f,     0.0f, 1.0f,      // 11 left top

                // left
            -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,     // 12 left bottom  
            -0.5f, -0.5f,  0.5f,     1.0f, 0.0f,     // 13 right bottom
            -0.5f,  0.5f,  0.5f,     1.0f, 1.0f,     // 14 right top
            -0.5f,  0.5f, -0.5f,     0.0f, 1.0f      // 15 left top
        };

        mVertexBuffer = Jerboa::VertexBuffer::Create(vertices, sizeof(vertices), Jerboa::VertexBufferUsage::Static,
        {
            { Jerboa::ShaderDataType::Float3 },
            { Jerboa::ShaderDataType::Float2 }
        });

        uint32_t indices[] = {
            0, 1, 2, /**/ 2, 3, 0,  // front
            4, 5, 6, /**/ 6, 7, 4,   // back,
            8, 9, 10, /**/ 10, 11, 8,   // right
            12, 13, 14, /**/ 14, 15, 12   // left
        };
        mIndexBuffer = Jerboa::IndexBuffer::Create(indices, sizeof(indices));

        mTestShader = Jerboa::Shader::Create("assets/shaders/Test.glsl");
        mTestTexture = Jerboa::Texture2D::Create("assets/textures/steel-wooden-container/diffuse.png", Jerboa::TextureType::Diffuse);
	}

	void EditorLayer::OnDetach() {
		JERBOA_LOG_INFO("EditorLayer deattached");
	}
	
	void EditorLayer::OnWindowResize(const Jerboa::WindowResizeEvent& evnt)
	{
		JERBOA_LOG_TRACE("window resized to {}x{}", evnt.width, evnt.height);
	}
}