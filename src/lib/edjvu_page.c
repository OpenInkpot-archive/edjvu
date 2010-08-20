#include <Evas.h>

#include <libdjvu/ddjvuapi.h>

#include "edjvu_private.h"
#include "edjvu_misc.h"
#include "Edjvu.h"

Edjvu_Page *
edjvu_page_new(const Edjvu_Document * doc)
{
    Edjvu_Page *page;

    if (!doc)
        return NULL;

    page = (Edjvu_Page *) malloc(sizeof(Edjvu_Page));
    if (!page)
        return NULL;

    page->doc = (Edjvu_Document *) doc;
    page->format = NULL;
    page->page = NULL;
    page->index = 0;

    page->format = ddjvu_format_create(DDJVU_FORMAT_GREY8, 0, 0);
    ddjvu_format_set_row_order(page->format, 1);
    ddjvu_format_set_y_direction(page->format, 1);

    return page;
}

void
edjvu_page_delete(Edjvu_Page * page)
{
    if (!page)
        return;

    if (page->page) {
        ddjvu_page_release(page->page);
        page->page = NULL;
    }

    if (page->format) {
        ddjvu_format_release(page->format);
        page->format = NULL;
    }

    free(page);
}

static void
edjvu_page_load(Edjvu_Page * page)
{
    if (!page)
        return;

    if (page->page)
        ddjvu_page_release(page->page);
    page->page = NULL;

    Edjvu_Document *doc = page->doc;
    if (!doc)
        return;

    page->page = ddjvu_page_create_by_pageno(doc->document, page->index);
    while (!ddjvu_page_decoding_done(page->page))
        handle_ddjvu_messages(doc->context, 1);
    if (ddjvu_page_decoding_error(page->page))
        page->page = NULL;
}

void
edjvu_page_render(Edjvu_Page * page, Evas_Object * o)
{
    edjvu_page_render_slice(page, o, 0, 0, -1, -1);
}

void
edjvu_page_render_slice(Edjvu_Page * page,
                        Evas_Object * o, int x, int y, int w, int h)
{
    ddjvu_rect_t prect;
    ddjvu_rect_t rrect;

    if (!page)
        return;

    if (!page->page)
        edjvu_page_load(page);

    Edjvu_Document *doc = page->doc;
    if (!doc)
        return;

    if (w == -1 || h == -1) {
        rrect.x = prect.x = 0;
        rrect.y = prect.y = 0;
        rrect.w = prect.w = ddjvu_page_get_width(page->page) * doc->zoom;
        rrect.h = prect.h = ddjvu_page_get_height(page->page) * doc->zoom;
    } else {
        // page
        prect.x = 0;
        prect.y = 0;
        prect.w = ddjvu_page_get_width(page->page) * doc->zoom;
        prect.h = ddjvu_page_get_height(page->page) * doc->zoom;

        if (x < 0)
            x = 0;
        if (y < 0)
            y = 0;

        // vieport
        rrect.x = x;
        rrect.y = y;
        rrect.w = w;
        rrect.h = h;

        if (rrect.w > prect.w)
            rrect.w = prect.w;

        if (rrect.h > prect.h)
            rrect.h = prect.h;

        if (prect.w < rrect.x + rrect.w)
            rrect.x = prect.w - rrect.w;

        if (prect.h < rrect.y + rrect.h)
            rrect.y = prect.h - rrect.h;
    }

    int width = rrect.w;
    int height = rrect.h;

    evas_object_image_size_set(o, width, height);
    evas_object_image_fill_set(o, 0, 0, width, height);

    char *buf = (char *) malloc(width * height);

    ddjvu_render_mode_t render_mode = DDJVU_RENDER_BLACK;
    switch (ddjvu_page_get_type(page->page)) {
    case DDJVU_PAGETYPE_UNKNOWN:
    case DDJVU_PAGETYPE_BITONAL:
        render_mode = DDJVU_RENDER_BLACK;
        break;
    case DDJVU_PAGETYPE_PHOTO:
    case DDJVU_PAGETYPE_COMPOUND:
        render_mode = DDJVU_RENDER_COLOR;
        break;
    }

    if (ddjvu_page_render(page->page, render_mode,
                          &prect, &rrect, page->format, rrect.w, buf)) {
        unsigned char *d = (char *) evas_object_image_data_get(o, 1);
        int stride = evas_object_image_stride_get(o);
        unsigned char *s = buf;
        for (int i = 0; i < height; i++) {
            memcpy(d, s, width);
            s += width;
            d += stride;
        }
    }

    evas_object_image_data_update_add(o, 0, 0, width, height);
    evas_object_resize(o, width, height);

    if (buf)
        free(buf);

    ddjvu_cache_clear(page->doc->context);
}

void
edjvu_page_page_set(Edjvu_Page * page, int p)
{
    if (!page)
        return;

    page->index = p;
    if (page->page)
        ddjvu_page_release(page->page);
    page->page = NULL;
}

int
edjvu_page_page_get(const Edjvu_Page * page)
{
    if (!page)
        return -1;

    return page->index;
}

void
edjvu_page_size_get(const Edjvu_Page * page, int *width, int *height)
{
    if (!page) {
        *width = 0;
        *height = 0;
        return;
    }

    if (!page->page)
        edjvu_page_load((Edjvu_Page *) page);

    *width = ddjvu_page_get_width(page->page);
    *height = ddjvu_page_get_height(page->page);
}

void
edjvu_page_content_geometry_get(const Edjvu_Page * page, int *x, int *y,
                                int *width, int *height)
{
    edjvu_page_size_get(page, width, height);
    *x = 0;
    *y = 0;
}

void
edjvu_page_scale_set(Edjvu_Page * page, double hscale, double vscale)
{
    if (!page || !page->doc)
        return;

    page->doc->zoom = hscale;
}

void
edjvu_page_scale_get(const Edjvu_Page * page,
                     double *hscale, double *vscale)
{
    if (!page || !page->doc) {
        *hscale = 0.0;
        *vscale = 0.0;
        return;
    }

    *hscale = *vscale = page->doc->zoom;
}
