#include <libdjvu/ddjvuapi.h>

void
handle_ddjvu_messages(ddjvu_context_t * ctx, int wait)
{
    const ddjvu_message_t *msg;
    if (wait)
        ddjvu_message_wait(ctx);
    while ((msg = ddjvu_message_peek(ctx))) {
        switch (msg->m_any.tag) {
        case DDJVU_ERROR:
            fprintf(stderr, "%s\n", msg->m_error.message);
            break;
        case DDJVU_INFO:
            fprintf(stderr, "%s\n", msg->m_info.message);
            break;
        case DDJVU_NEWSTREAM:
            break;
        default:
            break;
        }
        ddjvu_message_pop(ctx);
    }
}
