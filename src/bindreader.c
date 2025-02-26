#include <stdlib.h>
#include "bindreader.h"

BindElement readPrimitive(ByteReader* reader, PrimitiveType type) {
    switch (type)
    {
        case _byte:
            return createByteElement(readByte(reader));
        case _bool:
            return createBoolElement(readBoolean(reader));
        case _int:
            return createLongElement(readVarInt(reader));
        case _float:
            return createFloatElement(readFloat(reader));
        case _double:
            return createDoubleElement(readDouble(reader));
        case _string:
        {
            char* s = readString(reader);
            BindElement result = createStringElement(s);
            free(s);
            return result;
        }
        default:
            break;
    }
    return createNullElement();
}

BindElement readArray(ByteReader* reader) {
    uint64_t length = readVarInt(reader);

    BindElement* tmp = (BindElement*) calloc(length, sizeof(BindElement));
    if(tmp == NULL) return createNullElement();

    for(int i = 0; i < length; i++)
        *(tmp + i) = readElement(reader);

    BindElement result = createArrayElement(tmp, length);
    free(tmp);
    
    return result;
}

BindElement readObject(ByteReader* reader) {
    uint64_t count = readVarInt(reader);
    BindElement result = createObjectElement();
    while(count--) {
        char* key = readString(reader);
        BindElement element = readElement(reader);
        addElementToObject(result, key, element);
        free(key);
    }
    return result;
}

BindElement readElement(ByteReader* reader) {
    byte rawType = readByte(reader);
    BindType type = parseBindType(rawType);
    switch (type)
    {
        case PRIMITIVE:
            return readPrimitive(reader, parsePrimitiveType(rawType));
        case ARRAY:
            return readArray(reader);
        case OBJECT:
            return readObject(reader);
        case B_NULL:
            return createNullElement();
        default:
            break;
    }

    return createNullElement();
}