.syntax unified
.cpu cortex-m4
.thumb


.global PendSV_Handler
.type PendSV_Handler, %function


.extern currentTask
.extern Scheduler_SelectNextTask



PendSV_Handler:


    /*
        Get current PSP
    */

    MRS     r0, PSP



    /*
        If PSP is zero:
        first task start
    */

    CBZ     r0, RestoreNext



    /*
        Save R4-R11
    */

    STMDB   r0!, {r4-r11}



    /*
        Save PSP into current TCB
    */

    LDR     r1, =currentTask

    LDR     r2, [r1]

    STR     r0, [r2]



RestoreNext:


    /*
        Get next task
    */

BL Scheduler_SelectNextTask

/* r0 already contains currentTask */

LDR r0, [r0]



    /*
        Restore R4-R11
    */

    LDMIA   r0!,{r4-r11}



    /*
        Set PSP
    */

    MSR     PSP,r0



    /*
        Return to Thread using PSP
    */

    LDR     lr, =0xFFFFFFFD

    BX      lr