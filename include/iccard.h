#ifndef __ICCARD_H__
#define __ICCARD_H__
#include "main.h"


#define ICCARD_DATA_SIZE_PER_BLOCK 16
#define KEYA_SIZE 6
#define ICCARD_ID_SIZE 4

enum {
	SECTOR0 =0,//used
	SECTOR1,//used

	SECTOR2,//not used
	SECTOR3,
	SECTOR4,
	SECTOR5,
	SECTOR6,
	SECTOR7,
	SECTOR8,
	SECTOR9,
	SECTOR10,
	SECTOR11,
	SECTOR12,
	SECTOR13,
	SECTOR14,
	SECTOR15
};

enum{
	BLOCK0 = 0,
	BLOCK1 = 1,
	BLOCK2 = 2,

	BLOCK4 = 4,
	BLOCK5 = 5,
	BLOCK6 = 6
};

#pragma pack (1)/*byte alignment*/

typedef struct block0_data{
	u32 card_id;
	u8 reserved[12];
}b0_data_t;


typedef union block0{
	u8 b0_buf[ICCARD_DATA_SIZE_PER_BLOCK];
	b0_data_t b0_data;
}iccard_b0_u;

typedef struct iccard{
	iccard_b0_u b0;
	u8 block1[ICCARD_DATA_SIZE_PER_BLOCK];
	u8 block2[ICCARD_DATA_SIZE_PER_BLOCK];
	//u8 block3[ICCARD_DATA_SIZE_PER_BLOCK];
	u8 block4[ICCARD_DATA_SIZE_PER_BLOCK];
	u8 block5[ICCARD_DATA_SIZE_PER_BLOCK];
	u8 block6[ICCARD_DATA_SIZE_PER_BLOCK];
	//u8 block7[ICCARD_DATA_SIZE_PER_BLOCK];
}iccard_t;

#pragma pack()/*stop alignment*/


errno_t ic_spi_is_ok(void);

void test_ic_spi_is_ok(void);

void ic_check_spi(void);

void iccard_init(void);

void test_iccard_read_id(void);

errno_t get_iccard_data(iccard_t *ic);

void test_get_iccard_data(void);

void iccard_thread(void);


#endif
