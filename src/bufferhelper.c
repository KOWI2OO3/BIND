#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bufferhelper.h"

#define CHAR_BIT 8

void writeVarInt(ByteWriter* writer, uint64_t value) {
    byte bits = (byte) ceil(log2((double)(value + 1)));
    writeByte(writer, bits);

    for(byte i = 0; i < bits; i++) {
        writeBit(writer, value & 0b1);
        value >>= 1;
    }
}

uint64_t readVarInt(ByteReader* reader) {
    byte bits = readByte(reader);

    uint64_t result = 0;
    for(byte i = 0; i < bits; i++)
        result |= ((uint64_t) readBit(reader)) << i;
    
    return result;
}

void writeBoolean(ByteWriter* writer, bool value) {
    writeBit(writer, value);
}

bool readBoolean(ByteReader* reader) {
    return readBit(reader);
}

typedef union
{
    float f;
    uint32_t u;
} ufloat;

void writeFloat(ByteWriter* writer, float value) {
    ufloat fu = { .f = value };
    uint32_t i = sizeof(float) * CHAR_BIT - 1;

    while(i--)
        writeBit(writer, fu.u >> i & 0x1);
}

float readFloat(ByteReader* reader) {
    ufloat result = { .f = 0.0f };
    uint32_t i = sizeof(float) * CHAR_BIT - 1;

    while(i--)
        result.u |= readBit(reader) << i;

    return result.f;
}

typedef union
{
    double d;
    uint64_t u;
} udouble;

void writeDouble(ByteWriter* writer, double value) {
    udouble du = { .d = value };
    uint32_t i = sizeof(double) * CHAR_BIT;

    while(i--){
        writeBit(writer, du.u & 0b1);
        du.u >>= 1;
    }
}

double readDouble(ByteReader* reader) {
    udouble result = { .d = 0.0f };
    uint32_t bits = sizeof(double) * CHAR_BIT;

    for(uint32_t i = 0; i < bits; i++)
        result.u |= ((uint64_t) readBit(reader)) << i;

    return result.d;
}

void writeString(ByteWriter* writer, char* s) {
    while(*s != '\0') {
       writeByte(writer, *s);
       s++;
   }
}

char* readString(ByteReader* reader) {
    uint64_t offset = 0;
    uint64_t count = 20;
    char* result = (char*)calloc(count, sizeof(char));
    char c = readByte(reader);
    while(c != '\0') {
        // append to string
        if(offset > count) {
            // Reallocate
            count *= 2;
            char* tmp = calloc(count, sizeof(char));
            strcpy_s(tmp, strlen(result), result);
            free(result);
            result = tmp;
        }

        *(result + offset++) = c;

        c = readByte(reader);
    }
    return result;
}
