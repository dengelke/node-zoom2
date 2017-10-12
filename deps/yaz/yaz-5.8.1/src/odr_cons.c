/* This file is part of the YAZ toolkit.
 * Copyright (C) Index Data
 * See the file LICENSE for details.
 */

/**
 * \file odr_cons.c
 * \brief Implements ODR constructed codec.
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>

#include "odr-priv.h"

void odr_setlenlen(ODR o, int len)
{
    o->op->lenlen = len;
}

int odr_constructed_begin(ODR o, void *xxp, int zclass, int tag,
                          const char *name)
{
    int res;
    int cons = 1;
    int lenlen = o->op->lenlen;

    if (o->error)
        return 0;
    o->op->lenlen = 1; /* reset lenlen */
    if (o->op->t_class < 0)
    {
        o->op->t_class = zclass;
        o->op->t_tag = tag;
    }
    res = ber_tag(o, xxp, o->op->t_class, o->op->t_tag, &cons, 1, name);
    if (res < 0)
        return 0;
    if (!res || !cons)
        return 0;

    /* push the odr_constack */
    if (o->op->stack_top && o->op->stack_top->next)
    {
        /* reuse old entry */
        o->op->stack_top = o->op->stack_top->next;
    }
    else if (o->op->stack_top && !o->op->stack_top->next)
    {
        /* must allocate new entry (not first) */
        int sz = 0;
        struct odr_constack *st;
        /* check size first */
        for (st = o->op->stack_top; st; st = st->prev)
            sz++;

        if (sz >= ODR_MAX_STACK)
        {
            odr_seterror(o, OSTACK, 30);
            return 0;
        }
        o->op->stack_top->next = (struct odr_constack *)
            odr_malloc(o, sizeof(*o->op->stack_top));
        o->op->stack_top->next->prev = o->op->stack_top;
        o->op->stack_top->next->next = 0;

        o->op->stack_top = o->op->stack_top->next;
    }
    else if (!o->op->stack_top)
    {
        /* stack empty */
        if (!o->op->stack_first)
        {
            /* first item must be allocated */
            o->op->stack_first = (struct odr_constack *)
                odr_malloc(o, sizeof(*o->op->stack_top));
            o->op->stack_first->prev = 0;
            o->op->stack_first->next = 0;
        }
        o->op->stack_top = o->op->stack_first;
        assert(o->op->stack_top->prev == 0);
    }
    o->op->stack_top->lenb = o->op->bp;
    o->op->stack_top->len_offset = odr_tell(o);
    o->op->stack_top->name = name ? name : "?";
    if (o->direction == ODR_ENCODE)
    {
        static char dummy[sizeof(int)+1];

        o->op->stack_top->lenlen = lenlen;

        if (odr_write(o, dummy, lenlen) < 0)  /* dummy */
        {
            ODR_STACK_POP(o);
            return 0;
        }
    }
    else if (o->direction == ODR_DECODE)
    {
        if ((res = ber_declen(o->op->bp, &o->op->stack_top->len,
                              odr_max(o))) < 0)
        {
            odr_seterror(o, OOTHER, 31);
            ODR_STACK_POP(o);
            return 0;
        }
        o->op->stack_top->lenlen = res;
        o->op->bp += res;
        if (o->op->stack_top->len > odr_max(o))
        {
            odr_seterror(o, OOTHER, 32);
            ODR_STACK_POP(o);
            return 0;
        }
    }
    else if (o->direction == ODR_PRINT)
    {
        odr_prname(o, name);
        odr_printf(o, "{\n");
        o->op->indent++;
    }
    else
    {
        odr_seterror(o, OOTHER, 33);
        ODR_STACK_POP(o);
        return 0;
    }
    o->op->stack_top->base = o->op->bp;
    o->op->stack_top->base_offset = odr_tell(o);
    return 1;
}

int odr_constructed_more(ODR o)
{
    if (o->error)
        return 0;
    if (ODR_STACK_EMPTY(o))
        return 0;
    if (o->op->stack_top->len >= 0)
        return o->op->bp - o->op->stack_top->base < o->op->stack_top->len;
    else
        return (!(*o->op->bp == 0 && *(o->op->bp + 1) == 0));
}

int odr_constructed_end(ODR o)
{
    int res;
    int pos;

    if (o->error)
        return 0;
    if (ODR_STACK_EMPTY(o))
    {
        odr_seterror(o, OOTHER, 34);
        return 0;
    }
    switch (o->direction)
    {
    case ODR_DECODE:
        if (o->op->stack_top->len < 0)
        {
            if (*o->op->bp++ == 0 && *(o->op->bp++) == 0)
            {
                ODR_STACK_POP(o);
                return 1;
            }
            else
            {
                odr_seterror(o, OOTHER, 35);
                return 0;
            }
        }
        else if (o->op->bp - o->op->stack_top->base !=
                 o->op->stack_top->len)
        {
            odr_seterror(o, OCONLEN, 36);
            return 0;
        }
        ODR_STACK_POP(o);
        return 1;
    case ODR_ENCODE:
        pos = odr_tell(o);
        odr_seek(o, ODR_S_SET, o->op->stack_top->len_offset);
        if ((res = ber_enclen(o, pos - o->op->stack_top->base_offset,
                              o->op->stack_top->lenlen, 1)) < 0)
        {
            odr_seterror(o, OLENOV, 37);
            return 0;
        }
        odr_seek(o, ODR_S_END, 0);
        if (res == 0)   /* indefinite encoding */
        {
            if (odr_putc(o, 0) < 0 || odr_putc(o, 0) < 0)
                return 0;
        }
        ODR_STACK_POP(o);
        return 1;
    case ODR_PRINT:
        ODR_STACK_POP(o);
        o->op->indent--;
        odr_prname(o, 0);
        odr_printf(o, "}\n");
        return 1;
    default:
        odr_seterror(o, OOTHER, 38);
        return 0;
    }
}
/*
 * Local variables:
 * c-basic-offset: 4
 * c-file-style: "Stroustrup"
 * indent-tabs-mode: nil
 * End:
 * vim: shiftwidth=4 tabstop=8 expandtab
 */

