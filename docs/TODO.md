### gauzy::type::UiString 宽窄双缓冲
Renderer::drawText —— utf8ToWide 在每次渲染调用时都进行分配
文件: src/graphic/Renderer.cpp:113
const auto wideText = util::utf8ToWide(text);
标签文本在每次 render() 调用时都会被转换为宽字符串（包括分配）。如果标签不经常改变，将 UTF-8 到宽字符串的转换结果缓存到 Label 或 TextFormat 中可以避免这种重复分配。
