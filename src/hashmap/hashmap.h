#pragma once

#include "common.h"

typedef uint64_t (*hashFunction)(char* key);
typedef void (*IteratorCallback)(char* key, void* data);
typedef void (*DestroyDataCallback)(void* data); 

typedef struct Entry {
    struct Entry* next;
    char* key;
    void* data;
} Entry;

typedef struct Node {
    struct Node* next;
    char key[];
} Node;

typedef struct HashMap;

HashMap* createHashMap(uint32_t key_space);

void insertData(HashMap* hashmap, char* key, void* data);
void* getData(HashMap* hashmap, char* key);
void iterate(HashMap* hashmap, IteratorCallback callback);
void removeData(HashMap* hashmap, char* key, DestroyDataCallback callback);
void clearHashMap(HashMap* hashmap, DestroyDataCallback callback);
void destroyHashMap(HashMap* hashmap, DestroyDataCallback callback);
bool hasKey(HashMap* hashmap, char* key);
Node* listKeys(HashMap* hashmap);