# CPUandIOScheduling--OS_Simulation
This program simulates some aspects of operating systems. There is no real system programming involved. The whole simulation is based on the text inputs that the program receives from the user.

# Approaches for Components
CPU Scheduling: Preemptive Priority CPU-Scheduling. Higher numbers mean higher priority.
I/O-queues: First-Come-First-Serve(FCFS)
Memory: Contiguous Memory Management with “best fit” approach.

# Start of Program
At the start, the program will ask the user two questions:

**How much RAM memory is there on the simulated computer?** Your program receives the number in bytes (no kilobytes or words). The user can enter any number up to 4000000000 (4 billions). 

**How many hard disks does the simulated computer have?** The enumeration of the hard disks starts with 0.

# Simulation Begins & User Inputs
After these questions are answered, the simulation begins. The program constantly listens for the user inputs. The user's
inputs will signal some system events. The program simulates the corresponding system behavior. The possible inputs are:

**A priority memory_size**          ‘A’ input means that a new process has been created. This process has a **priority** and requires **memory_size** bytes of memory. 

**t**         The process that is currently using the CPU terminates. It leaves the system immediately. Memory used by the terminated process is released. 

**d number file_name**       The process that currently uses the CPU requests the hard disk **number**. It wants to read or write the file **file_name**.

**D number**   The hard disk **number** has finished the work for the process that is currently using it.

**S r**     Shows the process currently using the CPU and processes waiting in the ready-queue. 

**S i**      Shows what processes are currently using the hard disks and what processes are waiting to use them. For each busy hard disk, the process that is using it and it's I/O-queue. The enumeration of hard disks starts from 0. Processes in I/O-queue are displayed in the correct queue order. 

**S m**   Shows the state of memory. Shows the range of memory addresses used by each process in the system.

