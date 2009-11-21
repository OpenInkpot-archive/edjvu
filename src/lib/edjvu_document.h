#ifndef __EDJVU_DOCUMENT_H__
#define __EDJVU_DOCUMENT_H__

Edjvu_Document *edjvu_document_new(const char *filename);
void edjvu_document_delete(Edjvu_Document * document);
int edjvu_document_page_count_get(const Edjvu_Document * document);

/*
char *edjvu_document_title_get(const Edjvu_Document * doc);
char *edjvu_document_author_get(const Edjvu_Document * doc);
char *edjvu_document_subject_get(const Edjvu_Document * doc);
char *edjvu_document_keywords_get(const Edjvu_Document * doc);
char *edjvu_document_creator_get(const Edjvu_Document * doc);
char *edjvu_document_producer_get(const Edjvu_Document * doc);
char *edjvu_document_creation_date_get(const Edjvu_Document * doc);
char *edjvu_document_mod_date_get(const Edjvu_Document * doc);
*/

#endif                          /* __EDJVU_DOCUMENT_H__ */
