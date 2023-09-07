#pragma once

#include <dxgi.h>
#include <d3d11_3.h>

class DXSwapChain
{
public:
    void InitWindow();

    void Frame();

    void InitD3D();

    HWND windowHandle;

    ID3D11DeviceContext *devCon;
    ID3D11Device *device;
    IDXGISwapChain *swapChain;
    ID3D11Texture2D *renderTargetTexture;
    ID3D11RenderTargetView *renderTargetTextureView;

    ID3D11Texture2D *sharedTexture;
    IDXGIResource1 *sharedTextureResource;
    HANDLE sharedTextureHandle;
};