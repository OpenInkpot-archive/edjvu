#ifndef __EDJVU_PRIVATE_H__
#define __EDJVU_PRIVATE_H__

#include <Eina.h>
#include <libdjvu/ddjvuapi.h>

typedef struct _Edjvu_Document Edjvu_Document;
typedef struct _Edjvu_Index_Item Edjvu_Index_Item;
typedef struct _Edjvu_Page Edjvu_Page;

struct _Edjvu_Page {
    Edjvu_Document *doc;
    int index;

    ddjvu_page_t *page;
    ddjvu_format_t *format;
    ddjvu_render_mode_t render_mode;
};

struct _Edjvu_Document {
    char *filename;
    char *doctitle;

    ddjvu_context_t *context;
    ddjvu_document_t *document;

    int pagecount;
    float zoom;
};

struct _Edjvu_Index_Item {
    char *title;
    int page;
    Eina_List *children;
};

#endif                          /* __EDJVU_PRIVATE_H__ */
