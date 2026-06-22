#include "graphic/Renderer.hpp"

#include <stdexcept>

template<typename T>
inline void safeRelease(T **ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != nullptr)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = nullptr;
    }
}

namespace gauzy::graphic {
    Renderer::Renderer(type::WindowHandle handle) : windowHandle(handle)
    {
        D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pD2DFactory);

        pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(static_cast<HWND>(windowHandle)),
            &pRenderTarget);
        updateSize();

        DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&pDWriteFactory));

        if(pRenderTarget == nullptr || pDWriteFactory == nullptr)
        {
            throw std::runtime_error("Failed to intialize D2D");
        }
    }


    void Renderer::updateSize()
    {
        RECT clientRect;
        GetClientRect(static_cast<HWND>(windowHandle), &clientRect);

        auto size = D2D1::SizeU(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
        renderTargetSize = size;

        pRenderTarget->Resize(size);
    }

    type::SizeU Renderer::getSize() const
    {
        return renderTargetSize;
    }

    void Renderer::release()
    {
        safeRelease(&pRenderTarget);
        safeRelease(&pD2DFactory);
        safeRelease(&pDWriteFactory);
    }

    void Renderer::drawLine(const type::Position2F& start, const type::Position2F& end)
    {

    }

} // namespace gauzy::graphic
