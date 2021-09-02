//-----------------------------------------------------------------------------
// Copyright (c) 2006-2008 dhpoware. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------
//
// This is the second demo in the OpenGL camera demo series and it builds
// on the previous demo (http://www.dhpoware.com/downloads/GLCamera1.zip).
//
// In this demo we modify the implementation of the Camera class so that it is
// quaternion based rather than vector based.
//
//-----------------------------------------------------------------------------

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

#if defined(_DEBUG)
#include <crtdbg.h>
#endif

#include "GL_ARB_multitexture.h"
#include "WGL_ARB_multisample.h"
#include "bitmap.h"
#include "camera.h"
#include "gl_font.h"
#include "input.h"

#include "../../utilites/utmath.h"

CPlane3 plane(vec3(0.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f));

//-----------------------------------------------------------------------------
// Constants.
//-----------------------------------------------------------------------------

#define APP_TITLE "OpenGL Quaternion Camera Demo"

// Windows Vista compositing support.
#if !defined(PFD_SUPPORT_COMPOSITION)
#define PFD_SUPPORT_COMPOSITION 0x00008000
#endif

// GL_EXT_texture_filter_anisotropic
#define GL_TEXTURE_MAX_ANISOTROPY_EXT     0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

const Vector3 CAMERA_ACCELERATION(8.0f, 8.0f, 8.0f);
const float   CAMERA_FOVX = 90.0f;
const Vector3 CAMERA_POS(0.0f, 1.0f, 0.0f);
const float   CAMERA_SPEED_ROTATION = 0.2f;
const float   CAMERA_SPEED_FLIGHT_YAW = 100.0f;
const Vector3 CAMERA_VELOCITY(2.0f, 2.0f, 2.0f);
const float   CAMERA_ZFAR = 100.0f;
const float   CAMERA_ZNEAR = 0.1f;

const float   FLOOR_WIDTH = 16.0f;
const float   FLOOR_HEIGHT = 16.0f;
const float   FLOOR_TILE_S = 8.0f;
const float   FLOOR_TILE_T = 8.0f;

//-----------------------------------------------------------------------------
// Globals.
//-----------------------------------------------------------------------------

HWND      g_hWnd;
HDC       g_hDC;
HGLRC     g_hRC;
HINSTANCE g_hInstance;
int       g_framesPerSecond;
int       g_windowWidth;
int       g_windowHeight;
int       g_msaaSamples;
int       g_maxAnisotrophy;
bool      g_isFullScreen;
bool      g_hasFocus;
bool      g_enableVerticalSync;
bool      g_displayHelp;
bool      g_flightModeEnabled;
GLuint    g_floorColorMapTexture;
GLuint    g_floorLightMapTexture;
GLuint    g_floorDisplayList;
GLFont    g_font;
Camera    g_camera;
Vector3   g_cameraBoundsMax;
Vector3   g_cameraBoundsMin;
float     g_cameraRotationSpeed = CAMERA_SPEED_ROTATION;

//-----------------------------------------------------------------------------
// Functions Prototypes.
//-----------------------------------------------------------------------------

void    Cleanup();
void    CleanupApp();
HWND    CreateAppWindow(const WNDCLASSEX &wcl, const char *pszTitle);
void    EnableVerticalSync(bool enableVerticalSync);
bool    ExtensionSupported(const char *pszExtensionName);
float   GetElapsedTimeInSeconds();
void    GetMovementDirection(Vector3 &direction);
bool    Init();
void    InitApp();
void    InitGL();
void    LimitFrameRate(float frameRateLimit, float frameTimeSeconds);
GLuint  LoadTexture(const char *pszFilename);
GLuint  LoadTexture(const char *pszFilename, GLint magFilter, GLint minFilter,
                    GLint wrapS, GLint wrapT);
