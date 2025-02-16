#include <stdlib.h>
#include <string.h>
#include "bindtypes.h"
#include "hashmap/hashmap.h"

BindElement createPrimitiveElement(PrimitiveType type, void* data) {
    BindElement element;
    element.type = PRIMITIVE | type << 2;
    element.data = data;
    return element;
}

bool isPrimitive(byte type) {
    return parseBindType(type) == PRIMITIVE;
}

BindType parseBindType(byte type) {
    return type & 0b11;
}

PrimitiveType parsePrimitiveType(byte type) {
    if(!isPrimitive(type)) return -1;
    return type >> 2;
}

BindType getElementType(BindElement element) {
    return parseBindType(element.type);
}

PrimitiveType getPrimitiveType(BindElement element) {
    return parsePrimitiveType(element.type);
}

BindElement createNullElement() {
    BindElement element;
    element.type = B_NULL;
    element.data = NULL;
    return element;
}

// Creating primitive elements
#pragma region 

BindElement createByteElement(byte value) {
    byte* data = (byte*) malloc(sizeof(byte));
    *data = value;
    return createPrimitiveElement(_byte, data);
}

BindElement createBoolElement(bool value) {
    bool* data = (bool*) malloc(sizeof(bool));
    *data = value;
    return createPrimitiveElement(_bool, data);
}

BindElement createShortElement(uint16_t value) {
    uint64_t* data = (uint64_t*) malloc(sizeof(uint64_t));
    *data = (uint64_t) value;
    return createPrimitiveElement(_int, data);
}

BindElement createIntElement(uint32_t value) {
    uint64_t* data = (uint64_t*) malloc(sizeof(uint64_t));
    *data = (uint64_t) value;
    return createPrimitiveElement(_int, data);
}

BindElement createLongElement(uint64_t value) {
    uint64_t* data = (uint64_t*) malloc(sizeof(uint64_t));
    *data = value;
    return createPrimitiveElement(_int, data);
}

BindElement createFloatElement(float value) {
    float* data = (float*) malloc(sizeof(float));
    *data = value;
    return createPrimitiveElement(_float, data);
}

BindElement createDoubleElement(double value) {
    double* data = (double*) malloc(sizeof(double));
    if(data == NULL) return createNullElement();

    *data = value;
    return createPrimitiveElement(_double, data);
}

BindElement createStringElement(char* value) {
    char* data = (char*) malloc(strlen(value) + 1);
    strcpy(data, value);
    return createPrimitiveElement(_string, data);
}

#pragma endregion

// Retrieving primitives
#pragma region 
byte asByte(BindElement element) {
    if(!isPrimitive(element.type) || getPrimitiveType(element) != _byte)
        return 0;
    return *((byte*)element.data);
}

bool asBoolean(BindElement element) {
    if(!isPrimitive(element.type) || getPrimitiveType(element) != _bool)
        return false;
    return *((bool*)element.data);
}

uint64_t asLong(BindElement element) {
    if(!isPrimitive(element.type) || getPrimitiveType(element) != _int)
        return 0l;
    return *((uint64_t*)element.data);
}

uint16_t asShort(BindElement element) {
    return (uint16_t) asLong(element);
}

uint32_t asInt(BindElement element) {
    return (uint32_t) asLong(element);
}

float asFloat(BindElement element) {
    if(!isPrimitive(element.type) || getPrimitiveType(element) != _float)
        return 0.0f;
    return *((float*)element.data);
}

double asDouble(BindElement element) {
    if(!isPrimitive(element.type) || getPrimitiveType(element) != _double)
        return 0.0;
    return *((double*)element.data);
}

char* asString(BindElement element) {
    if(!isPrimitive(element.type) || getPrimitiveType(element) != _string)
        return "";
    char* elementData = (char*)element.data;
    char* result = malloc(strlen(elementData));
    strcpy(result, elementData);
    return result;
}

#pragma endregion

// Array logic
#pragma region 
typedef struct {
    uint64_t length;
    BindElement* head;
} ArrayMeta;

BindElement createArrayElement(BindElement* data, uint64_t length) {
    BindElement element;
    element.type = ARRAY;

    ArrayMeta* meta = malloc(sizeof(ArrayMeta));
    meta->length = length;
    meta->head = length == 0 ? NULL : malloc(length * sizeof(BindElement));
    // Copy data into new memory location
    for(uint64_t i = 0; i < length; i++)
        *(meta->head + i) = *(data + i);

    element.data = meta;
    return element;
}

