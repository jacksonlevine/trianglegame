//
// Created by jack on 5/5/2025.
//

#include "Render.h"

#include "../Shader.h"
#include "../Texture.h"
#include "../Texture2DArray.h"
#include "../comp/AnimState.h"
#include "../comp/Direction.h"
#include "../comp/MusicMachine.h"
#include "../comp/Position.h"
#include "../comp/TriGuy.h"

void renderTriGuys(entt::registry& reg, const jl::Camera& cam)
{
    static jl::Shader shader(
    R"glsl(

#version 450 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;


layout(location = 2) in vec2 instancePos;
layout(location = 3) in float anim;
layout(location = 4) in float heading;

uniform float time;
uniform mat4 mvp;

out vec2 vTexCoord;
out float animindex;

void main() {

    int framenum = int(mod(time*60.0, 60.0));
    float xUvOffset = float(framenum) / 60.0;
    float yUvOffset = -1.0 * (floor(heading * 16.0) / 16.0);
    animindex = anim;
    gl_Position = mvp * vec4(aPos + instancePos, 0.05, 1.0);
    vTexCoord = aTexCoord + vec2(xUvOffset, yUvOffset);
}

        )glsl",
    R"glsl(

#version 450 core
in vec2 vTexCoord;
in float animindex;
out vec4 FragColor;

uniform sampler2DArray uTexture;


void main() {
    FragColor = texture(uTexture, vec3(vTexCoord, int(round(animindex))));
}

        )glsl",
    "triGuyShader"
    );

    static GLuint vao = 0;
    static GLuint basevbo = 0;
    static GLuint instancesvbo = 0;

    if (vao == 0)
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &basevbo);
        glGenBuffers(1, &instancesvbo);


        float halfheight = 1.0f;
        float halfwidth = 1.0f;

        static std::vector<float> baseVertices = {
            -halfwidth, +halfheight,     0.0f,        15.0f/16.0f,
            -halfwidth, -halfheight,     0.0f,        1.0f,
            +halfwidth, -halfheight,     1.0f/60.0f,  1.0f,

            +halfwidth, -halfheight,     1.0f/60.0f,  1.0f,
            +halfwidth, +halfheight,     1.0f/60.0f,  15.0f/16.0f,
            -halfwidth, +halfheight,     0.0f,        15.0f/16.0f,
        };

        glBindBuffer(GL_ARRAY_BUFFER, basevbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * baseVertices.size(), baseVertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

    } else
    [[likely]]{
        glBindVertexArray(vao);
    }


        
    

    glUseProgram(shader.shaderID);

    struct TriGuyInstance
    {
        glm::vec2 pos;
        float anim;
        float heading;
    };

    static std::vector<TriGuyInstance> instances = {};

    instances.clear();

    const auto view = reg.view<TriGuy, Position, AnimState, Direction>();
    for (const auto entity : view)
    {
        const auto [animname] = view.get<AnimState>(entity);
        const auto [heading, direction] = view.get<Direction>(entity);
        const auto [position] = view.get<Position>(entity);
        instances.push_back(TriGuyInstance{
            .pos = position,
            .anim = static_cast<float>(animname),
            .heading = heading});
    }

    std::sort(instances.begin(), instances.end(), [](const TriGuyInstance& lhs, const TriGuyInstance& rhs)
    {
        return lhs.pos.y > rhs.pos.y;
    });

    glBindBuffer(GL_ARRAY_BUFFER, instancesvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TriGuyInstance) * instances.size(), instances.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TriGuyInstance), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(TriGuyInstance), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(TriGuyInstance), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);

    static std::vector<std::string> textures = {
    "resourcestoembed/triguy.png",
    "resourcestoembed/triguy2.png",
    "resourcestoembed/triguy3.png"};
    static jl::Texture2DArray texture(textures);

    static GLuint timeloc = glGetUniformLocation(shader.shaderID, "time");
    static GLuint texloc = glGetUniformLocation(shader.shaderID, "uTexture");
    static GLuint mvploc = glGetUniformLocation(shader.shaderID, "mvp");

    glUniformMatrix4fv(mvploc, 1, GL_FALSE, glm::value_ptr(cam.mvp));

    glUniform1f(timeloc, glfwGetTime());

    texture.bind(0);
    glUniform1i(texloc, 0);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instances.size());

}