void    Log(const char *pszMessage);
void    PerformCameraCollisionDetection();
void    ProcessUserInput();
void    RenderFloor();
void    RenderFrame();
void    RenderText();
void    SetProcessorAffinity();
void    ToggleFullScreen();
void    UpdateCamera(float elapsedTimeSec);
void    UpdateFrame(float elapsedTimeSec);
void    UpdateFrameRate(float elapsedTimeSec);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------------------
// Functions.
//-----------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
#if defined _DEBUG
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif

    MSG msg = {0};
    WNDCLASSEX wcl = {0};

    wcl.cbSize = sizeof(wcl);
    wcl.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wcl.lpfnWndProc = WindowProc;
    wcl.cbClsExtra = 0;
    wcl.cbWndExtra = 0;
    wcl.hInstance = g_hInstance = hInstance;
    wcl.hIcon = LoadIcon(0, IDI_APPLICATION);
    wcl.hCursor = LoadCursor(0, IDC_ARROW);
    wcl.hbrBackground = 0;
    wcl.lpszMenuName = 0;
    wcl.lpszClassName = "GLWindowClass";
    wcl.hIconSm = 0;

    if (!RegisterClassEx(&wcl))
        return 0;

    g_hWnd = CreateAppWindow(wcl, APP_TITLE);

    if (g_hWnd)
    {
        SetProcessorAffinity();

        if (Init())
        {
            ShowWindow(g_hWnd, nShowCmd);
            UpdateWindow(g_hWnd);

            while (true)
            {
                while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
                {
                    if (msg.message == WM_QUIT)
                        break;

                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                if (msg.message == WM_QUIT)
                    break;

                if (g_hasFocus)
                {
                    UpdateFrame(GetElapsedTimeInSeconds());
                    RenderFrame();
                    SwapBuffers(g_hDC);
                }
                else
                {
                    WaitMessage();
                }
            }
        }

        Cleanup();
        UnregisterClass(wcl.lpszClassName, hInstance);
    }

    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_ACTIVATE:
        switch (wParam)
        {
        default:
            break;

        case WA_ACTIVE:
        case WA_CLICKACTIVE:
            Mouse::instance().attach(hWnd);
            g_hasFocus = true;
            break;

        case WA_INACTIVE:
            if (g_isFullScreen)
                ShowWindow(hWnd, SW_MINIMIZE);
            Mouse::instance().detach();
            g_hasFocus = false;
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_SIZE:
        g_windowWidth = static_cast<int>(LOWORD(lParam));
        g_windowHeight = static_cast<int>(HIWORD(lParam));
        break;

    default:
        Mouse::instance().handleMsg(hWnd, msg, wParam, lParam);
        Keyboard::instance().handleMsg(hWnd, msg, wParam, lParam);
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Cleanup()
{
    CleanupApp();

    if (g_hDC)
    {
        if (g_hRC)
        {
            wglMakeCurrent(g_hDC, 0);
            wglDeleteContext(g_hRC);
            g_hRC = 0;
        }

        ReleaseDC(g_hWnd, g_hDC);
        g_hDC = 0;
    }
}

void CleanupApp()
{
    g_font.destroy();

    if (g_floorColorMapTexture)
    {
        glDeleteTextures(1, &g_floorColorMapTexture);
        g_floorColorMapTexture = 0;
    }

    if (g_floorLightMapTexture)
    {
        glDeleteTextures(1, &g_floorLightMapTexture);
        g_floorLightMapTexture = 0;
    }

    if (g_floorDisplayList)
    {
        glDeleteLists(g_floorDisplayList, 1);
        g_floorDisplayList = 0;
    }
}

HWND CreateAppWindow(const WNDCLASSEX &wcl, const char *pszTitle)
{
    // Create a window that is centered on the desktop. It's exactly 1/4 the
    // size of the desktop. Don't allow it to be resized.

    DWORD wndExStyle = WS_EX_OVERLAPPEDWINDOW;
    DWORD wndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
                     WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

    HWND hWnd = CreateWindowEx(wndExStyle, wcl.lpszClassName, pszTitle,
                    wndStyle, 0, 0, 0, 0, 0, 0, wcl.hInstance, 0);

    if (hWnd)
    {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        int halfScreenWidth = screenWidth / 2;
        int halfScreenHeight = screenHeight / 2;
        int left = (screenWidth - halfScreenWidth) / 2;
        int top = (screenHeight - halfScreenHeight) / 2;
        RECT rc = {0};

        SetRect(&rc, left, top, left + halfScreenWidth, top + halfScreenHeight);
        AdjustWindowRectEx(&rc, wndStyle, FALSE, wndExStyle);
        MoveWindow(hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);

        GetClientRect(hWnd, &rc);
        g_windowWidth = rc.right - rc.left;
        g_windowHeight = rc.bottom - rc.top;
    }

    return hWnd;
}

void EnableVerticalSync(bool enableVerticalSync)
{
    // WGL_EXT_swap_control.

    typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC)(GLint);

    static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT =
        reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(
        wglGetProcAddress("wglSwapIntervalEXT"));

    if (wglSwapIntervalEXT)
    {
        wglSwapIntervalEXT(enableVerticalSync ? 1 : 0);
        g_enableVerticalSync = enableVerticalSync;
    }
}

bool ExtensionSupported(const char *pszExtensionName)
{
    static const char *pszGLExtensions = 0;
    static const char *pszWGLExtensions = 0;

    if (!pszGLExtensions)
        pszGLExtensions = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));

    if (!pszWGLExtensions)
    {
        // WGL_ARB_extensions_string.

        typedef const char *(WINAPI * PFNWGLGETEXTENSIONSSTRINGARBPROC)(HDC);

        PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB =
            reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>(
            wglGetProcAddress("wglGetExtensionsStringARB"));

        if (wglGetExtensionsStringARB)
            pszWGLExtensions = wglGetExtensionsStringARB(wglGetCurrentDC());
    }

    if (!strstr(pszGLExtensions, pszExtensionName))
    {
        if (!strstr(pszWGLExtensions, pszExtensionName))
            return false;
    }

    return true;
}

