#pragma once

#define ASSERT_SIZE(type, size) static_assert(sizeof(type) == size, "sizeof("#type")" " != " #size)

#define BITMASK_ENUM_OPERATORS(type) \
inline bool operator&(type lhs, type rhs) \
{ \
    return (uint32_t(lhs) & uint32_t(rhs)); \
} \
inline type operator|(type lhs, type rhs) \
{ \
    return type(uint32_t(lhs) | uint32_t(rhs)); \
} \
inline type operator^(type lhs, type rhs) \
{ \
    return type(uint32_t(lhs) ^ uint32_t(rhs)); \
} \
inline type operator~(type lhs) \
{ \
    return type(~uint32_t(lhs)); \
} \
inline type operator&=(type& lhs, type rhs) \
{ \
    lhs = type(uint32_t(lhs) & uint32_t(rhs)); \
    return lhs; \
} \
inline type operator|=(type& lhs, type rhs) \
{ \
    lhs = type(uint32_t(lhs) | uint32_t(rhs)); \
    return lhs; \
} \
inline type operator^=(type& lhs, type rhs) \
{ \
    lhs = type(uint32_t(lhs) ^ uint32_t(rhs)); \
    return lhs; \
}