#include "graphic/Renderer.hpp"

#include <stdexcept>
#include <wil/result.h>

#include "comp/Component.hpp"

namespace gauzy::graphic {
    Renderer::Renderer(type::WindowHandle handle) : windowHandle(handle)
    {
        D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pD2DFactory);

        THROW_IF_FAILED(pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(static_cast<HWND>(windowHandle)),
            &pRenderTarget));
        updateSize();

        THROW_IF_FAILED(DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&pDWriteFactory)));
    }


    void Renderer::updateSize()
    {
        RECT clientRect;
        GetClientRect(static_cast<HWND>(windowHandle), &clientRect);

        auto size = D2D1::SizeU(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
        renderTargetSize = size;

        pRenderTarget->Resize(size);
    }


    void Renderer::render(comp::Component& component)
    {
        pRenderTarget->BeginDraw();

        pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::BlanchedAlmond));
        component.render(*this);

        pRenderTarget->EndDraw();
    }

    void Renderer::release()
    {
        pRenderTarget.reset();
        pDWriteFactory.reset();
        pD2DFactory.reset();
    }

    type::SizeU Renderer::getSize() const
    {
        return renderTargetSize;
    }

    void Renderer::drawLine(const type::Position2F& start, const type::Position2F& end)
    {

    }

} // namespace gauzy::graphic
