/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- DbcsAttribute.hpp

Abstract:
- helper class for storing double byte character set information about a cell in the output buffer.

Author(s):
- Austin Diviness (AustDi) 26-Jan-2018

Revision History:
--*/

#pragma once

class DbcsAttribute
{
public:
    enum class Attribute : BYTE
    {
        Single = 0x00,
        Leading = 0x01,
        Trailing = 0x02
    };

    constexpr DbcsAttribute() noexcept :
        _attribute{ Attribute::Single }
    {
    }

    constexpr DbcsAttribute(const Attribute attribute) noexcept :
        _attribute{ attribute }
    {
    }

    constexpr bool IsSingle() const noexcept
    {
        return _attribute == Attribute::Single;
    }

    constexpr bool IsLeading() const noexcept
    {
        return _attribute == Attribute::Leading;
    }

    constexpr bool IsTrailing() const noexcept
    {
        return _attribute == Attribute::Trailing;
    }

    constexpr bool IsDbcs() const noexcept
    {
        return IsLeading() || IsTrailing();
    }

    void SetSingle() noexcept
    {
        _attribute = Attribute::Single;
    }

    void SetLeading() noexcept
    {
        _attribute = Attribute::Leading;
    }

    void SetTrailing() noexcept
    {
        _attribute = Attribute::Trailing;
    }

    void Reset() noexcept
    {
        SetSingle();
    }

    WORD GeneratePublicApiAttributeFormat() const noexcept
    {
        WORD publicAttribute = 0;
        if (IsLeading())
        {
            WI_SetFlag(publicAttribute, COMMON_LVB_LEADING_BYTE);
        }
        if (IsTrailing())
        {
            WI_SetFlag(publicAttribute, COMMON_LVB_TRAILING_BYTE);
        }
        return publicAttribute;
    }

    static DbcsAttribute FromPublicApiAttributeFormat(WORD publicAttribute)
    {
        // it's not valid to be both a leading and trailing byte
        if (WI_AreAllFlagsSet(publicAttribute, COMMON_LVB_LEADING_BYTE | COMMON_LVB_TRAILING_BYTE))
        {
            THROW_HR(E_INVALIDARG);
        }

        DbcsAttribute attr;
        if (WI_IsFlagSet(publicAttribute, COMMON_LVB_LEADING_BYTE))
        {
            attr.SetLeading();
        }
        else if (WI_IsFlagSet(publicAttribute, COMMON_LVB_TRAILING_BYTE))
        {
            attr.SetTrailing();
        }
        return attr;
    }

    friend constexpr bool operator==(const DbcsAttribute& a, const DbcsAttribute& b) noexcept;

private:
    Attribute _attribute;

#ifdef UNIT_TESTING
    friend class TextBufferTests;
#endif
};

constexpr bool operator==(const DbcsAttribute& a, const DbcsAttribute& b) noexcept
{
    return a._attribute == b._attribute;
}
