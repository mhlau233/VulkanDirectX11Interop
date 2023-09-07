#include "DXSwapChain.h"

#include <Windows.h>

#include <stdexcept>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

constexpr auto WINDOW_WIDTH = 1024;
constexpr auto WINDOW_HEIGHT = 768;

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
    {
        DestroyWindow(hwnd);
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void DXSwapChain::InitWindow()
{
    LPCSTR CLASS_NAME = {"Sample Window Class"};

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    this->windowHandle = CreateWindowEx(0,
                                        CLASS_NAME,
                                        {"Learn to Program Windows"},
                                        WS_OVERLAPPEDWINDOW,

                                        // Size and position
                                        CW_USEDEFAULT,
                                        CW_USEDEFAULT,
                                        WINDOW_WIDTH,
                                        WINDOW_HEIGHT,

                                        NULL,         // Parent window
                                        NULL,         // Menu
                                        wc.hInstance, // Instance handle
                                        NULL          // Additional application data
    );

    if (!windowHandle)
    {
        std::runtime_error("Failed to create window");
    }
    else
    {
        ShowWindow(windowHandle, SW_NORMAL);
    }
}

void DXSwapChain::InitD3D()
{

    IDXGIFactory1 *factory;

    auto result = CreateDXGIFactory1(__uuidof(IDXGIFactory), (void **)&factory);

    IDXGIAdapter *adpt = nullptr;
    for (int i = 0; factory->EnumAdapters(i, &adpt) == S_OK; i++)
    {
        DXGI_ADAPTER_DESC adesc;
        adpt->GetDesc(&adesc);
        printf("dx11 gpu: %ws\n", adesc.Description);
        break;
    }

    DXGI_SWAP_CHAIN_DESC scd;

    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.Width = 1024;
    scd.BufferDesc.Height = 768;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = windowHandle;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    auto featureLevel = D3D_FEATURE_LEVEL_11_1;

    result = D3D11CreateDeviceAndSwapChain(adpt,
                                           D3D_DRIVER_TYPE_UNKNOWN,
                                           NULL,
                                           D3D11_CREATE_DEVICE_DEBUG,
                                           &featureLevel,
                                           1,
                                           D3D11_SDK_VERSION,
                                           &scd,
                                           &swapChain,
                                           &device,
                                           NULL,
                                           &devCon);

    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&renderTargetTexture);

    device->CreateRenderTargetView(renderTargetTexture, NULL, &renderTargetTextureView);

    devCon->OMSetRenderTargets(1, &renderTargetTextureView, NULL);

    D3D11_TEXTURE2D_DESC sharedTextureDesc = {};
    sharedTextureDesc.Width = WINDOW_WIDTH;
    sharedTextureDesc.Height = WINDOW_HEIGHT;
    sharedTextureDesc.MipLevels = 1;
    sharedTextureDesc.ArraySize = 1;
    sharedTextureDesc.SampleDesc = {1, 0};
    sharedTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sharedTextureDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX | D3D11_RESOURCE_MISC_SHARED_NTHANDLE;

    result = device->CreateTexture2D(&sharedTextureDesc, NULL, &sharedTexture);
    result = sharedTexture->QueryInterface(__uuidof(IDXGIResource1), (void **)&sharedTextureResource);
    result = sharedTextureResource->CreateSharedHandle(NULL,
                                                       GENERIC_ALL,
                                                       NULL,
                                                       &sharedTextureHandle);

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = WINDOW_WIDTH;
    viewport.Height = WINDOW_HEIGHT;

    devCon->RSSetViewports(1, &viewport);
}

void DXSwapChain::CleanD3D()
{
    swapChain->Release();
    device->Release();
    devCon->Release();
}

void DXSwapChain::Frame()
{
    IDXGIKeyedMutex *km;
    auto hr = sharedTextureResource->QueryInterface(__uuidof(IDXGIKeyedMutex), (void **)&km);

    km->AcquireSync(0, INFINITE);
    devCon->CopyResource(renderTargetTexture, sharedTexture);
    km->ReleaseSync(0);
    swapChain->Present(1, 0);
}