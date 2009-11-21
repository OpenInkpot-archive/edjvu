#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

#include <libdjvu/ddjvuapi.h>

#include "edjvu_private.h"
#include "edjvu_misc.h"
#include "Edjvu.h"

static void *
djvudoc_error(Edjvu_Document * doc)
{
    if (doc)
        free(doc);

    return NULL;
}

Edjvu_Document *
edjvu_document_new(const char *filename)
{
    Edjvu_Document *doc;

    doc = (Edjvu_Document *) malloc(sizeof(Edjvu_Document));
    if (!doc)
        return NULL;

    doc->pagecount = 0;
    doc->zoom = 1.0;

    doc->context = ddjvu_context_create("edjvu");
    if (!doc->context)
        return djvudoc_error(doc);

    doc->document =
        ddjvu_document_create_by_filename(doc->context, filename, 1);

    while (!ddjvu_document_decoding_done(doc->document))
        handle_ddjvu_messages(doc->context, 1);

    doc->pagecount = ddjvu_document_get_pagenum(doc->document);
    doc->filename = strdup(filename);

    if (doc->zoom < 0.1)
        doc->zoom = 0.1;
    if (doc->zoom > 3.0)
        doc->zoom = 3.0;

    return doc;
}

void
edjvu_document_delete(Edjvu_Document * doc)
{
    if (!doc)
        return;

    if (doc->document) {
        ddjvu_document_release(doc->document);
        doc->document = NULL;
    }

    if (doc->context) {
        ddjvu_context_release(doc->context);
        doc->context = NULL;
    }

    if (doc->filename) {
        free(doc->filename);
        doc->filename = NULL;
    }

    free(doc);
}

int
edjvu_document_page_count_get(const Edjvu_Document * doc)
{
    if (!doc)
        return 0;

    return doc->pagecount;
}

/*
static char *
edjvu_document_property_get(const Edjvu_Document * doc,
                            const char *property)
{
    return NULL;
}

char *
edjvu_document_title_get(const Edjvu_Document * doc)
{
    if (!doc)
        return NULL;

    return edjvu_document_property_get(doc, "Title");
}

char *
edjvu_document_author_get(const Edjvu_Document * doc)
{
    if (!doc)
        return NULL;

    return edjvu_document_property_get(doc, "Author");
}

char *
edjvu_document_subject_get(const Edjvu_Document * doc)
{
    if (!doc)
        return NULL;

    return edjvu_document_property_get(doc, "Subject");
}

char *
edjvu_document_keywords_get(const Edjvu_Document * doc)
{
    if (!doc)
        return NULL;

    return edjvu_document_property_get(doc, "Keywords");
}

char *
edjvu_document_creator_get(const Edjvu_Document * doc)
{
    if (!doc)
        return NULL;

    return edjvu_document_property_get(doc, "Creator");
}

char *
edjvu_document_producer_get(const Edjvu_Document * doc)
{
    if (!doc)
        return NULL;

    return edjvu_document_property_get(doc, "Producer");
}

static char *
edjvu_document_date_get(const Edjvu_Document * doc, const char *type)
{
    return NULL;
}

char *
edjvu_document_creation_date_get(const Edjvu_Document * doc)
{
    if (!doc)
        return NULL;

    return edjvu_document_date_get(doc, "CreationDate");
}

char *
edjvu_document_mod_date_get(const Edjvu_Document * doc)
{
    if (!doc)
        return NULL;

    return edjvu_document_date_get(doc, "ModDate");
}
*/