void renderMusicMachines(entt::registry& reg, const jl::Camera& cam)
{
        static jl::Shader shader(
    R"glsl(

#version 450 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;


layout(location = 2) in vec2 instancePos;
layout(location = 3) in float anim;

uniform float time;
uniform mat4 mvp;

out vec2 vTexCoord;
out float animindex;

void main() {

    int framenum = int(mod(time*60.0, 60.0));
    float xUvOffset = float(framenum) / 60.0;

    animindex = anim;
    gl_Position = mvp * vec4(aPos + instancePos, 0.06, 1.0);
    vTexCoord = aTexCoord + vec2(xUvOffset, 0.0);
}

        )glsl",
    R"glsl(

#version 450 core
in vec2 vTexCoord;
in float animindex;
out vec4 FragColor;

uniform sampler2DArray uTexture;


void main() {
    FragColor = texture(uTexture, vec3(vTexCoord, int(round(animindex))));
}

        )glsl",
    "musicMachineShader"
    );

    static GLuint vao = 0;
    static GLuint basevbo = 0;
    static GLuint instancesvbo = 0;

    if (vao == 0)
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &basevbo);
        glGenBuffers(1, &instancesvbo);


        float halfheight = 1.0f;
        float halfwidth = 1.0f;

        static std::vector<float> baseVertices = {
            -halfwidth, +halfheight,     0.0f,        0.0f,
            -halfwidth, -halfheight,     0.0f,        1.0f,
            +halfwidth, -halfheight,     1.0f/60.0f,  1.0f,

            +halfwidth, -halfheight,     1.0f/60.0f,  1.0f,
            +halfwidth, +halfheight,     1.0f/60.0f,  0.0f,
            -halfwidth, +halfheight,     0.0f,        0.0f,
        };

        glBindBuffer(GL_ARRAY_BUFFER, basevbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * baseVertices.size(), baseVertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

    } else
    [[likely]]{
        glBindVertexArray(vao);
    }





    glUseProgram(shader.shaderID);

    struct MusicMachineVisualInstance
    {
        glm::vec2 pos;
        float anim;
    };

    static std::vector<MusicMachineVisualInstance> instances = {};

    instances.clear();

    const auto view = reg.view<MusicMachine, Position, AnimState>();
    for (const auto entity : view)
    {
        const auto [animname] = view.get<AnimState>(entity);
        const auto [position] = view.get<Position>(entity);
        instances.push_back(MusicMachineVisualInstance{
            .pos = position,
            .anim = static_cast<float>(animname)});
    }

    std::sort(instances.begin(), instances.end(), [](const MusicMachineVisualInstance& lhs, const MusicMachineVisualInstance& rhs)
    {
        return lhs.pos.y > rhs.pos.y;
    });

    glBindBuffer(GL_ARRAY_BUFFER, instancesvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(MusicMachineVisualInstance) * instances.size(), instances.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MusicMachineVisualInstance), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(MusicMachineVisualInstance), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);


    static std::vector<std::string> textures = {
    "resourcestoembed/machine1.png",
    "resourcestoembed/machine1idle.png"};
    static jl::Texture2DArray texture(textures);

    static GLuint timeloc = glGetUniformLocation(shader.shaderID, "time");
    static GLuint texloc = glGetUniformLocation(shader.shaderID, "uTexture");
    static GLuint mvploc = glGetUniformLocation(shader.shaderID, "mvp");

    glUniformMatrix4fv(mvploc, 1, GL_FALSE, glm::value_ptr(cam.mvp));

    glUniform1f(timeloc, glfwGetTime());

    texture.bind(0);
    glUniform1i(texloc, 0);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instances.size());
}


void renderBackground(entt::registry &reg, const jl::Camera& cam)
{
    static jl::Texture texture("resourcestoembed/grass.png");
    static jl::Shader shader(
        R"glsl(
#version 450 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;

uniform mat4 mvp;

void main() {

    gl_Position = mvp * vec4(aPos, 0.0, 1.0);
   vTexCoord = aTexCoord;
}
        )glsl",
        R"glsl(
#version 450 core
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
    FragColor = texture(uTexture, vTexCoord);
}
        )glsl",
        "backgroundShader");

    glUseProgram(shader.shaderID);

    static GLuint vao = 0;
    static GLuint vbo = 0;

    if (vao == 0)
    {

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);

        auto uvheight =  10000 / 300.0f;
        auto uvwidth =  10000 / 500.0f;


        //We will have to figure out something to always have background in front of the camera. This is just for now
        auto halfbackgroundheight = 100.0f;

        static std::vector<float> baseVertices = {
            -halfbackgroundheight, -halfbackgroundheight,     -uvwidth/2.f, -uvheight/2.f,
            -halfbackgroundheight, halfbackgroundheight,     -uvwidth/2.f, uvheight/2.f,
            halfbackgroundheight, halfbackgroundheight,       uvwidth/2.f, uvheight/2.f,

            halfbackgroundheight, halfbackgroundheight,       uvwidth/2.f, uvheight/2.f,
            halfbackgroundheight, -halfbackgroundheight,       uvwidth/2.f, -uvheight/2.f,
            -halfbackgroundheight, -halfbackgroundheight,     -uvwidth/2.f, -uvheight/2.f,
        };
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * baseVertices.size(), baseVertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

    } else
    {
        glBindVertexArray(vao);
    }



    static GLuint texloc = glGetUniformLocation(shader.shaderID, "uTexture");
    static GLuint mvploc = glGetUniformLocation(shader.shaderID, "mvp");

    glUniformMatrix4fv(mvploc, 1, GL_FALSE, glm::value_ptr(cam.mvp));
    
    texture.bind_to_unit(0);
    glUniform1i(texloc, 0);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);

}
