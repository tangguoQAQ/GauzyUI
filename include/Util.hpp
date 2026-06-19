#pragma once

#include <string>

#include <Windows.h>

namespace gauzy::util
{
    std::wstring utf8ToWide(const std::string& utf8);

    std::string wideToUtf8(const std::wstring& wide);

} // namespace gauzy::util
