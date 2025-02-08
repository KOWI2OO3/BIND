#include <assert.h>
#include "bytebuffer.h"

void MyTest() {
    ByteWriter* writer = createByteWriter(20);
    writeByte(writer, (byte)0b0111101u);

    ByteReader* reader = createReader(writer);
    destroyWriter(writer);

    assert(readByte(reader) == (byte)0b0111101u);
    destroyReader(reader);
}

int main() {
    MyTest();
    
    return 0;
}
