#include "bindwriter.h"

void writePrimitive(ByteWriter* writer, PrimitiveType type, void* data) {
    switch (type)
    {
        case _byte:
            writeByte(writer, *((byte*)data));
            break;
        case _bool:
            writeBoolean(writer, *((bool*)data));
            break;
        case _int:
            writeVarInt(writer, *((uint64_t*)data));
            break;
        case _float:
            writeFloat(writer, *((float*)data));
            break;
        case _double:
            writeDouble(writer, *((double*)data));
            break;
        case _string:
            writeString(writer, (char*)data);
            break;
        default:
            break;
    }
}

void writeArray(ByteWriter* writer, BindElement array) {
    uint64_t length = getArrayLength(array);
    writeVarInt(writer, length);
    BindElement* head = getArrayHead(array);
    for(int i = 0; i < length; i++)
        writeElement(writer, *(head + i));
}

void writeElement(ByteWriter* writer, BindElement element) {
    BindType type = getElementType(element);
    switch (type)
    {
        case PRIMITIVE:
            writeByte(writer, element.type);
            writePrimitive(writer, getPrimitiveType(element), element.data);
            break;
        case ARRAY:
            writeByte(writer, element.type);
            writeArray(writer, element);
            break;
        case B_NULL:
            writeByte(writer, element.type);
            break;
        default:
            break;
    }
}