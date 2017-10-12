/* This file is part of the YAZ toolkit.
 * Copyright (C) Index Data
 * See the file LICENSE for details.
 */
#if HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <yaz/oid_util.h>
#include "test_odrcodec.h"

#include <yaz/test.h>

#define MYOID  "1.2.3.4.5.6.7.8.9.10.11.12.13.14.15.16.17.18.19"

void tst_MySequence1(ODR encode, ODR decode)
{
    int ret;
    char *ber_buf;
    int ber_len;
    Yc_MySequence *s = (Yc_MySequence *) odr_malloc(encode, sizeof(*s));
    Yc_MySequence *t;

    YAZ_CHECK(s);
    s->first = odr_intdup(encode, 12345);
    s->second = (Odr_oct *) odr_malloc(encode, sizeof(*s->second));
    s->second->buf = (char *) "hello";
    s->second->len = 5;
    s->third = odr_booldup(encode, 1);
    s->fourth = odr_nullval();
    s->fifth = odr_intdup(encode, YC_MySequence_enum1);

    s->myoid = odr_getoidbystr(decode, MYOID);

    ret = yc_MySequence(encode, &s, 0, 0);
    YAZ_CHECK(ret);
    if (!ret)
        return;

    ber_buf = odr_getbuf(encode, &ber_len, 0);

    odr_setbuf(decode, ber_buf, ber_len, 0);

    ret = yc_MySequence(decode, &t, 0, 0);
    YAZ_CHECK(ret);
    if (!ret)
        return;

    YAZ_CHECK(t);

    YAZ_CHECK(t->first && *t->first == 12345);

    YAZ_CHECK(t->second && t->second->buf && t->second->len == 5);

    YAZ_CHECK(t->second && t->second->buf && t->second->len == 5 &&
              memcmp(t->second->buf, "hello", t->second->len) == 0);

    YAZ_CHECK(t->third && *t->third == 1);

    YAZ_CHECK(t->fourth);

    YAZ_CHECK(t->fifth && *t->fifth == YC_MySequence_enum1);

    YAZ_CHECK(t->myoid);
    if (t->myoid)
    {
        Odr_oid *myoid = odr_getoidbystr(decode, MYOID);

        YAZ_CHECK(oid_oidcmp(myoid, t->myoid) == 0);
    }
}

void tst_MySequence2(ODR encode, ODR decode)
{
    int ret;
    Yc_MySequence *s = (Yc_MySequence *) odr_malloc(encode, sizeof(*s));

    YAZ_CHECK(s);
    s->first = 0;  /* deliberately miss this .. */
    s->second = (Odr_oct *) odr_malloc(encode, sizeof(*s->second));
    s->second->buf = (char *) "hello";
    s->second->len = 5;
    s->third = odr_booldup(encode, 1);
    s->fourth = odr_nullval();
    s->fifth = odr_intdup(encode, YC_MySequence_enum1);
    s->myoid = odr_getoidbystr(encode, MYOID);

    ret = yc_MySequence(encode, &s, 0, 0); /* should fail */
    YAZ_CHECK(!ret);

    YAZ_CHECK(odr_geterror(encode) == OREQUIRED);

    YAZ_CHECK(strcmp(odr_getelement(encode), "first") == 0);
    odr_reset(encode);

    YAZ_CHECK(odr_geterror(encode) == ONONE);

    YAZ_CHECK(strcmp(odr_getelement(encode), "") == 0);
}

void tst_MySequence3(ODR encode, ODR decode)
{
    char buf[40];
    int i;
    Yc_MySequence *t;

    srand(123);
    for (i = 0; i < 1000; i++)
    {
        int j;
        for (j = 0; j < (int) sizeof(buf); j++)
            buf[j] = rand();

        for (j = 1; j < (int) sizeof(buf); j++)
        {
            odr_setbuf(decode, buf, j, 0);
            yc_MySequence(decode, &t, 0, 0);
            odr_reset(decode);
        }
    }
}