float GetElapsedTimeInSeconds()
{
    // Returns the elapsed time (in seconds) since the last time this function
    // was called. This elaborate setup is to guard against large spikes in
    // the time returned by QueryPerformanceCounter().

    static const int MAX_SAMPLE_COUNT = 50;

    static float frameTimes[MAX_SAMPLE_COUNT];
    static float timeScale = 0.0f;
    static float actualElapsedTimeSec = 0.0f;
    static INT64 freq = 0;
    static INT64 lastTime = 0;
    static int sampleCount = 0;
    static bool initialized = false;

    INT64 time = 0;
    float elapsedTimeSec = 0.0f;

    if (!initialized)
    {
        initialized = true;
        QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&freq));
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&lastTime));
        timeScale = 1.0f / freq;
    }

    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&time));
    elapsedTimeSec = (time - lastTime) * timeScale;
    lastTime = time;

    if (fabsf(elapsedTimeSec - actualElapsedTimeSec) < 1.0f)
    {
        memmove(&frameTimes[1], frameTimes, sizeof(frameTimes) - sizeof(frameTimes[0]));
        frameTimes[0] = elapsedTimeSec;

        if (sampleCount < MAX_SAMPLE_COUNT)
            ++sampleCount;
    }

    actualElapsedTimeSec = 0.0f;

    for (int i = 0; i < sampleCount; ++i)
        actualElapsedTimeSec += frameTimes[i];

    if (sampleCount > 0)
        actualElapsedTimeSec /= sampleCount;

    return actualElapsedTimeSec;
}

