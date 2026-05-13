#include <print>
#include <vector>
#include <random>

#include "window.h"
#include "camera.h"
#include "renderer.h"

#include "boidManager.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Boids Flocking"

#define WORLD_SIZE 20.0f
#define AMOUNT_BOIDS 100

int main(void) {
    DG::Window window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    DG::Camera camera = DG::Camera::Create2D(WINDOW_WIDTH, WINDOW_HEIGHT, WORLD_SIZE);

    BoidManager boidManager{ AMOUNT_BOIDS, camera.GetHalfViewHeight(), camera.GetHalfViewWidth() };

    while(!window.ShouldClose()) {
        window.UpdateDeltaTime();

        DG::Renderer::Clear(0.2f, 0.3f, 0.3f, 1.0f);

        //Update and Render via BoidManager
        boidManager.Update(window.GetDeltaTime());
        boidManager.Draw(camera);

        window.PollAndSwap();
    }

    return 0;
}