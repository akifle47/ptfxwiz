#pragma once

#define ASSERT_SIZE(type, size) static_assert(sizeof(type) == size, "sizeof("#type")" " != " #size)