/*****************************************************************************
* Product: QF-nano public interface
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Jun 08, 2008
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef qfn_h
#define qfn_h

/**
* \file
* \ingroup qepn qfn qkn
* \brief Public QF-nano interface.
*
* This header file must be included in all modules that use QP-nano.
* Typically, this header file is included indirectly through the
* header file qpn_port.h.
*/

/****************************************************************************/
#if (QF_MAX_ACTIVE < 1) || (8 < QF_MAX_ACTIVE)
    #error "QF_MAX_ACTIVE not defined or out of range. Valid range is 1..8."
#endif
#if (defined QF_FSM_ACTIVE) && (defined Q_NFSM)
    #error "QF_FSM_ACTIVE and Q_NFSM cannot be defined at the same time."
#endif
#if (!defined QF_FSM_ACTIVE) && (defined Q_NHSM)
    #error "Q_NHSM defined without defining QF_FSM_ACTIVE."
#endif


/****************************************************************************/
/* default macros for accessing data in ROM */
#ifndef Q_ROM_BYTE
    /** \brief Macro to access a byte allocated in ROM
    *
    * Some compilers for Harvard-architecture MCUs, such as gcc for AVR, do
    * not generate correct code for accessing data allocated in the program
    * space (ROM). The workaround for such compilers is to explictly add
    * assembly code to access each data element allocated in the program
    * space. The macro Q_ROM_BYTE() retrieves a byte from the given ROM
    * address.
    *
    * The Q_ROM_BYTE() macro should be defined in the qpn_port.h header file
    * for each compiler that cannot handle correctly data allocated in ROM
    * (such as the gcc). If the macro is left undefined, the default
    * definition simply returns the argument and lets the compiler synthesize
    * the correct code.
    */
    #define Q_ROM_BYTE(rom_var_)   (rom_var_)
#endif
#ifndef Q_ROM_PTR
    /** \brief Macro to access a pointer allocated in ROM
    *
    * Some compilers for Harvard-architecture MCUs, such as gcc for AVR, do
    * not generate correct code for accessing data allocated in the program
    * space (ROM). The workaround for such compilers is to explictly add
    * assembly code to access each data element allocated in the program
    * space. The macro Q_ROM_PTR() retrieves an object-pointer from the given
    * ROM address. Please note that the pointer can be pointing to the object
    * in RAM or ROM.
    *
    * The Q_ROM_PTR() macro should be defined in the qpn_port.h header file
    * for each compiler that cannot handle correctly data allocated in ROM
    * (such as the gcc). If the macro is left undefined, the default
    * definition simply returns the argument and lets the compiler synthesize
    * the correct code.
    */
    #define Q_ROM_PTR(rom_var_)    (rom_var_)
#endif

#ifndef QF_TIMEEVT_CTR_SIZE
    /** \brief macro to override the default QTimeEvtCtr size.
    * Valid values 0, 1, 2, or 4; default 0
    */
    #define QF_TIMEEVT_CTR_SIZE 0
#endif
#if (QF_TIMEEVT_CTR_SIZE == 0)
#elif (QF_TIMEEVT_CTR_SIZE == 1)
    typedef uint8_t QTimeEvtCtr;
#elif (QF_TIMEEVT_CTR_SIZE == 2)
    /** \brief type of the Time Event counter, which determines the dynamic
    * range of the time delays measured in clock ticks.
    *
    * This typedef is configurable via the preprocessor switch
    * #QF_TIMEEVT_CTR_SIZE. The other possible values of this type are
    * as follows: \n
    * none when (QF_TIMEEVT_CTR_SIZE not defined or == 0), \n
    * uint8_t when (QF_TIMEEVT_CTR_SIZE == 1); \n
    * uint16_t when (QF_TIMEEVT_CTR_SIZE == 2); and \n
    * uint32_t when (QF_TIMEEVT_CTR_SIZE == 4).
    */
    typedef uint16_t QTimeEvtCtr;
#elif (QF_TIMEEVT_CTR_SIZE == 4)
    typedef uint32_t QTimeEvtCtr;
#else
    #error "QF_TIMER_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

/****************************************************************************/
/** \brief Active Object struct
*
* QActive is the base structure for derivation of active objects. Active
* objects in QF-nano are encapsulated tasks (each embedding a state machine
* and an event queue) that communicate with one another asynchronously by
* sending and receiving events. Within an active object, events are
* processed sequentially in a run-to-completion (RTC) fashion, while QF
* encapsulates all the details of thread-safe event exchange and queuing.
*
* \note ::QActive is not intended to be instantiated directly, but rather
* serves as the base structure for derivation of active objects in the
* application code.
*
* The following example illustrates how to derive an active object from
* QActive. Please note that the QActive member super_ is defined as the
* FIRST member of the derived struct.
* \include qfn_qactive.c
*
* \sa ::QActiveTag for the description of the data members \n \ref derivation
*/
typedef struct QActiveTag {

#ifndef QF_FSM_ACTIVE
    QHsm super;                 /**< derives from the ::QHsm base structure */
#else
    QFsm super;                 /**< derives from the ::QFsm base structure */
#endif

    /** \brief priority of the active object (1..QF_MAX_ACTIVE)
    */
    uint8_t prio;

    /** \brief offset to where next event will be inserted into the buffer
    */
    uint8_t head;

    /** \brief offset of where next event will be extracted from the buffer
    */
    uint8_t tail;

    /** \brief number of events currently present in the queue
    * (events in the ring buffer + 1 event in the state machine)
    */
    uint8_t nUsed;

#if (QF_TIMEEVT_CTR_SIZE != 0)
    /** \brief Time Event tick counter for the active object
    */
    QTimeEvtCtr tickCtr;
#endif
} QActive;


