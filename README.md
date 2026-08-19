# MiniRTOS 

## ARM Cortex-M4 Based Real-Time Operating System

MiniRTOS is a lightweight Real-Time Operating System developed from scratch for the **ARM Cortex-M4 architecture**.

The purpose of this project is to understand the internal operation of an RTOS kernel by implementing the fundamental mechanisms normally provided by commercial RTOS platforms such as **FreeRTOS**.

Instead of using an existing RTOS kernel, MiniRTOS implements its own:

* Task management
* Task Control Blocks (TCBs)
* Ready and blocked task lists
* Time-sliced scheduling
* SysTick-based system timing
* PendSV-based context switching
* ARM Cortex-M task stack initialization
* Task delay and blocking
* Semaphore synchronization
* Mutex-based resource protection
* Queue-based inter-task communication
* UART-based debugging
* RTOS task monitoring

The complete system is executed and tested using **QEMU**, allowing the RTOS to run without requiring a physical development board.

---

# Project Overview

MiniRTOS demonstrates the complete execution path of a small embedded RTOS:

```text
                Application Tasks
                       |
                       v
              +-------------------+
              |     MiniRTOS      |
              |      Kernel       |
              +-------------------+
                       |
        +--------------+--------------+
        |              |              |
        v              v              v
    Scheduler      Task Manager      IPC
        |              |              |
        +--------------+--------------+
                       |
                       v
                 SysTick / PendSV
                       |
                       v
                ARM Cortex-M4
                       |
                       v
                     QEMU
```

---

# Key Features

## Task Management

MiniRTOS provides its own task management mechanism.

Each task is represented using a **Task Control Block (TCB)** containing information required by the kernel to manage and execute the task.

The task management system handles:

* Task creation
* Task function pointer
* Task stack allocation
* Task stack pointer
* Task priority
* Task state
* Task delay information
* Task blocking reason
* Task time slice
* Task name
* Task list relationships

### Task States

MiniRTOS supports three primary task states:

```text
+----------+
|  READY   |
+----------+
     |
     v
+----------+
| RUNNING  |
+----------+
     |
     v
+----------+
| BLOCKED  |
+----------+
```

### READY

The task is eligible to execute but is currently waiting for CPU time.

### RUNNING

The task currently owns the CPU.

### BLOCKED

The task cannot execute until its blocking condition is satisfied.

---

# Task Creation API

MiniRTOS provides a simplified RTOS-style task creation API:

```c
xTaskCreate(
    taskFunction,
    taskName,
    priority
);
```

The API validates the task parameters, allocates a task control block and stack from the statically allocated task pool, initializes the task stack and adds the task to the scheduler's ready list.

---

# Task Stack Initialization

Each task receives its own stack.

During task creation, MiniRTOS manually constructs the initial Cortex-M exception stack frame.

The initial stack contains:

```text
xPSR
PC
LR
R12
R3
R2
R1
R0

R11
R10
R9
R8
R7
R6
R5
R4
```

The program counter is initialized with the task function address.

This allows the context-switching mechanism to restore the task context and begin execution of the task as if it had already been running.

---

# Scheduler

MiniRTOS implements a **time-sliced round-robin scheduler**.

The scheduler maintains separate lists for:

```text
READY TASKS
     |
     v
+----+----+----+----+
| T1 | T2 | T3 | T4 |
+----+----+----+----+

BLOCKED TASKS
     |
     v
+----+----+----+
| T5 | T6 | T7 |
+----+----+----+
```

The scheduler selects the next READY task and transfers execution to it.

### Scheduling mechanism

```text
              SysTick
                 |
                 v
          Scheduler_Tick()
                 |
                 v
       Update task delays
                 |
                 v
       Update time slice
                 |
                 v
          Time slice ends
                 |
                 v
          Trigger PendSV
                 |
                 v
        Context Switching
                 |
                 v
        Next READY Task
```

---

# Ready List

The scheduler maintains a linked list of READY tasks.

When a task becomes ready:

```text
Blocked
   |
   | condition satisfied
   v
ReadyList_Add()
   |
   v
READY
```

When a task becomes blocked:

```text
READY
   |
   | wait / delay / synchronization
   v
ReadyList_Remove()
   |
   v
BlockedList_Add()
```

