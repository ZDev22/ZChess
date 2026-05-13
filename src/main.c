/* licensed under GPL v3.0 see https://github.com/ZDev22/ZEngine/blob/main/LICENSE for current license */

/* ZENGINE */
#define ZENGINE_IMPLEMENTATION
#define ZENGINE_DEPS_DEFINED
#define ZENGINE_DISABLE_VSYNC
#define ZENGINE_MAX_FRAMES_IN_FLIGHT 1
#define ZENGINE_MAX_SPRITES 10000
#define ZENGINE_MAX_TEXTURES 100
#define ZENGINE_DEPTHMODE_FIRST
#include "zengine.h"

/* RGFW */
#define RGFW_IMPLEMENTATION
#define RGFW_VULKAN
#define RGFW_EXPORT
#define RGFW_ASSERT(x) (void)(x)

/* STB_IMAGE */
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT

/* DEPENDENCIES */
#include "deps/RGFW.h"
#include "deps/stb_image.h"

#undef RGFW_IMPLEMENTATION
#undef STB_IMAGE_IMPLEMENTATION

/* zchess */
#include "zchess/zchess.h"

int main() {
    zwindow = RGFW_createWindow("ZChess", 0, 0, 1920, 1080, (u64)0);
    ZEngineInit();
    initZChess();

    while (1) {
        RGFW_event event;
        while (RGFW_window_checkEvent(zwindow, &event)) {
            if (event.type == RGFW_windowResized) {
                framebufferResized = 1;
                break;
            }
            else if (event.type == RGFW_quit) {
                RGFW_window_close(zwindow);
                zwindow = NULL;
                ZEngineDeinit();
                exit(0);
            }
        }

        zchessTick();

        ZEngineRender();
    }
}

