#pragma once

#include "common.h"

typedef struct ByteBuffer ByteWriter;
typedef struct ByteBuffer ByteReader;

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 * Creates a reader from a byte writer
 * NOTE: this does not dispose the writer, for that call destroyWriter
 */
ByteReader* createReader(ByteWriter* writer);

/**
 * Creates a reader from a pointer to a byte array with a specified length
 */
ByteReader* createReaderFromPointer(byte* ptr, uint64_t length);

/**
 * Destroys a reader and free's up the data used
 * NOTE: this includes the underlying ByteBuffer
 */
void destroyReader(ByteReader* reader);

/**
 * Checkes whether the reader has a next byte to read
 */
bool hasNext(ByteReader* reader);

/**
 * Reads a single bit from the reader
 */
bool readBit(ByteReader* reader);

/**
 * Reads a single byte from the reader
 */
byte readByte(ByteReader* reader);

#ifdef __cplusplus
}
#endif