void GetMovementDirection(Vector3 &direction)
{
    static bool moveForwardsPressed = false;
    static bool moveBackwardsPressed = false;
    static bool moveRightPressed = false;
    static bool moveLeftPressed = false;
    static bool moveUpPressed = false;
    static bool moveDownPressed = false;

    Vector3 velocity = g_camera.getCurrentVelocity();
    Keyboard &keyboard = Keyboard::instance();
    
    direction.set(0.0f, 0.0f, 0.0f);

    if (keyboard.keyDown(Keyboard::KEY_W))
    {
        if (!moveForwardsPressed)
        {
            moveForwardsPressed = true;
            g_camera.setCurrentVelocity(velocity.x, velocity.y, 0.0f);
        }

        direction.z += 1.0f;
    }
    else
    {
        moveForwardsPressed = false;
    }

    if (keyboard.keyDown(Keyboard::KEY_S))
    {
        if (!moveBackwardsPressed)
        {
            moveBackwardsPressed = true;
            g_camera.setCurrentVelocity(velocity.x, velocity.y, 0.0f);
        }

        direction.z -= 1.0f;
    }
    else
    {
        moveBackwardsPressed = false;
    }

    if (keyboard.keyDown(Keyboard::KEY_D))
    {
        if (!moveRightPressed)
        {
            moveRightPressed = true;
            g_camera.setCurrentVelocity(0.0f, velocity.y, velocity.z);
        }

        direction.x += 1.0f;
    }
    else
    {
        moveRightPressed = false;
    }

    if (keyboard.keyDown(Keyboard::KEY_A))
    {
        if (!moveLeftPressed)
        {
            moveLeftPressed = true;
            g_camera.setCurrentVelocity(0.0f, velocity.y, velocity.z);
        }

        direction.x -= 1.0f;
    }
    else
    {
        moveLeftPressed = false;
    }
    
    if (keyboard.keyDown(Keyboard::KEY_E))
    {
        if (!moveUpPressed)
        {
            moveUpPressed = true;
            g_camera.setCurrentVelocity(velocity.x, 0.0f, velocity.z);
        }

        direction.y += 1.0f;
    }
    else
    {
        moveUpPressed = false;
    }

    if (keyboard.keyDown(Keyboard::KEY_Q))
    {
        if (!moveDownPressed)
        {
            moveDownPressed = true;
            g_camera.setCurrentVelocity(velocity.x, 0.0f, velocity.z);
        }

        direction.y -= 1.0f;
    }
    else
    {
        moveDownPressed = false;
    }
}

bool Init()
{
    try
    {
        InitGL();

        if (!ExtensionSupported("GL_ARB_multitexture"))
            throw std::runtime_error("Required extension not supported: GL_ARB_multitexture.");

        InitApp();
        return true;
    }
    catch (const std::exception &e)
    {
        std::ostringstream msg;

        msg << "Application initialization failed!" << std::endl << std::endl;
        msg << e.what();

        Log(msg.str().c_str());
        return false;
    }
}

void InitApp()
{
    // Setup fonts.

    if (!g_font.create("Arial", 10, GLFont::BOLD))
        throw std::runtime_error("Failed to create font.");

    // Load textures.

    if (!(g_floorColorMapTexture = LoadTexture("floor_color_map.jpg")))
        throw std::runtime_error("Failed to load texture: floor_color_map.jpg");

    if (!(g_floorLightMapTexture = LoadTexture("floor_light_map.jpg")))
        throw std::runtime_error("Failed to load texture: floor_light_map.jpg");

    // Setup camera.

    g_camera.perspective(CAMERA_FOVX,
        static_cast<float>(g_windowWidth) / static_cast<float>(g_windowHeight),
        CAMERA_ZNEAR, CAMERA_ZFAR);

    g_camera.setBehavior(Camera::CAMERA_BEHAVIOR_FIRST_PERSON);
    g_camera.setPosition(CAMERA_POS);
    g_camera.setAcceleration(CAMERA_ACCELERATION);
    g_camera.setVelocity(CAMERA_VELOCITY);

    g_cameraBoundsMax.set(FLOOR_WIDTH / 2.0f, 4.0f, FLOOR_HEIGHT / 2.0f);
    g_cameraBoundsMin.set(-FLOOR_WIDTH / 2.0f, CAMERA_POS.y, -FLOOR_HEIGHT / 2.0f);

    Mouse::instance().hideCursor(true);
    Mouse::instance().moveToWindowCenter();

    // Setup display list for the floor.

    g_floorDisplayList = glGenLists(1);
    glNewList(g_floorDisplayList, GL_COMPILE);   
    glBegin(GL_QUADS);
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);    
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
        glVertex3f(-FLOOR_WIDTH * 0.5f, 0.0f, FLOOR_HEIGHT * 0.5f);

        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, FLOOR_TILE_S, 0.0f);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
        glVertex3f(FLOOR_WIDTH * 0.5f, 0.0f, FLOOR_HEIGHT * 0.5f);

        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, FLOOR_TILE_S, FLOOR_TILE_T);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
        glVertex3f(FLOOR_WIDTH * 0.5f, 0.0f, -FLOOR_HEIGHT * 0.5f);

        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.00f, FLOOR_TILE_T);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
        glVertex3f(-FLOOR_WIDTH * 0.5f, 0.0f, -FLOOR_HEIGHT * 0.5f);
    glEnd();
    glEndList();
}

