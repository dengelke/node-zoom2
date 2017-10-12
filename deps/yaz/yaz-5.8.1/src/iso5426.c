/** \file iso5426.c
    \brief Character conversion, generated from ./codetables-iso5426.xml

    Generated automatically by charconv.tcl
*/
#include <string.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif

        struct yaz_iconv_trie_flat {
            char from[6];
            unsigned combining : 1;
            unsigned  to : 24;
        };
        struct yaz_iconv_trie_dir {
            int ptr : 15;
            unsigned combining : 1;
            unsigned  to : 24;
        };
        
        struct yaz_iconv_trie {
            struct yaz_iconv_trie_flat *flat;
            struct yaz_iconv_trie_dir *dir;
        };
    

        static unsigned long lookup(struct yaz_iconv_trie **ptrs, int ptr, unsigned char *inp,
                                    size_t inbytesleft, size_t *no_read, int *combining, unsigned mask, unsigned int boffset)
        {
            struct yaz_iconv_trie *t = ptrs[ptr-1];
            if (inbytesleft < 1)
                return 0;
            if (t->dir)
            {
                size_t ch = (inp[0] & mask) + boffset;
                unsigned long code;
                if (t->dir[ch].ptr)
                {
                    code = lookup(ptrs, t->dir[ch].ptr, inp+1, inbytesleft-1, no_read, combining, mask, boffset);
		    if (code)
		    {
			(*no_read)++;
			return code;
		    }
		}
                if (t->dir[ch].to)
                {
                    code = t->dir[ch].to;
		    *combining = t->dir[ch].combining;
                    *no_read = 1;
                    return code;
                }
            }
            else
            {
                struct yaz_iconv_trie_flat *flat = t->flat;
                while (flat->to)
                {
                    size_t len = strlen(flat->from);
                    if (len <= inbytesleft)
                    {
                        size_t i;
			for (i = 0; i < len; i++)
			{
			    if (((unsigned char *) flat->from)[i] != (inp[i] & mask) + boffset)
			        break;
		        }
                        if (i == len)
                        {
                            *no_read = len;
			    *combining = flat->combining;
                            return flat->to;
                        }
                    }
                    flat++;
                }
            }
            return 0;
        }
    