This separation allows the scheduler to consider only tasks that are currently eligible to execute.

---

# Blocked List

MiniRTOS maintains a separate blocked list.

A blocked task contains information such as:

* Blocking reason
* Delay ticks
* Blocking relationship

The scheduler checks the blocked list during every system tick.

For delay-blocked tasks:

```text
delayTicks > 0
      |
      v
delayTicks--
      |
      v
delayTicks == 0
      |
      v
Remove from Blocked List
      |
      v
Add to Ready List
```

Synchronization-blocked tasks are released by their respective IPC mechanisms.

---

# Time Management

MiniRTOS uses the **ARM Cortex-M SysTick timer** as the system timing source.

SysTick provides periodic kernel ticks.

The tick is used for:

* Task delays
* Task wake-up
* Time-slice management
* Periodic scheduling

### Task Delay

The RTOS provides:

```c
vTaskDelay(ticks);
```

When a task calls the delay API:

```text
Running Task
     |
     v
vTaskDelay()
     |
     v
Set delayTicks
     |
     v
Remove from Ready List
     |
     v
Add to Blocked List
     |
     v
Scheduler selects another task
```

When the requested number of ticks expires, the task is moved back to the READY state.

---

# ARM Cortex-M4 Context Switching

One of the main objectives of this project was to understand how an RTOS performs context switching at the processor level.

MiniRTOS uses the **PendSV exception** for task context switching.

The context-switching mechanism is implemented using ARM Cortex-M assembly and C code.

### Context Switch Flow

```text
Task A Running
      |
      v
SysTick Interrupt
      |
      v
Scheduler Tick
      |
      v
Time Slice Expired
      |
      v
PendSV Triggered
      |
      v
Save Task A Context
      |
      v
Select Task B
      |
      v
Restore Task B Context
      |
      v
Task B Running
```

---

# PSP-Based Task Execution

MiniRTOS uses the ARM Cortex-M **Process Stack Pointer (PSP)** for task execution.

The **Main Stack Pointer (MSP)** is used by the processor for exception/handler execution, while tasks execute using their PSP-based stack.

This separation is important for RTOS-based systems because it allows the kernel exception handlers and application tasks to use different stack contexts.

---

# PendSV

PendSV is used as the RTOS context-switch exception.

The scheduler does not perform the complete context switch directly inside the SysTick handler.

Instead:

```text
SysTick
   |
   v
Scheduler decides that switching is required
   |
   v
PendSV is triggered
   |
   v
Context is saved/restored
```

This separates the timing interrupt from the actual context-switching operation.

---

# Semaphore

MiniRTOS implements semaphore-based task synchronization.

The semaphore mechanism provides:

* Semaphore initialization
* Semaphore Give
* Semaphore Take
* Task blocking
* Task wake-up

Example:

```text
             Semaphore
                 |
        +--------+--------+
        |                 |
        v                 v
   Sender Task       Receiver Task
        |                 |
        | Give            |
        +-------> Signal  |
                          |
                          v
                    Receiver wakes
```

---

# Mutex

MiniRTOS implements mutex-based resource protection.

A mutex prevents multiple tasks from accessing a protected resource simultaneously.

Operations include:

```text
Mutex Lock
Mutex Unlock
```

---

# Queue

MiniRTOS implements a message queue for inter-task communication.

The queue uses a circular-buffer style structure with:

* Buffer
* Head index
* Tail index
* Element count
* Queue size

### Queue Communication

```text
        Producer Task
              |
              | Queue_Send()
              v
       +--------------+
       |    QUEUE     |
       |              |
       |  0  1  2  3 |
       +--------------+
              |
              | Queue_Receive()
              v
        Consumer Task
```

---

# UART Driver

A lightweight UART driver is implemented for debugging and RTOS monitoring.

The driver provides:

```c
UART_Init()
UART_PutChar()
UART_Print()
```

UART output is used to display:

* System initialization
* Task execution
* Semaphore operations
* Mutex operations
* Queue operations
* Task monitor information

---

# Idle Task

MiniRTOS includes an idle task that executes when there are no other eligible READY tasks.

Conceptually:

```text
          Are application
          tasks READY?
               |
          +----+----+
          |         |
         YES        NO
          |         |
          v         v
     Run task    Idle Task
```

The idle task provides a default execution path for the scheduler when application tasks are unavailable.

