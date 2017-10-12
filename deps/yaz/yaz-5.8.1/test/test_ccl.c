/* This file is part of the YAZ toolkit.
 * Copyright (C) Index Data
 * See the file LICENSE for details.
 */
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <yaz/ccl_xml.h>
#include <yaz/log.h>
#include <yaz/test.h>


static int tst_ccl_query(CCL_bibset bibset,
                         const char *query,
                         const char *result)
{
    CCL_parser parser = ccl_parser_create(bibset);
    int ret = 0;

    if (parser && bibset)
    {
        struct ccl_rpn_node *rpn;

        rpn = ccl_parser_find_str(parser, query);
        if (rpn)
        {
            /* parse ok. check that result is there and match */
            WRBUF wrbuf = wrbuf_alloc();
            ccl_pquery(wrbuf, rpn);

            /* check expect a result and that it matches */
            if (result && !strcmp(wrbuf_cstr(wrbuf), result))
                ret = 1;
            else
            {
                yaz_log(YLOG_WARN, "%s: result does not match", query);
                yaz_log(YLOG_WARN, " expected %s", result);
                yaz_log(YLOG_WARN, " got      %s", wrbuf_cstr(wrbuf));
                ret = 0;
            }
            ccl_rpn_delete(rpn);
            wrbuf_destroy(wrbuf);
        }
        else
        {
            if (result)
            {
                yaz_log(YLOG_WARN, "%s: parse failed", query);
                ret = 0;
            }
            else
                ret = 1;
        }
    }
    ccl_parser_destroy (parser);
    return ret;
}