/* TRIE: size 98 */
/* PAGE 0 */
struct yaz_iconv_trie_dir iso5426_42page0_dir[256] = {
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0x001D} /* RECORD TERMINATOR / GROUP SEPARATOR */,
  {0, 0, 0x001E} /* FIELD TERMINATOR / RECORD SEPARATOR */,
  {0, 0, 0x001F} /* SUBFIELD DELIMITER / UNIT SEPARATOR */,
  {0, 0, 0x0020} /* SPACE, BLANK / SPACE */,
  {0, 0, 0x0021} /* EXCLAMATION MARK */,
  {0, 0, 0x0022} /* QUOTATION MARK */,
  {0, 0, 0x0023} /* NUMBER SIGN */,
  {0, 0, 0x0024} /* DOLLAR SIGN */,
  {0, 0, 0x0025} /* PERCENT SIGN */,
  {0, 0, 0x0026} /* AMPERSAND */,
  {0, 0, 0x0027} /* APOSTROPHE */,
  {0, 0, 0x0028} /* OPENING PARENTHESIS / LEFT PARENTHESIS */,
  {0, 0, 0x0029} /* CLOSING PARENTHESIS / CLOSING PARENTHESIS */,
  {0, 0, 0x002A} /* ASTERISK */,
  {0, 0, 0x002B} /* PLUS SIGN */,
  {0, 0, 0x002C} /* COMMA */,
  {0, 0, 0x002D} /* HYPHEN-MINUS */,
  {0, 0, 0x002E} /* PERIOD, DECIMAL POINT / FULL STOP */,
  {0, 0, 0x002F} /* SLASH / SOLIDUS */,
  {0, 0, 0x0030} /* DIGIT ZERO */,
  {0, 0, 0x0031} /* DIGIT ONE */,
  {0, 0, 0x0032} /* DIGIT TWO */,
  {0, 0, 0x0033} /* DIGIT THREE */,
  {0, 0, 0x0034} /* DIGIT FOUR */,
  {0, 0, 0x0035} /* DIGIT FIVE */,
  {0, 0, 0x0036} /* DIGIT SIX */,
  {0, 0, 0x0037} /* DIGIT SEVEN */,
  {0, 0, 0x0038} /* DIGIT EIGHT */,
  {0, 0, 0x0039} /* DIGIT NINE */,
  {0, 0, 0x003A} /* COLON */,
  {0, 0, 0x003B} /* SEMICOLON */,
  {0, 0, 0x003C} /* LESS-THAN SIGN */,
  {0, 0, 0x003D} /* EQUALS SIGN */,
  {0, 0, 0x003E} /* GREATER-THAN SIGN */,
  {0, 0, 0x003F} /* QUESTION MARK */,
  {0, 0, 0x0040} /* COMMERCIAL AT */,
  {0, 0, 0x0041} /* LATIN CAPITAL LETTER A */,
  {0, 0, 0x0042} /* LATIN CAPITAL LETTER B */,
  {0, 0, 0x0043} /* LATIN CAPITAL LETTER C */,
  {0, 0, 0x0044} /* LATIN CAPITAL LETTER D */,
  {0, 0, 0x0045} /* LATIN CAPITAL LETTER E */,
  {0, 0, 0x0046} /* LATIN CAPITAL LETTER F */,
  {0, 0, 0x0047} /* LATIN CAPITAL LETTER G */,
  {0, 0, 0x0048} /* LATIN CAPITAL LETTER H */,
  {0, 0, 0x0049} /* LATIN CAPITAL LETTER I */,
  {0, 0, 0x004A} /* LATIN CAPITAL LETTER J */,
  {0, 0, 0x004B} /* LATIN CAPITAL LETTER K */,
  {0, 0, 0x004C} /* LATIN CAPITAL LETTER L */,
  {0, 0, 0x004D} /* LATIN CAPITAL LETTER M */,
  {0, 0, 0x004E} /* LATIN CAPITAL LETTER N */,
  {0, 0, 0x004F} /* LATIN CAPITAL LETTER O */,
  {0, 0, 0x0050} /* LATIN CAPITAL LETTER P */,
  {0, 0, 0x0051} /* LATIN CAPITAL LETTER Q */,
  {0, 0, 0x0052} /* LATIN CAPITAL LETTER R */,
  {0, 0, 0x0053} /* LATIN CAPITAL LETTER S */,
  {0, 0, 0x0054} /* LATIN CAPITAL LETTER T */,
  {0, 0, 0x0055} /* LATIN CAPITAL LETTER U */,
  {0, 0, 0x0056} /* LATIN CAPITAL LETTER V */,
  {0, 0, 0x0057} /* LATIN CAPITAL LETTER W */,
  {0, 0, 0x0058} /* LATIN CAPITAL LETTER X */,
  {0, 0, 0x0059} /* LATIN CAPITAL LETTER Y */,
  {0, 0, 0x005A} /* LATIN CAPITAL LETTER Z */,
  {0, 0, 0x005B} /* OPENING SQUARE BRACKET / LEFT SQUARE BRACKET */,
  {0, 0, 0x005C} /* REVERSE SLASH / REVERSE SOLIDUS */,
  {0, 0, 0x005D} /* CLOSING SQUARE BRACKET / RIGHT SQUARE BRACKET */,
  {0, 0, 0x005E} /* SPACING CIRCUMFLEX / CIRCUMFLEX ACCENT */,
  {0, 0, 0x005F} /* SPACING UNDERSCORE / LOW LINE */,
  {0, 0, 0x0060} /* SPACING GRAVE / GRAVE ACCENT */,
  {0, 0, 0x0061} /* LATIN SMALL LETTER A */,
  {0, 0, 0x0062} /* LATIN SMALL LETTER B */,
  {0, 0, 0x0063} /* LATIN SMALL LETTER C */,
  {0, 0, 0x0064} /* LATIN SMALL LETTER D */,
  {0, 0, 0x0065} /* LATIN SMALL LETTER E */,
  {0, 0, 0x0066} /* LATIN SMALL LETTER F */,
  {0, 0, 0x0067} /* LATIN SMALL LETTER G */,
  {0, 0, 0x0068} /* LATIN SMALL LETTER H */,
  {0, 0, 0x0069} /* LATIN SMALL LETTER I */,
  {0, 0, 0x006A} /* LATIN SMALL LETTER J */,
  {0, 0, 0x006B} /* LATIN SMALL LETTER K */,
  {0, 0, 0x006C} /* LATIN SMALL LETTER L */,
  {0, 0, 0x006D} /* LATIN SMALL LETTER M */,
  {0, 0, 0x006E} /* LATIN SMALL LETTER N */,
  {0, 0, 0x006F} /* LATIN SMALL LETTER O */,
  {0, 0, 0x0070} /* LATIN SMALL LETTER P */,
  {0, 0, 0x0071} /* LATIN SMALL LETTER Q */,
  {0, 0, 0x0072} /* LATIN SMALL LETTER R */,
  {0, 0, 0x0073} /* LATIN SMALL LETTER S */,
  {0, 0, 0x0074} /* LATIN SMALL LETTER T */,
  {0, 0, 0x0075} /* LATIN SMALL LETTER U */,
  {0, 0, 0x0076} /* LATIN SMALL LETTER V */,
  {0, 0, 0x0077} /* LATIN SMALL LETTER W */,
  {0, 0, 0x0078} /* LATIN SMALL LETTER X */,
  {0, 0, 0x0079} /* LATIN SMALL LETTER Y */,
  {0, 0, 0x007A} /* LATIN SMALL LETTER Z */,
  {0, 0, 0x007B} /* OPENING CURLY BRACKET / LEFT CURLY BRACKET */,
  {0, 0, 0x007C} /* VERTICAL BAR (FILL) / VERTICAL LINE */,
  {0, 0, 0x007D} /* CLOSING CURLY BRACKET / RIGHT CURLY BRACKET */,
  {0, 0, 0x007E} /* SPACING TILDE / TILDE */,
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};
struct yaz_iconv_trie iso5426_42page0 = {
  0, iso5426_42page0_dir
};
struct yaz_iconv_trie *iso5426_42ptrs [] = {
 &iso5426_42page0,
0, };