BindElement createEmptyArrayElement() {
    return createArrayElement(NULL, 0);
}

ArrayMeta* getArrayMeta(BindElement element) {
    if(element.type != ARRAY) return NULL;
    return (ArrayMeta*)element.data;
}

bool appendElementToArray(BindElement array, BindElement element) {
    ArrayMeta* meta = getArrayMeta(array);
    if(meta == NULL) return false;

    BindElement* old = meta->head;
    meta->head = (BindElement*) malloc(++meta->length * sizeof(BindElement));
    if(meta->head == NULL) {
        meta->head = old;
        return false;
    }
    
    for(int i = 0; i < meta->length - 1; i++)
        *(meta->head + i) = *(old + i);

    *(meta->head + (meta->length - 1)) = element;
    free(old);

    return true;
}

uint64_t getArrayLength(BindElement array) {
    ArrayMeta* meta = getArrayMeta(array);
    if(meta == NULL) return 0;

    return meta->length;
}

void iterateArray(BindElement array, Iterable callback) {
    ArrayMeta* meta = getArrayMeta(array);
    if(meta == NULL) return; 

    for(int i = 0; i < meta->length; i++)
        callback(*(meta->head + i));
}

BindElement* getArrayHead(BindElement array) {
    ArrayMeta* meta = getArrayMeta(array);
    if(meta == NULL) return NULL;
    return meta->head;
}

BindElement getElementAt(BindElement array, uint64_t index) {
    ArrayMeta* meta = getArrayMeta(array);
    if(meta == NULL || meta->length <= index) return createNullElement();

    return *(meta->head + index);
}

#pragma endregion

// Object Logic
#pragma region 

BindElement createObjectElement() {
    BindElement result;
    result.type = OBJECT;
    result.data = createHashMap(32);
    return result;
}

void addElementToObject(BindElement bindObject, char* key, BindElement element) {
    if(bindObject.type != OBJECT) return;
    insertData(bindObject.data, key, element);
}

BindElement getElementByKey(BindElement bindObject, char* key) {
    if(bindObject.type != OBJECT) return createNullElement();
    return getData(bindObject.data, key);
}

void iterateObject(BindElement bindObject, ObjectIterable iterableCallback) {
    if(bindObject.type != OBJECT) return;
    iterate(bindObject.data, iterableCallback);
}

ObjectEntry* getEntries(BindElement bindObject) {
    if(bindObject.type != OBJECT) return NULL;
    Node* listedNode = listKeys(bindObject.data);
    ObjectEntry* prev = NULL;
    while(listedNode != NULL) 
    {
        ObjectEntry* entry = (ObjectEntry*)malloc(sizeof(ObjectEntry));
        if(entry == NULL) return NULL;

        entry->key = listedNode->key;
        entry->data = getData(bindObject.data, entry->key);
        entry->next = prev;
        
        prev = entry;

        Node* tmp = listedNode;
        listedNode = tmp->next;
        free(tmp);
    }
    return prev;
}

void destroyObjectEntries(ObjectEntry* entries) {
    while(entries != NULL) {
        free(entries->key);
        
        ObjectEntry* tmp = entries;
        entries = entries->next;
        free(tmp);
    }
}

HashMap* getDataTable(BindElement bindObject) {
    if(bindObject.type != OBJECT) return NULL;
    return bindObject.data;
}

void removeElementByKey(BindElement bindObject, char* key) {
    if(bindObject.type != OBJECT) return;
    removeData(bindObject.data, key, destroyElement);
}

bool containsKey(BindElement bindObject, char* key) {
    if(bindObject.type != OBJECT) return false;
    return hasKey(bindObject.data, key);
}

#pragma endregion

void destroyElement(BindElement element) {
    switch (element.type)
    {
        case OBJECT:
        {
            if(element.data != NULL)
                destroyHashMap(element.data, destroyElement);
            return;
        }
        case ARRAY:
        {
            ArrayMeta* meta = getArrayMeta(element);
            if(meta != NULL && meta->head != NULL)
                for(int i = 0; i < meta->length; i++)
                    destroyElement(*(meta->head + i));
                
                free(meta->head);
            break;
        }
        default:
            break;
    }
    
    if(element.data != NULL)
        free(element.data);
}