void InitGL()
{
	AllocConsole();
	freopen("conout$", "w", stdout);


    if (!(g_hDC = GetDC(g_hWnd)))
        throw std::runtime_error("GetDC() failed.");

    int pf = 0;
    PIXELFORMATDESCRIPTOR pfd = {0};
    OSVERSIONINFO osvi = {0};

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    if (!GetVersionEx(&osvi))
        throw std::runtime_error("GetVersionEx() failed.");

    // When running under Windows Vista or later support desktop composition.

    if (osvi.dwMajorVersion > 6 || (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion >= 0))
        pfd.dwFlags |=  PFD_SUPPORT_COMPOSITION;

    ChooseBestMultiSampleAntiAliasingPixelFormat(pf, g_msaaSamples);

    if (!pf)
        pf = ChoosePixelFormat(g_hDC, &pfd);

    if (!SetPixelFormat(g_hDC, pf, &pfd))
        throw std::runtime_error("SetPixelFormat() failed.");

    if (!(g_hRC = wglCreateContext(g_hDC)))
        throw std::runtime_error("wglCreateContext() failed.");

    if (!wglMakeCurrent(g_hDC, g_hRC))
        throw std::runtime_error("wglMakeCurrent() failed.");

    EnableVerticalSync(false);

    // Check for GL_EXT_texture_filter_anisotropic support.

    if (ExtensionSupported("GL_EXT_texture_filter_anisotropic"))
        glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &g_maxAnisotrophy);
    else
        g_maxAnisotrophy = 1;
}

GLuint LoadTexture(const char *pszFilename)
{
    return LoadTexture(pszFilename, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR,
        GL_REPEAT, GL_REPEAT);
}

GLuint LoadTexture(const char *pszFilename, GLint magFilter, GLint minFilter,
                   GLint wrapS, GLint wrapT)
{
    GLuint id = 0;
    Bitmap bitmap;

    if (bitmap.loadPicture(pszFilename))
    {
        // The Bitmap class loads images and orients them top-down.
        // OpenGL expects bitmap images to be oriented bottom-up.
        bitmap.flipVertical();

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

        if (g_maxAnisotrophy > 1)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                g_maxAnisotrophy);
        }

        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, bitmap.width, bitmap.height,
            GL_BGRA_EXT, GL_UNSIGNED_BYTE, bitmap.getPixels());
    }

    return id;
}

void Log(const char *pszMessage)
{
    MessageBox(0, pszMessage, "Error", MB_ICONSTOP);
}

void PerformCameraCollisionDetection()
{
    const Vector3 &pos = g_camera.getPosition();
    Vector3 newPos(pos);

    if (pos.x > g_cameraBoundsMax.x)
        newPos.x = g_cameraBoundsMax.x;

    if (pos.x < g_cameraBoundsMin.x)
        newPos.x = g_cameraBoundsMin.x;

    if (pos.y > g_cameraBoundsMax.y)
        newPos.y = g_cameraBoundsMax.y;

    if (pos.y < g_cameraBoundsMin.y)
        newPos.y = g_cameraBoundsMin.y;

    if (pos.z > g_cameraBoundsMax.z)
        newPos.z = g_cameraBoundsMax.z;

    if (pos.z < g_cameraBoundsMin.z)
        newPos.z = g_cameraBoundsMin.z;

    g_camera.setPosition(newPos);
}

