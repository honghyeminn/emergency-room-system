#include "sharing.h"
#include "hash.h"
#include "heap.h"
#include <stdio.h>

void displayPatientOrder(int patient_id) {
    Patient* target = searchHash(patient_id);
    if (target == NULL) {
        printf(">> 등록되지 않은 환자 번호입니다.\n");
        return;
    }
    if (target->status == CANCELLED) {
        printf(">> 이미 접수가 취소된 환자입니다.\n");
        return;
    }
    if (target->status == TREATED) {
        printf(">> 병상 배정된 환자입니다.\n");
        return;
    }

    int higher_ktas_count = 0;
    int same_ktas_ahead_count = 0;

    // 힙 배열을 순회하며 앞순서 대기자 분석
    for (int i = 0; i < heapSize; i++) {
        if (heap[i].status != WAITING) continue;
        if (heap[i].id == patient_id) continue;

        if (heap[i].ktas > target->ktas) {
            higher_ktas_count++;
        }
        else if (heap[i].ktas == target->ktas && heap[i].priority_score > target->priority_score) {
            same_ktas_ahead_count++;
        }
    }

    printf("\n=========================================\n");
    printf("실시간 대기 현황 및 안내 (보호자용)\n");
    printf("=========================================\n");
    printf(" 환자명: %s (KTAS 등급: %d)\n", target->name, (6 - target->ktas));
    printf("-----------------------------------------\n");
    printf("나보다 위급한 중증 환자 수: %d명\n", higher_ktas_count);
    printf("동일 등급 내 먼저 접수된 대기자: %d명\n", same_ktas_ahead_count);
    printf("현재 예상 진료 순서: [ %d 번째 ]\n", higher_ktas_count + same_ktas_ahead_count + 1);
    printf("※ 응급실 특성상 위급 환자 유입 시 순서는 변동될 수 있습니다.\n");
    printf("=========================================\n");
}

int cancelPatient(int patient_id) {
    Patient* p = searchHash(patient_id);
    if (p == NULL) {
        printf(">> 해당 번호로 등록된 환자가 없습니다.\n");
        return 0;
    }

    if (p->status != WAITING) {
        printf(">> 취소할 수 없는 상태(이미 진료 중이거나 취소됨)입니다.\n");
        return 0;
    }

    // 1. 해시 테이블 원본의 상태를 취소로 변경
    p->status = CANCELLED;

    // 2. 힙(대기열) 내부의 복사본 데이터 상태도 동기화
    for (int i = 0; i < heapSize; i++) {
        if (heap[i].id == patient_id) {
            heap[i].status = CANCELLED;
            break;
        }
    }

    printf("%s 환자님의 대기 접수가 취소되었습니다.\n", p->name);
    return 1;
}
