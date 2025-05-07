



#include "PrecompHeader.h"
#include "core/App.h"

int SWIDTH = 1024;
int SHEIGHT = 1024;


float globalScale = 1.0f;
glm::vec2 globalOffset = glm::vec2(0.f);

float aDeltaTime = 0.0f;

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

int main() {
    try {
        App app;
        app.run();
    } catch (std::exception &e) {
        std::cout << e.what() << '\n';
        return 1;
    }
    return 0;
}
