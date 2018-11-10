#include "stm32f4xx.h"
#include "stm32f4xx_hal_conf.h"

#define SINE_SAMPLES 256

uint16_t data_sample = 256;

uint16_t sine_lookup[SINE_SAMPLES] = {

		2048,2098,2148,2198,2248,2298,2348,2398,2447,2496,2545,2594,2642,2690,2737,2784,
		2831,2877,2923,2968,3013,3057,3100,3143,3185,3226,3267,3307,3346,3385,3423,3459,
		3495,3530,3565,3598,3630,3662,3692,3722,3750,3777,3804,3829,3853,3876,3898,3919,
		3939,3958,3975,3992,4007,4021,4034,4045,4056,4065,4073,4080,4085,4089,4093,4094,
		4095,4094,4093,4089,4085,4080,4073,4065,4056,4045,4034,4021,4007,3992,3975,3958,
		3939,3919,3898,3876,3853,3829,3804,3777,3750,3722,3692,3662,3630,3598,3565,3530,
		3495,3459,3423,3385,3346,3307,3267,3226,3185,3143,3100,3057,3013,2968,2923,2877,
		2831,2784,2737,2690,2642,2594,2545,2496,2447,2398,2348,2298,2248,2198,2148,2098,
		2048,1997,1947,1897,1847,1797,1747,1697,1648,1599,1550,1501,1453,1405,1358,1311,
		1264,1218,1172,1127,1082,1038,995,952,910,869,828,788,749,710,672,636,
		600,565,530,497,465,433,403,373,345,318,291,266,242,219,197,176,
		156,137,120,103,88,74,61,50,39,30,22,15,10,6,2,1,
		0,1,2,6,10,15,22,30,39,50,61,74,88,103,120,137,
		156,176,197,219,242,266,291,318,345,373,403,433,465,497,530,565,
		600,636,672,710,749,788,828,869,910,952,995,1038,1082,1127,1172,1218,
		1264,1311,1358,1405,1453,1501,1550,1599,1648,1697,1747,1797,1847,1897,1947,1997};

//Tabla del seno como complemento a 2. Para ser transmitido por I2S.

uint16_t SINE_COMP[256] = {

   0x0000, 0x0324, 0x0647, 0x096a, 0x0c8b, 0x0fab, 0x12c8, 0x15e2,
   0x18f8, 0x1c0b, 0x1f19, 0x2223, 0x2528, 0x2826, 0x2b1f, 0x2e11,
   0x30fb, 0x33de, 0x36ba, 0x398c, 0x3c56, 0x3f17, 0x41ce, 0x447a,
   0x471c, 0x49b4, 0x4c3f, 0x4ebf, 0x5133, 0x539b, 0x55f5, 0x5842,
   0x5a82, 0x5cb4, 0x5ed7, 0x60ec, 0x62f2, 0x64e8, 0x66cf, 0x68a6,
   0x6a6d, 0x6c24, 0x6dca, 0x6f5f, 0x70e2, 0x7255, 0x73b5, 0x7504,
   0x7641, 0x776c, 0x7884, 0x798a, 0x7a7d, 0x7b5d, 0x7c29, 0x7ce3,
   0x7d8a, 0x7e1d, 0x7e9d, 0x7f09, 0x7f62, 0x7fa7, 0x7fd8, 0x7ff6,
   0x7fff, 0x7ff6, 0x7fd8, 0x7fa7, 0x7f62, 0x7f09, 0x7e9d, 0x7e1d,
   0x7d8a, 0x7ce3, 0x7c29, 0x7b5d, 0x7a7d, 0x798a, 0x7884, 0x776c,
   0x7641, 0x7504, 0x73b5, 0x7255, 0x70e2, 0x6f5f, 0x6dca, 0x6c24,
   0x6a6d, 0x68a6, 0x66cf, 0x64e8, 0x62f2, 0x60ec, 0x5ed7, 0x5cb4,
   0x5a82, 0x5842, 0x55f5, 0x539b, 0x5133, 0x4ebf, 0x4c3f, 0x49b4,
   0x471c, 0x447a, 0x41ce, 0x3f17, 0x3c56, 0x398c, 0x36ba, 0x33de,
   0x30fb, 0x2e11, 0x2b1f, 0x2826, 0x2528, 0x2223, 0x1f19, 0x1c0b,
   0x18f8, 0x15e2, 0x12c8, 0x0fab, 0x0c8b, 0x096a, 0x0647, 0x0324,
   0x0000, 0xfcdc, 0xf9b9, 0xf696, 0xf375, 0xf055, 0xed38, 0xea1e,
   0xe708, 0xe3f5, 0xe0e7, 0xdddd, 0xdad8, 0xd7da, 0xd4e1, 0xd1ef,
   0xcf05, 0xcc22, 0xc946, 0xc674, 0xc3aa, 0xc0e9, 0xbe32, 0xbb86,
   0xb8e4, 0xb64c, 0xb3c1, 0xb141, 0xaecd, 0xac65, 0xaa0b, 0xa7be,
   0xa57e, 0xa34c, 0xa129, 0x9f14, 0x9d0e, 0x9b18, 0x9931, 0x975a,
   0x9593, 0x93dc, 0x9236, 0x90a1, 0x8f1e, 0x8dab, 0x8c4b, 0x8afc,
   0x89bf, 0x8894, 0x877c, 0x8676, 0x8583, 0x84a3, 0x83d7, 0x831d,
   0x8276, 0x81e3, 0x8163, 0x80f7, 0x809e, 0x8059, 0x8028, 0x800a,
   0x8000, 0x800a, 0x8028, 0x8059, 0x809e, 0x80f7, 0x8163, 0x81e3,
   0x8276, 0x831d, 0x83d7, 0x84a3, 0x8583, 0x8676, 0x877c, 0x8894,
   0x89bf, 0x8afc, 0x8c4b, 0x8dab, 0x8f1e, 0x90a1, 0x9236, 0x93dc,
   0x9593, 0x975a, 0x9931, 0x9b18, 0x9d0e, 0x9f14, 0xa129, 0xa34c,
   0xa57e, 0xa7be, 0xaa0b, 0xac65, 0xaecd, 0xb141, 0xb3c1, 0xb64c,
   0xb8e4, 0xbb86, 0xbe32, 0xc0e9, 0xc3aa, 0xc674, 0xc946, 0xcc22,
   0xcf05, 0xd1ef, 0xd4e1, 0xd7da, 0xdad8, 0xdddd, 0xe0e7, 0xe3f5,
   0xe708, 0xea1e, 0xed38, 0xf055, 0xf375, 0xf696, 0xf9b9, 0xfcdc
};