/** \brief Active object constructor.
*
* \a me pointer the active object structure derived from ::QActive.
* \a initial is the pointer to the initial state of the active object.
*
* \note Must be called exactly ONCE for each active object
* in the application before calling QF_run().
*/
#ifndef QF_FSM_ACTIVE
    #define QActive_ctor(me_, initial_)  QHsm_ctor(&(me_)->super, initial_)
#else
    #define QActive_ctor(me_, initial_)  QFsm_ctor(&(me_)->super, initial_)
#endif


#if (Q_PARAM_SIZE != 0)
    /** \brief Posts an event \a e directly to the event queue of the acitve
    * object \a prio using the First-In-First-Out (FIFO) policy. This
    * function briefly locks and unlocks interrupts to protect the
    * queue integrity.
    *
    * Direct event posting is the only asynchronous communication method
    * available in QF-nano. The following example illustrates how the
    * Ped active object posts directly the PED_WAITING event to the PELICAN
    * crossing active object.
    * \include qfn_post.c
    *
    * The producer of the event (Ped in this case) must only "know"
    * a pointer recipient (&AO_Pelican), but the specific definition of
    * the Pelican structure is not required.
    *
    * Direct event posting should not be confused with direct event
    * dispatching. In contrast to asynchronous event posting through event
    * queues, direct event dispatching is synchronous. Direct event
    * dispatching occurs when you call QHsm_dispatch(), or QFsm_dispatch()
    * function.
    *
    * \note This function is intended only to be used at the task level
    * and shouln never be used inside ISRs.
    */
    void QActive_post(QActive *me, QSignal sig, QParam par);

    /** \brief Posts an event \a e directly to the event queue of the acitve
    * object \a prio using the First-In-First-Out (FIFO) policy. This
    * function does NOT lock/unlock interrupts when nesting of interrupts
    * is not allowed. Also, this function never calls the QK-nano scheduler,
    * because synchronous task preemptions are never necessary inside ISRs.
    *
    * \note This function is intended only to be used inside ISRs and you
    * should never use at the task level.
    *
    * \sa QF_post()
    */
    void QActive_postISR(QActive *me, QSignal sig, QParam par);
#else
    void QActive_post(QActive *me, QSignal sig);
    void QActive_postISR(QActive *me, QSignal sig);
#endif

#if (QF_TIMEEVT_CTR_SIZE != 0)

    /** \brief Processes all armed time events at every clock tick.
    *
    * \note This function can be only calle from the ISR-level. You must also
    * guarantee that QF_tick() runs to completion before it is called again.
    *
    * The following example illustrates the call to QF_tick():
    * \include qfn_tick.c
    */
    void QF_tick(void);

#if (QF_TIMEEVT_CTR_SIZE == 1)                 /* single-byte tick counter? */

    /** \brief Arm the QP-nano one-shot time event. Since the tick counter
    * is a single byte in this case, the time event can be atomically
    * armed without using a critical section.
    *
    * Arms the time event of the active object \param me_ to expire in
    * \param tout_ clock ticks (one-shot time event). Upon the expiration,
    * the time event posts the reserved signal Q_TIMEOUT_SIG directly
    * into the event queue of the active object \param me_.
    *
    * After posting, the time event gets automatically disarmed.
    *
    * The time event can be disarmed (stoped) at any time by calling the
    * QActive_disarm() function. Also, a one-shot time event can be re-armed
    * to fire in a different number of clock ticks by calling
    * QActive_arm() again.
    *
    * The following example shows how to arm a one-shot time event from
    * a state machine of an active object:
    * \include qfn_arm.c
    */
    #define QActive_arm(me_, tout_) ((me_)->tickCtr = (QTimeEvtCtr)(tout_))

    /** \brief Disarm a time event. Since the tick counter
    * is a single byte in this case, the time event can be atomically
    * disarmed without using a critical section.
    *
    * The time event of the active object \param me_ gets disarmed (stopped).
    *
    * \note You should not assume that the Q_TIMEOUT_SIG event will not
    * arrive after you disarm the time event. The timeout evetn could be
    * already in the event queue.
    */
    #define QActive_disarm(me_)     ((me_)->tickCtr = (QTimeEvtCtr)0)

