#ifndef __EDJVU_PAGE_H__
#define __EDJVU_PAGE_H__

#include <Evas.h>

#include <libdjvu/ddjvuapi.h>

#include "edjvu_private.h"

Edjvu_Page *edjvu_page_new(const Edjvu_Document * doc);

void edjvu_page_delete(Edjvu_Page * page);

void edjvu_page_render(Edjvu_Page * page, Evas_Object * o);

void edjvu_page_render_slice(Edjvu_Page * page,
                             Evas_Object * o, int x, int y, int w, int h);

void edjvu_page_page_set(Edjvu_Page * page, int p);

int edjvu_page_page_get(const Edjvu_Page * page);

void edjvu_page_size_get(const Edjvu_Page * page, int *width, int *height);

void edjvu_page_content_geometry_get(const Edjvu_Page * page, int *x,
                                     int *y, int *width, int *height);

void edjvu_page_scale_set(Edjvu_Page * page, double hscale, double vscale);

void edjvu_page_scale_get(const Edjvu_Page * page,
                          double *hscale, double *vscale);

#endif                          /* __EDJVU_PAGE_H__ */
