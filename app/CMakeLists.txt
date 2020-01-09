cmake_minimum_required(VERSION 3.10.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0 -std=c++17 -Wno-undefined-var-template -Wno-writable-strings -Wno-unsequenced -Wno-switch")

# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,10000000")

add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

target_include_directories(native_app_glue PUBLIC
        ${ANDROID_NDK}/sources/android/native_app_glue
)

find_library(log-lib
        log
)



set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
add_library(
    # name of the library
    bubble_fighter-c

    # set as shared lib
    SHARED

    # relative paths to main files

    src/main/cpp/core/Matrix3.cpp
    src/main/cpp/core/Math2D.cpp
    src/main/cpp/core/SpriteCoords.cpp

    src/main/cpp/core/Logger.cpp
    src/main/cpp/core/Resource.cpp
    src/main/cpp/core/ResourceManager.cpp
    src/main/cpp/core/ResourceSystem.cpp

    src/main/cpp/core/DataStream.cpp
    src/main/cpp/core/AndroidDataStream.cpp

    src/main/cpp/core/AndroidDataProvider.cpp

    src/main/cpp/core/Color.cpp

    src/main/cpp/core/Image.cpp

    src/main/cpp/core/ShadingParamsPassthru.cpp

    src/main/cpp/core/ScriptLoader.cpp
    src/main/cpp/core/_2d/MaterialManager.cpp

    src/main/cpp/core/Texture.cpp
    src/main/cpp/core/TextureManager.cpp

    src/main/cpp/core/Shader.cpp
    src/main/cpp/core/ShaderManager.cpp

    src/main/cpp/core/ShadingProgramParams.cpp
    src/main/cpp/core/ShadingProgram.cpp
    src/main/cpp/core/ShadingProgramManager.cpp

    src/main/cpp/core/ImageSprite.cpp
    src/main/cpp/core/ImageSpriteManager.cpp

    src/main/cpp/core/SpritedFont.cpp
    src/main/cpp/core/SpritedFontManager.cpp

    src/main/cpp/core/_2d/ViewPort.cpp

    src/main/cpp/core/_2d/SceneNode.cpp
    src/main/cpp/core/_2d/Camera.cpp

    src/main/cpp/core/_2d/Renderable.cpp
    src/main/cpp/core/_2d/SingleSprite.cpp
    src/main/cpp/core/_2d/ParticleSystem.cpp


    src/main/cpp/core/_2d/MovableObject.cpp

    src/main/cpp/core/_2d/SceneManager.cpp

    src/main/cpp/core/RenderStateCashe.cpp
    src/main/cpp/core/RenderSystem.cpp

    src/main/cpp/core/GraphicBuffer.cpp
    src/main/cpp/core/GLutils.cpp

    src/main/cpp/core/Controller.cpp
    src/main/cpp/core/ControllerManager.cpp

    src/main/cpp/core/TouchControls.cpp
    src/main/cpp/core/InputManager.cpp

    src/main/cpp/core/_2d/ObjectFactory.cpp


    src/main/cpp/bubble_fighter-c.cpp

)

target_link_libraries(
        bubble_fighter-c
        android
        native_app_glue
        ${log-lib}
        EGL
        GLESv2
)