void tst1(int pass)
{
    CCL_bibset bibset = ccl_qual_mk();
    char tstline[128];

    YAZ_CHECK(bibset);
    if (!bibset)
        return;

    if (pass == 1)
    {
        CCL_bibset nbibset = ccl_qual_dup(bibset);
        ccl_qual_rm(&bibset);
        bibset = nbibset;
    }

    switch(pass)
    {
    case 0:
        ccl_qual_fitem(bibset, "term dc.title", "comb");
        ccl_qual_fitem(bibset, "u=4    s=pw t=l,r", "ti");
        ccl_qual_fitem(bibset, "1=1016 s=al,pw t=r",    "term");
        ccl_qual_fitem(bibset, "t=x", "reg");
        ccl_qual_fitem(bibset, "t=z", "z");
        ccl_qual_fitem(bibset, "1=/my/title",       "dc.title");
        ccl_qual_fitem(bibset, "r=r,omiteq",        "date");
        ccl_qual_fitem(bibset, "r=o",         "x");
        ccl_qual_fitem(bibset, "dc.title", "title");
        ccl_qual_fitem(bibset, "s=ag", "ag");
        break;
    case 1:
        strcpy(tstline, "ti u=4    s=pw t=l,r");
        ccl_qual_line(bibset, tstline);

        strcpy(tstline, "term 1=1016 s=al,pw t=r  # default term");
        ccl_qual_line(bibset, tstline);

        strcpy(tstline, "reg t=x");
        ccl_qual_line(bibset, tstline);

        strcpy(tstline, "z t=z");
        ccl_qual_line(bibset, tstline);

        strcpy(tstline, "dc.title 1=/my/title");
        ccl_qual_line(bibset, tstline);

        strcpy(tstline, "date r=r,omiteq # ordered relation");
        ccl_qual_line(bibset, tstline);

        strcpy(tstline, "x r=o # ordered relation");
        ccl_qual_line(bibset, tstline);

        strcpy(tstline, "title dc.title # alias");
        ccl_qual_line(bibset, tstline);

        strcpy(tstline, "comb term dc.title # combination");
        ccl_qual_line(bibset, tstline);

        strcpy(tstline, "ag s=ag");
        ccl_qual_line(bibset, tstline);
        break;
    case 2:
        ccl_qual_buf(bibset, "ti u=4    s=pw t=l,r\n"
                     "term 1=1016 s=al,pw t=r\r\n"
                     "\n"
                     "reg t=x\r\n"
                     "z t=z\r\n"
                     "dc.title 1=/my/title\n"
                     "date r=r,omiteq\n"
                     "x r=o\n"
                     "title dc.title\n"
                     "comb term dc.title\n"
                     "ag s=ag\n"
            );
        break;
    case 3:
#if YAZ_HAVE_XML2
        if (1)
        {
            xmlDocPtr doc;
            int r;
            const char *addinfo = 0;
            const char *xml_str =
                "<cclmap>\n"
                " <qual name=\"ti\">\n"
                "   <attr type=\"u\" value=\"4\"/>\n"
                "   <attr type=\"s\" value=\"pw\"/>\n"
                "   <attr type=\"t\" value=\"l,r\"/>\n"
                " </qual>\n"
                " <qual name=\"term\">\n"
                "   <attr type=\"1\" value=\"1016\"/>\n"
                "   <attr type=\"s\" value=\"al,pw\"/>\n"
                "   <attr type=\"t\" value=\"r\"/>\n"
                " </qual>\n"
                " <qual name=\"reg\">\n"
                "   <attr type=\"t\" value=\"x\"/>\n"
                " </qual>\n"
                " <qual name=\"z\">\n"
                "   <attr type=\"t\" value=\"z\"/>\n"
                " </qual>\n"
                " <qual name=\"dc.title\">\n"
                "   <attr type=\"1\" value=\"/my/title\"/>\n"
                " </qual>\n"
                " <qual name=\"date\">\n"
                "   <attr type=\"r\" value=\"r,omiteq\"/>\n"
                " </qual>\n"
                " <qual name=\"x\">\n"
                "   <attr type=\"r\" value=\"o\"/>\n"
                " </qual>\n"
                " <qual name=\"title\">\n"
                "   <qual name=\"dc.title\"/>\n"
                " </qual>\n"
                " <qual name=\"comb\">\n"
                "   <qual name=\"term\"/>\n"
                "   <qual name=\"dc.title\"/>\n"
                " </qual>\n"
                " <qual name=\"ag\">\n"
                "   <attr type=\"s\" value=\"ag\"/>\n"
                " </qual>\n"
                "</cclmap>\n";

            doc = xmlParseMemory(xml_str, strlen(xml_str));
            YAZ_CHECK(doc);

            r = ccl_xml_config(bibset, xmlDocGetRootElement(doc), &addinfo);
            YAZ_CHECK_EQ(r, 0);

            xmlFreeDoc(doc);
        }
        break;
#else
        return;
#endif
    default:
        YAZ_CHECK(0);
        return;
    }

    YAZ_CHECK(tst_ccl_query(bibset, "x1", "@attr 4=2 @attr 1=1016 x1 "));

    YAZ_CHECK(tst_ccl_query(bibset, "k\xc3\xb8" "benhavn", "@attr 4=2 @attr 1=1016 k\xc3\xb8" "benhavn "));

    YAZ_CHECK(tst_ccl_query(bibset, "k\xf8" "benhavn", "@attr 4=2 @attr 1=1016 ""k\xf8" "benhavn "));

    YAZ_CHECK(tst_ccl_query(bibset, "(((((x1)))))", "@attr 4=2 @attr 1=1016 x1 "));
    YAZ_CHECK(tst_ccl_query(bibset, "x1 and x2",
                  "@and "
                  "@attr 4=2 @attr 1=1016 x1 "
                  "@attr 4=2 @attr 1=1016 x2 "));
    YAZ_CHECK(tst_ccl_query(bibset, "ti=x3", "@attr 4=2 @attr 1=4 x3 "));
    YAZ_CHECK(tst_ccl_query(bibset, "dc.title=x4", "@attr 1=/my/title x4 "));
    YAZ_CHECK(tst_ccl_query(bibset, "dc.title=(x4)", "@attr 1=/my/title x4 "));
    YAZ_CHECK(tst_ccl_query(bibset, "x1 and", 0));
    YAZ_CHECK(tst_ccl_query(bibset, "tix=x5", 0));

    YAZ_CHECK(tst_ccl_query(bibset, "a%b",
                  "@prox 0 1 0 2 k 2 "
                  "@attr 4=2 @attr 1=1016 a "
                  "@attr 4=2 @attr 1=1016 b "));
    YAZ_CHECK(tst_ccl_query(bibset, "a%(b)",
                  "@prox 0 1 0 2 k 2 "
                  "@attr 4=2 @attr 1=1016 a "
                  "@attr 4=2 @attr 1=1016 b "));
    YAZ_CHECK(tst_ccl_query(bibset, "(a)%(b)",
                  "@prox 0 1 0 2 k 2 "
                  "@attr 4=2 @attr 1=1016 a "
                  "@attr 4=2 @attr 1=1016 b "));
    YAZ_CHECK(tst_ccl_query(bibset, "a%1b",
                  "@prox 0 1 0 2 k 2 "
                  "@attr 4=2 @attr 1=1016 a "
                  "@attr 4=2 @attr 1=1016 b "));

    YAZ_CHECK(tst_ccl_query(bibset, "a%2b",
                  "@prox 0 2 0 2 k 2 "
                  "@attr 4=2 @attr 1=1016 a "
                  "@attr 4=2 @attr 1=1016 b "));

    YAZ_CHECK(tst_ccl_query(bibset, "(a)%2(b)",
                  "@prox 0 2 0 2 k 2 "
                  "@attr 4=2 @attr 1=1016 a "
                  "@attr 4=2 @attr 1=1016 b "));

    YAZ_CHECK(tst_ccl_query(bibset, "a%19b",
                  "@prox 0 19 0 2 k 2 "
                  "@attr 4=2 @attr 1=1016 a "
                  "@attr 4=2 @attr 1=1016 b "));

    YAZ_CHECK(tst_ccl_query(bibset, "spid%�serne",
                  "@prox 0 1 0 2 k 2 "
                  "@attr 4=2 @attr 1=1016 spid "
                  "@attr 4=2 @attr 1=1016 �serne "));

    YAZ_CHECK(tst_ccl_query(bibset, "a!b",
                  "@prox 0 1 1 2 k 2 "
                  "@attr 4=2 @attr 1=1016 a "
                  "@attr 4=2 @attr 1=1016 b "));
    YAZ_CHECK(tst_ccl_query(bibset, "a!2b",
                  "@prox 0 2 1 2 k 2 "
                  "@attr 4=2 @attr 1=1016 a "
                  "@attr 4=2 @attr 1=1016 b "));

    YAZ_CHECK(tst_ccl_query(bibset, "a% (b or dc.title=c)",
                            "@prox 0 1 0 2 k 2 "
                            "@attr 4=2 @attr 1=1016 a "
                            "@or @attr 4=2 @attr 1=1016 b "
#if YAZ_781
                            "@or @attr 4=2 @attr 1=/my/title c "
                            "@attr 4=2 @attr 1=1016 c "
#else
                            "@attr 4=2 @attr 1=1016 @attr 1=/my/title c "
#endif
    ));

    YAZ_CHECK(tst_ccl_query(bibset, "(a b) % (c)",
                            "@prox 0 1 0 2 k 2 @and "
                            "@attr 4=2 @attr 1=1016 a @attr 4=2 @attr 1=1016 b "
                            "@attr 4=2 @attr 1=1016 c " ));

    YAZ_CHECK(tst_ccl_query(bibset, "date=1980",
                            "1980 "));
    YAZ_CHECK(tst_ccl_query(bibset, "(date=1980)",
                            "1980 "));
    YAZ_CHECK(tst_ccl_query(bibset, "date>1980",
                            "@attr 2=5 1980 "));
    YAZ_CHECK(tst_ccl_query(bibset, "date>=1980",
                            "@attr 2=4 1980 "));
    YAZ_CHECK(tst_ccl_query(bibset, "date<1980",
                            "@attr 2=1 1980 "));
    YAZ_CHECK(tst_ccl_query(bibset, "date<=1980",
                            "@attr 2=2 1980 "));
    YAZ_CHECK(tst_ccl_query(bibset, "date=234-1990",
                            "@and @attr 2=4 234 @attr 2=2 1990 "));
    YAZ_CHECK(tst_ccl_query(bibset, "date=234- 1990",
                            "@and @attr 2=4 234 @attr 2=2 1990 "));
    YAZ_CHECK(tst_ccl_query(bibset, "date=234 -1990",
                            "@and @attr 2=4 234 @attr 2=2 1990 "));
    YAZ_CHECK(tst_ccl_query(bibset, "date=234 - 1990",
                            "@and @attr 2=4 234 @attr 2=2 1990 "));
    YAZ_CHECK(tst_ccl_query(bibset, "date=-1980",
                            "@attr 2=2 1980 "));
    YAZ_CHECK(tst_ccl_query(bibset, "date=- 1980",
                            "@attr 2=2 1980 "));
    YAZ_CHECK(tst_ccl_query(bibset, "x=-1980",
                            "@attr 2=3 -1980 "));
    YAZ_CHECK(tst_ccl_query(bibset, "x=- 1980",
                            "@attr 2=2 1980 "));
    YAZ_CHECK(tst_ccl_query(bibset, "x= -1980",
                            "@attr 2=3 -1980 "));
    YAZ_CHECK(tst_ccl_query(bibset, "x=234-1990",
                            "@attr 2=3 234-1990 "));
    YAZ_CHECK(tst_ccl_query(bibset, "x=234 - 1990",
                            "@and @attr 2=4 234 @attr 2=2 1990 "));
    YAZ_CHECK(tst_ccl_query(bibset, "ti=a,b",
                            "@attr 4=1 @attr 1=4 a,b "));
    YAZ_CHECK(tst_ccl_query(bibset, "ti=a, b",
                            "@attr 4=1 @attr 1=4 \"a, b\" "));
    YAZ_CHECK(tst_ccl_query(bibset, "ti=a-b",
                            "@attr 4=2 @attr 1=4 a-b "));
    YAZ_CHECK(tst_ccl_query(bibset, "ti=a - b",
                            "@attr 4=1 @attr 1=4 \"a - b\" "));

    YAZ_CHECK(tst_ccl_query(bibset, "a?",
                            "@attr 5=1 @attr 4=2 @attr 1=1016 a "));
    YAZ_CHECK(tst_ccl_query(bibset, "a b",
                            "@and @attr 4=2 @attr 1=1016 a "
                            "@attr 4=2 @attr 1=1016 b "));

    YAZ_CHECK(tst_ccl_query(bibset, "a b?",
                            "@and @attr 4=2 @attr 1=1016 a "
                            "@attr 5=1 @attr 4=2 @attr 1=1016 b "));

    YAZ_CHECK(tst_ccl_query(bibset, "title=a",
                            "@attr 1=/my/title a "));

    YAZ_CHECK(tst_ccl_query(bibset, "reg=a?b#\"c?\"",
                            "@attr 5=102 a.*b.c\\\\? "));
    YAZ_CHECK(tst_ccl_query(bibset, "z=a?b#\"c?\"",
                            "@attr 5=104 a?b#c\\\\? "));

    YAZ_CHECK(tst_ccl_query(bibset, "reg=\\(",
                            "( "));
    YAZ_CHECK(tst_ccl_query(bibset, "z=\\(",
                            "( "));

    YAZ_CHECK(tst_ccl_query(bibset, "z=a b#",
                            "@attr 5=104 \"a b#\" "));

    YAZ_CHECK(tst_ccl_query(bibset, "reg=\\\"",
                            "\"\\\"\" "));
    YAZ_CHECK(tst_ccl_query(bibset, "z=\\\"",
                            "\"\\\"\" "));

    YAZ_CHECK(tst_ccl_query(bibset, "reg=.",
                            ". "));
    YAZ_CHECK(tst_ccl_query(bibset, "z=.",
                            ". "));

    YAZ_CHECK(tst_ccl_query(bibset, "reg=\".\"",
                            ". "));
    YAZ_CHECK(tst_ccl_query(bibset, "z=\".\"",
                            ". "));

    YAZ_CHECK(tst_ccl_query(bibset, "reg=?\\?",
                            "@attr 5=102 .*\\\\? "));
    YAZ_CHECK(tst_ccl_query(bibset, "z=?\\?",
                            "@attr 5=104 ?\\\\? "));

    YAZ_CHECK(tst_ccl_query(bibset, "reg=\"?\\?\"",
                            "?? "));
    YAZ_CHECK(tst_ccl_query(bibset, "z=\"?\\?\"",
                            "?? "));

    YAZ_CHECK(tst_ccl_query(bibset, "reg=\\\\",
                            "\\\\ "));
    YAZ_CHECK(tst_ccl_query(bibset, "z=\\\\",
                            "\\\\ "));

    YAZ_CHECK(tst_ccl_query(bibset, "\\\\",
                            "@attr 4=2 @attr 1=1016 \\\\ "));

    YAZ_CHECK(tst_ccl_query(bibset, "comb=a",
                            "@or @attr 4=2 @attr 1=1016 a "
                            "@attr 1=/my/title a "));

    YAZ_CHECK(tst_ccl_query(bibset, "a? b?",
                            "@and @attr 5=1 @attr 4=2 @attr 1=1016 a "
                            "@attr 5=1 @attr 4=2 @attr 1=1016 b "));

    YAZ_CHECK(tst_ccl_query(bibset, "\"a\"? \"b?\"",
                            "@and @attr 5=1 @attr 4=2 @attr 1=1016 a "
                            "@attr 4=2 @attr 1=1016 b? "));

    YAZ_CHECK(tst_ccl_query(bibset, "@and",
                            "@attr 4=2 @attr 1=1016 \\@and "));

    YAZ_CHECK(tst_ccl_query(bibset, "a@and",
                            "@attr 4=2 @attr 1=1016 a@and "));

    YAZ_CHECK(tst_ccl_query(bibset, "}",
                            "@attr 4=2 @attr 1=1016 } "));

    YAZ_CHECK(tst_ccl_query(bibset, "{",
                            "@attr 4=2 @attr 1=1016 \"{\" "));

    YAZ_CHECK(tst_ccl_query(bibset, "\"a b c\"",
                            "@attr 4=1 @attr 1=1016 \"a b c\" "));

    YAZ_CHECK(tst_ccl_query(bibset, "\"a b  c  \"",
                            "@attr 4=1 @attr 1=1016 \"a b  c  \" "));

    YAZ_CHECK(tst_ccl_query(bibset, "ag=a",
                            "@attr 4=2 a "));

    YAZ_CHECK(tst_ccl_query(bibset, "ag=a b",
                            "@attr 4=2 \"a b\" "));

    YAZ_CHECK(tst_ccl_query(bibset, "ag=a b \"c d\"",
                            "@and @attr 4=2 \"a b\" @attr 4=1 \"c d\" "));

    YAZ_CHECK(tst_ccl_query(bibset, "ag=a b \"c\"",
                            "@attr 4=2 \"a b c\" "));

    YAZ_CHECK(tst_ccl_query(bibset, "ag=a b \"\"",
                            "@attr 4=2 \"a b \" "));

    YAZ_CHECK(tst_ccl_query(bibset, "ag=a \"b c\" d",
                            "@and @and "
                            "@attr 4=2 a @attr 4=1 \"b c\" @attr 4=2 d "));

    YAZ_CHECK(tst_ccl_query(bibset, "ag=\"a b c\"",
                            "@attr 4=1 \"a b c\" "));

    YAZ_CHECK(tst_ccl_query(bibset, "ag=\"a b c\" \"d e\"",
                            "@and @attr 4=1 \"a b c\" @attr 4=1 \"d e\" "));
    ccl_qual_rm(&bibset);
}

