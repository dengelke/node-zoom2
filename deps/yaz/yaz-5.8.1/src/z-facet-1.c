/** \file z-facet-1.c
    \brief ASN.1 Module UserInfoFormat-facet-1

    Generated automatically by YAZ ASN.1 Compiler 0.4
*/
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <yaz/z-facet-1.h>

int z_FacetList(ODR o, Z_FacetList **p, int opt, const char *name)
{
	if (!odr_initmember(o, p, sizeof(**p)))
		return odr_missing(o, opt, name);
	if (odr_sequence_of(o, (Odr_fun) z_FacetField, &(*p)->elements,
		&(*p)->num, name))
		return 1;
	if (o->direction == ODR_DECODE)
		*p = 0;
	return odr_missing(o, opt, name);
}

int z_FacetField(ODR o, Z_FacetField **p, int opt, const char *name)
{
	if (!odr_sequence_begin(o, p, sizeof(**p), name))
		return odr_missing(o, opt, name) && odr_ok (o);
	return
		odr_implicit_tag(o, z_AttributeList,
			&(*p)->attributes, ODR_CONTEXT, 1, 0, "attributes") &&
		odr_implicit_settag(o, ODR_CONTEXT, 2) &&
		(odr_sequence_of(o, (Odr_fun) z_FacetTerm, &(*p)->terms,
		  &(*p)->num_terms, "terms") || odr_ok(o)) &&
		odr_sequence_end(o);
}

int z_FacetTerm(ODR o, Z_FacetTerm **p, int opt, const char *name)
{
	if (!odr_sequence_begin(o, p, sizeof(**p), name))
		return odr_missing(o, opt, name) && odr_ok (o);
	return
		z_Term(o, &(*p)->term, 0, "term") &&
		odr_integer(o, &(*p)->count, 0, "count") &&
		odr_sequence_end(o);
}
