/* This file is part of the YAZ toolkit.
 * Copyright (C) Index Data
 * See the file LICENSE for details.
 */

#include <stdio.h>
#include <string.h>

#include <yaz/nmem.h>
#include <yaz/xmalloc.h>
#include <yaz/zoom.h>

const char *my_callback(void *handle, const char *name)
{
    if (!strcmp(name, "async"))
        return "1";
    return 0;
}

int main(int argc, char **argv)
{
    int i;
    int no = argc - 4;
    ZOOM_connection z[500]; /* allow at most 500 connections */
    ZOOM_resultset r[500];  /* and result sets .. */
    ZOOM_query q;
    ZOOM_options o;

    o = ZOOM_options_create();
    if (argc < 5)
    {
        fprintf(stderr, "usage:\n%s target1 .. targetN query strategy sort\n",
                *argv);
        exit(2);
    }
    if (no > 500)
        no = 500;

    /* function my_callback called when reading options .. */
    ZOOM_options_set_callback(o, my_callback, 0);

    /* get 20 (at most) records from beginning */
    ZOOM_options_set(o, "count", "20");

    ZOOM_options_set(o, "implementationName", "sortapp");
    ZOOM_options_set(o, "preferredRecordSyntax", "usmarc");
    ZOOM_options_set(o, "elementSetName", "B");

    /* create query */
    q = ZOOM_query_create();
    if (strncmp("cql:", argv[argc-3], 4) == 0)
    {
        if (ZOOM_query_cql(q, argv[argc-3] + 4))
        {
            printf("bad CQL: %s\n", argv[argc-3] + 4);
            exit(1);
        }
    }
    else if (ZOOM_query_prefix(q, argv[argc-3]))
    {
        printf("bad PQF: %s\n", argv[argc-3]);
        exit(1);
    }
    if (ZOOM_query_sortby2(q, argv[argc-2], argv[argc-1]))
    {
        printf("bad sort spec: %s\n", argv[argc-1]);
        exit(1);
    }
    /* connect - and search all */
    for (i = 0; i < no; i++)
    {
        z[i] = ZOOM_connection_create(o);
        ZOOM_connection_connect(z[i], argv[i+1], 0);
        r[i] = ZOOM_connection_search(z[i], q);
    }

    /* network I/O */
    while (ZOOM_event(no, z))
        ;

    /* handle errors */
    for (i = 0; i < no; i++)
    {
        int error;
        const char *errmsg, *addinfo;
        if ((error = ZOOM_connection_error(z[i], &errmsg, &addinfo)))
            fprintf(stderr, "%s error: %s (%d) %s\n",
                    ZOOM_connection_option_get(z[i], "host"),
                    errmsg, error, addinfo);
        else
        {
            /* OK, no major errors. Look at the result count */
            int pos;
            printf("%s: %ld hits\n", ZOOM_connection_option_get(z[i], "host"),
                   (long) ZOOM_resultset_size(r[i]));
            /* go through first 20 records at target */
            for (pos = 0; pos < 20; pos++)
            {
                ZOOM_record rec;
                const char *db, *syntax, *str;
                int record_len, syntax_len;

                rec = ZOOM_resultset_record(r[i], pos);
                /* get database for record and record itself at pos */

                db = ZOOM_record_get(rec,  "database", 0);
                str = ZOOM_record_get(rec, "xml", &record_len);
                syntax = ZOOM_record_get(rec, "syntax", &syntax_len);
                /* if rec is non-null, we got a record for display */
                if (str)
                {
                    printf("%d %s %s\n", pos+1, syntax,
                           (db ? db : "unknown"));
                    if (rec)
                    {
                        if (fwrite(str, 1, record_len, stdout) !=
                            (size_t) record_len)
                            printf("write to stdout failed\n");
                    }
                    printf("\n");
                }
            }
        }
    }

    /* destroy stuff and exit */
    ZOOM_query_destroy(q);
    for (i = 0; i < no; i++)
    {
        ZOOM_resultset_destroy(r[i]);
        ZOOM_connection_destroy(z[i]);
    }
    ZOOM_options_destroy(o);
    exit(0);
}
/*
 * Local variables:
 * c-basic-offset: 4
 * c-file-style: "Stroustrup"
 * indent-tabs-mode: nil
 * End:
 * vim: shiftwidth=4 tabstop=8 expandtab
 */

