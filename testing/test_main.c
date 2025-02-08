#include <assert.h>
#include "bytebuffer.h"

void MyTest() {
    ByteWriter* writer = createByteWriter(20);
    writeByte(writer, 0b0111101u);

    ByteReader* reader = createReader(writer);
    destroyWriter(writer);

    assert(readByte(reader) == 0b0111101u);
}

void main() {
    MyTest();
}
