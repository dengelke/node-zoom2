/* This file is part of the YAZ toolkit.
 * Copyright (C) Index Data
 * See the file LICENSE for details.
 */
/**
 * \file opac_to_xml.c
 * \brief Implements OPAC to XML conversion
 */
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <yaz/proto.h>
#include <yaz/marcdisp.h>

static void opac_element_str(WRBUF wrbuf, yaz_iconv_t cd1, yaz_iconv_t cd2,
                             int l, const char *elem,
                             const char *data)
{
    if (data)
    {
        size_t pos;
        while (--l >= 0)
            wrbuf_puts(wrbuf, " ");
        wrbuf_puts(wrbuf, "<");
        wrbuf_puts(wrbuf, elem);
        wrbuf_puts(wrbuf, ">");

        pos = wrbuf->pos; /* save position */
        if (wrbuf_iconv_write2(wrbuf, cd1, data, strlen(data), wrbuf_xmlputs_n)
            && cd2)
        {
            wrbuf->pos = pos; /* rewind to pos */
            wrbuf_iconv_write2(wrbuf, cd2, data, strlen(data), wrbuf_xmlputs_n);
        }
        wrbuf_puts(wrbuf, "</");
        wrbuf_puts(wrbuf, elem);
        wrbuf_puts(wrbuf, ">\n");
    }
}

static void opac_element_bool(WRBUF wrbuf, int l, const char *elem, int *data)
{
    if (data)
    {
        while (--l >= 0)
            wrbuf_puts(wrbuf, " ");
        wrbuf_puts(wrbuf, "<");
        wrbuf_puts(wrbuf, elem);
        if (*data)
            wrbuf_puts(wrbuf, " value=\"1\"");
        else
            wrbuf_puts(wrbuf, " value=\"0\"");
        wrbuf_puts(wrbuf, "/>\n");
    }
}