void ProcessUserInput()
{
    Keyboard &keyboard = Keyboard::instance();
    Mouse &mouse = Mouse::instance();

    if (keyboard.keyPressed(Keyboard::KEY_ESCAPE))
        PostMessage(g_hWnd, WM_CLOSE, 0, 0);

    if (keyboard.keyDown(Keyboard::KEY_LALT) || keyboard.keyDown(Keyboard::KEY_RALT))
    {
        if (keyboard.keyPressed(Keyboard::KEY_ENTER))
            ToggleFullScreen();
    }

    if (keyboard.keyPressed(Keyboard::KEY_H))
        g_displayHelp = !g_displayHelp;

    if (keyboard.keyPressed(Keyboard::KEY_M))
        mouse.smoothMouse(!mouse.isMouseSmoothing());

    if (keyboard.keyPressed(Keyboard::KEY_V))
        EnableVerticalSync(!g_enableVerticalSync);

    if (keyboard.keyPressed(Keyboard::KEY_ADD) || keyboard.keyPressed(Keyboard::KEY_NUMPAD_ADD))
    {
        g_cameraRotationSpeed += 0.01f;

        if (g_cameraRotationSpeed > 1.0f)
            g_cameraRotationSpeed = 1.0f;
    }

    if (keyboard.keyPressed(Keyboard::KEY_SUBTRACT) || keyboard.keyPressed(Keyboard::KEY_NUMPAD_SUBTRACT))
    {
        g_cameraRotationSpeed -= 0.01f;

        if (g_cameraRotationSpeed <= 0.0f)
            g_cameraRotationSpeed = 0.01f;
    }

    if (keyboard.keyPressed(Keyboard::KEY_PERIOD))
    {
        mouse.setWeightModifier(mouse.weightModifier() + 0.1f);

        if (mouse.weightModifier() > 1.0f)
            mouse.setWeightModifier(1.0f);
    }     
    
    if (keyboard.keyPressed(Keyboard::KEY_COMMA))
    {
        mouse.setWeightModifier(mouse.weightModifier() - 0.1f);

        if (mouse.weightModifier() < 0.0f)
            mouse.setWeightModifier(0.0f);
    }

    if (keyboard.keyPressed(Keyboard::KEY_SPACE))
    {
        g_flightModeEnabled = !g_flightModeEnabled;

        if (g_flightModeEnabled)
        {
            g_camera.setBehavior(Camera::CAMERA_BEHAVIOR_FLIGHT);
        }
        else
        {
            const Vector3 &cameraPos = g_camera.getPosition();

            g_camera.setBehavior(Camera::CAMERA_BEHAVIOR_FIRST_PERSON);
            g_camera.setPosition(cameraPos.x, CAMERA_POS.y, cameraPos.z);
        }
    }
}

void RenderFloor()
{
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_floorColorMapTexture);

    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_floorLightMapTexture);

    glCallList(g_floorDisplayList);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void RenderFrame()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);

    glViewport(0, 0, g_windowWidth, g_windowHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&g_camera.getProjectionMatrix()[0][0]);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(&g_camera.getViewMatrix()[0][0]);

    RenderFloor();
    RenderText();

	//TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS 
	CRay ray;
	ray.SetOrigin(g_camera.m_eye.x, g_camera.m_eye.y, g_camera.m_eye.z);
	ray.SetDirection(g_camera.m_viewDir.x, g_camera.m_viewDir.y, g_camera.m_viewDir.z);

	vec3 intersect;
	printf("( %f %f %f )  ( %f %f %f )\n", g_camera.m_eye.x, g_camera.m_eye.y, g_camera.m_eye.z, g_camera.m_viewDir.x, g_camera.m_viewDir.y, g_camera.m_viewDir.z);
	if (ray.PlaneIntersection5(plane.m_origin, plane.m_normal, ray.m_origin, ray.m_direction, intersect)) {
		glPushMatrix();
		glLineWidth(2.0);
		glBegin(GL_LINES);
		glVertex3f(intersect.x, intersect.y, intersect.z);
		glVertex3f(intersect.x, intersect.y + 10.f, intersect.z);
		glEnd();
		glLineWidth(1.0);
		glPopMatrix();
	}
	//TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS TESTS 
}