static void tst_berint32(ODR encode, ODR decode)
{
    char *buf = 0;
    int len = 0;
    Odr_int val;
    Odr_int ret_val;
    int r;

    val = 0;
    odr_reset(encode);
    r = ber_integer(encode, &val);
    YAZ_CHECK_EQ(r, 1);
    buf = odr_getbuf(encode, &len, 0);
    YAZ_CHECK(buf);
    YAZ_CHECK_EQ(len, 2);
    YAZ_CHECK_EQ(buf[0], 1);
    YAZ_CHECK_EQ(buf[1], 0);

    odr_reset(decode);
    odr_setbuf(decode, buf, len, 0);
    ber_integer(decode, &ret_val);
    YAZ_CHECK_EQ(ret_val, 0);

    val = 1;
    odr_reset(encode);
    r = ber_integer(encode, &val);
    YAZ_CHECK_EQ(r, 1);
    buf = odr_getbuf(encode, &len, 0);
    YAZ_CHECK(buf);
    YAZ_CHECK_EQ(len, 2);
    YAZ_CHECK_EQ(buf[0], 1);
    YAZ_CHECK_EQ(buf[1], 1);

    odr_reset(decode);
    odr_setbuf(decode, buf, len, 0);
    ber_integer(decode, &ret_val);
    YAZ_CHECK_EQ(ret_val, 1);

    val = -1;
    odr_reset(encode);
    r = ber_integer(encode, &val);
    YAZ_CHECK_EQ(r, 1);
    buf = odr_getbuf(encode, &len, 0);
    YAZ_CHECK(buf);
    YAZ_CHECK_EQ(len, 2);
    YAZ_CHECK_EQ(buf[0], 1);
    YAZ_CHECK_EQ((unsigned char) buf[1], 255);

    odr_reset(decode);
    odr_setbuf(decode, buf, len, 0);
    ber_integer(decode, &ret_val);
    YAZ_CHECK_EQ(ret_val, -1);

    val = 127;
    odr_reset(encode);
    r = ber_integer(encode, &val);
    YAZ_CHECK_EQ(r, 1);
    buf = odr_getbuf(encode, &len, 0);
    YAZ_CHECK(buf);
    YAZ_CHECK_EQ(len, 2);
    YAZ_CHECK_EQ(buf[0], 1);
    YAZ_CHECK_EQ(buf[1], 127);

    odr_reset(decode);
    odr_setbuf(decode, buf, len, 0);
    ber_integer(decode, &ret_val);
    YAZ_CHECK_EQ(ret_val, 127);


    val = 128;
    odr_reset(encode);
    r = ber_integer(encode, &val);
    YAZ_CHECK_EQ(r, 1);
    buf = odr_getbuf(encode, &len, 0);
    YAZ_CHECK(buf);
    YAZ_CHECK_EQ(len, 3);
    YAZ_CHECK_EQ(buf[0], 2);
    YAZ_CHECK_EQ(buf[1], 0);
    YAZ_CHECK_EQ(((unsigned char *) buf)[2], 128);

    odr_reset(decode);
    odr_setbuf(decode, buf, len, 0);
    ber_integer(decode, &ret_val);
    YAZ_CHECK_EQ(ret_val, 128);

    val = 2147483647; /* 2^31-1 */
    odr_reset(encode);
    r = ber_integer(encode, &val);
    YAZ_CHECK_EQ(r, 1);
    buf = odr_getbuf(encode, &len, 0);
    YAZ_CHECK(buf);
    YAZ_CHECK_EQ(len, 5);
    YAZ_CHECK_EQ(buf[0], 4);
    YAZ_CHECK_EQ(buf[1], 127);
    YAZ_CHECK_EQ((unsigned char) buf[2], 255);
    YAZ_CHECK_EQ((unsigned char) buf[3], 255);
    YAZ_CHECK_EQ((unsigned char) buf[4], 255);

    odr_reset(decode);
    odr_setbuf(decode, buf, len, 0);
    ber_integer(decode, &ret_val);
    YAZ_CHECK_EQ(ret_val, 2147483647);

    val = -2147483647L -1; /* -2^31 */
    odr_reset(encode);
    r = ber_integer(encode, &val);
    YAZ_CHECK_EQ(r, 1);
    buf = odr_getbuf(encode, &len, 0);
    YAZ_CHECK(buf);
    YAZ_CHECK_EQ(len, 5);
    YAZ_CHECK_EQ(buf[0], 4);
    YAZ_CHECK_EQ((unsigned char) buf[1], 128);
    YAZ_CHECK_EQ(buf[2], 0);
    YAZ_CHECK_EQ(buf[3], 0);
    YAZ_CHECK_EQ(buf[4], 0);

    odr_reset(decode);
    odr_setbuf(decode, buf, len, 0);
    ber_integer(decode, &ret_val);
    YAZ_CHECK_EQ(ret_val, (Odr_int) -2147483647L -1);
}

