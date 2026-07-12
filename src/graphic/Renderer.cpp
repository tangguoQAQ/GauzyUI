#include "graphic/Renderer.hpp"

#include <wil/result.h>

#include "comp/Component.hpp"
#include "Util.hpp"

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

    const wil::com_ptr<ID2D1Brush>& Renderer::getOrCreateBrush(const Brush& brush) const
    {
        const std::size_t hash{ brush.hash() };
        if(auto it = brushMap.find(hash); it != brushMap.end())
        {
            return it->second;
        }

        if(const auto* solidBrush = dynamic_cast<const SolidColorBrush*>(&brush))
        {
            ID2D1SolidColorBrush* pBrush{ nullptr };
            THROW_IF_FAILED(pRenderTarget->CreateSolidColorBrush(static_cast<D2D1::ColorF>(solidBrush->color),
                brush.toD2DProperties(), &pBrush));

            return brushMap[hash] = pBrush;
        }
        else
        {
            throw std::runtime_error("Unsupported brush type.");
        }
    }

    const wil::com_ptr<IDWriteTextFormat>& Renderer::getOrCreateTextFormat(const TextFormat& textFormat) const
    {
        const std::size_t hash{ textFormat.hash() };
        if(auto it = textFormatCache.find(hash); it != textFormatCache.end())
        {
            return it->second;
        }

        IDWriteTextFormat* pTextFormat{ nullptr };
        const auto wideFontFamily = util::utf8ToWide(textFormat.fontFamily);
        const auto wideLocaleName = util::utf8ToWide(textFormat.localeName);
        THROW_IF_FAILED(pDWriteFactory->CreateTextFormat(
            wideFontFamily.c_str(),
            nullptr,
            textFormat.fontWeight,
            textFormat.fontStyle,
            textFormat.fontStretch,
            textFormat.fontSize,
            wideLocaleName.c_str(),
            &pTextFormat));

        return textFormatCache[hash] = pTextFormat;
    }

    void Renderer::drawLine(const type::Position2F& start, const type::Position2F& end,
        const Brush& brush) const
    {
        const auto pBrush = getOrCreateBrush(brush);

        pRenderTarget->DrawLine(static_cast<D2D1_POINT_2F>(start),
            static_cast<D2D1_POINT_2F>(end),
            pBrush.get());
    }

    void gauzy::graphic::Renderer::drawText(const type::Position2F& position, const std::string& text,
        const Brush& brush, const TextFormat& textFormat) const
    {
        const auto pBrush = getOrCreateBrush(brush);
        const auto pTextFormat = getOrCreateTextFormat(textFormat);

        const auto wideText = util::utf8ToWide(text);
        const D2D1_RECT_F layoutRect{ position.x(), position.y(),
            static_cast<FLOAT>(renderTargetSize.x()), static_cast<FLOAT>(renderTargetSize.y()) };

        pRenderTarget->DrawText(wideText.c_str(), static_cast<UINT32>(wideText.length()),
            pTextFormat.get(), &layoutRect, pBrush.get());
    }

    void Renderer::drawRect(const type::Position2F & topLeft, const type::SizeF & size,
        const Brush & brush) const
    {
        const auto pBrush = getOrCreateBrush(brush);

        const D2D1_RECT_F rect{ topLeft.x(), topLeft.y(),
            topLeft.x() + size.x(), topLeft.y() + size.y() };

        pRenderTarget->DrawRectangle(&rect, pBrush.get());
    }

    void Renderer::fillRect(const type::Position2F& topLeft, const type::SizeF& size, const Brush& brush) const
    {
        const auto pBrush = getOrCreateBrush(brush);

        const D2D1_RECT_F rect{ topLeft.x(), topLeft.y(),
            topLeft.x() + size.x(), topLeft.y() + size.y() };

        pRenderTarget->FillRectangle(&rect, pBrush.get());
    }

} // namespace gauzy::graphic