---

# System Output

# MiniRTOS boot

![MiniRTOS Boot](<image/boot.png>)

---

# Task Monitor 

![MiniRTOS Task Monitor](<image/task_monitor.png>)

---

# Project Architecture

```text
                         MiniRTOS
                            |
              +-------------+-------------+
              |                           |
        Application                   RTOS Kernel
          Tasks                           |
              |            +--------------+--------------+
              |            |              |              |
              |       Task Manager    Scheduler         IPC
              |            |              |              |
              |            |              |        +-----+-----+
              |            |              |        |     |     |
              |            |              |      SEM  MUTEX QUEUE
              |            |              |
              |            |          SysTick
              |            |              |
              |            +--------------+
              |                           |
              +---------------------------+
                                          |
                                     PendSV
                                          |
                                          v
                                  ARM Cortex-M4
                                          |
                                          v
                                         QEMU
```

---

# 20. Project Directory Structure

```text
MiniRTOS/
│
├── src/
│   ├── scheduler.c
│   ├── task.c
│   ├── systick.c
│   ├── semaphore.c
│   ├── mutex.c
│   ├── queue.c
│   └── drivers/
│
├── inc/
│   └── Header files
│
├── startup/
│   └── startup.c
│
├── linker/
│   └── linker.ld
│
└── README.md
```



---

# 21. Development Environment

## Processor Architecture

```text
ARM Cortex-M4
```

## Instruction Set

```text
Thumb / Thumb-2
```

## Compiler

```text
arm-none-eabi-gcc
```

## Debugger

```text
arm-none-eabi-gdb
```

## Simulator

```text
QEMU
```

## QEMU Machine

```text
lm3s6965evb
```

## Host Platform

```text
Windows
PowerShell
```

---

# 22. Building the Project

The project is built using the ARM GNU Embedded Toolchain.

From the MiniRTOS project directory:

```powershell
arm-none-eabi-gcc -g -mcpu=cortex-m4 -mthumb -ffreestanding -fno-builtin -nostdlib -Iinc `
src/main.c `
src/task.c `
src/scheduler.c `
src/rtos.c `
src/systick.c `
src/cortex_m.c `
src/pendsv.c `
src/mutex.c `
src/semaphore.c `
src/queue.c `
src/uart.c `
src/list.c `
src/idle.c `
src/monitor.c `
startup/startup.c `
src/pendsv.S `
-T linker/linker.ld `
-o MiniRTOS.elf
```

Successful compilation produces:

```text
MiniRTOS.elf
```

---

# 23. Running MiniRTOS in QEMU

Start the RTOS using:

```powershell
qemu-system-arm -M lm3s6965evb -cpu cortex-m4 -kernel MiniRTOS.elf -nographic -monitor none -serial stdio
```

The UART output will appear directly in the terminal.

---

# 24. Debugging with GDB

MiniRTOS can be debugged using ARM GDB together with QEMU's GDB server.

Start QEMU in debug mode:

```powershell
qemu-system-arm -M lm3s6965evb -cpu cortex-m4 -kernel MiniRTOS.elf -S -s
```

Then start:

```powershell
arm-none-eabi-gdb MiniRTOS.elf
```

Connect to QEMU:

```gdb
target remote localhost:1234
```

Breakpoints can then be placed inside RTOS components such as:

```text
Queue_Send()
Queue_Receive()
Scheduler_Tick()
Scheduler_SelectNextTask()
Task_Create()
```

This was used to verify task execution and IPC behavior during development.

---

# This Project Demonstrates

This project demonstrates practical understanding of:

* Embedded C
* ARM Cortex-M architecture
* RTOS kernel concepts
* Task management
* Scheduling
* Context switching
* Exception handling
* SysTick
* PendSV
* PSP/MSP concepts
* Task stack initialization
* Synchronization
* Inter-task communication
* UART debugging
* GDB debugging
* QEMU-based embedded simulation
* Low-level C and ARM assembly interaction

---

# Future Improvements 
 
* Priority based scheduling 
* Priority inheritance for mutex 
* Dynamic memory allocation 
* Software timers 
* Event groups 
* Power management 
* Hardware board porting 

---

**Rohit Patil**

Embedded Systems | ARM Cortex-M | RTOS Development

---