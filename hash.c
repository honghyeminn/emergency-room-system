#include "hash.h"
#include <stdio.h>
#include <string.h>

#define TABLE_SIZE 100

// �ؽ� ���̺� �迭 (������ �迭 �ƴ�, ���� ����)
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

    // �� �ڸ��� ã�� ������ �̵�
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
            return &hashTable[index]; // �ּ� ��ȯ
        }
        index = (index + 1) % TABLE_SIZE;
        // �� ���� �� �������� ����
        if (index == startIndex) break;
    }
    return NULL; // �� ã���� ���
}
