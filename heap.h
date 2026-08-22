#pragma once
#ifndef HEAP_H
#define HEAP_H

#include "common.h" // Patient 구조체를 사용하기 위해 인클루드

// Max Heap 전역 변수 선언 (extern을 써서 heap.c에 있는 변수를 공유)
extern Patient heap[];
extern int heapSize;

// 함수 선언
void initHeap();
void insertHeap(Patient p);
Patient extractMax();
void applyAging();
int updatePatientSeverity(int patient_id, KtasLevel new_ktas, int new_detail_score);

#endif