void tst2(void)
{
    CCL_bibset bibset = ccl_qual_mk();

    YAZ_CHECK(bibset);
    if (!bibset)
        return;

    ccl_qual_fitem(bibset, "u=4    s=pw t=l,r", "ti");
    ccl_qual_fitem(bibset, "1=1016 s=al,pw t=z",    "term");

    YAZ_CHECK(tst_ccl_query(bibset, "a?#",
                            "@attr 5=104 @attr 4=2 @attr 1=1016 a?# "));

    YAZ_CHECK(tst_ccl_query(bibset, "a b?#",
                            "@and @attr 4=2 @attr 1=1016 a @attr 5=104 @attr 4=2 @attr 1=1016 b?# "));

    YAZ_CHECK(tst_ccl_query(bibset, "a*",
                            "@attr 4=2 @attr 1=1016 a* "));

    YAZ_CHECK(tst_ccl_query(bibset, "a?",
                            "@attr 5=104 @attr 4=2 @attr 1=1016 a? "));

    ccl_qual_fitem(bibset, "*", "@truncation");
    YAZ_CHECK(tst_ccl_query(bibset, "a*",
                            "@attr 5=104 @attr 4=2 @attr 1=1016 a? "));

    YAZ_CHECK(tst_ccl_query(bibset, "a?",
                            "@attr 4=2 @attr 1=1016 a? "));

    ccl_qual_fitem(bibset, "?", "@mask");
    YAZ_CHECK(tst_ccl_query(bibset, "a?",
                            "@attr 5=104 @attr 4=2 @attr 1=1016 a# "));


    ccl_qual_fitem(bibset, "", "@mask");
    ccl_qual_fitem(bibset, "", "@truncation");
    YAZ_CHECK(tst_ccl_query(bibset, "a?#",
                            "@attr 4=2 @attr 1=1016 a?# "));

    ccl_qual_fitem(bibset, "og", "@and");
    ccl_qual_fitem(bibset, "eller", "@or");
    ccl_qual_fitem(bibset, "ikke", "@not");

    YAZ_CHECK(tst_ccl_query(bibset, "a og b eller c ikke d",
                            "@not @or @and @attr 4=2 @attr 1=1016 a "
                            "@attr 4=2 @attr 1=1016 b "
                            "@attr 4=2 @attr 1=1016 c "
                            "@attr 4=2 @attr 1=1016 d "));
    ccl_qual_rm(&bibset);
}


