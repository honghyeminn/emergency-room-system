#pragma once
#ifndef HASH_H
#define HASH_H

#include "common.h"

// 함수 선언
int hashFunction(int id);
void insertHash(Patient p);
Patient* searchHash(int id); // 환자 데이터의 주소(포인터)를 반환해서 실시간 상태 변경이 가능하도록 함

#endif
