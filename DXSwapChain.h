#pragma once

#include <d3d11.h>
#include <dxgi.h>

class DXSwapChain
{
public:
    void InitWindow();

    void Frame();

    void InitD3D();
    void CleanD3D();

    HWND windowHandle;

    ID3D11DeviceContext *devCon;
    ID3D11Device *device;
    IDXGISwapChain *swapChain;
    ID3D11Texture2D *renderTargetTexture;
    ID3D11RenderTargetView *renderTargetTextureView;
};