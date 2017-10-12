/* This file is part of the YAZ toolkit.
 * Copyright (C) Index Data
 * See the file LICENSE for details.
 */
/**
 * \file odr_tag.c
 * \brief Implements ODR IMPLICIT tagging codec
 */
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "odr-priv.h"

int odr_peektag(ODR o, int *zclass, int *tag, int *cons)
{
    if (o->direction != ODR_DECODE)
    {
        odr_seterror(o, OOTHER, 48);
        return 0;
    }
    if (ODR_STACK_NOT_EMPTY(o) && !odr_constructed_more(o))
        return 0;
    if (ber_dectag(o->op->bp, zclass, tag, cons, odr_max(o)) <= 0)
    {
        odr_seterror(o, OREQUIRED, 49);
        return 0;
    }
    return 1;
}

int odr_implicit_settag(ODR o, int zclass, int tag)
{
    if (o->error)
        return 0;
    if (o->op->t_class < 0)
    {
        o->op->t_class = zclass;
        o->op->t_tag = tag;
    }
    return 1;
}

int odr_initmember(ODR o, void *p, int size)
{
    char **pp = (char **) p;

    if (o->error)
        return 0;
    if (o->direction == ODR_DECODE)
        *pp = (char *)odr_malloc(o, size);
    else if (!*pp)
    {
        o->op->t_class = -1;
        return 0;
    }
    return 1;
}
/*
 * Local variables:
 * c-basic-offset: 4
 * c-file-style: "Stroustrup"
 * indent-tabs-mode: nil
 * End:
 * vim: shiftwidth=4 tabstop=8 expandtab
 */