void tst_addinfo(void)
{
    const char *addinfo;
    int r;
    CCL_bibset bibset = ccl_qual_mk();

    r = ccl_qual_fitem2(bibset, "u=4    s=pw t=l,r", "ti", &addinfo);
    YAZ_CHECK(r == 0 && addinfo == 0);

    r = ccl_qual_fitem2(bibset, "1=1016 s=al,pw t=z", "term", &addinfo);
    YAZ_CHECK(r == 0 && addinfo == 0);

    r = ccl_qual_fitem2(bibset, "x=", "term", &addinfo);
    YAZ_CHECK(r != 0 && addinfo != 0);

    r = ccl_qual_fitem2(bibset, "12=3", "term", &addinfo);
    YAZ_CHECK(r == 0 && addinfo == 0);

    r = ccl_qual_fitem2(bibset, "ab=3", "term", &addinfo);
    YAZ_CHECK(r != 0 && addinfo != 0);

    r = ccl_qual_fitem2(bibset, "x=ab", "term", &addinfo);
    YAZ_CHECK(r != 0 && addinfo != 0);

    r = ccl_qual_fitem2(bibset, "s=ab", "term", &addinfo);
    YAZ_CHECK(r == 0 && addinfo == 0);

    ccl_qual_rm(&bibset);
}

int main(int argc, char **argv)
{
    YAZ_CHECK_INIT(argc, argv);
    YAZ_CHECK_LOG();
    tst1(0);
    tst1(1);
    tst1(2);
    tst1(3);
    tst2();
    tst_addinfo();
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

