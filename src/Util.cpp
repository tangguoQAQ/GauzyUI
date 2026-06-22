#include "Util.hpp"

namespace gauzy::util
{
    std::wstring utf8ToWide(const std::string& utf8)
    {
        if(utf8.empty())
        {
            return std::wstring{};
        }
        int size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
        std::wstring result(size - 1, 0);  // -1 排除结尾 \0
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, result.data(), size);

        return result;
    }

    std::string wideToUtf8(const std::wstring& wide)
    {
        if(wide.empty())
        {
            return std::string{};
        }
        int size = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string result(size - 1, 0);
        WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, result.data(), size, nullptr, nullptr);

        return result;
    }

} // namespace gauzy::util
