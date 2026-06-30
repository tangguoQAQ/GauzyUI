#include "graphic/Renderer.hpp"

#include <wil/result.h>

#include "comp/Component.hpp"

namespace gauzy::graphic {
    Renderer::Renderer(const type::WindowHandle& handle) : windowHandle(handle)
    {
        THROW_IF_FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pD2DFactory));

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

        const auto size = D2D1::SizeU(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
        renderTargetSize = size;

        THROW_IF_FAILED(pRenderTarget->Resize(size));
    }


    void Renderer::render(comp::Component& component)
    {
        pRenderTarget->BeginDraw();

        pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::BlanchedAlmond));
        component.render(*this);

        THROW_IF_FAILED(pRenderTarget->EndDraw());
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

    SolidColorBrush Renderer::registerSolidColorBrush(const SolidColorBrush& brush)
    {
        if(const std::size_t hash{ brush.hash() };
            brushMap.find(hash) == brushMap.end())
        {
            ID2D1SolidColorBrush* pBrush{ nullptr };
            THROW_IF_FAILED(pRenderTarget->CreateSolidColorBrush(static_cast<D2D1::ColorF>(brush.color),
                brush.toD2DProperties(), &pBrush));

            brushMap[hash] = pBrush;
        }

        return brush;
    }

    void Renderer::drawLine(const type::Position2F& start, const type::Position2F& end, const Brush& brush) const
    {
        const auto pBrush = brushMap.at(brush.hash());

        pRenderTarget->DrawLine(static_cast<D2D1_POINT_2F>(start),
            static_cast<D2D1_POINT_2F>(end),
            pBrush.get());
    }

} // namespace gauzy::graphic
