#ifndef __EDJVU_INDEX_H__
#define __EDJVU_INDEX_H__

Edjvu_Index_Item *edjvu_index_item_new();
const char *edjvu_index_item_title_get(const Edjvu_Index_Item * item);
Eina_List *edjvu_index_item_children_get(const Edjvu_Index_Item * item);
int edjvu_index_item_page_get(const Edjvu_Document * document,
                              const Edjvu_Index_Item * item);
Eina_List *edjvu_index_new(const Edjvu_Document * document);
void edjvu_index_delete(Eina_List * index);

#endif                          /* __EDJVU_INDEX_H__ */
