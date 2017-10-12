/* This file is part of the YAZ toolkit.
 * Copyright (C) Index Data
 * See the file LICENSE for details.
 */

/**
 * \file marc_read_line.c
 * \brief Implements reading of MARC in line format
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <yaz/marcdisp.h>
#include <yaz/wrbuf.h>
#include <yaz/yaz-util.h>

static int yaz_gets(int (*getbyte)(void *client_data),
                    void (*ungetbyte)(int b, void *client_data),
                    void *client_data,
                    WRBUF w)
{
    size_t sz = 0;
    int ch = getbyte(client_data);

    while (ch != '\0' && ch != '\r' && ch != '\n')
    {
        wrbuf_putc(w, ch);
        sz++;
        ch = getbyte(client_data);
    }
    if (ch == '\r')
    {
        ch = getbyte(client_data);
        if (ch != '\n' && ch != '\0')
            ungetbyte(ch, client_data);
    }
    else if (ch == '\n')
    {
        ch = getbyte(client_data);
        if (ch != '\r' && ch != '\0')
            ungetbyte(ch, client_data);
    }
    if (sz)
    {
        return 1;
    }
    return 0;
}

static int yaz_marc_line_gets(int (*getbyte)(void *client_data),
                              void (*ungetbyte)(int b, void *client_data),
                              void *client_data,
                              WRBUF w)
{
    int more;

    wrbuf_rewind(w);
    more = yaz_gets(getbyte, ungetbyte, client_data, w);
    if (!more)
        return 0;

    while (more)
    {
        int i;
        for (i = 0; i<4; i++)
        {
            int ch = getbyte(client_data);
            if (ch != ' ')
            {
                if (ch)
                    ungetbyte(ch, client_data);
                return 1;
            }
        }
        if (wrbuf_len(w) > 60 && wrbuf_buf(w)[wrbuf_len(w)-1] == '=')
            wrbuf_cut_right(w, 1);
        else
            wrbuf_puts(w, " ");
        more = yaz_gets(getbyte, ungetbyte, client_data, w);
    }
    return 1;
}


int yaz_marc_read_line(yaz_marc_t mt,
                       int (*getbyte)(void *client_data),
                       void (*ungetbyte)(int b, void *client_data),
                       void *client_data)
{
    int indicator_length;
    int identifier_length;
    int base_address;
    int length_data_entry;
    int length_starting;
    int length_implementation;
    int marker_ch = 0;
    int marker_skip = 0;
    int header_created = 0;
    WRBUF wrbuf_line = wrbuf_alloc();

    yaz_marc_reset(mt);

    while (yaz_marc_line_gets(getbyte, ungetbyte, client_data, wrbuf_line))
    {
        const char *line = wrbuf_cstr(wrbuf_line);
        int val;
        size_t line_len = strlen(line);
        if (line_len == 0)       /* empty line indicates end of record */
        {
            if (header_created)
                break;
        }
        else if (line[0] == '$') /* indicates beginning/end of record */
        {
            if (header_created)
                break;
        }
        else if (line[0] == '(') /* annotation, skip it */
            ;
        else if (line_len == 24 && atoi_n_check(line, 5, &val))
        {
            /* deal with header lines:  00366nam  22001698a 4500
            */

            if (header_created)
                break;
            yaz_marc_set_leader(mt, line,
                                &indicator_length,
                                &identifier_length,
                                &base_address,
                                &length_data_entry,
                                &length_starting,
                                &length_implementation);
            header_created = 1;
        }
        else if (line_len > 4 && line[0] != ' ' && line[1] != ' '
                 && line[2] != ' ' && line[3] == ' ' )
        {
            /* deal with data/control lines: 245 12 ........ */
            char tag[4];
            const char *datafield_start = line+6;
            marker_ch = 0;
            marker_skip = 0;

            memcpy(tag, line, 3);
            tag[3] = '\0';
            if (line_len >= 8) /* control - or datafield ? */
            {
                if (*datafield_start == ' ')
                    datafield_start++;  /* skip blank after indicator */

                if (strchr("$_*", *datafield_start))
                {
                    marker_ch = *datafield_start;
                    if (datafield_start[2] == ' ')
                        marker_skip = 1; /* subfields has blank before data */
                }
            }
            if (!header_created)
            {
                const char *leader = "01000cam  2200265 i 4500";

                yaz_marc_set_leader(mt, leader,
                                    &indicator_length,
                                    &identifier_length,
                                    &base_address,
                                    &length_data_entry,
                                    &length_starting,
                                    &length_implementation);
                header_created = 1;
            }

            if (marker_ch == 0)
            {   /* control field */
                yaz_marc_add_controlfield(mt, tag, line+4, strlen(line+4));
            }
            else
            {   /* data field */
                const char *indicator = line+4;
                int indicator_len = 2;
                const char *cp = datafield_start;

                yaz_marc_add_datafield(mt, tag, indicator, indicator_len);
                for (;;)
                {
                    const char *next;
                    size_t len;

                    assert(cp[0] == marker_ch);
                    cp++;
                    next = cp;
                    while ((next = strchr(next, marker_ch)))
                    {
                        if ((next[1] >= 'A' && next[1] <= 'Z')
                            ||(next[1] >= 'a' && next[1] <= 'z')
                            ||(next[1] >= '0' && next[1] <= '9'))
                        {
                            if (!marker_skip)
                                break;
                            else if (next[2] == ' ')
                                break;
                        }
                        next++;
                    }
                    len = strlen(cp);
                    if (next)
                        len = next - cp - marker_skip;

                    if (marker_skip)
                    {
                        /* remove ' ' after subfield marker */
                        char *cp_blank = strchr(cp, ' ');
                        if (cp_blank)
                        {
                            len--;
                            while (cp_blank != cp)
                            {
                                cp_blank[0] = cp_blank[-1];
                                cp_blank--;
                            }
                            cp++;
                        }
                    }
                    yaz_marc_add_subfield(mt, cp, len);
                    if (!next)
                        break;
                    cp = next;
                }
            }
        }
        else
        {
            yaz_marc_cprintf(mt, "Ignoring line: %s", line);
        }
    }
    wrbuf_destroy(wrbuf_line);
    if (!header_created)
        return -1;
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

