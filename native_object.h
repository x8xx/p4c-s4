#ifndef BACKENDS_S4_SWITCH_NATIVE_OBJECT_H_
#define BACKENDS_S4_SWITCH_NATIVE_OBJECT_H_

#include <cstdint>

namespace S4 {
struct native_object {
    const uint8_t* obj;
    uint64_t size;
};
}

#endif
