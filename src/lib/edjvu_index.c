#include <string.h>
#include <libdjvu/ddjvuapi.h>
#include <libdjvu/miniexp.h>

#include <Ecore.h>

#include "edjvu_private.h"
#include "edjvu_misc.h"
#include "Edjvu.h"

static void edjvu_index_fill(Ecore_List * items, miniexp_t outline);
static void edjvu_index_unfill(Ecore_List * items);

/* Index item */
Edjvu_Index_Item *
edjvu_index_item_new()
{
    Edjvu_Index_Item *item;

    item = (Edjvu_Index_Item *) malloc(sizeof(Edjvu_Index_Item));
    if (!item)
        return NULL;

    item->title = NULL;
    item->page = -1;
    item->children = NULL;

    return item;
}

void
edjvu_index_item_delete(Edjvu_Index_Item * item)
{
    if (!item)
        return;

    if (item->title)
        free(item->title);
    if (item->children) {
        Edjvu_Index_Item *i;

        ecore_list_first_goto(item->children);
        while ((i = (Edjvu_Index_Item *) ecore_list_next(item->children)))
            edjvu_index_item_delete(i);
    }
    free(item);
}

const char *
edjvu_index_item_title_get(const Edjvu_Index_Item * item)
{
    if (!item)
        return NULL;

    return item->title;
}

Ecore_List *
edjvu_index_item_children_get(const Edjvu_Index_Item * item)
{
    if (!item)
        return NULL;

    return item->children;
}

int
edjvu_index_item_page_get(const Edjvu_Document * doc,
                          const Edjvu_Index_Item * item)
{
    if (!item)
        return -1;

    return item->page;
}

/* Index */

Ecore_List *
edjvu_index_new(const Edjvu_Document * doc)
{
    Ecore_List *index = NULL;

    if (!doc)
        return index;

    index = ecore_list_new();

    miniexp_t outline;
    while ((outline =
            ddjvu_document_get_outline(doc->document)) == miniexp_dummy)
        handle_ddjvu_messages(doc->context, 1);

    if (miniexp_listp(outline)
        && miniexp_length(outline) > 0
        && miniexp_symbolp(miniexp_car(outline))
        && !strcmp(miniexp_to_name(miniexp_car(outline)), "bookmarks"))
        edjvu_index_fill(index, miniexp_cdr(outline));

    ddjvu_miniexp_release(doc->document, outline);

    return index;
}

void
edjvu_index_delete(Ecore_List * index)
{
    if (!index)
        return;

    edjvu_index_unfill(index);
}

static void
edjvu_index_fill(Ecore_List * items, miniexp_t outline)
{
    if (!items)
        return;

    if (!miniexp_listp(outline))
        return;

    miniexp_t cur = miniexp_car(outline);

    if (miniexp_consp(cur)
        && miniexp_length(cur) > 0 && miniexp_stringp(miniexp_car(cur))
        && miniexp_stringp(miniexp_cadr(cur))) {
        Edjvu_Index_Item *item;
        item = edjvu_index_item_new();
        item->title = strdup(miniexp_to_str(miniexp_car(cur)));

        const char *page = miniexp_to_str(miniexp_cadr(cur));
        if (page && *page == '#')
            item->page = atoi(page + 1) - 1;

        ecore_list_append(items, item);

        if (miniexp_cddr(cur) != miniexp_nil) {
            item->children = ecore_list_new();
            edjvu_index_fill(item->children, miniexp_cddr(cur));
        }
    }

    if (miniexp_cdr(outline) != miniexp_nil)
        edjvu_index_fill(items, miniexp_cdr(outline));
}

static void
edjvu_index_unfill(Ecore_List * items)
{
    Edjvu_Index_Item *item;

    if (!items)
        return;

    ecore_list_first_goto(items);
    while ((item = (Edjvu_Index_Item *) ecore_list_next(items))) {
        if (item->title)
            free(item->title);
        if (item->children)
            edjvu_index_unfill(item->children);

        free(item);
    }
    ecore_list_destroy(items);
}
