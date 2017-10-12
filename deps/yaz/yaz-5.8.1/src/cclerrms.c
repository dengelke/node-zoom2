/* This file is part of the YAZ toolkit.
 * Copyright (C) Index Data
 * See the file LICENSE for details.
 */
/**
 * \file cclerrms.c
 * \brief Implements CCL error code to error string map.
 *
 * This source file implements mapping between CCL error code and
 * their string equivalents.
 */
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <yaz/ccl.h>

static char *err_msg_array[] = {
    "Ok",
    "Search word expected",
    "')' expected",
    "Set name expected",
    "Operator expected",
    "Unbalanced ')'",
    "Unknown qualifier",
    "Qualifiers applied twice",
    "'=' expected",
    "Bad relation",
    "Left truncation not supported",
    "Both left - and right truncation not supported",
    "Right truncation not supported",
    "Embedded truncation not supported",
    "Single character mask not supported"
};

const char *ccl_err_msg(int ccl_errno)
{
    if (ccl_errno >= 0 &&
        ccl_errno < sizeof(err_msg_array)/sizeof(*err_msg_array))
        return err_msg_array[ccl_errno];
    else
        return 0;
}
/*
 * Local variables:
 * c-basic-offset: 4
 * c-file-style: "Stroustrup"
 * indent-tabs-mode: nil
 * End:
 * vim: shiftwidth=4 tabstop=8 expandtab
 */

