#ifndef __GLOBAL
#define __GLOBAL

#include <Arduino.h>

// TODO : 클래스간 공유 데이터 부분 만들기 보다 그냥 간단하게 전역변수로 만들어서 사용하는게 나을듯 ( 싱글스레드에 곂칠일이 없음, data race가 일어날 가능성 없어보임.)
unsigned int led_loop_time = 50;

#endif
