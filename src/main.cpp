#include <print>
#include <vector>
#include <random>

#include "core/application.h"
#include "core/Layer.h"
#include "renderer/camera.h"
#include "renderer/renderer.h"

#include "imgui.h"

#include "boidManager.h"


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Boids Flocking"

#define WORLD_SIZE 50.0f
#define AMOUNT_BOIDS 100

class BoidsLayer : public DG::Layer {
public:
    BoidsLayer()
        :
        m_camera(
            DG::Camera::Create2D(WINDOW_WIDTH, WINDOW_HEIGHT, WORLD_SIZE)
        ),
        m_settings({
            .seperationCoefficient = 0.4f,
            .cohesionCoefficient = 0.8f,
            .alignmentCoefficient = 0.8f,
            .visibilityArea = 5.0f
        }),
        m_boidManager({ AMOUNT_BOIDS,
          m_camera.GetHalfViewHeight(),
          m_camera.GetHalfViewWidth(),
          &m_settings
        })
    {}

    ~BoidsLayer() {}

    void OnAttach() override {}
    void OnDettach() override {}

    void OnUpdate(float deltaTime) override {
        DG::Renderer::Clear(0.2f, 0.3f, 0.3f, 1.0f);

        m_boidManager.Update(deltaTime);
        m_boidManager.Draw(m_camera);
    }

    void OnGuiDraw() override {
        ImGui::SetNextWindowSize(ImVec2(250.0f, 100.0f), ImGuiCond_FirstUseEver);

        ImGui::Begin("Flocking Parameters");
        ImGui::SliderFloat("Seperation", &m_settings.seperationCoefficient, 0.0f, 1.0f);
        ImGui::SliderFloat("Cohesion", &m_settings.cohesionCoefficient, 0.0f, 1.0f);
        ImGui::SliderFloat("Alignment", &m_settings.alignmentCoefficient, 0.0f, 1.0f);
        ImGui::End();
    }

private:
    DG::Camera m_camera;

    FlockingSettings m_settings;
    BoidManager m_boidManager;
};

class FlockingSimulation : public DG::Application {
public:
    FlockingSimulation()
        : DG::Application({ .windowWidth = WINDOW_WIDTH, .windowHeight = WINDOW_HEIGHT, .windowTitle = WINDOW_TITLE })
    { 
        PushLayer(std::make_unique<BoidsLayer>());
    }  
};

int main(void) {
    FlockingSimulation app;
    app.Run();
    return 0;
}