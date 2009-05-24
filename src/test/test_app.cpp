#include <stdio.h>
#include <stdlib.h>
#include <ddf/ddf.h>
#include <assert.h>

#include "graphics_device.h"
#include "../util/debugrenderer.h"

#include "sdl/SDL.h"
#ifndef _WIN32
#include <sys/time.h>
#endif

const float Vertices[] = {
        -5, -5, 0,
        5, -5, 0,
        -5, 5, 0,
        5, 5, 0,
        -5, -5, 5,
        5, -5, 5,
        -5, 5, 5,
        5, 5, 5};

const float Normals[] = {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1};
const float UVs[] = {0.000499, 0.000499, 0.5, 0.9995, 0.000499, 0.5, 0.000499, 0.999501, 0.5, 0.999501, 0.9995, 0.5};
const int Indices[] = {0, 1, 2, 1,3,2, 4, 5, 6}; //{2, 0, 2, 0, 1, 0, 3, 2, 3, 1, 3, 1, 3, 2, 3, 0, 1, 0};


#include <vectormath/cpp/vectormath_aos.h>

using namespace Vectormath::Aos;

HGFXVertexProgram m_VertexProgram;
HGFXFragmentProgram m_FragmentProgram;



static void GameLoop();


#ifdef _WIN32
static DWORD last_idle_time;
#else
static struct timeval last_idle_time;
#endif

static float GetDt()
{
    float dt;
#ifdef _WIN32
    DWORD time_now;
    time_now = GetTickCount();
    dt = (float) (time_now - last_idle_time) / 1000.0;
#else
    // Figure out time elapsed since last call to idle function
    struct timeval time_now;
    gettimeofday(&time_now, NULL);
    dt = (float)(time_now.tv_sec  - last_idle_time.tv_sec) +
    1.0e-6*(time_now.tv_usec - last_idle_time.tv_usec);
#endif
    // save time_now for next time
    last_idle_time = time_now;

    return dt;
}

GLuint loc;
void LoadPrograms()
{
    char buf[1024*128];
    uint32_t size;
    FILE* f;
    f = fopen("build/default/src/test/simple.arbvp", "rb");
    assert(f);
    size = fread(buf, 1, sizeof(buf), f);
    m_VertexProgram = GFXCreateVertexProgram(buf, size);
    fclose(f);

    f = fopen("build/default/src/test/simple.arbfp", "rb");
    assert(f);
    size = fread(buf, 1, sizeof(buf), f);
    m_FragmentProgram = GFXCreateFragmentProgram(buf, size);
    fclose(f);
}

int main(int argc, char *argv[])
{
    GFXHDevice device;
    GFXSCreateDeviceParams params;

    params.m_DisplayWidth = 800;
    params.m_DisplayHeight = 600;
    params.m_AppTitle = "libgraphics test";
    params.m_Fullscreen = false;
    params.m_PrintDeviceInfo = false;

    device = GFXCreateDevice(&argc, argv, &params);

    GFXHContext context = GFXGetContext();


    GFXEnableState(context, GFX_DEPTH_TEST);

//    float ar = (float)params.m_DisplayWidth / (float)params.m_DisplayHeight;
    GFXSetViewport(context, params.m_DisplayWidth, params.m_DisplayHeight);

    // run once to initialise
    GetDt();

    uint8_t *keys;

    LoadPrograms();
    bool work = true;
    while (work)
    {
        SDL_Event event;
        while ( SDL_PollEvent(&event) )
        {
          switch(event.type) {
            case SDL_VIDEORESIZE:
              break;

            case SDL_KEYDOWN:
                break;
            case SDL_KEYUP:
                break;

            case SDL_QUIT:
                work = false;
                break;
          }
        }

        keys = SDL_GetKeyState(NULL);

        if ( keys[SDLK_ESCAPE] )
          work = false;

        GameLoop();
    }
    GFXDestroyDevice();
    return 0;
}

static void GameLoop()
{
//    float dt = GetDt();

    GFXHContext context = GFXGetContext();
    GFXClear(context, GFX_CLEAR_COLOUR_BUFFER|GFX_CLEAR_DEPTH_BUFFER,
            125, 80, 160, 0, 1.0, 0);


    GFXSetVertexProgram(context, m_VertexProgram);
    GFXSetFragmentProgram(context, m_FragmentProgram);

    Matrix4 proj = Matrix4::perspective(0.5, 1.333333, 0.1f, 500.0f);



    Matrix4 view = Matrix4::lookAt(  Point3(50.0, 30.0, 50.0),
                            Point3(0, 0, 0),
                            Vector3(0, 1, 0)
                            );


    Matrix4 mat = Matrix4::identity();
    mat.setTranslation(Vector3(0, 0, 0));

    Matrix4 vp = proj*view;
    GFXSetVertexConstantBlock(context, (const Vector4*)&vp, 0, 4);
    GFXSetVertexConstantBlock(context, (const Vector4*)&mat, 4, 4);

    Vector4 v(0.0, 0.0, 1.0, 0.0);
    GFXSetFragmentConstant(context, &v, 0);

    GFXSetVertexStream(context, 0, 3, GFX_TYPE_FLOAT, 0, (void*) &Vertices[0]);
    GFXSetVertexStream(context, 1, 2, GFX_TYPE_FLOAT, 0, (void*) &UVs[0]);
    GFXSetVertexStream(context, 2, 3, GFX_TYPE_FLOAT, 0, (void*) &Normals[0]);


    GFXUtil::DebugRendererCube(5, Vector3(0, 0, 0));

    GFXFlip();

}

