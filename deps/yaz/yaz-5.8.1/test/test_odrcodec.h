/** \file test_odrcodec.h
    \brief ASN.1 Module test_odrcodec

    Generated automatically by YAZ ASN.1 Compiler 0.4
*/

#ifndef test_odrcodec_H
#define test_odrcodec_H

#include <yaz/odr.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Yc_MySequence Yc_MySequence;
YAZ_EXPORT int yc_MySequence(ODR o, Yc_MySequence **p, int opt, const char *name);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
struct Yc_MySequence {
	Odr_int *first;
	Odr_oct *second;
	Odr_bool *third;
	Odr_null *fourth;
#define YC_MySequence_enum1 0
#define YC_MySequence_enum2 1
	Odr_int *fifth;
	Odr_oid *myoid;
};

#ifdef __cplusplus
}
#endif
#endif
