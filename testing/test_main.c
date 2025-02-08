#include <stdio.h>
#include <assert.h>
#include "bytebuffer.h"

void MyTest() {
    ByteWriter* writer = createByteWriter(20);
    writeByte(writer, 0b0111101);

    ByteReader* reader = createReader(writer);
    destroyWriter(writer);

    assert(readByte(reader) == 0b0111101);
}

void main() {
    MyTest();
}
