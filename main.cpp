#include "DXSwapChain.h"

int main() {
    DXSwapChain dxSwapChain;
    dxSwapChain.InitWindow();
    dxSwapChain.InitD3D();
    dxSwapChain.Frame();
    return 0;
}