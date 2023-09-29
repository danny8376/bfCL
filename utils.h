#pragma once

#include <signal.h> // I sure do hope this works on MS VS; for use with signal handling.
#include <stdint.h>

// a crude cross Windows/POSIX high precision timer
#ifdef _WIN32

#include <Windows.h>
typedef LARGE_INTEGER TimeHP;
#define get_hp_time QueryPerformanceCounter

#ifdef __MINGW64__
#define LL "ll"
#else
#define LL "I64"
#endif

#else

#include <time.h>
typedef struct timespec TimeHP;
void get_hp_time(TimeHP *pt);

#define LL "ll"

#endif

long long hp_time_diff(TimeHP *pt0, TimeHP *pt1);

int hex2bytes(unsigned char *out, unsigned byte_len, const char *in, int critical);

const char * hexdump(const void *a, unsigned l, int space);

char * read_file(const char *file_name, size_t *p_size);

void read_files(unsigned num_files, const char *file_names[], char *ptrs[], size_t sizes[]);

void dump_to_file(const char *file_name, const void *buf, size_t len);

int cpu_has_rdrand();

int rdrand_fill(unsigned long long *p, size_t size);

char * trim(char *in);

#ifdef _BFCL_C
uint32_t stop_bfcl;

uint32_t seedminer_mode;

uint32_t worker_mode;

uint32_t stdio_mode;

uint32_t reduced_work_size_mode;
#else
extern uint32_t stop_bfcl;

extern uint32_t seedminer_mode;

extern uint32_t worker_mode;

extern uint32_t stdio_mode;

extern uint32_t reduced_work_size_mode;
#endif

void real_sleep(int sleep_sec);

void intHandler();
