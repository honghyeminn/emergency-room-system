#pragma once
#ifndef COMMON_H
#define COMMON_H

typedef enum { WAITING, TREATED, CANCELLED, DISCHARGED } PatientStatus;
typedef enum { KTAS_1 = 5, KTAS_2 = 4, KTAS_3 = 3, KTAS_4 = 2, KTAS_5 = 1 } KtasLevel;

typedef struct {
    int id;
    char name[20];
    KtasLevel ktas;
    int detail_score;       // 피드백 반영 사항: 동일 KTAS 내 세부 중증도 (1~10점)
    int arrival_time;
    int priority_score;     // 점수 공식: (KTAS * 10000) + (detail_score * 100)
    PatientStatus status;
} Patient;

#endif
