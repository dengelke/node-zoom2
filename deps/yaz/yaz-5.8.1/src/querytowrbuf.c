/* This file is part of the YAZ toolkit.
 * Copyright (C) Index Data
 * See the file LICENSE for details.
 */
/** \file querytowrbuf.c
    \brief Convert Z39.50 Z_Query to PQF (as WRBUF string)
 */
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <assert.h>

#include <yaz/logrpn.h>
#include <yaz/querytowrbuf.h>
#include <yaz/oid_db.h>

void yaz_encode_pqf_term(WRBUF b, const char *term, int len)
{
    int i;
    for (i = 0; i < len; i++)
        if (strchr(" \"{", term[i]))
            break;
    if (len > 0 && i == len)
    {
        if (term[0] == '@')
            wrbuf_putc(b, '\\');
        for (i = 0; i < len; i++)
        {
            if (term[i] == '\\')
                wrbuf_putc(b, '\\');
            wrbuf_putc(b, term[i]);
        }
    }
    else
    {
        wrbuf_putc(b, '"');
        for (i = 0; i < len; i++)
        {
            if (term[i] == '"' || term[i] == '\\')
                wrbuf_putc(b, '\\');
            wrbuf_putc(b, term[i]);
        }
        wrbuf_putc(b, '"');
    }
    wrbuf_putc(b, ' ');
}

static void yaz_attribute_element_to_wrbuf(WRBUF b,
                                           const Z_AttributeElement *element)
{
    int i;

    wrbuf_puts(b, "@attr ");
    if (element->attributeSet)
    {
        char oid_name_str[OID_STR_MAX];
        const char *setname = yaz_oid_to_string_buf(element->attributeSet,
                                                    0, oid_name_str);
        if (setname)
        {
            wrbuf_puts(b, setname);
            wrbuf_puts(b, " ");
        }
    }
    wrbuf_printf(b, ODR_INT_PRINTF "=", *element->attributeType);
    switch (element->which)
    {
    case Z_AttributeValue_numeric:
        wrbuf_printf(b, ODR_INT_PRINTF, *element->value.numeric);
        break;
    case Z_AttributeValue_complex:
        for (i = 0; i < element->value.complex->num_list; i++)
        {
            if (i)
                wrbuf_puts(b, ",");
            if (element->value.complex->list[i]->which ==
                Z_StringOrNumeric_string)
                wrbuf_puts(b, element->value.complex->list[i]->u.string);
            else if (element->value.complex->list[i]->which ==
                     Z_StringOrNumeric_numeric)
                wrbuf_printf(b, ODR_INT_PRINTF,
                             *element->value.complex->list[i]->u.numeric);
        }
        break;
    default:
        wrbuf_puts(b, "@attr 1=unknown");
    }
    wrbuf_puts(b, " ");
}

static const char *complex_op_name(const Z_Operator *op)
{
    switch (op->which)
    {
    case Z_Operator_and:
        return "and";
    case Z_Operator_or:
        return "or";
    case Z_Operator_and_not:
        return "not";
    case Z_Operator_prox:
        return "prox";
    default:
        return "unknown complex operator";
    }
}

static void yaz_attributes_to_wrbuf(WRBUF w, const Z_AttributeList *al)
{
    int i;
    for (i = 0; i < al->num_attributes; i++)
        yaz_attribute_element_to_wrbuf(w, al->attributes[i]);
}

static void yaz_apt_to_wrbuf(WRBUF b, const Z_AttributesPlusTerm *zapt)
{
    yaz_attributes_to_wrbuf(b, zapt->attributes);
    switch (zapt->term->which)
    {
    case Z_Term_general:
        yaz_encode_pqf_term(b, (const char *)zapt->term->u.general->buf,
                            zapt->term->u.general->len);
        break;
    case Z_Term_characterString:
        wrbuf_puts(b, "@term string ");
        yaz_encode_pqf_term(b, zapt->term->u.characterString,
                            strlen(zapt->term->u.characterString));
        break;
    case Z_Term_numeric:
        wrbuf_printf(b, "@term numeric " ODR_INT_PRINTF " ",
                     *zapt->term->u.numeric);
        break;
    case Z_Term_null:
        wrbuf_puts(b, "@term null x");
        break;
    default:
        wrbuf_printf(b, "@term null unknown%d ", zapt->term->which);
    }
}

