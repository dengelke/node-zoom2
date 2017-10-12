/* This file is part of the YAZ toolkit.
 * Copyright (C) Index Data
 * See the file LICENSE for details.
 */

#include <stdlib.h>
#include <stdio.h>
#include <yaz/xmalloc.h>
#include <yaz/zoom.h>

int main(int argc, char **argv)
{
    ZOOM_connection z;
    ZOOM_resultset r;
    int error;
    const char *errmsg, *addinfo;

    if (argc != 3)
    {
        fprintf (stderr, "usage:\n%s target query\n", *argv);
        fprintf (stderr, " eg.  bagel.indexdata.dk/gils computer\n");
        exit (1);
    }
    z = ZOOM_connection_new (argv[1], 0);

    if ((error = ZOOM_connection_error(z, &errmsg, &addinfo)))
    {
        fprintf (stderr, "Error: %s (%d) %s\n", errmsg, error, addinfo);
        exit (2);
    }

    ZOOM_connection_option_set(z, "saveAPDU", "1");
    r = ZOOM_connection_search_pqf (z, argv[2]);
    if ((error = ZOOM_connection_error(z, &errmsg, &addinfo)))
        fprintf (stderr, "Error: %s (%d) %s\n", errmsg, error, addinfo);
    else
        printf ("Result count: %ld\n", (long) ZOOM_resultset_size(r));
    puts("APDUlog");
    puts(ZOOM_connection_option_get(z, "APDU"));
    ZOOM_resultset_destroy (r);
    ZOOM_connection_destroy (z);
    exit (0);
}
/*
 * Local variables:
 * c-basic-offset: 4
 * c-file-style: "Stroustrup"
 * indent-tabs-mode: nil
 * End:
 * vim: shiftwidth=4 tabstop=8 expandtab
 */

