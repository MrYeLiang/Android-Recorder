cmake_minimum_required(VERSION 3.4.1)

add_library(native-lib
             SHARED
             src/main/cpp/controller.cpp
             src/main/cpp/matrix.cpp
             src/main/cpp/common/message_queue.cpp
             src/main/cpp/common/message_handler.cpp
             src/main/cpp/common/preview_render.cpp
             src/main/cpp/egl/egl_core.cpp
             src/main/cpp/opengl/opengl_render.cpp
             src/main/cpp/opengl/texture_frame.cpp
             src/main/cpp/opengl/gpu_texture_frame.cpp
             src/main/cpp/opengl/gpu_texture_frame.cpp
             src/main/cpp/opengl/gpu_texture_frame.cpp
             src/main/cpp/texture_copier/texture_frame_copier.cpp
             src/main/cpp/texture_copier/gpu_texture_frame_copier.cpp
             src/main/cpp/encoder/video_encoder_adapter.cpp
             src/main/jni/PreviewScheduler.cpp )


find_library(log-lib
              log )

target_link_libraries(native-lib
                      android
                      GLESv2
                      EGL
                       ${log-lib} )