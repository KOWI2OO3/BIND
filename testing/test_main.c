#include <assert.h>
#include <stdio.h>
#include "bufferhelper.h"

void MyTest() {
    ByteWriter* writer = createByteWriter(20);
    writeByte(writer, (byte)0b0111101u);

    ByteReader* reader = createReader(writer);
    destroyWriter(writer);

    assert(readByte(reader) == (byte)0b0111101u);
    destroyReader(reader);
}

void BufferHelperPrimitiveInt() { 
    ByteWriter* writer = createByteWriter(20);
    writeVarInt(writer, 1);
    assert(getLength(writer) == 2);
    
    ByteReader* reader = createReader(writer);
    destroyWriter(writer);

    assert(readVarInt(reader) == 1);
    destroyReader(reader);
}

void BufferHelperPrimitiveBool() { 
    ByteWriter* writer = createByteWriter(20);
    writeBoolean(writer, 1);
    assert(getLength(writer) == 1);
    
    ByteReader* reader = createReader(writer);
    destroyWriter(writer);

    assert(readBoolean(reader) == 1);
    destroyReader(reader);
}

void BufferHelperPrimitiveFloat() { 
    ByteWriter* writer = createByteWriter(20);
    writeFloat(writer, 2.5f);
    assert(getLength(writer) == sizeof(float));
    
    ByteReader* reader = createReader(writer);
    destroyWriter(writer);

    float epsilon = (float)1e-3;

    float result = readFloat(reader);
    assert((2.5f - epsilon) <= result && result <= (2.5f + epsilon));
    destroyReader(reader);
}

void BufferHelperPrimitiveDouble() { 
    ByteWriter* writer = createByteWriter(20);
    writeDouble(writer, 5.3e5);
    assert(getLength(writer) == sizeof(double) + 1);
    
    ByteReader* reader = createReader(writer);
    destroyWriter(writer);

    double epsilon = 1e-5;

    double result = readDouble(reader);
    printf("%lf", result);
    assert(5.3e5 - epsilon < result && result < 5.3e5 + epsilon);
    destroyReader(reader);
}

int main() {
    MyTest();
    BufferHelperPrimitiveInt();
    BufferHelperPrimitiveBool();
    BufferHelperPrimitiveFloat();
    BufferHelperPrimitiveDouble();
    
    return 0;
}
