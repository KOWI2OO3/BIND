#pragma once

#include "common.h"

struct ByteBuffer;

typedef struct ByteBuffer ByteWriter;
typedef struct ByteBuffer ByteReader;

/**
 * Create a byte writer of the specified length in bytes
 */
ByteWriter* createByteWriter(uint64_t length);

/**
 * Destroys a writer and free's up the data used
 * NOTE: this includes the underlying ByteBuffer
 */
void destroyWriter(ByteWriter* writer);

/**
 * Writes a single bit to the buffer
 */
void writeBit(ByteWriter* writer, bool value);

/**
 * Writes a byte to the buffer
 */
void writeByte(ByteWriter* writer, byte value);

/**
 * Gets the used space of the buffer in bytes
 */
uint64_t getLength(ByteWriter* writer);

ByteReader* createReader(ByteWriter* writer);
ByteReader* createReaderFromPointer(byte* ptr, uint64_t length);
void destroyReader(ByteReader* reader);
bool hasNext(ByteReader* reader);
bool readBit(ByteReader* reader);
byte readByte(ByteReader* reader);