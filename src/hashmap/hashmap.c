#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

typedef struct HashMap {
    uint32_t key_space;
    hashFunction hash_function;
    Entry* buckets[];
} HashMap;

uint64_t hash(char* key) {
    if(key == NULL) return 0;

    uint64_t result = 0;

    for(char* ptr = key; *ptr != '\0'; ptr++)
        result += (uint64_t)*ptr;

    return result;
}

HashMap* createHashMap(uint32_t key_space) {
    if(key_space <= 0) return NULL;

    HashMap* result = malloc(sizeof(Entry) * key_space + sizeof(HashMap));
    if(result == NULL) return NULL;

    result->key_space = key_space;
    result->hash_function = hash;
    for(uint32_t i = 0; i < key_space; i++)
        result->buckets[i] = NULL;

    return result;
}

void setHashFunction(HashMap* hashmap, hashFunction hashFunction) {
    hashmap->hash_function = hashFunction;
}

Entry* getEntry(HashMap* hashmap, char* key) {
    uint64_t hash = hashmap->hash_function(key);
    uint32_t index = (uint32_t) (hash % hashmap->key_space);

    Entry* entry = hashmap->buckets[index];
    while(entry != NULL && strcmp(entry->key, key) != 0)
        entry = entry->next;
    
    return entry;
}

void insertData(HashMap* hashmap, char* key, BindElement data) {
    if(hashmap == NULL || key == NULL) return;

    uint64_t hash = hashmap->hash_function(key);
    uint32_t index = (uint32_t) (hash % hashmap->key_space);

    Entry* entry = getEntry(hashmap, key);

    // Handle key not in bucket
    if(entry == NULL) {
        if((entry = (Entry*)malloc(sizeof(Entry))) == NULL)
            return;
        
        entry->next = hashmap->buckets[index];
        entry->data = data;
        entry->key = malloc(strlen(key) + 1);
        strcpy(entry->key, key);

        hashmap->buckets[index] = entry;
    }

    // Resolve collision
    else {
        // Override data as the only handling of insert
        entry->data = data;
    }
}

BindElement getData(HashMap* hashmap, char* key) {
    if(hashmap == NULL || key == NULL) return createNullElement();
    Entry* entry = getEntry(hashmap, key);
    return entry != NULL ? entry->data : createNullElement();
}

void iterate(HashMap* hashmap, IteratorCallback callback) {
    if(hashmap == NULL || callback == NULL) return;

    for(uint32_t i = 0; i < hashmap->key_space; i++) {
        for(Entry* entry = hashmap->buckets[i]; entry != NULL; entry = entry->next) {
            callback(entry->key, entry->data);
        }
    }
}

void removeData(HashMap* hashmap, char* key, DestroyDataCallback callback) {
    if(hashmap == NULL || callback == NULL || key == NULL) return;

    uint64_t hash = hashmap->hash_function(key);
    uint32_t index = (uint32_t) (hash % hashmap->key_space);

    Entry* prev = NULL;
    Entry* entry = hashmap->buckets[index];
    while(entry != NULL && strcmp(entry->key, key) != 0) {
        prev = entry;
        entry = entry->next;
    }

    if(entry == NULL) return;

    // Remove node from linked list
    if(hashmap->buckets[index] == entry) {
        hashmap->buckets[index] = NULL;
    }else {
        prev->next = entry->next;
        entry->next = NULL;
    }

    // free data
    callback(entry->data);
    free(entry->key);
    free(entry);
}

void clearHashMap(HashMap* hashmap, DestroyDataCallback callback) {
    if(hashmap == NULL || callback == NULL) return;

    for(uint32_t i = 0; i < hashmap->key_space; i++) {
        Entry* entry = hashmap->buckets[i];
        while(entry != NULL) {
            Entry* current = entry;
            entry = entry->next;

            callback(current->data);
            free(current->key);
            free(current);
        } 
        hashmap->buckets[i] = NULL;
    }
}

void destroyHashMap(HashMap* hashmap, DestroyDataCallback callback) {
    if(hashmap == NULL) return;

    clearHashMap(hashmap, callback);
    free(hashmap);
}

bool hasKey(HashMap* hashmap, char* key) {
    if(hashmap == NULL || key == NULL) return false;
    Entry* entry = getEntry(hashmap, key);
    return entry != NULL;
}

Node* listKeys(HashMap* hashmap) {
    if(hashmap == NULL) return NULL;

    Node* node = NULL;

    for(uint32_t i = 0; i < hashmap->key_space; i++) {
        for(Entry* entry = hashmap->buckets[i]; entry != NULL; entry = entry->next) {
            Node* tmp = malloc(sizeof(Node));
            if(tmp == NULL) continue;

            tmp->key = (char*)calloc(strlen(entry->key) + 1, sizeof(char));

            tmp->next = node;
            strcmp(tmp->key, entry->key);
            node = tmp;
        }
    }

    return node;
}

void freeNode(Node* node) {
    while(node != NULL) {
        free(node->key);
        Node* tmp = node;
        node = node->next;
        free(tmp);
    }
}