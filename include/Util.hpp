#pragma once

#include <string>
#include <string_view>

#include <Windows.h>

#include "type/BasicTypes.hpp"

namespace gauzy::util
{
    std::wstring utf8ToWide(const std::string& utf8);
    std::wstring utf8ToWide(const std::string_view& utf8);

    std::string wideToUtf8(const std::wstring& wide);
} // namespace gauzy::util
