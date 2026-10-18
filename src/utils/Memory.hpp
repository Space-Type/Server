/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Memory utility functions
*/

#pragma once

#include <cstring>


namespace RType::Utils {
    /**
     * Copies the specified value into
     * the destination via memcpy().
     * The given destination buffer is
     * then advanced to the first byte
     * after the copied value.
     */
    template <typename Dest, typename Val>
    inline void copyFromValue(Dest*& dest, const Val& value) noexcept
    {
        std::memcpy(dest, &value, sizeof(Val));
        *reinterpret_cast<char**>(&dest) += sizeof(Val);
    }

    /**
     * Copies sizeof(Dest) bytes from
     * the given source to dest via
     * memcpy().
     * The given source pointer will
     * be advanced to the first byte
     * after the copied value.
     */
    template <typename Dest, typename Src>
    inline void copyToValue(Dest& dest, const Src*& src)
    {
        std::memcpy(&dest, src, sizeof(Dest));
        *reinterpret_cast<const char**>(&src) += sizeof(Dest);
    }
}