static void tst_berint64(ODR encode, ODR decode)
{
#if NMEM_64
    char *buf = 0;
    int len = 0;
    Odr_int val;
    Odr_int ret_val;
    int r;

    val = (Odr_int) 2 * 2147483648UL; /* 2^32 */
    odr_reset(encode);
    r = ber_integer(encode, &val);
    YAZ_CHECK_EQ(r, 1);
    buf = odr_getbuf(encode, &len, 0);
    YAZ_CHECK(buf);
    YAZ_CHECK_EQ(len, 6);
    YAZ_CHECK_EQ(buf[0], 5);
    YAZ_CHECK_EQ(buf[1], 1);
    YAZ_CHECK_EQ(buf[2], 0);
    YAZ_CHECK_EQ(buf[3], 0);
    YAZ_CHECK_EQ(buf[4], 0);
    YAZ_CHECK_EQ(buf[5], 0);

    odr_reset(decode);
    odr_setbuf(decode, buf, len, 0);
    ber_integer(decode, &ret_val);
    YAZ_CHECK_EQ(ret_val, val);

    val = (Odr_int) -2 * 2147483648UL; /* -2^32 */
    odr_reset(encode);
    r = ber_integer(encode, &val);
    YAZ_CHECK_EQ(r, 1);
    buf = odr_getbuf(encode, &len, 0);
    YAZ_CHECK(buf);
    YAZ_CHECK_EQ(len, 6);
    YAZ_CHECK_EQ(buf[0], 5);
    YAZ_CHECK_EQ((unsigned char) buf[1], 255);
    YAZ_CHECK_EQ(buf[2], 0);
    YAZ_CHECK_EQ(buf[3], 0);
    YAZ_CHECK_EQ(buf[4], 0);
    YAZ_CHECK_EQ(buf[5], 0);

    odr_reset(decode);
    odr_setbuf(decode, buf, len, 0);
    ber_integer(decode, &ret_val);
    YAZ_CHECK_EQ(ret_val, val);

    val = (Odr_int) 1000 * 1000000000L; /* 10^12 */
    odr_reset(encode);
    r = ber_integer(encode, &val);
    YAZ_CHECK_EQ(r, 1);
    buf = odr_getbuf(encode, &len, 0);
    YAZ_CHECK(buf);
    YAZ_CHECK_EQ(len, 7);
    YAZ_CHECK_EQ(buf[0], 6);
    YAZ_CHECK_EQ(buf[1], 0);
    YAZ_CHECK_EQ((unsigned char) buf[2], 232);
    YAZ_CHECK_EQ((unsigned char) buf[3], 212);
    YAZ_CHECK_EQ((unsigned char) buf[4], 165);
    YAZ_CHECK_EQ(buf[5], 16);
    YAZ_CHECK_EQ(buf[6], 0);

    odr_reset(decode);
    odr_setbuf(decode, buf, len, 0);
    ber_integer(decode, &ret_val);
    YAZ_CHECK_EQ(ret_val, val);
#endif
}

static void tst(void)
{
    ODR odr_encode = odr_createmem(ODR_ENCODE);
    ODR odr_decode = odr_createmem(ODR_DECODE);

    YAZ_CHECK(odr_encode);
    YAZ_CHECK(odr_decode);

    tst_MySequence1(odr_encode, odr_decode);
    tst_MySequence2(odr_encode, odr_decode);
    tst_MySequence3(odr_encode, odr_decode);

    tst_berint32(odr_encode, odr_decode);
    tst_berint64(odr_encode, odr_decode);

    odr_destroy(odr_encode);
    odr_destroy(odr_decode);
}

/* example from documentation.. 'Using Odr' */
void do_nothing_useful(Odr_int value)
{
    ODR encode, decode;
    Odr_int *valp, *resvalp;
    char *bufferp;
    int len;

    /* allocate streams */
    if (!(encode = odr_createmem(ODR_ENCODE)))
        return;
    if (!(decode = odr_createmem(ODR_DECODE)))
        return;

    valp = &value;
    if (odr_integer(encode, &valp, 0, 0) == 0)
    {
        printf("encoding went bad\n");
        return;
    }
    bufferp = odr_getbuf(encode, &len, 0);
    printf("length of encoded data is %d\n", len);

    /* now let's decode the thing again */
    odr_setbuf(decode, bufferp, len, 0);
    if (odr_integer(decode, &resvalp, 0, 0) == 0)
    {
        printf("decoding went bad\n");
        return;
    }
    /* ODR_INT_PRINTF format for printf (such as %d) */
    printf("the value is " ODR_INT_PRINTF "\n", *resvalp);

    /* clean up */
    odr_destroy(encode);
    odr_destroy(decode);
}

int main(int argc, char **argv)
{
    YAZ_CHECK_INIT(argc, argv);
    tst();
    YAZ_CHECK_TERM;
}

/*
 * Local variables:
 * c-basic-offset: 4
 * c-file-style: "Stroustrup"
 * indent-tabs-mode: nil
 * End:
 * vim: shiftwidth=4 tabstop=8 expandtab
 */

