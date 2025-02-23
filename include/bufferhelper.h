#pragma once

#include "bytebuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

void writeVarInt(ByteWriter* writer, uint64_t value);
uint64_t readVarInt(ByteReader* reader);

void writeBoolean(ByteWriter* writer, bool value);
bool readBoolean(ByteReader* reader);

void writeFloat(ByteWriter* writer, float value);
float readFloat(ByteReader* reader);

void writeDouble(ByteWriter* writer, double value);
double readDouble(ByteReader* reader);

void writeString(ByteWriter* writer, char* s);
char* readString(ByteReader* reader);


#ifdef __cplusplus
}
#endif