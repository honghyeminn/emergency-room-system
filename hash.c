#include "hash.h"
#include <stdio.h>
#include <string.h>

#define TABLE_SIZE 100

// 해시 테이블 배열 (포인터 배열 아님, 직접 저장)
Patient hashTable[TABLE_SIZE];

void initHash() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i].id = -1;
    }
}

int hashFunction(int id) {
    return id % TABLE_SIZE;
}

void insertHash(Patient p) {
    int index = hashFunction(p.id);

    // 빈 자리를 찾을 때까지 이동
    while (hashTable[index].id != -1) {
        index = (index + 1) % TABLE_SIZE;
    }
    hashTable[index] = p;
}

Patient* searchHash(int id) {
    int index = hashFunction(id);
    int startIndex = index;

    while (hashTable[index].id != -1) {
        if (hashTable[index].id == id) {
            return &hashTable[index]; // 주소 반환
        }
        index = (index + 1) % TABLE_SIZE;
        // 한 바퀴 다 돌았으면 종료
        if (index == startIndex) break;
    }
    return NULL; // 못 찾았을 경우
}
