cmake_minimum_required(VERSION 3.28)



if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
else()
    message(FATAL_ERROR "VCPKG_ROOT environment variable not set")
endif()

if (MSVC)
    add_compile_options(/fsanitize=address)
    add_link_options(/INCREMENTAL:NO)
endif()

project(TriangleGame)

add_compile_definitions(_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS_RELEASE "/MD /O2 /Ob2 /DNDEBUG /MP") # Ensure NDEBUG is set in Release mode
set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG")    # Ensure _DEBUG is set in Debug mode


find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(SndFile CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")
find_package(EnTT CONFIG REQUIRED)
# this is heuristically generated, and may not be correct
find_package(rapidcheck CONFIG REQUIRED)
find_package(Aubio CONFIG REQUIRED)



find_package(imgui CONFIG REQUIRED)

find_package(unofficial-omniverse-physx-sdk CONFIG REQUIRED)
find_package(cereal CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)


message(STATUS "Available targets in CMake:")
get_property(TARGETS_LIST GLOBAL PROPERTY TARGETS)
foreach(TARGET ${TARGETS_LIST})
    message(STATUS "Target: ${TARGET}")
endforeach()




add_executable(TriangleGame
        src/main.cpp
        src/PrecompHeader.h
        src/Camera.cpp
        src/Camera.h
        src/Shader.cpp
        src/Shader.h
        src/Texture.cpp
        src/Texture.h
        src/Texture2DArray.cpp
        src/Texture2DArray.h
        src/Transform.cpp
        src/Transform.h
        src/comp/TriGuy.h
        src/comp/Position.h
        src/comp/AnimState.h
        src/core/App.cpp
        src/core/App.h
        src/core/Game.cpp
        src/core/Game.h
        src/core/Factories.cpp
        src/core/Factories.h
        src/comp/Direction.h
        src/sys/Render.cpp
        src/sys/Render.h
        src/util/DirHelpers.h
        src/comp/Behaviors.h
        src/sys/TriGuyBehaviors.cpp
        src/sys/TriGuyBehaviors.h
        src/comp/MusicMachine.h
        src/core/AudioContext.cpp
        src/core/AudioContext.h
)


target_link_libraries(TriangleGame PRIVATE OpenAL::OpenAL)
target_link_libraries(TriangleGame PRIVATE Aubio::aubio)

function(add_common_libraries target)

    # note: 3 additional targets are not displayed.
    target_link_libraries(${target} PRIVATE rapidcheck)
    
    target_precompile_headers(${target} PRIVATE ${CMAKE_SOURCE_DIR}/src/PrecompHeader.h)

    target_link_libraries(${target} PRIVATE EnTT::EnTT)

    target_link_libraries(${target} PRIVATE TBB::tbb)

    target_link_libraries(${target} PRIVATE cereal::cereal)
    # Include directories
    target_include_directories(${target} PUBLIC ${CMAKE_SOURCE_DIR}/include)
    
    target_link_directories(${target} PUBLIC ${CMAKE_SOURCE_DIR}/lib)
    target_include_directories(${target} PRIVATE ${TINYGLTF_INCLUDE_DIRS})
    target_link_libraries(${target} PRIVATE imgui::imgui)


    
    # target_link_libraries(${target} PRIVATE SharedThings)
    
    target_link_libraries(${target} PRIVATE     unofficial::omniverse-physx-sdk::PhysXCooking
            unofficial::omniverse-physx-sdk::PhysXFoundation
            unofficial::omniverse-physx-sdk::PhysXCommon
            unofficial::omniverse-physx-sdk::sdk)
    
    # Optional: import the defined target to copy over the GPU acceleration libraries (3rd party provided by NVIDIA)
    if(TARGET unofficial::omniverse-physx-sdk::gpu-library)
        if(UNIX)
            # Add rpath setting to find .so libraries on unix based systems
            set_target_properties(${target} PROPERTIES
                    BUILD_WITH_INSTALL_RPATH TRUE
                    INSTALL_RPATH "$ORIGIN"
            )
        endif()
        add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:unofficial::omniverse-physx-sdk::gpu-library>
                $<TARGET_FILE_DIR:${target}>)
        if(WIN32)
            add_custom_command(TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:unofficial::omniverse-physx-sdk::gpu-device-library>
                    $<TARGET_FILE_DIR:${target}>)
        endif()
    else()
        message(WARNING "GPU acceleration library target not defined - GPU acceleration will NOT be available!")
    endif()
    target_link_libraries(${target} PRIVATE glfw glm::glm GLEW::GLEW)
    target_link_libraries(${target} PRIVATE SndFile::sndfile)
endfunction()


add_common_libraries(TriangleGame)


