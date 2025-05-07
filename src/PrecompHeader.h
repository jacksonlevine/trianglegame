//
// Created by jack on 5/5/2025.
//

#ifndef PRECOMPHEADER_H
#define PRECOMPHEADER_H


#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <glm\glm.hpp>
#include <physx/PxPhysicsAPI.h>
#include <physx/cooking/PxCooking.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <tiny_gltf.h>
#include <stb_image.h>
#include <PxRigidStatic.h>
#include <cassert>
#include <unordered_set>
#include <memory>
#include <filesystem>
#include <ranges>
#include <iterator>
#include <algorithm>
#include <variant>
#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>
#include <bitset>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <entt/entt.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/array.hpp>
#include <tbb/tbb.h>
#include <tbb/concurrent_unordered_set.h>


extern int SWIDTH;
extern int SHEIGHT;

extern float globalScale;
extern glm::vec2 globalOffset;

extern float aDeltaTime;

#endif //PRECOMPHEADER_H