static void yaz_rpnstructure_to_wrbuf(WRBUF b, const Z_RPNStructure *zs)
{
    if (zs->which == Z_RPNStructure_complex)
    {
        Z_Operator *op = zs->u.complex->roperator;
        wrbuf_printf(b, "@%s ", complex_op_name(op) );
        if (op->which== Z_Operator_prox)
        {
            if (!op->u.prox->exclusion)
                wrbuf_putc(b, 'n');
            else if (*op->u.prox->exclusion)
                wrbuf_putc(b, '1');
            else
                wrbuf_putc(b, '0');

            wrbuf_printf(b, " " ODR_INT_PRINTF " %d "
                         ODR_INT_PRINTF " ", *op->u.prox->distance,
                         *op->u.prox->ordered,
                         *op->u.prox->relationType);

            switch(op->u.prox->which)
            {
            case Z_ProximityOperator_known:
                wrbuf_putc(b, 'k');
                break;
            case Z_ProximityOperator_private:
                wrbuf_putc(b, 'p');
                break;
            default:
                wrbuf_printf(b, "%d", op->u.prox->which);
            }
            if (op->u.prox->u.known)
                wrbuf_printf(b, " " ODR_INT_PRINTF " ", *op->u.prox->u.known);
            else
                wrbuf_printf(b, " 0 ");
        }
        yaz_rpnstructure_to_wrbuf(b,zs->u.complex->s1);
        yaz_rpnstructure_to_wrbuf(b,zs->u.complex->s2);
    }
    else if (zs->which == Z_RPNStructure_simple)
    {
        if (zs->u.simple->which == Z_Operand_APT)
            yaz_apt_to_wrbuf(b, zs->u.simple->u.attributesPlusTerm);
        else if (zs->u.simple->which == Z_Operand_resultSetId)
        {
            wrbuf_printf(b, "@set ");
            yaz_encode_pqf_term(b, zs->u.simple->u.resultSetId,
                                strlen(zs->u.simple->u.resultSetId));
        }
        else
            wrbuf_puts(b, "(unknown simple structure)");
    }
    else
        wrbuf_puts(b, "(unknown structure)");
}

void yaz_rpnquery_to_wrbuf(WRBUF b, const Z_RPNQuery *rpn)
{
    if (rpn->attributeSetId)
    {
        char oid_name_str[OID_STR_MAX];
        const char *oid_name = yaz_oid_to_string_buf(rpn->attributeSetId,
                                                     0, oid_name_str);
        if (oid_name)
            wrbuf_printf(b, "@attrset %s ", oid_name);
    }
    yaz_rpnstructure_to_wrbuf(b, rpn->RPNStructure);
    wrbuf_chop_right(b);
}

void yaz_query_to_wrbuf(WRBUF b, const Z_Query *q)
{
    assert(q);
    assert(b);
    switch (q->which)
    {
    case Z_Query_type_1:
    case Z_Query_type_101:
        wrbuf_puts(b,"RPN ");
        yaz_rpnquery_to_wrbuf(b, q->u.type_1);
        break;
    case Z_Query_type_2:
        wrbuf_puts(b, "CCL ");
        wrbuf_write(b, (const char *) q->u.type_2->buf, q->u.type_2->len);
        break;
    case Z_Query_type_100:
        wrbuf_puts(b, "Z39.58 ");
        wrbuf_write(b, (const char *) q->u.type_100->buf, q->u.type_100->len);
        break;
    case Z_Query_type_104:
        if (q->u.type_104->which == Z_External_CQL)
        {
            wrbuf_puts(b, "CQL ");
            wrbuf_puts(b, q->u.type_104->u.cql);
        }
        else
            wrbuf_printf(b,"UNKNOWN type 104 query %d", q->u.type_104->which);
    }
}

void yaz_scan_to_wrbuf(WRBUF b, const Z_AttributesPlusTerm *zapt,
                       const Odr_oid *attrbute_set)
{
    /* should print attr set here */
    wrbuf_puts(b, "RPN ");
    yaz_apt_to_wrbuf(b, zapt);
}

void wrbuf_diags(WRBUF b, int num_diagnostics, Z_DiagRec **diags)
{
    /* we only dump the first diag - that keeps the log cleaner. */
    wrbuf_puts(b," ERROR ");
    if (diags[0]->which != Z_DiagRec_defaultFormat)
        wrbuf_puts(b,"(diag not in default format?)");
    else
    {
        Z_DefaultDiagFormat *e = diags[0]->u.defaultFormat;
        if (e->condition)
            wrbuf_printf(b, ODR_INT_PRINTF " ",*e->condition);
        else
            wrbuf_puts(b, "?? ");
        if ((e->which==Z_DefaultDiagFormat_v2Addinfo) && (e->u.v2Addinfo))
            wrbuf_puts(b, e->u.v2Addinfo);
        else if ((e->which==Z_DefaultDiagFormat_v3Addinfo) && (e->u.v3Addinfo))
            wrbuf_puts(b, e->u.v3Addinfo);
        wrbuf_puts(b, " ");
    }
}

void yaz_facet_list_to_wrbuf(WRBUF w, const Z_FacetList *fl)
{
    int i;
    for (i = 0; i < fl->num; i++)
    {
        Z_FacetField *ff = fl->elements[i];

        if (i)
            wrbuf_puts(w, ", ");
        yaz_attributes_to_wrbuf(w, ff->attributes);
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