void yaz_opac_decode_wrbuf2(yaz_marc_t mt, Z_OPACRecord *r, WRBUF wrbuf,
                            yaz_iconv_t cd2)
{
    int i;
    yaz_iconv_t cd1 = yaz_marc_get_iconv(mt);

    wrbuf_puts(wrbuf, "<opacRecord>\n");
    if (r->bibliographicRecord)
    {
        Z_External *ext = r->bibliographicRecord;

        wrbuf_puts(wrbuf, "  <bibliographicRecord>\n");
        if (ext->which == Z_External_octet)
            yaz_marc_decode_wrbuf(mt, (const char *) ext->u.octet_aligned->buf,
                                  ext->u.octet_aligned->len, wrbuf);
        wrbuf_puts(wrbuf, "  </bibliographicRecord>\n");
    }
    wrbuf_puts(wrbuf, "<holdings>\n");

    for (i = 0; i < r->num_holdingsData; i++)
    {
        Z_HoldingsRecord *h = r->holdingsData[i];
        wrbuf_puts(wrbuf, " <holding>\n");

        if (h->which == Z_HoldingsRecord_marcHoldingsRecord)
        {
            Z_External *ext = h->u.marcHoldingsRecord;

            wrbuf_puts(wrbuf, "  <marcHoldingsRecord>\n");
            if (ext->which == Z_External_octet)
                yaz_marc_decode_wrbuf(mt, (const char *)
                                      ext->u.octet_aligned->buf,
                                      ext->u.octet_aligned->len, wrbuf);
            wrbuf_puts(wrbuf, "  </marcHoldingsRecord>\n");
        }
        else if (h->which == Z_HoldingsRecord_holdingsAndCirc)
        {
            int j;

            Z_HoldingsAndCircData *d = h->u.holdingsAndCirc;

            opac_element_str(wrbuf, cd1, cd2, 2, "typeOfRecord",
                             d->typeOfRecord);
            opac_element_str(wrbuf, cd1, cd2, 2, "encodingLevel",
                             d->encodingLevel);
            opac_element_str(wrbuf, cd1, cd2, 2, "format",
                             d->format);
            opac_element_str(wrbuf, cd1, cd2, 2, "receiptAcqStatus",
                             d->receiptAcqStatus);
            opac_element_str(wrbuf, cd1, cd2, 2, "generalRetention",
                             d->generalRetention);
            opac_element_str(wrbuf, cd1, cd2, 2, "completeness",
                             d->completeness);
            opac_element_str(wrbuf, cd1, cd2, 2, "dateOfReport",
                             d->dateOfReport);
            opac_element_str(wrbuf, cd1, cd2, 2, "nucCode", d->nucCode);
            opac_element_str(wrbuf, cd1, cd2, 2, "localLocation",
                             d->localLocation);
            opac_element_str(wrbuf, cd1, cd2, 2, "shelvingLocation",
                             d->shelvingLocation);
            opac_element_str(wrbuf, cd1, cd2, 2, "callNumber",
                             d->callNumber);
            opac_element_str(wrbuf, cd1, cd2, 2, "shelvingData",
                             d->shelvingData);
            opac_element_str(wrbuf, cd1, cd2, 2, "copyNumber",
                             d->copyNumber);
            opac_element_str(wrbuf, cd1, cd2, 2, "publicNote",
                             d->publicNote);
            opac_element_str(wrbuf, cd1, cd2, 2, "reproductionNote",
                             d->reproductionNote);
            opac_element_str(wrbuf, cd1, cd2, 2, "termsUseRepro",
                             d->termsUseRepro);
            opac_element_str(wrbuf, cd1, cd2, 2, "enumAndChron",
                             d->enumAndChron);
            if (d->num_volumes)
            {
                wrbuf_puts (wrbuf, "  <volumes>\n");
                for (j = 0; j<d->num_volumes; j++)
                {
                    wrbuf_puts (wrbuf, "   <volume>\n");
                    opac_element_str(wrbuf, cd1, cd2, 4, "enumeration",
                                     d->volumes[j]->enumeration);
                    opac_element_str(wrbuf, cd1, cd2, 4, "chronology",
                                     d->volumes[j]->chronology);
                    opac_element_str(wrbuf, cd1, cd2, 4, "enumAndChron",
                                     d->volumes[j]->enumAndChron);
                    wrbuf_puts(wrbuf, "   </volume>\n");
                }
                wrbuf_puts(wrbuf, "  </volumes>\n");
            }
            if (d->num_circulationData)
            {
                wrbuf_puts (wrbuf, "  <circulations>\n");
                for (j = 0; j<d->num_circulationData; j++)
                {
                    wrbuf_puts(wrbuf,"   <circulation>\n");
                    opac_element_bool(wrbuf, 4, "availableNow",
                                      d->circulationData[j]->availableNow);
                    opac_element_str(wrbuf, cd1, cd2, 4,  "availabilityDate",
                                     d->circulationData[j]->availablityDate);
                    opac_element_str(wrbuf, cd1, cd2, 4, "availableThru",
                                      d->circulationData[j]->availableThru);
                    opac_element_str(wrbuf, cd1, cd2, 4, "restrictions",
                                      d->circulationData[j]->restrictions);
                    opac_element_str(wrbuf, cd1, cd2, 4, "itemId",
                                      d->circulationData[j]->itemId);
                    opac_element_bool(wrbuf, 4, "renewable",
                                      d->circulationData[j]->renewable);
                    opac_element_bool(wrbuf, 4, "onHold",
                                      d->circulationData[j]->onHold);
                    opac_element_str(wrbuf, cd1, cd2, 4, "enumAndChron",
                                      d->circulationData[j]->enumAndChron);
                    opac_element_str(wrbuf, cd1, cd2, 4, "midspine",
                                      d->circulationData[j]->midspine);
                    opac_element_str(wrbuf, cd1, cd2, 4, "temporaryLocation",
                                      d->circulationData[j]->temporaryLocation);
                    wrbuf_puts(wrbuf, "   </circulation>\n");
                }
                wrbuf_puts(wrbuf, "  </circulations>\n");
            }
        }
        wrbuf_puts(wrbuf, " </holding>\n");
    }
    wrbuf_puts(wrbuf, "</holdings>\n");
    wrbuf_puts(wrbuf, "</opacRecord>\n");
}

void yaz_opac_decode_wrbuf(yaz_marc_t mt, Z_OPACRecord *r, WRBUF wrbuf)
{
    yaz_opac_decode_wrbuf2(mt, r, wrbuf, 0);
}

int yaz_opac_check_marc21_coding(const char *charset, Z_OPACRecord *r)
{
    if (r->bibliographicRecord)
    {
        Z_External *ext = r->bibliographicRecord;
        if (ext->which == Z_External_octet)
        {
            return yaz_marc_check_marc21_coding(
                charset,
                (const char *) ext->u.octet_aligned->buf,
                ext->u.octet_aligned->len);
        }
    }
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

