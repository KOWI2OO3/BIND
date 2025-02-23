#pragma once
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PRIMITIVE,
    OBJECT,
    ARRAY,
    B_NULL
} BindType;

typedef enum {
    _byte,
    _bool,
    _int,
    _float,
    _double,
    _string
} PrimitiveType;

typedef struct {
    uint8_t type;
    void* data;
} BindElement;

typedef struct ObjectEntry {
    char* key;
    BindElement data;
    struct ObjectEntry* next;
} ObjectEntry;

typedef void (*Iterable)(BindElement element);
typedef void (*ObjectIterable)(char* key, BindElement element);

BindType parseBindType(byte type);
PrimitiveType parsePrimitiveType(byte type);

BindType getElementType(BindElement element);
PrimitiveType getPrimitiveType(BindElement element);

/**
 * Creates a null element, signifying there is no data
 */
BindElement createNullElement();

BindElement createByteElement(byte value);
BindElement createBoolElement(bool value);
BindElement createShortElement(uint16_t value);
BindElement createIntElement(uint32_t value);
BindElement createLongElement(uint64_t value);
BindElement createFloatElement(float value);
BindElement createDoubleElement(double value);

/**
 * Creates a primitive bind element of string type.
 * 
 * NOTE: the pointer given as parameter will not be freed and 
 * it will only be copied into a different memory location for the element
 */
BindElement createStringElement(char* value);

byte asByte(BindElement element);
bool asBoolean(BindElement element);
uint64_t asLong(BindElement element);
uint16_t asShort(BindElement element);
uint32_t asInt(BindElement element);
float asFloat(BindElement element);
double asDouble(BindElement element);

/**
 * Gets the value of the bind element as a string.
 * NOTE: the pointer returned is a copy not the original data
 */
char* asString(BindElement element);

BindElement createArrayElement(BindElement* data, uint64_t length);
BindElement createEmptyArrayElement();
bool appendElementToArray(BindElement array, BindElement element);

uint64_t getArrayLength(BindElement array);
BindElement* getArrayHead(BindElement array); 
void iterateArray(BindElement array, Iterable callback);
BindElement getElementAt(BindElement array, uint64_t index);

BindElement createObjectElement();
void addElementToObject(BindElement bindObject, char* key, BindElement element);
BindElement getElementByKey(BindElement bindObject, char* key);
void iterateObject(BindElement bindObject, ObjectIterable iterableCallback);
ObjectEntry* getEntries(BindElement bindObject);
void destroyObjectEntries(ObjectEntry* entries);
void removeElementByKey(BindElement bindObject, char* key);
bool containsKey(BindElement bindObject, char* key);

void destroyElement(BindElement element);

#ifdef __cplusplus
}
#endif