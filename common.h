#pragma once
#ifndef COMMON_H
#define COMMON_H

typedef enum { WAITING, TREATED, CANCELLED, DISCHARGED } PatientStatus;
typedef enum { KTAS_1 = 5, KTAS_2 = 4, KTAS_3 = 3, KTAS_4 = 2, KTAS_5 = 1 } KtasLevel;

typedef struct {
    int id;
    char name[20];
    KtasLevel ktas;
    int detail_score;       // �ǵ�� �ݿ� ����: ���� KTAS �� ���� ������ (1~10��)
    int arrival_time;
    int priority_score;     // ���� ����: (KTAS * 10000) + (detail_score * 100)
    PatientStatus status;
} Patient;

#endif
