#pragma once

extern "C++"
{
    template<typename T, size_t Size>
    struct ARRAY_CONTAINER
    {
        T arr[Size];
    };
}