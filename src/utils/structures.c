#include "../headers/structures.h"

/**
 * Crea un pacchetto per il clock.
 * @param args  Gli argomenti del pacchetto.
 * @param exm   Il modo di esecuzione.
 */
ClockPacket *create_clock_packet(va_list args, ExecutionMode exm)
{
    ClockPacket *clock_pkt = MALLOC(ClockPacket, 1);

    if (exm == THREAD)
    {
        (*clock_pkt).carriage.t.mutex = va_arg(args, pthread_mutex_t *);
    }
    else
    {
        (*clock_pkt).carriage.p.c = va_arg(args, pipe_t);
        (*clock_pkt).carriage.p.se = va_arg(args, pipe_t);
        (*clock_pkt).carriage.p.s = va_arg(args, pipe_t);
    }

    (*clock_pkt).time_left = va_arg(args, unsigned int *);
    (*clock_pkt).cancelled = false;

    return clock_pkt;
}

/**
 * Crea un pacchetto per l'entità ridotta.
 * @param args  Gli argomenti del pacchetto.
 * @param exm   Il modo di esecuzione.
 */
TinyEntityMovePacket *create_tiny_entity_move_packet(va_list args, ExecutionMode exm)
{
    TinyEntityMovePacket *tiny_entity_move_pkt = MALLOC(TinyEntityMovePacket, 1);

    if (exm == THREAD)
    {
        (*tiny_entity_move_pkt).carriage.t.entity_mutex = va_arg(args, pthread_mutex_t *);
        (*tiny_entity_move_pkt).carriage.t.entity_action = va_arg(args, Action *);
    }
    else
    {
        (*tiny_entity_move_pkt).carriage.p.c = va_arg(args, pipe_t);
        (*tiny_entity_move_pkt).carriage.p.se = va_arg(args, pipe_t);
        (*tiny_entity_move_pkt).carriage.p.s = va_arg(args, pipe_t);
    }

    (*tiny_entity_move_pkt).id = va_arg(args, unsigned int);
    (*tiny_entity_move_pkt).cancelled = false;

    return tiny_entity_move_pkt;
}

/**
 * Crea un pacchetto per l'entità.
 * @param args  Gli argomenti del pacchetto.
 * @param exm   Il modo di esecuzione.
 */
EntityMovePacket *create_entity_move_packet(va_list args, ExecutionMode exm)
{
    EntityMovePacket *entity_move_pkt = MALLOC(EntityMovePacket, 1);

    (*entity_move_pkt).sub_packet = *create_tiny_entity_move_packet(args, exm);
    (*entity_move_pkt).default_action = va_arg(args, Action);

    return entity_move_pkt;
}

/**
 * Crea un pacchetto.
 * @param rqtype    Il tipo di richiesta.
 * @param cntype    Il tipo di contenuto (CLOCK_PKG, FROG_PKG, ENTITY_PKG).
 * @param ...       Gli argomenti del pacchetto.
 */
Package *pack(RequestType rqtype, ContentType cntype, ...)
{
    Package *pkg = MALLOC(Package, 1);

    va_list args;
    va_start(args, cntype);

    (*pkg).rqtype = MALLOC(RequestType, 1);
    (*pkg).cntype = MALLOC(ContentType, 1);

    *(*pkg).rqtype = rqtype;
    *(*pkg).cntype = cntype;

    switch (cntype)
    {
        case CLOCK_PKG:
            pkg->arg.cpkt = create_clock_packet(args, rqtype);
            break;
        case FROG_PKG:
            pkg->arg.tempkt = create_tiny_entity_move_packet(args, rqtype);
            break;
        case ENTITY_PKG:
            pkg->arg.empkt = create_entity_move_packet(args, rqtype);
            break;
        default:
            break;
    }

    va_end(args);

    return pkg;
}

/**
 * Dealloca un pacchetto.
 * @param pkg   Il pacchetto.
 */
void destroy_package(Package *pkg)
{
    free((*pkg).rqtype);
    free((*pkg).cntype);
    free(pkg);
}