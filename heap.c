#include "heap.h"
#include <stdio.h>

#define MAX_HEAP 100

Patient heap[MAX_HEAP];
int heapSize = 0;

void initHeap() { heapSize = 0; }

void swap(Patient* a, Patient* b) {
    Patient temp = *a;
    *a = *b;
    *b = temp;
}

//  Re-heapify Up (위로 끌어올리기) 함수 분리
void reheapifyUp(int current) {
    while (current > 0) {
        int parent = (current - 1) / 2;
        if (heap[current].priority_score > heap[parent].priority_score) {
            swap(&heap[current], &heap[parent]);
            current = parent;
        }
        else {
            break;
        }
    }
}

// Re-heapify Down (아래로 내리기) 함수 분리
void reheapifyDown(int current) {
    while (current * 2 + 1 < heapSize) {
        int left = current * 2 + 1;
        int right = current * 2 + 2;
        int largest = current;

        if (heap[left].priority_score > heap[largest].priority_score)
            largest = left;
        if (right < heapSize && heap[right].priority_score > heap[largest].priority_score)
            largest = right;

        if (largest != current) {
            swap(&heap[current], &heap[largest]);
            current = largest;
        }
        else {
            break;
        }
    }
}

void insertHeap(Patient p) {
    if (heapSize >= MAX_HEAP) {
        printf("대기열이 가득 찼습니다!\n");
        return;
    }
    heap[heapSize] = p;
    reheapifyUp(heapSize); // 분리한 함수 호출
    heapSize++;
}

Patient extractMax() {
    Patient empty_patient = { -1, "", 0, 0, 0, 0, CANCELLED };
    while (heapSize > 0) {
        Patient maxPatient = heap[0];
        heap[0] = heap[heapSize - 1];
        heapSize--;
        reheapifyDown(0); // 분리한 함수 호출

        if (maxPatient.status != CANCELLED) {
            return maxPatient;
        }
    }
    return empty_patient;
}

// 접수된 환자의 응급도 변화 시 특정 환자의 위치를 재조정하는 함수
int updatePatientSeverity(int patient_id, KtasLevel new_ktas, int new_detail_score) {
    int target_idx = -1;

    // 1. 힙 배열에서 대상 환자의 인덱스 찾기
    for (int i = 0; i < heapSize; i++) {
        if (heap[i].id == patient_id && heap[i].status == WAITING) {
            target_idx = i;
            break;
        }
    }
    if (target_idx == -1) return 0; // 대기 중인 환자를 못 찾음

    int old_score = heap[target_idx].priority_score;
    int new_score = (new_ktas * 10000) + (new_detail_score * 100);

    // 2. 점수 업데이트
    heap[target_idx].ktas = new_ktas;
    heap[target_idx].detail_score = new_detail_score;
    heap[target_idx].priority_score = new_score;

    // 3. 점수가 올랐으면 Up, 내렸으면 Down하여 힙 구조 복구
    if (new_score > old_score) {
        reheapifyUp(target_idx);
    }
    else if (new_score < old_score) {
        reheapifyDown(target_idx);
    }
    return 1;
}
