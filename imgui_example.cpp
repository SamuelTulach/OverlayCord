/*
 * This code contains code snippets that you can copy over into DX11 ImGui example project
 * Keep in mind that this implementation is not the most optimised one (repetitive calls, whole buffer copy)
 */

void SendBuffer(IDXGISwapChain* pSwapChain)
{
    ID3D11Device* pDevice;
    pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&pDevice));

    ID3D11DeviceContext* pContext;
    pDevice->GetImmediateContext(&pContext);

    ID3D11Texture2D* pBackBuffer;
    pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));

    D3D11_TEXTURE2D_DESC txtDesc;
    pBackBuffer->GetDesc(&txtDesc);
    txtDesc.Usage = D3D11_USAGE_STAGING;
    txtDesc.BindFlags = 0;
    txtDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    ID3D11Texture2D* pBackBufferStaging;
    HRESULT status = pDevice->CreateTexture2D(&txtDesc, nullptr, &pBackBufferStaging);
    pContext->CopyResource(pBackBufferStaging, pBackBuffer);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    status = g_pd3dDeviceContext->Map(pBackBufferStaging, 0, D3D11_MAP_READ, 0, &mappedResource);

    unsigned int bufferSize = targetViewportHeight * mappedResource.RowPitch;
    memcpy(mainHeader->ActualBuffer, mappedResource.pData, bufferSize);

    mainHeader->Framecount++;

    g_pd3dDeviceContext->Unmap(pBackBufferStaging, 0);

    pBackBufferStaging->Release();
    pBackBuffer->Release();
}

bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // Don't forget to use the correct colorspace
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    /* ... */
}
