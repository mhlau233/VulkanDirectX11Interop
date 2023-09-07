#include "DXSwapChain.h"
#include "VulkanContext.h"

int main()
{
    DXSwapChain dxSwapChain;
    dxSwapChain.InitWindow();
    dxSwapChain.InitD3D();

    VulkanContext vulkanContext;
    vulkanContext.Init(dxSwapChain.sharedTextureHandle);

    while (1)
    {
        MSG msg = {0};
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);

            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                break;
        }

        vulkanContext.drawFrame();

        dxSwapChain.Frame();
    }
    return 0;
}