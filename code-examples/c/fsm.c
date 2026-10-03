/**
 * @file fsm.c
 * @brief Table-driven Finite State Machine (FSM) implementation in C99.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef enum {
    STATE_IDLE,
    STATE_ACTIVE,
    STATE_ERROR,
    STATE_MAX
} State_t;

typedef enum {
    EVENT_START,
    EVENT_FAULT,
    EVENT_RESET,
    EVENT_MAX
} Event_t;

/* Action callback prototype */
typedef void (*StateAction_t)(void);

static void Action_OnStart(void) {
    printf("  [Action] Transitioned to ACTIVE state: Motor started.\n");
}

static void Action_OnFault(void) {
    printf("  [Action] Transitioned to ERROR state: Emergency shutdown!\n");
}

static void Action_OnReset(void) {
    printf("  [Action] Transitioned to IDLE state: Fault cleared.\n");
}

/* State Transition Structure */
typedef struct {
    State_t       next_state;
    StateAction_t action;
} Transition_t;

/* State Transition Table [CurrentState][Event] */
static const Transition_t s_transition_table[STATE_MAX][EVENT_MAX] = {
    [STATE_IDLE] = {
        [EVENT_START] = { STATE_ACTIVE, Action_OnStart },
        [EVENT_FAULT] = { STATE_ERROR,  Action_OnFault },
        [EVENT_RESET] = { STATE_IDLE,   NULL },
    },
    [STATE_ACTIVE] = {
        [EVENT_START] = { STATE_ACTIVE, NULL },
        [EVENT_FAULT] = { STATE_ERROR,  Action_OnFault },
        [EVENT_RESET] = { STATE_IDLE,   Action_OnReset },
    },
    [STATE_ERROR] = {
        [EVENT_START] = { STATE_ERROR,  NULL }, /* Cannot start from error */
        [EVENT_FAULT] = { STATE_ERROR,  NULL },
        [EVENT_RESET] = { STATE_IDLE,   Action_OnReset },
    }
};

typedef struct {
    State_t current_state;
} FSM_t;

void FSM_Init(FSM_t *fsm) {
    fsm->current_state = STATE_IDLE;
}

void FSM_Dispatch(FSM_t *fsm, Event_t event) {
    if (fsm->current_state >= STATE_MAX || event >= EVENT_MAX) return;

    Transition_t trans = s_transition_table[fsm->current_state][event];
    fsm->current_state = trans.next_state;

    if (trans.action != NULL) {
        trans.action();
    }
}

int main(void) {
    printf("=== Running Table-Driven FSM Unit Tests ===\n");

    FSM_t fsm;
    FSM_Init(&fsm);
    assert(fsm.current_state == STATE_IDLE);

    /* Event START -> Transition to ACTIVE */
    FSM_Dispatch(&fsm, EVENT_START);
    assert(fsm.current_state == STATE_ACTIVE);

    /* Event FAULT -> Transition to ERROR */
    FSM_Dispatch(&fsm, EVENT_FAULT);
    assert(fsm.current_state == STATE_ERROR);

    /* Attempt START from ERROR -> Stays in ERROR */
    FSM_Dispatch(&fsm, EVENT_START);
    assert(fsm.current_state == STATE_ERROR);

    /* Event RESET -> Transition to IDLE */
    FSM_Dispatch(&fsm, EVENT_RESET);
    assert(fsm.current_state == STATE_IDLE);

    printf("FSM Table Dispatch Tests PASSED successfully!\n");
    return 0;
}
