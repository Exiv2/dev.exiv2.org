        // Schemas
        { "http://rs.tdwg.org/dwc/terms/",                 "dwc",          xmpDwCInfo,       "XMP Darwin Core schema" },

   extern const XmpPropertyInfo xmpDwCInfo[] = {
	{ "acceptedNameUsage",			"acceptedNameUsage",		"Text",	xmpText,	xmpExternal,	"The full name, with authorship and date information if known, of the currently valid (zoological) or accepted (botanical) taxon." },
	{ "acceptedNameUsageID",		"acceptedNameUsageID",		"Text",	xmpText,	xmpExternal,	"An identifier for the name usage (documented meaning of the name according to a source) of the currently valid (zoological) or accepted (botanical) taxon ." },
	{ "behavior",				"behavior",				"Text",	xmpText,	xmpExternal,	"A description of the behavior shown by the subject at the time the Occurrence was recorded. Recommended best practice is to use a controlled vocabulary ." },
	{ "bibliographicCitation",		"bibliographicCitation",	"Text",	xmpText,	xmpExternal,	"A bibliographic reference for the resource as a statement indicating how this record should be cited (attributed) when used. Recommended practice is to include sufficient bibliographic detail to identify the resource as unambiguously as possible ." },
	{ "earliestEraOrLowestErathem",	"earliestEraOrLowestErathem",	"Text",	xmpText,	xmpExternal,	"The full name of the earliest possible geochronologic era or lowest chronostratigraphic erathem attributable to the stratigraphic horizon from which the cataloged item was collected ." },
	{ "identifiedBy",				"identifiedBy",			"Text",	xmpText,	xmpExternal,	"A list (concatenated and separated) of names of people, groups, or organizations who assigned the Taxon to the subject ." },
	{ "individualCount",			"individualCount",		"Text",	xmpText,	xmpExternal,	"The number of individuals represented present at the time of the Occurrence ." },
	{ "scientificName",			"scientificName",			"Text",	xmpText,	xmpExternal,	"The full scientific name, with authorship and date information if known. When forming part of an Identification, this should be the name in lowest level taxonomic rank that can be determined. This term should not contain identification qualifications, which should instead be supplied in the IdentificationQualifier term ." },
	{ "scientificNameAuthorship",		"scientificNameAuthorship",	"Text",	xmpText,	xmpExternal,	"The authorship information for the scientificName formatted according to the conventions of the applicable nomenclaturalCode ." },
	{ "scientificNameID",			"scientificNameID",		"Text",	xmpText,	xmpExternal,	"An identifier for the nomenclatural (not taxonomic) details of a scientific name ." },
	{ "verbatimTaxonRank",			"verbatimTaxonRank",		"Text",	xmpText,	xmpExternal,	"The taxonomic rank of the most specific name in the scientificName as it appears in the original record ." },
	{ "vernacularName",			"vernacularName",			"Text",	xmpText,	xmpExternal,	"A common or vernacular name ." },
      // End of list marker
        { 0, 0, 0, invalidTypeId, xmpInternal, 0 }
    };
