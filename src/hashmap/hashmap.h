#pragma once

#include "common.h"
#include "bindtypes.h"

typedef uint64_t (*hashFunction)(char* key);
typedef void (*IteratorCallback)(char* key, BindElement data);
typedef void (*DestroyDataCallback)(BindElement data); 

typedef struct Entry {
    struct Entry* next;
    char* key;
    BindElement data;
} Entry;

typedef struct Node {
    struct Node* next;
    char key[];
} Node;

typedef struct HashMap HashMap;

HashMap* createHashMap(uint32_t key_space);

void insertData(HashMap* hashmap, char* key, BindElement data);
BindElement getData(HashMap* hashmap, char* key);
void iterate(HashMap* hashmap, IteratorCallback callback);
void removeData(HashMap* hashmap, char* key, DestroyDataCallback callback);
void clearHashMap(HashMap* hashmap, DestroyDataCallback callback);
void destroyHashMap(HashMap* hashmap, DestroyDataCallback callback);
bool hasKey(HashMap* hashmap, char* key);
Node* listKeys(HashMap* hashmap);
void freeNode(Node* node);