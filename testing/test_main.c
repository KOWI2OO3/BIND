#include <assert.h>
#include <stdio.h>
#include "bind.h"

void MyTest() {
    ByteWriter* writer = createByteWriter(20);
    writeByte(writer, (byte)0b0111101u);

    ByteReader* reader = createReader(writer);
    destroyWriter(writer);

    assert(readByte(reader) == (byte)0b0111101u);
    destroyReader(reader);
}

void ReadingEmptyBuffer() {
    ByteWriter* writer = createByteWriter(0);
    ByteReader* reader = createReader(writer);
    destroyWriter(writer);
    readByte(reader);
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

void BufferHelperPrimitiveInt2() { 
    ByteWriter* writer = createByteWriter(20);
    writeVarInt(writer, 2);
    assert(getLength(writer) == 2);
    
    ByteReader* reader = createReader(writer);
    destroyWriter(writer);

    assert(readVarInt(reader) == 2);
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

void BindBoolElement() {
    ByteWriter* writer = createByteWriter(20);

    BindElement element = createBoolElement(true);
    writeElement(writer, element);
    
    ByteReader* reader = createReader(writer);
    destroyWriter(writer);
    destroyElement(element);

    element = readElement(reader);
    assert(getElementType(element) == PRIMITIVE);

    bool data = asBoolean(element);
    assert(data == true);

    destroyElement(element);
    destroyReader(reader);
}

void BindDoubleElement() {
    ByteWriter* writer = createByteWriter(20);

    BindElement element = createDoubleElement(5.6e9);
    writeElement(writer, element);
    
    ByteReader* reader = createReader(writer);
    destroyWriter(writer);
    destroyElement(element);

    element = readElement(reader);
    assert(getElementType(element) == PRIMITIVE);

    double data = asDouble(element);
    assert(data == 5.6e9);

    destroyElement(element);
    destroyReader(reader);
}

void BindArrayTest() {
    ByteWriter* writer = createByteWriter(20);

    BindElement element = createDoubleElement(5.6e9);
    BindElement element2 = createDoubleElement(7.3);
    BindElement array = createEmptyArrayElement();
    appendElementToArray(array, element);
    appendElementToArray(array, element2);

    // array length should be 2 at this point
    assert(getArrayLength(array) == 2);

    writeElement(writer, array);
    destroyElement(array);
    
    ByteReader* reader = createReader(writer);
    destroyWriter(writer);

    array = readElement(reader);

    // Checking if both elements are read again
    assert(getArrayLength(array) == 2);

    element = getElementAt(array, 0);
    assert(asDouble(element) == 5.6e9);
    element2 = getElementAt(array, 1);
    assert(asDouble(element2) == 7.3);
    
    destroyElement(array);
    destroyReader(reader);
}


int main() {
    MyTest();
    ReadingEmptyBuffer();
    BufferHelperPrimitiveInt();
    BufferHelperPrimitiveInt2();
    BufferHelperPrimitiveBool();
    BufferHelperPrimitiveFloat();
    BufferHelperPrimitiveDouble();

    BindBoolElement();
    BindDoubleElement();
    BindArrayTest();
    
    return 0;
}
