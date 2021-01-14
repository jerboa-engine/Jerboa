#include "jerboa-pch.h"
#include "Application.h"

#include "Jerboa/Platform/Platform.h"

#include "Jerboa/UI/ImGui/ImGuiApp.h"

#include "Jerboa/Rendering/Renderer.h"
#include "Jerboa/Rendering/Renderer.h"
#include "Jerboa/Rendering/VertexBuffer.h"
#include "Jerboa/Rendering/IndexBuffer.h"
#include "Jerboa/Rendering/Shader.h"
#include "Jerboa/Rendering//Texture.h"

#include "Jerboa/Platform/OpenGL/OpenGL_VertexArray.h"

namespace Jerboa {
    Application::Application(const ApplicationProps& props)
        : mWindow(std::unique_ptr<Window>(Window::Create(props.windowProps))),
        mWindowResizeObserver(EventObserver::Create(mWindow->GetEventBus().lock().get(), this, &Application::OnWindowResize)),
        mWindowCloseObserver(EventObserver::Create(mWindow->GetEventBus().lock().get(), this, &Application::OnWindowClose)),
        mKeyPressedObserver(EventObserver::Create(mWindow->GetEventBus().lock().get(), this, &Application::OnKeyPressed)),
        mKeyReleasedObserver(EventObserver::Create(mWindow->GetEventBus().lock().get(), this, &Application::OnKeyReleased)),
        mKeyRepeatObserver(EventObserver::Create(mWindow->GetEventBus().lock().get(), this, &Application::OnKeyRepeat)),
        mMouseMovedObserver(EventObserver::Create(mWindow->GetEventBus().lock().get(), this, &Application::OnMouseMoved)),
        mMouseScrolledObserver(EventObserver::Create(mWindow->GetEventBus().lock().get(), this, &Application::OnMouseScrolled)),
        mMouseButtonPressedObserver(EventObserver::Create(mWindow->GetEventBus().lock().get(), this, &Application::OnMouseButtonPressed)),
        mMouseButtonReleasedObserver(EventObserver::Create(mWindow->GetEventBus().lock().get(), this, &Application::OnMouseButtonReleased))
    {
        mWindow->SetVSync(true);
    }

    void Application::Run() {
        Init();

        auto vao = OpenGL_VertexArray();
        glBindVertexArray(vao);

        float vertices[] = {
            // pos                  // tex coords
            -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,     // 0 left bottom  
             0.5f, -0.5f, 0.0f,     1.0f, 0.0f,     // 1 right bottom
             0.5f,  0.5f, 0.0f,     1.0f, 1.0f,     // 2 right top
            -0.5f,  0.5f, 0.0f,     0.0f, 1.0f      // 3 left top
        };

        auto vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices), VertexBufferUsage::Static, 
        {
            { ShaderDataType::Float3 },
            { ShaderDataType::Float2 }
        });
        
        uint32_t indices[] = { 0, 1, 2, /**/ 2, 3, 0 };
        auto indexBuffer = IndexBuffer::Create(indices, sizeof(indices));

        //auto shader = Shader::Create("assets/shaders/Test.vert", "assets/shaders/Test.frag");
        auto shader = Shader::Create("assets/shaders/Test.glsl");
        shader->Bind();

        shader->SetInt("texture_diffuse", 0);
        auto texture = Texture2D::Create("assets/textures/cartoony-brown-stone.png", TextureType::Diffuse);
        texture->Bind(0);

        while (mRunning) {
            Renderer::Clear();

            Renderer::Draw(indexBuffer->GetCount());

            for (Layer* layer : mLayerStack)
                layer->OnUpdate();

            RenderImGui();

            mWindow->Update();
        }
        ShutDown();
    }

    void Application::Init()
    {
        JERBOA_LOG_INFO("Initializing application");
        
        Platform::SetRenderAPI(RenderAPI::OpenGL);
        UI::ImGuiApp::Initialize(mWindow.get());

        OnInit();
    }

    void Application::ShutDown()
    {
        JERBOA_LOG_INFO("Shutting down application");

        UI::ImGuiApp::ShutDown();

        OnShutdown();
    }

    void Application::RenderImGui()
    {
        Jerboa::UI::ImGuiApp::BeginFrame();
        
        for (Layer* layer : mLayerStack)
            layer->OnImGuiRender();

        Jerboa::UI::ImGuiApp::EndFrame();
    }

    void Application::PushLayer(Layer* layer) {
        mLayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay) {
        mLayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::OnWindowResize(const WindowResizeEvent& evnt)
    {
        Layer::GetSharedEventBus()->Publish(evnt);
    }

    void Application::OnWindowClose(const WindowCloseEvent& evnt)
    {
        mRunning = false;
    }

    void Application::OnKeyPressed(const KeyPressedEvent& evnt)
    {
        JERBOA_LOG_TRACE("Pressed '{}' (mods {}), ", GetKeyName(evnt.key), evnt.modifiers);
    }

    void Application::OnKeyReleased(const KeyReleasedEvent& evnt)
    {
        JERBOA_LOG_TRACE("Released '{}' (mods {}), ", GetKeyName(evnt.key), evnt.modifiers);
    }

    void Application::OnKeyRepeat(const KeyRepeatEvent& evnt)
    {
        JERBOA_LOG_TRACE("Continiously pressing '{}' (mods {}), ", GetKeyName(evnt.key), evnt.modifiers);
    }

    void Application::OnMouseMoved(const MouseMovedEvent& evnt)
    {
        JERBOA_LOG_TRACE("Mouse moved ({}, {})", evnt.x, evnt.y);
    }

    void Application::OnMouseScrolled(const MouseScrolledEvent& evnt)
    {
        JERBOA_LOG_TRACE("Mouse scrolled ({}, {})", evnt.xOffset, evnt.yOffset);
    }

    void Jerboa::Application::OnMouseButtonPressed(const MouseButtonPressedEvent& evnt)
    {
        JERBOA_LOG_TRACE("Pressed mouse button {} (modifiers {})", evnt.button, evnt.modifiers);
    }

    void Jerboa::Application::OnMouseButtonReleased(const MouseButtonReleasedEvent& evnt)
    {
        JERBOA_LOG_TRACE("Released mouse button {} (modifiers {})", evnt.button, evnt.modifiers);
    }
}