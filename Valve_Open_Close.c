#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define VALVE_TIMEOUT_MS 5000

typedef enum
{
    VALVE_CLOSED,
    VALVE_OPENING,
    VALVE_OPENED,
    VALVE_CLOSING,
    VALVE_FAULT
} ValveState_t;

typedef struct
{
    ValveState_t state;

    bool cmdOpen;
    bool cmdClose;
    bool resetFault;

    bool sensorOpen;
    bool sensorClose;

    bool outOpen;
    bool outClose;

    uint32_t timerStart;

} Valve_t;

/* Example function supplied by platform */
extern uint32_t GetSystemTimeMs(void);

void Valve_Init(Valve_t *v)
{
    v->state = VALVE_CLOSED;

    v->outOpen = false;
    v->outClose = false;

    v->timerStart = 0;
}

void Valve_Update(Valve_t *v)
{
    uint32_t now = GetSystemTimeMs();

    switch (v->state)
    {
        case VALVE_CLOSED:

            v->outOpen = false;
            v->outClose = false;

            if (v->cmdOpen)
            {
                v->state = VALVE_OPENING;
                v->timerStart = now;
            }
            break;

        case VALVE_OPENING:

            v->outOpen = true;
            v->outClose = false;

            if (v->sensorOpen)
            {
                v->outOpen = false;
                v->state = VALVE_OPENED;
            }
            else if ((now - v->timerStart) >= VALVE_TIMEOUT_MS)
            {
                v->outOpen = false;
                v->state = VALVE_FAULT;
            }
            break;

        case VALVE_OPENED:

            v->outOpen = false;
            v->outClose = false;

            if (v->cmdClose)
            {
                v->state = VALVE_CLOSING;
                v->timerStart = now;
            }
            break;

        case VALVE_CLOSING:

            v->outOpen = false;
            v->outClose = true;

            if (v->sensorClose)
            {
                v->outClose = false;
                v->state = VALVE_CLOSED;
            }
            else if ((now - v->timerStart) >= VALVE_TIMEOUT_MS)
            {
                v->outClose = false;
                v->state = VALVE_FAULT;
            }
            break;

        case VALVE_FAULT:

            v->outOpen = false;
            v->outClose = false;

            if (v->resetFault)
            {
                if (v->sensorClose)
                {
                    v->state = VALVE_CLOSED;
                }
                else if (v->sensorOpen)
                {
                    v->state = VALVE_OPENED;
                }
            }
            break;

        default:
            v->state = VALVE_FAULT;
            break;
    }
}
