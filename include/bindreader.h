#pragma once

#include "bindtypes.h"
#include "bufferhelper.h"

#ifdef __cplusplus
extern "C" {
#endif

BindElement readElement(ByteReader* reader);

#ifdef __cplusplus
}
#endif