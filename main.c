#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "heap.h"
#include "hash.h"
#include "sharing.h"

int main() {
    int choice, id;
    Patient new_patient;

    // 병상 관리 변수 세팅
    const int MAX_BEDS = 100;
    int available_beds = MAX_BEDS;
    
    initHeap();
    initHash(); // 해시 테이블 초기화

    while (1) {
        printf("\n=========================================\n");
        printf("    스마트 응급실 환자 관리 시스템\n");
        printf("=========================================\n");
        printf("1. 신규 환자 접수\n");
        printf("2. 다음 환자 진료 및 병상 배정\n");
        printf("3. 환자/보호자용 대기 순서 조회\n");
        printf("4. 접수 취소\n");
        printf("5. 환자 응급도 변경\n");
        printf("6. 환자 퇴원 처리\n");
        printf("메뉴 선택: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("\n[신규 환자 접수]\n");
            printf("환자 ID(생년월일): ");
            scanf("%d", &new_patient.id);
            printf("이름: ");
            scanf("%s", new_patient.name);

            // KTAS 등급 입력
            printf("KTAS 등급 (1=최위급 ~ 5=비응급): ");
            int ktas_input;
            while (1) {
                scanf("%d", &ktas_input);
                if (ktas_input >= 1 && ktas_input <= 5) {
                    break; // 정상 범위 입력 시 루프 탈출
                }
                printf(">> 입력 오류!! KTAS 등급은 1에서 5 사이의 숫자만 가능합니다.\n");
                printf(">> KTAS 등급 재입력 (1~5): ");
            }

            // 세부 중증도 입력 및 예외 처리 (1~10)
            printf("세부 중증도 (1~10점): ");
            while (1) {
                scanf("%d", &new_patient.detail_score);
                if (new_patient.detail_score >= 1 && new_patient.detail_score <= 10) {
                    break; // 정상 범위 입력 시 루프 탈출
                }
                printf(">> [입력 오류] 세부 중증도는 1에서 10 사이의 숫자만 가능합니다.\n");
                printf(">> 세부 중증도 재입력 (1~10): ");
            }

            // 1단계가 점수가 높도록 역산 (1단계->5점, 5단계->1점)
            new_patient.ktas = (KtasLevel)(6 - ktas_input);
            new_patient.arrival_time = 0; // 실제 구현 시 time() 함수 사용 가능
            new_patient.status = WAITING;
            // 복합 우선순위 점수 계산 (중증도 우선) ***중증도 세분화로 수정***
            new_patient.priority_score = (new_patient.ktas * 10000) + (new_patient.detail_score * 100);

            insertHash(new_patient); // 해시에 O(1) 탐색용으로 저장
            insertHeap(new_patient); // 힙에 위급도 순서대로 삽입
            printf(">> %s 환자 접수 완료! (위급도 점수: %d)\n", new_patient.name, new_patient.priority_score);
            break;

        case 2:
            printf("\n[수동 병상 배정]\n");
            // 병상 여유가 있는지 먼저 체크 (음수 방지)
            if (available_beds <= 0) {
                printf(">> 가용 병상이 부족합니다! 퇴원 처리가 먼저 필요합니다.\n");
            }
            else {
                Patient target = extractMax();
                if (target.id == -1) {
                    printf(">> 대기 중인 환자가 없습니다.\n");
                }
                else {
                    available_beds--; // 병상 수 차감
                    printf(">> 병상 배정 완료: %s (ID: %d)\n", target.name, target.id);
                    Patient* p = searchHash(target.id);
                    if (p != NULL) p->status = TREATED;
                }
            }
            break;

        case 3:
            printf("\n[실시간 대기 순서 조회]\n");
            printf("조회할 환자 ID 입력: ");
            scanf("%d", &id);
            displayPatientOrder(id);
            break;

        case 4:
            printf("\n[대기 접수 취소]\n");
            printf("취소할 환자 ID 입력: ");
            scanf("%d", &id);
            cancelPatient(id);
            break;

        case 5:
            printf("\n[환자 응급도 변경]\n");
            printf("응급도를 수정할 환자 ID 입력: ");
            scanf("%d", &id);

            printf("새로운 KTAS 등급 (1~5): ");
            int new_ktas_input;
            scanf("%d", &new_ktas_input);
            KtasLevel new_ktas = (KtasLevel)(6 - new_ktas_input);

            printf("새로운 세부 중증도 (1~10점): ");
            int new_detail_score;
            scanf("%d", &new_detail_score);

            // 힙 재조정
            if (updatePatientSeverity(id, new_ktas, new_detail_score)) {
                // 힙이 업데이트 후 해시 테이블 원본 데이터도 동기화!
                Patient* target_hash = searchHash(id);
                if (target_hash != NULL) {
                    target_hash->ktas = new_ktas;
                    target_hash->detail_score = new_detail_score;
                    target_hash->priority_score = (new_ktas * 10000) + (new_detail_score * 100);
                }
                printf(">> 상태가 업데이트되어 대기열 우선순위가 재조정되었습니다.\n");
            }
            else {
                printf(">> 대기 중인 환자를 찾을 수 없습니다.\n");
            }
            break;

        case 6:
            printf("\n[환자 퇴원 및 병상 정리]\n");
            printf("퇴원할 환자 ID 입력: ");
            scanf("%d", &id);

            Patient* p = searchHash(id);
            if (p == NULL) {
                printf(">> 등록되지 않은 환자입니다.\n");
            }
            else if (p->status != TREATED) {
                printf(">> 병상에 배정된 환자가 아닙니다.\n");
            }
            else {
                // 1. 환자 퇴원 처리 및 빈병상 처리
                p->status = DISCHARGED;
                if (available_beds < MAX_BEDS) {
                    available_beds++; // 초과 방지
                }
                printf(">> %s 환자님의 퇴원 처리가 완료되었습니다.\n", p->name);

                // 2. 대기 환자 자동 재배정
                printf(">> 대기 환자 여부를 확인합니다...\n");
                Patient next_target = extractMax(); // 힙에서 1순위 대기자 추출

                if (next_target.id == -1) {
                    printf(">> 현재 대기 중인 환자가 없어 빈 병상으로 유지됩니다.\n");
                }
                else {
                    // 대기자가 있으면 방금 나온 병상에 즉시 투입
                    available_beds--;
                    Patient* next_p = searchHash(next_target.id);
                    if (next_p != NULL) next_p->status = TREATED;
                    printf(">> [자동 배정] 대기 중이던 1순위 환자가 병상에 배정되었습니다: %s (ID: %d)\n", next_target.name, next_target.id);
                }
            }
            break;

            return 0;

        default:
            printf(">> 잘못된 입력입니다. 다시 선택해주세요.\n");
        }
    }
}
