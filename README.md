# MiniRTOS v1.0

## ARM Cortex-M4 Based Real-Time Operating System

MiniRTOS is a lightweight Real-Time Operating System developed from scratch to understand the internal working principles of embedded RTOS kernels.

The project implements a complete RTOS execution flow including task management, scheduling, context switching, synchronization mechanisms, inter-task communication and hardware-level simulation using ARM Cortex-M4.

The goal of this project is to understand how commercial RTOS kernels such as FreeRTOS work internally by building the fundamental components from the ground up.

---

# Features Implemented

## 1. Task Management

Implemented complete task handling mechanism:

* Task creation API
* Task Control Block (TCB)
* Individual task stack allocation
* Task initialization
* Task state management

Supported task states:

```
READY
RUNNING
BLOCKED
```

---

# 2. Scheduler

MiniRTOS contains a cooperative/preemptive style scheduler based on SysTick timing.

Features:

* Round Robin scheduling
* Ready list management
* Blocked task management
* Time slice based switching
* Automatic task selection
* Periodic scheduler tick

Scheduler flow:

```
        SysTick Interrupt

              |
              v

        Scheduler Tick

              |
              v

       Select Next Task

              |
              v

        PendSV Trigger

              |
              v

      Context Switching
```

---

# 3. ARM Cortex-M Context Switching

Implemented ARM Cortex-M4 context switching mechanism using PendSV exception.

Features:

* PSP based task switching
* Exception stack frame handling
* Register save and restore
* Task stack initialization
* Low level assembly context switch

Context switch flow:

```
Running Task

      |
      |
   SysTick

      |
      |
   PendSV Exception

      |
      |
 Save Current Context

      |
      |
 Restore Next Task Context

      |
      |
 Resume New Task
```

---

# 4. Timing Management

Implemented RTOS timing services:

* SysTick timer driver
* System tick generation
* Task delay API
* Task wake-up mechanism
* Time based scheduling

---

# 5. Inter Task Communication (IPC)

## Semaphore

Implemented binary semaphore mechanism.

Features:

* Semaphore Give
* Semaphore Take
* Task synchronization
* Blocking and waking mechanism

Example:

```
Sender Task

     |
     |
Semaphore Give

     |
     v

Receiver Task Wakes
```

---

## Mutex

Implemented mutex for resource protection.

Features:

* Mutex Lock
* Mutex Unlock
* Critical section protection
* Task waiting mechanism

Example:

```
Task 1

  Lock Resource

       |
       v

  Critical Section

       |
       v

  Unlock Resource


Task 2 waits until resource becomes free
```

---

## Queue

Implemented message queue system.

Features:

* Producer-consumer communication
* Circular buffer implementation
* Queue Send
* Queue Receive
* Data synchronization between tasks

---

# 6. Device Drivers

## UART Driver

Implemented UART based debugging output.

Used for:

* RTOS status monitoring
* Task execution logs
* Debug messages

---

# 7. Task Monitor

Implemented RTOS monitoring system.

Displays:

* Task name
* Current state
* Running task
* Ready tasks
* Blocked tasks

Example:

```
====================================
        MiniRTOS Task Monitor
====================================

NAME            STATE
------------------------------------
APP             RUNNING
IDLE            READY
UART            BLOCKED
QUEUE_RX        BLOCKED
SENSOR          READY
------------------------------------
```

---

# Demo Output

Example execution:

## Boot Sequence

![MiniRTOS Boot](<image/boot.png>)

---

## Task Monitor

![MiniRTOS Task Monitor](image/task_monitor.png>)

---

# Project Structure

```
MiniRTOS

│
├── src
│   ├── scheduler.c
│   ├── task.c
│   ├── systick.c
│   ├── semaphore.c
│   ├── mutex.c
│   ├── queue.c
│   └── drivers
│
├── inc
│   └── Header files
│
├── startup
│   └── startup.c
│
├── linker
│   └── linker.ld
│
└── README.md
```

---

# Hardware / Software Environment

## Target Architecture

```
ARM Cortex-M4
```

## Simulation Platform

```
QEMU lm3s6965evb
```

## Compiler

```
ARM GNU Toolchain
arm-none-eabi-gcc
```

## Debugger

```
ARM GDB
```

---

# Build Instructions

Install ARM GNU Toolchain.

Compile:

```bash
arm-none-eabi-gcc \
-mcpu=cortex-m4 \
-mthumb \
-T linker/linker.ld \
-o MiniRTOS.elf
```

Output:

```
MiniRTOS.elf
```

---

# Running Simulation

Start QEMU:

```bash
qemu-system-arm \
-M lm3s6965evb \
-cpu cortex-m4 \
-kernel MiniRTOS.elf \
-nographic \
-monitor none \
-serial stdio
```

---

# Architecture Overview

```
             Application Tasks

                    |
                    |

              MiniRTOS Kernel

 ------------------------------------------------

 Scheduler | Task Manager | IPC | Timer | Memory

 ------------------------------------------------

                    |

             ARM Cortex-M4 Hardware

```

---

# Learning Outcomes

Through this project, I learned:

* Internal working of RTOS kernels
* Task scheduling algorithms
* ARM Cortex-M exception handling
* PendSV based context switching
* Stack frame management
* Synchronization mechanisms
* Inter-task communication
* Embedded debugging techniques

---

# Future Improvements

Possible future enhancements:

* Priority based scheduling
* Priority inheritance for mutex
* Dynamic memory allocation
* Software timers
* Event groups
* Power management
* Hardware board porting

---

# Author

**Rohit Patil**

Embedded Systems | ARM Cortex-M | RTOS Development