void RenderText()
{
    std::ostringstream output;

    if (g_displayHelp)
    {
        output
            << "First person camera behavior" << std::endl
            << "  Press W and S to move forwards and backwards" << std::endl
            << "  Press A and D to strafe left and right" << std::endl
            << "  Press E and Q to move up and down" << std::endl
            << "  Move mouse to free look" << std::endl
            << std::endl
            << "Flight camera behavior" << std::endl
            << "  Press W and S to move forwards and backwards" << std::endl
            << "  Press A and D to yaw left and right" << std::endl
            << "  Press E and Q to move up and down" << std::endl
            << "  Move mouse to pitch and roll" << std::endl
            << std::endl
            << "Press M to enable/disable mouse smoothing" << std::endl
            << "Press V to enable/disable vertical sync" << std::endl
            << "Press + and - to change camera rotation speed" << std::endl
            << "Press , and . to change mouse sensitivity" << std::endl
            << "Press SPACE to toggle between flight and first person behavior" << std::endl
            << "Press ALT and ENTER to toggle full screen" << std::endl
            << "Press ESC to exit" << std::endl
            << std::endl
            << "Press H to hide help";
    }
    else
    {
        Mouse &mouse = Mouse::instance();

        output.setf(std::ios::fixed, std::ios::floatfield);
        output << std::setprecision(2);

        output
            << "FPS: " << g_framesPerSecond << std::endl
            << "Multisample anti-aliasing: " << g_msaaSamples << "x" << std::endl
            << "Anisotropic filtering: " << g_maxAnisotrophy << "x" << std::endl
            << "Vertical sync: " << (g_enableVerticalSync ? "enabled" : "disabled") << std::endl
            << std::endl
            << "Camera" << std::endl
            << "  Position:"
            << " x:" << g_camera.getPosition().x
            << " y:" << g_camera.getPosition().y
            << " z:" << g_camera.getPosition().z << std::endl
            << "  Velocity:"
            << " x:" << g_camera.getCurrentVelocity().x
            << " y:" << g_camera.getCurrentVelocity().y
            << " z:" << g_camera.getCurrentVelocity().z << std::endl
            << "  Rotation speed: " << g_cameraRotationSpeed << std::endl
            << "  Behavior: " << (g_flightModeEnabled ? "Flight" : "First person") << std::endl
            << std::endl
            << "Mouse" << std::endl
            << "  Smoothing: " << (mouse.isMouseSmoothing() ? "enabled" : "disabled") << std::endl
            << "  Sensitivity: " << mouse.weightModifier() << std::endl
            << std::endl
            << "Press H to display help";
    }

    g_font.begin();
    g_font.setColor(1.0f, 1.0f, 0.0f);
    g_font.drawText(1, 1, output.str().c_str());
    g_font.end();
}

void SetProcessorAffinity()
{
    // Assign the current thread to one processor. This ensures that timing
    // code runs on only one processor, and will not suffer any ill effects
    // from power management.
    //
    // Based on DXUTSetProcessorAffinity() function from the DXUT framework.

    DWORD_PTR dwProcessAffinityMask = 0;
    DWORD_PTR dwSystemAffinityMask = 0;
    HANDLE hCurrentProcess = GetCurrentProcess();

    if (!GetProcessAffinityMask(hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask))
        return;

    if (dwProcessAffinityMask)
    {
        // Find the lowest processor that our process is allowed to run against.

        DWORD_PTR dwAffinityMask = (dwProcessAffinityMask & ((~dwProcessAffinityMask) + 1));

        // Set this as the processor that our thread must always run against.
        // This must be a subset of the process affinity mask.

        HANDLE hCurrentThread = GetCurrentThread();

        if (hCurrentThread != INVALID_HANDLE_VALUE)
        {
            SetThreadAffinityMask(hCurrentThread, dwAffinityMask);
            CloseHandle(hCurrentThread);
        }
    }

    CloseHandle(hCurrentProcess);
}

