#pragma once

#include "bindtypes.h"
#include "bufferhelper.h"

#ifdef __cplusplus
extern "C" {
#endif

void writeElement(ByteWriter* writer, BindElement element);

#ifdef __cplusplus
}
#endif