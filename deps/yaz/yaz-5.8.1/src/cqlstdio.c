/* This file is part of the YAZ toolkit.
 * Copyright (C) Index Data
 * See the file LICENSE for details.
 */
/**
 * \file cqlstdio.c
 * \brief Implements query stream reading using FILE handle.
 */
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <yaz/cql.h>

static int getbyte_stream(void *client_data)
{
    FILE *f = (FILE*) client_data;

    int c = fgetc(f);
    if (c == EOF)
        return 0;
    return c;
}

static void ungetbyte_stream(int c, void *client_data)
{
    FILE *f = (FILE*) client_data;

    if (c == 0)
        c = EOF;
    ungetc(c, f);
}

int cql_parser_stdio(CQL_parser cp, FILE *f)
{
    return cql_parser_stream(cp, getbyte_stream, ungetbyte_stream, f);
}


/*
 * Local variables:
 * c-basic-offset: 4
 * c-file-style: "Stroustrup"
 * indent-tabs-mode: nil
 * End:
 * vim: shiftwidth=4 tabstop=8 expandtab
 */