void ToggleFullScreen()
{
    static DWORD savedExStyle;
    static DWORD savedStyle;
    static RECT rcSaved;

    g_isFullScreen = !g_isFullScreen;

    if (g_isFullScreen)
    {
        // Moving to full screen mode.

        savedExStyle = GetWindowLong(g_hWnd, GWL_EXSTYLE);
        savedStyle = GetWindowLong(g_hWnd, GWL_STYLE);
        GetWindowRect(g_hWnd, &rcSaved);

        SetWindowLong(g_hWnd, GWL_EXSTYLE, 0);
        SetWindowLong(g_hWnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
        SetWindowPos(g_hWnd, HWND_TOPMOST, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

        g_windowWidth = GetSystemMetrics(SM_CXSCREEN);
        g_windowHeight = GetSystemMetrics(SM_CYSCREEN);

        SetWindowPos(g_hWnd, HWND_TOPMOST, 0, 0,
            g_windowWidth, g_windowHeight, SWP_SHOWWINDOW);
    }
    else
    {
        // Moving back to windowed mode.

        SetWindowLong(g_hWnd, GWL_EXSTYLE, savedExStyle);
        SetWindowLong(g_hWnd, GWL_STYLE, savedStyle);
        SetWindowPos(g_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

        g_windowWidth = rcSaved.right - rcSaved.left;
        g_windowHeight = rcSaved.bottom - rcSaved.top;

        SetWindowPos(g_hWnd, HWND_NOTOPMOST, rcSaved.left, rcSaved.top,
            g_windowWidth, g_windowHeight, SWP_SHOWWINDOW);
    }

    g_camera.perspective(CAMERA_FOVX,
        static_cast<float>(g_windowWidth) / static_cast<float>(g_windowHeight),
        CAMERA_ZNEAR, CAMERA_ZFAR);
}

void UpdateCamera(float elapsedTimeSec)
{
    float heading = 0.0f;
    float pitch = 0.0f;
    float roll = 0.0f;
    Vector3 direction;
    Mouse &mouse = Mouse::instance();

    GetMovementDirection(direction);

    switch (g_camera.getBehavior())
    {
    case Camera::CAMERA_BEHAVIOR_FIRST_PERSON:
        pitch = mouse.yDistanceFromWindowCenter() * g_cameraRotationSpeed;
        heading = -mouse.xDistanceFromWindowCenter() * g_cameraRotationSpeed;
        
        g_camera.rotate(heading, pitch, 0.0f);
        break;

    case Camera::CAMERA_BEHAVIOR_FLIGHT:
        heading = -direction.x * CAMERA_SPEED_FLIGHT_YAW * elapsedTimeSec;
        pitch = -mouse.yDistanceFromWindowCenter() * g_cameraRotationSpeed;
        roll = -mouse.xDistanceFromWindowCenter() * g_cameraRotationSpeed;
        
        g_camera.rotate(heading, pitch, roll);
        direction.x = 0.0f; // ignore yaw motion when updating camera velocity
        break;
    }

    g_camera.updatePosition(direction, elapsedTimeSec);
    PerformCameraCollisionDetection();

    mouse.moveToWindowCenter();
}

void UpdateFrame(float elapsedTimeSec)
{
    UpdateFrameRate(elapsedTimeSec);

    Mouse::instance().update();
    Keyboard::instance().update();

    UpdateCamera(elapsedTimeSec);
    ProcessUserInput();
}

void UpdateFrameRate(float elapsedTimeSec)
{
    static float accumTimeSec = 0.0f;
    static int frames = 0;

    accumTimeSec += elapsedTimeSec;

    if (accumTimeSec > 1.0f)
    {
        g_framesPerSecond = frames;

        frames = 0;
        accumTimeSec = 0.0f;
    }
    else
    {
        ++frames;
    }
}