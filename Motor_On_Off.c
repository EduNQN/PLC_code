#include <stdint.h>
#include <stdbool.h>

#define MOTOR_TIMEOUT_MS 5000

typedef enum
{
    MOTOR_STOPPED,
    MOTOR_STARTING,
    MOTOR_RUNNING,
    MOTOR_STOPPING,
    MOTOR_FAULT

} MotorState_t;

typedef struct
{
    MotorState_t state;

    /* Commands */
    bool cmdStart;
    bool cmdStop;
    bool resetFault;

    /* Inputs */
    bool runFeedback;

    /* Outputs */
    bool motorOutput;

    /* Internal */
    uint32_t timerStart;

} Motor_t;

/* Platform dependent */
extern uint32_t GetSystemTimeMs(void);

void Motor_Init(Motor_t *m)
{
    m->state = MOTOR_STOPPED;
    m->motorOutput = false;
    m->timerStart = 0;
}

void Motor_Update(Motor_t *m)
{
    uint32_t now = GetSystemTimeMs();

    switch(m->state)
    {
        case MOTOR_STOPPED:

            m->motorOutput = false;

            if(m->cmdStart)
            {
                m->motorOutput = true;
                m->timerStart = now;
                m->state = MOTOR_STARTING;
            }
            break;

        case MOTOR_STARTING:

            m->motorOutput = true;

            if(m->runFeedback)
            {
                m->state = MOTOR_RUNNING;
            }
            else if((now - m->timerStart) >= MOTOR_TIMEOUT_MS)
            {
                m->motorOutput = false;
                m->state = MOTOR_FAULT;
            }
            break;

        case MOTOR_RUNNING:

            m->motorOutput = true;

            if(m->cmdStop)
            {
                m->motorOutput = false;
                m->timerStart = now;
                m->state = MOTOR_STOPPING;
            }
            break;

        case MOTOR_STOPPING:

            m->motorOutput = false;

            if(!m->runFeedback)
            {
                m->state = MOTOR_STOPPED;
            }
            else if((now - m->timerStart) >= MOTOR_TIMEOUT_MS)
            {
                m->state = MOTOR_FAULT;
            }
            break;

        case MOTOR_FAULT:

            m->motorOutput = false;

            if(m->resetFault)
            {
                if(m->runFeedback)
                {
                    m->state = MOTOR_RUNNING;
                }
                else
                {
                    m->state = MOTOR_STOPPED;
                }
            }
            break;

        default:
            m->state = MOTOR_FAULT;
            break;
    }
}