#else                                            /* multi-byte tick counter */

    /** \brief Arm the QP-nano one-shot time event. Since the tick counter
    * is a multi-byte variable in this case, the operation must be
    * performed inside a critical section.
    *
    * Arms the time event of the active object \a me to expire in
    * \a tout clock ticks (one-shot time event). Upon the expiration,
    * the time event posts the reserved signal Q_TIMEOUT_SIG directly
    * into the event queue of the active object \a me.
    *
    * After posting, the time event gets automatically disarmed.
    *
    * The time event can be disarmed (stoped) at any time by calling the
    * QActive_disarm() function. Also, a one-shot time event can be re-armed
    * to fire in a different number of clock ticks by calling
    * QActive_arm() again.
    *
    * The following example shows how to arm a one-shot time event from
    * a state machine of an active object:
    * \include qfn_arm.c
    */
    void QActive_arm(QActive *me, QTimeEvtCtr tout);

    /** \brief Disarm a time event. Since the tick counter
    * is a multi-byte variable in this case, the operation must be
    * performed inside a critical section.
    *
    * The time event of the active object \a me gets disarmed (stopped).
    *
    * \note You should not assume that the Q_TIMEOUT_SIG event will not
    * arrive after you disarm the time event. The timeout evetn could be
    * already in the event queue.
    */
    void QActive_disarm(QActive *me);

#endif                                        /* (QF_TIMEEVT_CTR_SIZE == 1) */

#endif                                        /* (QF_TIMEEVT_CTR_SIZE != 0) */

/* protected methods ...*/

/** \brief QF-nano termination.
*
* This function terminates QF and performs any necessary cleanup.
* In QF-nano this function is defined in the BSP. Many QF ports might not
* require implementing QF_stop() at all, because many embedded applications
* don't have anything to exit to.
*/
void QF_stop(void);

/** \brief Startup QF-nano callback.
*
* The timeline for calling QF_onStartup() depends on the particular
* QF port. In most cases, QF_onStartup() is called from QF_run(), right
* before starting any multitasking kernel or the background loop.
*
* \sa QF initialization example for ::QActiveCB.
*/
void QF_onStartup(void);

/** \brief Transfers control to QF to run the application.
*
* QF_run() implemetns the simple non-preemptive scheduler. QF_run() must be
* called from your startup code after you initialize the QF and define at
* least one active object control block in QF_active[].
*
* \note When the Quantum Kernel (QK) is used as the underlying real-time
* kernel for the QF, all platfrom dependencies are handled in the QK, so
* no porting of QF is necessary. In other words, you only need to recompile
* the QF platform-independent code with the compiler for your platform, but
* you don't need to provide any platform-specific implementation (so, no
* qf_port.c file is necessary). Moreover, QK implements the function QF_run()
* in a platform-independent way, in the modile qk.c.
*/
void QF_run(void);


#ifndef QK_PREEMPTIVE
    /** \brief QF idle callback (customized in BSPs for QF)
    *
    * QF_onIdle() is called by the non-preemptive scheduler built into QF-nano
    * when the QF-nano detects that no events are available for active objects
    * (the idle condition). This callback gives the application an opportunity
    * to enter a power-saving CPU mode, or perform some other idle processing.
    *
    * \note QF_onIdle() is invoked with interrupts LOCKED because the idle
    * condition can be asynchronously changed at any time by an interrupt.
    * QF_onIdle() MUST unlock the interrupts internally, but not before
    * putting the CPU into the low-power mode. (Ideally, unlocking interrupts
    * and low-power mode should happen atomically). At the very least, the
    * function MUST unlock interrupts, otherwise interrups will be locked
    * permanently.
    *
    * \note QF_onIdle() is not used in the PREEMPTIVE configuration. When
    * QK_PREEMPTIVE macro is defined, the preemptive kernel QK-nano is used
    * instead of the non-preemptive QF-nano scheduler. QK-nano uses a
    * different idle callback \sa QK_onIdle().
    */
void QF_onIdle(void);
#endif

/****************************************************************************/
/** \brief QActive Control Block
*
* QActiveCB represents the constant information that the QF-nano needs
* to manage the active object. QActiveCB objects are grouped in the
* array QF_active[], which typically can be placed in ROM.
*
* The following example illustrates how to allocate and initialize the
* QActive control blocks in the array QF_active[].
* \include qfn_main.c
*/
typedef struct QActiveCBTag {
    QActive *act;        /**< \brief pointer to the active object structure */
    QEvent *queue;            /**< \brief pointer to the event queue buffer */
    uint8_t end;                  /**< \brief the length of the ring buffer */
} QActiveCB;

                                           /** active object control blocks */
extern QActiveCB const Q_ROM Q_ROM_VAR QF_active[];

/** \brief Ready set of QF-nano.
*
* The QF-nano ready set keeps track of active objects that are ready to run.
* The ready set represents each active object as a bit, with the bits
* assigned according to priorities of the active objects. The bit is set
* if the corresponding active object is ready to run (i.e., has one or
* more events in its event queue) and zero if the event queue is empty.
* The QF-nano ready set is one byte-wide, which corresponds to 8 active
* objects maximum.
*/
extern uint8_t volatile QF_readySet_;

#endif                                                             /* qfn_h */