unsigned long yaz_iso5426_42_conv
            (unsigned char *inp, size_t inbytesleft, size_t *no_read, int *combining, unsigned mask, unsigned int boffset)
        {
            unsigned long code;
            
            code = lookup(iso5426_42ptrs, 1, inp, inbytesleft, no_read, combining, mask, boffset);
            if (!code)
            {
                *no_read = 1;
            }
            return code;
        }
    
/* TRIE: size 76 */
/* PAGE 0 */
struct yaz_iconv_trie_dir iso5426_45page0_dir[256] = {
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0x0098} /* NON-SORT BEGIN / START OF STRING */,
  {0, 0, 0x009C} /* NON-SORT END / STRING TERMINATOR */,
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0x00A1} /* INVERTED EXCLAMATION MARK */,
  {0, 0, 0x201E} /* LOW DOUBLE COMMA QUOTATION MARK */,
  {0, 0, 0x00A3} /* BRITISH POUND / POUND SIGN */,
  {0, 0, 0x0024} /* DOLLAR SIGN */,
  {0, 0, 0x00A5} /* YEN SIGN */,
  {0, 0, 0x2020} /* DAGGER */,
  {0, 0, 0x00A7} /* SECTION SIGN */,
  {0, 0, 0x2032} /* PRIME */,
  {0, 0, 0x2018} /* SINGLE TURNED COMMA QUOTATION MARK */,
  {0, 0, 0x201C} /* DOUBLE TURNED COMMA QUOTATION MARK */,
  {0, 0, 0x00AB} /* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK (LEFT POINTING GUILLEMET) */,
  {0, 0, 0x266D} /* MUSIC FLAT SIGN (FLAT) */,
  {0, 0, 0x00A9} /* COPYRIGHT SIGN */,
  {0, 0, 0x2117} /* SOUND RECORDING COPYRIGHT */,
  {0, 0, 0x00AE} /* PATENT MARK / REGISTERED SIGN */,
  {0, 0, 0x02BB} /* AYN / MODIFIER LETTER TURNED COMMA */,
  {0, 0, 0x02BC} /* ALIF / MODIFIER LETTER APOSTROPHE */,
  {0, 0, 0x201A} /* SINGLE LOW-9 QUOTATION MARK (LOW SINGLE COMMA QUOTATION MARK) */,
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0x2021} /*  */,
  {0, 0, 0x00B7} /* MIDDLE DOT */,
  {0, 0, 0x2033} /* DOUBLE PRIME */,
  {0, 0, 0x2019} /* RIGHT SINGLE QUOTATION MARK (SINGLE COMMA QUOTATION MARK) */,
  {0, 0, 0x201D} /* RIGHT DOUBLE QUOTATION MARK (DOUBLE COMMA QUOTATION MARK) */,
  {0, 0, 0x00BB} /* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK (RIGHT POINTING GUILLEMET) */,
  {0, 0, 0x266F} /* MUSIC SHARP SIGN */,
  {0, 0, 0x02B9} /* SOFT SIGN, PRIME / MODIFIER LETTER PRIME */,
  {0, 0, 0x02BA} /* HARD SIGN, DOUBLE PRIME / MODIFIER LETTER DOUBLE PRIME */,
  {0, 0, 0x00BF} /* INVERTED QUESTION MARK */,
  {0, 1, 0x0309} /* PSEUDO QUESTION MARK / COMBINING HOOK ABOVE */,
  {0, 1, 0x0300} /* GRAVE / COMBINING GRAVE ACCENT (Varia) */,
  {0, 1, 0x0301} /* ACUTE / COMBINING ACUTE ACCENT (Oxia) */,
  {0, 1, 0x0302} /* CIRCUMFLEX / COMBINING CIRCUMFLEX ACCENT */,
  {0, 1, 0x0303} /* TILDE / COMBINING TILDE */,
  {0, 1, 0x0304} /* MACRON / COMBINING MACRON */,
  {0, 1, 0x0306} /* BREVE / COMBINING BREVE (Vrachy) */,
  {0, 1, 0x0307} /* SUPERIOR DOT / COMBINING DOT ABOVE */,
  {0, 1, 0x0308} /* U+034F COMBINING GRAPHEME JOINER (CGJ) / tréma */,
  {0, 1, 0x0308} /* UMLAUT, DIAERESIS / COMBINING DIAERESIS (Dialytika) */,
  {0, 1, 0x030A} /* CIRCLE ABOVE, ANGSTROM / COMBINING RING ABOVE */,
  {0, 1, 0x0315} /* HIGH COMMA, OFF CENTER / COMBINING COMMA ABOVE RIGHT */,
  {0, 1, 0x0313} /* HIGH COMMA, CENTERED / COMBINING COMMA ABOVE (Psili) */,
  {0, 1, 0x030B} /* DOUBLE ACUTE / COMBINING DOUBLE ACUTE ACCENT */,
  {0, 1, 0x031B} /* COMBINING HORN (NON-SPACING HORN) */,
  {0, 1, 0x030C} /* HACEK / COMBINING CARON */,
  {0, 1, 0x0327} /* CEDILLA / COMBINING CEDILLA */,
  {0, 1, 0x031C} /* RIGHT CEDILLA / COMBINING LEFT HALF RING BELOW */,
  {0, 1, 0x0326} /* LEFT HOOK (COMMA BELOW) / COMBINING COMMA BELOW */,
  {0, 1, 0x0328} /* RIGHT HOOK, OGONEK / COMBINING OGONEK */,
  {0, 1, 0x0325} /* CIRCLE BELOW / COMBINING RING BELOW */,
  {0, 1, 0x032E} /* UPADHMANIYA / COMBINING BREVE BELOW */,
  {0, 1, 0x0323} /* DOT BELOW / COMBINING DOT BELOW */,
  {0, 1, 0x0324} /* DOUBLE DOT BELOW / COMBINING DIAERESIS BELOW */,
  {0, 1, 0x0332} /* UNDERSCORE / COMBINING LOW LINE */,
  {0, 1, 0x0333} /* DOUBLE UNDERSCORE / COMBINING DOUBLE LOW LINE */,
  {0, 1, 0x0329} /* COMBINING VERTICAL LINE BELOW (NON-SPACING VERTICAL LINE BELOW) */,
  {0, 1, 0x032D} /* COMBINING CIRCUMFLEX ACCENT BELOW (NON-SPACING CIRCUMFLEX BELOW) */,
  {0, 0, 0},
  {0, 1, 0x0360} /* DOUBLE TILDE, FIRST HALF / COMBINING DOUBLE TILDE */,
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0x00C6} /* UPPERCASE DIGRAPH AE / LATIN CAPITAL LIGATURE AE */,
  {0, 0, 0x0110} /* UPPERCASE D WITH CROSSBAR / LATIN CAPITAL LETTER D WITH STROKE */,
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0x0132} /* LATIN CAPITAL LIGATURE IJ (LATIN CAPITAL LETTER I J) */,
  {0, 0, 0},
  {0, 0, 0x0141} /* UPPERCASE POLISH L / LATIN CAPITAL LETTER L WITH STROKE */,
  {0, 0, 0x00D8} /* UPPERCASE SCANDINAVIAN O / LATIN CAPITAL LETTER O WITH STROKE */,
  {0, 0, 0x0152} /* UPPERCASE DIGRAPH OE / LATIN CAPITAL LIGATURE OE */,
  {0, 0, 0},
  {0, 0, 0x00DE} /* UPPERCASE ICELANDIC THORN / LATIN CAPITAL LETTER THORN (Icelandic) */,
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0x00E6} /* LOWERCASE DIGRAPH AE / LATIN SMALL LIGATURE AE */,
  {0, 0, 0x0111} /* LOWERCASE D WITH CROSSBAR / LATIN SMALL LETTER D WITH STROKE */,
  {0, 0, 0x00F0} /* LOWERCASE ETH / LATIN SMALL LETTER ETH (Icelandic) */,
  {0, 0, 0},
  {0, 0, 0x0131} /* LOWERCASE TURKISH I / LATIN SMALL LETTER DOTLESS I */,
  {0, 0, 0x0133} /* LATIN SMALL LIGATURE IJ (LATIN SMALL LETTER I J) */,
  {0, 0, 0},
  {0, 0, 0x0142} /* LOWERCASE POLISH L / LATIN SMALL LETTER L WITH STROKE */,
  {0, 0, 0x00F8} /* LOWERCASE SCANDINAVIAN O / LATIN SMALL LETTER O WITH STROKE */,
  {0, 0, 0x0153} /* LOWERCASE DIGRAPH OE / LATIN SMALL LIGATURE OE */,
  {0, 0, 0x00DF} /* ESZETT SYMBOL */,
  {0, 0, 0x00FE} /* LOWERCASE ICELANDIC THORN / LATIN SMALL LETTER THORN (Icelandic) */,
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};
struct yaz_iconv_trie iso5426_45page0 = {
  0, iso5426_45page0_dir
};
struct yaz_iconv_trie *iso5426_45ptrs [] = {
 &iso5426_45page0,
0, };

unsigned long yaz_iso5426_45_conv
            (unsigned char *inp, size_t inbytesleft, size_t *no_read, int *combining, unsigned mask, unsigned int boffset)
        {
            unsigned long code;
            
            code = lookup(iso5426_45ptrs, 1, inp, inbytesleft, no_read, combining, mask, boffset);
            if (!code)
            {
                *no_read = 1;
            }
            return code;
        }
    
