#include <stdlib.h>
#include "bytebuffer.h"

struct ByteBuffer {
    byte* head;
    uint64_t offset;
    byte bit_offset;
    uint64_t length;
};

/**
 * Dynamically scales up the internal buffer of the writer
 */
void dynamicScale(ByteWriter* writer, uint64_t length) {
    if(length < writer->length) 
        return;
    
    byte* head_ptr = calloc(writer->length * 2, sizeof(byte));
    for(uint64_t i = 0; i < writer->length; i++) 
        *(head_ptr + i) = *(writer->head + i);
    
    free(writer->head);
    writer->head = head_ptr;
    writer->length = writer->length * 2;
}

/**
 * Makes the memory allocated to the resuling byte pointer to be of the size of the length
 * NOTE: this does not free the given pointer
 */
byte* reduceMemoryProfile(byte* ptr, uint64_t length) {
    byte* result = calloc(length, sizeof(byte));

    for (uint64_t i = 0; i < length; i++)
        *(result + i) = *(ptr + i);
    
    return result;
}

void writeBit(ByteWriter* writer, bool value) {
    value = value > 0;

    uint8_t shift = (7u - writer->bit_offset);
    value <<= shift;
    byte* ptr = writer->head + writer->offset;
    *ptr |= value;

    writer->bit_offset++;
    if(writer->bit_offset > 7) {
        writer->bit_offset = 0;
        writer->offset++;
        dynamicScale(writer, writer->offset + 1);
    }
}

void writeByte(ByteWriter* writer, byte value) {
    for(uint8_t i = 0; i < 8; i++) {
        writeBit(writer, value & 0b01);
        value >>= 1;
    }
}

uint64_t getLength(ByteWriter* writer) {
    return writer->offset + 1;
}

ByteWriter* createByteWriter(uint64_t length) {
    byte* head = (byte*) calloc(length + 1, sizeof(byte));
    ByteWriter* result = (ByteWriter*) malloc(sizeof(ByteWriter));
    result->offset = 0;
    result->bit_offset = 0;
    result->head = head;
    result->length = length;
    return result;
}

void destroyWriter(ByteWriter* writer) {
    free(writer->head);
    free(writer);
}

ByteReader* createReader(ByteWriter* writer) {
    uint64_t length = getLength(writer);
    return createReaderFromPointer(reduceMemoryProfile(writer->head, length), length);
    // destroyWriter(writer);
}

ByteReader* createReaderFromPointer(byte* ptr, uint64_t length) {
    ByteReader* result = malloc(sizeof(ByteReader));
    result->head = ptr;
    result->offset = 0;
    result->bit_offset = 0;
    result->length = length;
    return result;
}

void destroyReader(ByteReader* reader) {
    free(reader->head);
    free(reader);
}

bool hasNext(ByteReader* reader) {
    return reader->offset < reader->length;
}

bool readBit(ByteReader* reader) {
    if(!hasNext(reader))
        return false;

    uint8_t shift = (7u - reader->bit_offset);
    byte mask = 1 << shift;

    byte* ptr = reader->head + reader->offset;
    byte result = (*ptr & mask) >> shift;

    reader->bit_offset++;
    if(reader->bit_offset > 7) {
        reader->bit_offset = 0;
        reader->offset++;
    }

    return result;
}

byte readByte(ByteReader* reader) {
    byte result = 0;

    for(uint8_t i = 0; i < 8; i++) {
        if(!hasNext(reader)) break;
        result |= readBit(reader) << i;
    }

    return result;
}