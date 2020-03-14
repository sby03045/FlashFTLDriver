#ifndef __H_BENCH__
#define __H_BENCH__
#include "../include/settings.h"
#include "../include/container.h"
#include "measurement.h"
#include <stdio.h>

#define PRINTPER 1
#define ALGOTYPE 300
#define LOWERTYPE 300
#define BENCHNUM 16

#ifdef CDF
#define TIMESLOT 10 //micro sec
#endif

#define BENCHSETSIZE (1024+1)
typedef struct{
	FSTYPE type;
	KEYT key;
	V_PTR value;
	uint32_t range;
	uint32_t length;
	int mark;
}bench_value;

typedef struct{
	uint32_t start;
	uint32_t end;
	uint64_t number;
	bench_type type;
}bench_meta;

typedef struct{
	uint64_t total_micro;
	uint64_t cnt;
	uint64_t max;
	uint64_t min;
}bench_ftl_time;

typedef struct{
	uint64_t algo_mic_per_u100[12];
	uint64_t lower_mic_per_u100[12];
	uint64_t algo_sec,algo_usec;
	uint64_t lower_sec,lower_usec;
#ifdef CDF
	uint64_t write_cdf[1000000/TIMESLOT+1];
	uint64_t read_cdf[1000000/TIMESLOT+1];
#endif
	uint64_t read_cnt,write_cnt;
	bench_ftl_time ftl_poll[ALGOTYPE][LOWERTYPE];
	MeasureTime bench;
}bench_data;

typedef struct{
	bench_value *body[BENCHSETSIZE];
	uint32_t bech;
	uint32_t benchsetsize;
	volatile uint64_t n_num;//request throw num
	volatile uint64_t m_num;
	volatile uint64_t r_num;//request end num
	bool finish;
	bool empty;
	int mark;
	uint64_t notfound;
	uint64_t write_cnt;
	uint64_t read_cnt;

	bench_type type;
	MeasureTime benchTime;
	MeasureTime benchTime2;
	uint64_t cache_hit;
}monitor;

typedef struct{
	int n_num;
	int m_num;
	monitor *m;
	bench_meta *meta;
	bench_data *datas;
	lower_info *li;
	uint32_t error_cnt;
}master;

void bench_init();
void bench_add(bench_type type,uint32_t start, uint32_t end,uint64_t number);
bench_value* get_bench();
void bench_refresh(bench_type, uint32_t start, uint32_t end, uint64_t number);
void bench_free();

void bench_print();
void bench_li_print(lower_info *,monitor *);
bool bench_is_finish_n(int n);
bool bench_is_finish();

void bench_cache_hit(int mark);
void bench_reap_data(request *const,lower_info *);
void bench_reap_nostart(request *const);
char *bench_lower_type(int);

void bench_custom_init(MeasureTime *mt, int idx);
void bench_custom_start(MeasureTime *mt,int idx);
void bench_custom_A(MeasureTime *mt,int idx);
void bench_custom_print(MeasureTime *mt, int idx);
int bench_set_params(int argc, char **argv,char **targv);

#ifdef CDF
void bench_cdf_print(uint64_t, uint8_t istype, bench_data*);
#endif
void bench_update_ftltime(bench_data *_d, request *const req);
void bench_type_cdf_print(bench_data *_d);
void free_bnech_all();
void free_bench_one(bench_value *);
#endif

void seqget(uint32_t, uint32_t,monitor *);
void seqset(uint32_t,uint32_t,monitor*);
void seqrw(uint32_t,uint32_t,monitor *);
void randget(uint32_t,uint32_t,monitor*);
void fillrand(uint32_t,uint32_t,monitor*);
void randset(uint32_t,uint32_t,monitor*);
void randrw(uint32_t,uint32_t,monitor*);
void mixed(uint32_t,uint32_t,int percentage,monitor*);
int my_itoa(uint32_t key, char **_target);