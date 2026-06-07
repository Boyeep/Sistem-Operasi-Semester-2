# **Observe the Process Landscape with** 

# **Motivation**

A Linux system is always running many processes, even when the user thinks “nothing is happening.” Understanding process information is the foundation for studying operating systems because the kernel schedules processes, assigns them identifiers, tracks their states, manages their memory, and exposes their runtime properties through kernel interfaces. Before students can understand resource contention, scheduling, or memory layout, they need to become comfortable identifying a process, interpreting its state, and relating user-space commands to kernel-maintained information. This exercise introduces process observation using both snapshot tools and live monitoring tools. It also helps students compare command output with the `/proc` pseudo-filesystem, which is one of the most important Linux interfaces for system introspection.

## **Task**

Identify and compare the information provided by `ps`, `top`, and `/proc` for several running processes.

## **Detailed steps**

1. Run:  
   **ps \-eo pid,ppid,user,stat,ni,pri,comm,%cpu,%mem \--sort=-%cpu | head \-20**  
   Study the meaning of each column. Pay attention to PID, PPID, process state (`STAT`), nice value, priority, CPU percentage, and memory percentage. This command gives a structured snapshot of process activity and is useful for understanding how Linux describes processes in a non-interactive format.  
2. Start `top`:  
     
   **top**  
   Observe how the process list changes live. Compare the live ordering with the static snapshot from `ps`. Note how `top` emphasizes dynamic resource consumption, while `ps` is often better for scripted inspection. Watch for changes in CPU and memory use over time.  
3. Pick one PID from the `ps` output and inspect:

   **cat /proc/\<PID\>/status**  
   **cat /proc/\<PID\>/stat**  
   **cat /proc/\<PID\>/cmdline**  
   Compare these files with the `ps` output. `/proc/<PID>/status` is human-readable and includes state, memory counters, thread count, and capabilities. `/proc/<PID>/stat` is denser and more machine-oriented. `/proc/<PID>/cmdline` shows how the process was invoked.  
4. Repeat for:

* one shell process  
* one system daemon  
* one process currently using noticeable CPU

## **Expected outcomes** 

Students should be able to explain that `ps` provides a point-in-time view, `top` provides a real-time dynamic view, and `/proc` exposes the kernel’s raw process information. They should recognize basic states such as running, sleeping, and zombie if present. They should also understand that PID and PPID encode process relationships, while nice and priority hint at scheduler behavior. A successful outcome is the ability to trace a process from a high-level monitoring tool down to its kernel-maintained metadata.

# **Trace Parent-Child Relationships and Process Creation**

## **Motivation**

A process does not exist in isolation. It is created by another process, inherits execution context, and participates in a family structure that reveals how user commands, services, and sessions are organized. Understanding parent-child relationships is essential in operating systems because process creation, process reaping, session control, and signal propagation all depend on these hierarchies. Students often learn the theory of `fork()`, `exec()`, and `wait()` abstractly, but a Linux system makes those relationships visible through tools such as `ps`, `pstree`, and `/proc`. This exercise connects process theory with observation and helps students understand how interactive shells, daemons, and worker processes form trees.

## **Task**

Inspect process hierarchies and explain how processes are related to one another.

## **Detailed steps**

1. Display a process tree:

**pstree \-p**

Observe how the init system, login/session processes, shells, and child processes are arranged. The `-p` option includes PIDs, making it easier to cross-reference with other tools.

2. Show parent-child data using `ps`:

**ps \-eo pid,ppid,pgid,sid,comm \--forest**

Focus on the meaning of:

* `PPID` — parent process ID  
* `PGID` — process group ID  
* `SID` — session ID

  These fields reveal relationships beyond simple parentage. Process groups and sessions matter for job control, terminals, and signal delivery.  
3. Open a second shell and start a background command:  
   **sleep 300 &**  
   **jobs \-l**  
   Inspect it with:  
   **ps \-o pid,ppid,pgid,sid,stat,cmd \-p \<PID\>**  
   Relate the background process to the shell that created it.  
4. Terminate the process and observe whether the shell reaps it:  
   **kill \<PID\>**  
   **ps \-p \<PID\>**

## **Expected outcomes** 

Students should see that every user-created process usually has a parent, and that shells act as process creators for interactive workloads. They should understand the difference between parent-child relationships and process groups or sessions. They should be able to explain that Linux tracks not just who created a process, but also which job-control context it belongs to. This prepares them for later tasks about signals, scheduling, and terminal-driven control.

# **Inspect Open Resources** 

## **Motivation**

Resource management in Linux goes far beyond CPU time and RAM. Processes use files, directories, devices, pipes, sockets, and memory-mapped objects, and the kernel tracks these as open resources. `lsof` is one of the best tools for demonstrating the operating systems idea that many resources are represented through file descriptors or file-like abstractions. This exercise helps students connect process activity with the resources held by each process. It also reinforces the idea that a process can be “idle” in CPU terms while still owning valuable kernel-managed resources. Understanding open resources is critical for debugging file lock issues, investigating port conflicts, studying interprocess communication, and understanding how applications interact with the filesystem and network stack.

## **Task**

Use `lsof` to discover what resources a process is holding.

## **Detailed steps**

1. Run:

   **lsof | head \-40**  
   Look at columns such as:  
* `COMMAND`  
* `PID`  
* `USER`  
* `FD`  
* `TYPE`  
* `NAME`

  Interpret examples of file descriptors like `cwd`, `txt`, `mem`, and numbered descriptors such as `0u`, `1u`, `2u`.  
2. Pick a familiar process, such as your shell or an editor:

   **echo $$**  
   **lsof \-p \<PID\>**  
   Explain what the process has open. Identify standard input/output/error, current working directory, executable text image, shared libraries, and possibly terminal devices.  
3. Create a temporary file and keep it open in another shell using a text editor or `tail -f`. Then run:

   **lsof /path/to/file**  
   Observe which process currently holds the file.  
4. Inspect network-related resources:

   **lsof \-i**  
   Then narrow the search:  
   **lsof \-i :22**  
   or another port active on the system.

## **Expected outcomes** 

Students should understand that `lsof` reveals the relationship between processes and kernel-managed resources. They should identify different descriptor types and explain why a process may have open shared libraries, terminal devices, sockets, and working directories. They should recognize that files can remain “in use” because an active process still holds them open. This is a direct and practical introduction to resource management in Linux.

# **Find Which Process Owns a Port or File** 

## **Motivation**

In a multiuser, multitasking operating system, resource conflicts are common. A port may already be in use, a file may be busy, or a mounted device may refuse to unmount because some process still references it. Linux provides several tools that answer the practical question: “Which process is using this resource?” This exercise is valuable because it moves students from passive observation into troubleshooting and operational reasoning. `ss` and `netstat` connect socket usage to processes, while `fuser` links files, mount points, and network sockets to owners. Together these tools demonstrate how Linux binds system resources to processes and how administrators diagnose contention in real systems.

## **Task**

Identify which process owns a network socket or a file resource.

## **Detailed steps**

1. Show listening and connected sockets:  
   **ss \-tulpn**  
   Inspect TCP and UDP listeners. The output reveals local address, port, and owning process information where permitted.  
2. Compare with:  
   **netstat \-tulpn**  
   If available on the system, compare formatting and coverage with `ss`. Students should note that `ss` is the more modern tool on many Linux systems.  
3. Pick one active listening port and identify its owner:  
   **ss \-ltnp | grep ':22'**  
   Replace `22` with another port if needed. Explain how the process and PID information is shown.  
4. Use `fuser` on a file:  
   **fuser /path/to/file**  
   Then use:  
   **fuser \-v /path/to/file**  
   Interpret which processes are referencing the file.  
5. Optionally inspect a mount point:

   **fuser \-vm /mount/point**  
   This is useful for understanding why a filesystem might be busy.

## **Expected outcomes**

Students should be able to map network sockets and files back to the responsible process. They should understand that sockets are also process-owned resources and that a process can prevent an operation such as unmounting because it still has a reference inside the affected filesystem. They should also distinguish between modern and legacy network inspection tools. The deeper lesson is that Linux resource conflicts are usually resolvable by identifying the owning process.

# **Measure Per-Process CPU and I/O Activity** 

## **Motivation**

Some tools show a current snapshot, while others show behavior over time. Operating systems are dynamic, so temporal observation matters. A process that appears quiet at one instant may periodically wake, perform I/O, consume CPU, or create load spikes. `pidstat` is a strong educational tool because it collects per-process statistics repeatedly, allowing students to observe behavior patterns rather than one-time values. This is especially useful for learning about CPU consumption, minor and major faults, context switches, and I/O activity. The exercise trains students to think like system investigators: not just “what is the system doing now,” but “how does this process behave over several intervals?”

## **Task**

Use `pidstat` to monitor the behavior of one or more processes over time.

## **Detailed steps**

1. Start a CPU-intensive command in another shell, for example:

   **yes \> /dev/null**  
   Find its PID:  
   **pgrep yes**  
2. Monitor CPU usage:

   **pidstat \-p \<PID\> 1 10**  
   This records statistics every second for ten intervals. Observe CPU percentage and scheduling-related counters.  
3. Monitor memory-related behavior:

   **pidstat \-r \-p \<PID\> 1 10**  
   Look for page fault activity and memory counters. Explain the difference between minor and major faults where visible.  
4. Monitor I/O behavior using a different command, such as copying a large file:

   **pidstat \-d 1 10**  
   Observe which processes perform reads and writes over time.  
5. Stop the test processes and compare the recorded patterns.

## **Expected outcomes** 

Students should discover that `pidstat` provides a time series rather than a static snapshot. They should be able to describe how CPU-bound and I/O-bound processes look different across intervals. They should also recognize page faults and I/O rates as indicators of memory and storage activity. A successful outcome is the ability to characterize process behavior patterns rather than merely listing processes.

# **Memory Pressure**

## **Motivation**

Memory management is a central operating systems function, but beginners often misunderstand Linux memory reporting because cached memory, buffers, swap, and “available” memory are not intuitive. This exercise teaches students how Linux reports memory at the system level and how to interpret those numbers correctly. It is especially valuable for challenging the misconception that high memory usage always means a problem. By comparing `free`, `vmstat`, and `/proc/meminfo`, students learn that memory is actively used for caching, that paging activity indicates pressure, and that system-wide memory behavior must be interpreted carefully. This task builds the foundation for later exercises on page faults, memory maps, and program loading.

## **Task**

Observe Linux memory usage under normal and stressed conditions.

## **Detailed steps**

1. Record baseline memory status:  
   **free \-h**  
   **vmstat 1 5**  
   **cat /proc/meminfo | head \-40**  
   Compare the three views. `free` summarizes usage, `vmstat` shows trends and paging-related counters, and `/proc/meminfo` exposes detailed kernel accounting.  
2. Identify these fields in `/proc/meminfo`:

* `MemTotal`  
* `MemFree`  
* `MemAvailable`  
* `Buffers`  
* `Cached`  
* `SwapTotal`  
* `SwapFree`

  Explain what each represents and why `MemAvailable` is often more informative than `MemFree`.  
3. Create moderate memory load, for example by opening large applications or using a safe test program if available. Then rerun:

   **free \-h**  
   **vmstat 1 10**  
4. Focus on `vmstat` columns related to:

* runnable processes  
* swap in/out  
* block I/O  
* system interrupts  
* CPU time

Relate memory pressure to broader system activity.

## **Expected outcomes** 

Students should conclude that Linux uses free memory aggressively for caching and that low “free” memory alone is not necessarily bad. They should understand the difference between available memory and strictly unused memory. They should also recognize swap activity and paging as signs of pressure. The educational result is more accurate interpretation of Linux memory statistics and reduced confusion about cached memory behavior.

#  **Inspect a Process Memory Map with `pmap` and `/proc/<PID>/maps`**

## **Motivation**

The concept of process memory layout is often taught as a diagram showing text, data, heap, stack, and shared libraries. Linux allows students to inspect this layout directly for real processes. This is important because it connects the abstract model of an address space to actual mapped regions managed by the kernel. Tools such as `pmap` and `/proc/<PID>/maps` show where the executable is loaded, where libraries are mapped, where the heap and stack are located, and how virtual memory is segmented. This exercise is essential for bridging program loading theory and memory management practice. It also helps students understand that a process sees virtual memory regions, not raw physical memory.

## **Task**

Examine the virtual memory map of a running process.

## **Detailed steps**

1. Start a simple process such as:  
   **sleep 300 &**  
   Record the PID.  
2. Inspect the memory map:

   **pmap \<PID\>**  
   Then get a more detailed view:  
   **pmap \-x \<PID\>**  
   Discuss address ranges, mapping sizes, dirty pages if shown, and totals.  
3. Inspect the raw kernel view:

   **cat /proc/\<PID\>/maps**  
   Look for:  
* executable mapping  
* shared libraries  
* heap  
* stack  
* anonymous mappings  
* special mappings such as `[vdso]`

4. Compare a simple process like `sleep` with a larger application such as a shell or editor. Note how the number and variety of mappings differ.

## **Expected outcomes** 

Students should see that each process has a virtual address space divided into multiple mapped regions, not just one contiguous block. They should identify the stack, heap, executable segments, and shared libraries. They should understand that the memory layout shown here is a runtime result of program loading and dynamic linking. This exercise also reinforces the distinction between logical memory layout and physical memory implementation.

# **Observe Shared Libraries and Dynamic Linking** 

## **Motivation**

Many Linux programs are not fully self-contained binaries. They depend on shared libraries that are loaded at runtime, reducing duplication and enabling code reuse. Understanding this is essential for program loading, memory sharing, and binary structure. `ldd` gives students a simple way to see which shared objects a program depends on and where they are resolved. This exercise matters because it connects executable startup, memory mapping, library search paths, and runtime dependencies. It also provides a practical explanation for why many processes map the same library files, which links naturally to resource efficiency and shared memory concepts.

## **Task**

Use `ldd` to inspect shared library dependencies of Linux executables.

## **Detailed steps**

1. Inspect a common executable:  
   **ldd /bin/ls**  
   Observe which libraries are required and where they are located.  
2. Compare with another executable:

   **ldd /bin/bash**  
   Discuss why a shell usually depends on more libraries than a small utility.  
3. Pick a process currently running that corresponds to an executable, and compare its `ldd` output with:

   **cat /proc/\<PID\>/maps**  
   Look for matching library paths in the memory map.  
4. Note the loader, often something like `ld-linux...so`. Explain its role in loading and resolving shared libraries.

## **Expected outcomes** 

Students should understand that many executables rely on shared libraries resolved at runtime. They should see that the libraries listed by `ldd` usually appear as memory mappings when the program runs. They should also recognize the dynamic loader as an important participant in program startup. This task reinforces the connection between binary dependencies and runtime memory layout.

# **Examine Executable File Structure** 

## **Motivation**

An executable file is not just a blob of instructions. Linux typically uses the ELF format, which stores metadata needed for loading, linking, relocation, debugging, and execution. For operating systems students, learning ELF is important because it shows what the kernel loader and dynamic linker are working with before a program begins execution. `readelf` is especially valuable because it exposes headers, sections, segments, symbols, and dynamic linking information in a readable form. This exercise helps students understand how a binary is organized on disk and why that structure matters to memory layout and program loading. It also distinguishes between sections used for linking and segments used for execution.

## **Task**

Use `readelf` to analyze the structure of ELF executables.

## **Detailed steps**

1. Inspect the ELF header:

   **readelf \-h /bin/ls**  
   Interpret fields such as class, machine type, entry point, and number of headers.  
2. Show section headers:

   **readelf \-S /bin/ls**  
   Look for `.text`, `.data`, `.rodata`, `.bss`, `.dynsym`, and other notable sections. Explain what kind of information each section generally holds.  
3. Show program headers:

   **readelf \-l /bin/ls**  
   Focus on loadable segments and compare them conceptually with runtime memory maps. Explain that segments are more directly related to execution than sections.  
4. Inspect dynamic information:

   **readelf \-d /bin/ls**  
   Discuss needed shared libraries, relocation information, and dynamic tags.

## **Expected outcomes** 

Students should learn that an ELF executable contains structured metadata describing how it should be loaded and linked. They should distinguish sections from segments and understand why program headers are especially important for runtime loading. They should also recognize that shared library requirements are encoded inside the binary, not guessed magically by the system. This provides a concrete bridge from file format to runtime execution.

# **Disassemble Machine Code** 

## **Motivation**

Operating systems ultimately manage executing machine instructions, not just source code. Although a full assembly course is not necessary here, students benefit from seeing that executables contain actual instructions organized into code sections. `objdump` allows them to view disassembly and inspect low-level structure without compiling anything themselves. This exercise matters because it connects ELF sections such as `.text` to real instructions and reinforces the notion that program loading places binary code into executable memory. It also encourages students to interpret symbols, function boundaries, and the layout of code within a binary. For systems education, this visibility demystifies what the kernel is actually launching.

## **Task**

Inspect machine code and section information using `objdump`.

## **Detailed steps**

1. View file headers and sections:

   **objdump \-h /bin/ls**  
   Compare the listed sections with those seen earlier in `readelf`.  
2. Disassemble the executable:

   **objdump \-d /bin/ls | less**  
   Examine the assembly representation of functions. Explain that this is the instruction stream that will eventually execute in memory.  
3. If symbols are available, focus on a named function or on the program entry sequence. Compare section addresses and code addresses with earlier ELF header information.

4. Optionally inspect relocation or symbol-related data:

   **objdump \-x /bin/ls | less**

## **Expected outcomes** 

Students should understand that binaries contain executable instruction sequences and that disassembly is one way to inspect the low-level contents of the `.text` region. They should also notice that section-level metadata and machine code are closely related. The main conceptual result is connecting the abstract idea of “a running program” with the concrete bytes and instructions stored in the executable file.

# **Relate ELF Segments to Runtime Memory Layout**

## **Motivation**

One of the most important conceptual steps in systems learning is connecting the structure of a binary on disk to the layout of the process in memory after loading. Students often study ELF headers and process memory maps separately, but the deeper insight is that program headers guide how parts of the binary become mapped regions in virtual memory. This exercise is valuable because it integrates previous tasks into one narrative: ELF segments on disk are transformed into mapped memory areas during execution. Students also learn that additional mappings, such as shared libraries and stack regions, appear at runtime even though they are not all directly encoded as simple fixed regions in the executable file.

## **Task**

Compare `readelf -l` output with `/proc/<PID>/maps` for the same executable.

## **Detailed steps**

1. Choose a simple program such as `/bin/sleep` and inspect its program headers:

   **readelf \-l /bin/sleep**  
   Record the loadable segments and note their permissions conceptually, such as readable, executable, or writable.  
2. Launch the program:

   **sleep 300 &**  
   Record its PID.  
3. Inspect:

   **cat /proc/\<PID\>/maps**  
   Look for the mapped regions corresponding to the executable. Compare address ranges and permissions with the program header expectations.  
4. Also identify mappings that are not simply the main executable segments, such as:

* shared libraries  
* heap  
* stack  
* anonymous mappings  
* virtual dynamic shared objects

5. Explain which parts come from the binary and which are created by runtime support or the kernel.

## **Expected outcomes** 

Students should be able to describe how ELF loadable segments correspond to runtime memory mappings, even if addresses and additional regions make the picture more complex. They should understand that program loading is a transformation process: the executable file provides a blueprint, but the final address space also includes libraries and runtime-created regions. This exercise strengthens their understanding of loaders, virtual memory, and executable structure.

# **Observe Scheduling Behavior Motivation**

Scheduling is one of the core functions of an operating system, and Linux gives users a limited but meaningful way to influence scheduling through niceness. Many students memorize that lower nice values mean higher scheduling preference, but they do not observe the effect in practice. This exercise uses CPU-bound workloads to make scheduler behavior visible through `top` and process metadata. It is educationally important because it connects abstract scheduling policy to concrete outcomes such as CPU share and responsiveness. It also introduces the difference between allowed user influence and privileged control, which is an important systems concept. By adjusting niceness and observing effects, students gain intuition about scheduler priorities under contention.

## **Task**

Create competing CPU-bound processes and observe how niceness affects scheduling.

## **Detailed steps**

1. Start two CPU-bound commands in separate shells:

   **yes \> /dev/null**  
   Start two instances and record both PIDs.  
2. Inspect their nice and priority values:

   **ps \-o pid,ni,pri,comm \-p \<PID1\>,\<PID2\>**  
3. Change the niceness of one process:  
   **renice 10 \-p \<PID1\>**  
   Then verify:  
   **ps \-o pid,ni,pri,%cpu,comm \-p \<PID1\>,\<PID2\>**  
4. Use `top` to observe CPU behavior over time:

**Top**

Watch the CPU percentages of both processes. Explain how competing CPU-bound tasks are affected when their scheduling preference differs.

5. Stop both test processes after observation.

## **Expected outcomes**

Students should observe that niceness influences scheduling preference, especially under CPU contention. They should learn that increasing a process’s nice value usually reduces its priority relative to competitors. The exact percentages may vary, but the lower-priority process should generally receive less favorable CPU scheduling. This demonstrates that Linux scheduling policy can be influenced from user space within limits.

# **Pin Processes to CPUs and Observe Core-Level Behavior**

## **Motivation**

Modern systems are multicore, so scheduling is no longer just about time-sharing one CPU. Linux must decide both when a process runs and on which CPU it runs. CPU affinity is therefore an important extension of scheduling study. `taskset` allows students to control or inspect the set of CPUs a process may use, which makes scheduling decisions more visible. This task is pedagogically useful because it helps students understand load distribution, affinity, cache locality, and the distinction between system-wide scheduling policy and per-process placement constraints. It also introduces the practical reality that resource management often includes restricting where a process may execute, not only how much time it gets.

## **Task**

Set CPU affinity for processes and observe the effect on scheduling behavior.

## **Detailed steps**

1. Determine the number of CPUs:  
   **nproc** or **lscpu**  
2. Start a CPU-bound process:

**yes \> /dev/null &**

Record the PID.

3. Check its current affinity:

**taskset \-p \<PID\>**

4. Restrict it to one CPU:  
   **taskset \-pc 0 \<PID\>**  
   If needed, replace `0` with a valid CPU number on the system.  
5. Observe with:

   **top**  
   or, if available:  
   **pidstat \-p \<PID\> 1 5**  
   Then compare by allowing more CPUs:  
   **taskset \-pc 0,1 \<PID\>**

## **Expected outcomes** 

Students should understand that CPU affinity restricts where a process is allowed to run, which is different from priority. They should see that binding a process to a single CPU can affect load distribution and may influence performance under contention. They should also recognize that affinity is a placement constraint within the scheduler, not a replacement for scheduling policy itself.

# **Explore Real-Time Scheduling Attributes** 

## **Motivation**

Linux supports more than one scheduling class. In addition to the normal fair scheduling used for typical tasks, there are real-time policies intended for workloads that require tighter scheduling guarantees. Even if students do not become kernel developers, they should understand that scheduling classes exist and that policy choice matters. `chrt` provides a practical way to inspect or, where permitted, set real-time scheduling attributes. This exercise is important because it extends the student’s view beyond niceness into scheduler classes and priorities. It also highlights privilege boundaries: some scheduling operations require elevated permissions because incorrect real-time settings can disrupt a system.

## **Task**

Inspect scheduling policy and, if permitted, experiment with `chrt`.

## **Detailed steps**

1. Inspect the current scheduling attributes of a normal process:

   **chrt \-p \<PID\>**  
   Run this on a shell process and on a CPU-bound test process. Observe the reported policy and priority.  
2. Compare with:  
   **ps \-o pid,cls,rtprio,pri,ni,cmd \-p \<PID\>**  
   Interpret the scheduling class (`CLS`) and related fields.  
3. If you have permission, launch a command with a specific policy:

   **chrt \-f 10 sleep 30**  
   If permission is denied, document the reason and explain why real-time policies are protected.  
4. Compare the reported scheduling metadata with a normal process.

## **Expected outcomes**

Students should understand that Linux scheduling involves classes and policies, not only niceness. They should identify normal versus real-time-related attributes and explain why such policies are controlled carefully. Even without changing policies, observing them is educational because it reveals the richer structure of Linux scheduling beyond everyday processes.

# **Follow Program Startup with `strace`, `time`, and Process Mapping**

## **Motivation**

Program loading is not a single invisible event. When a program starts, the kernel loads the executable, sets up the process image, and hands off to runtime components such as the dynamic loader, which opens libraries, maps memory, and prepares execution. Students often learn “exec starts a program” without seeing the many steps involved. This exercise makes startup visible using `strace` to observe system calls and `time` to measure execution cost. By relating file opens, memory mappings, and dynamic dependencies, students gain a powerful integrated understanding of program loading. This is one of the most valuable exercises because it unifies binaries, libraries, memory maps, and runtime behavior.

## **Task**

Observe what happens when a dynamically linked program starts.

## **Detailed steps**

1. Run:

   **time ls \> /dev/null**  
   Record the execution timing. Explain that startup cost includes loading and runtime initialization, not only program logic.  
2. Trace startup system calls:

   **strace \-o ls.trace ls \> /dev/null**  
   Open the trace and look for important calls such as:  
* `execve`  
* `openat`  
* `mmap`  
* `access`  
* `close`  
  Explain what these calls suggest about loading the executable and its libraries.  
3. Compare with:  
   **ldd /bin/ls**  
   **readelf \-d /bin/ls**  
   Relate the required libraries to the file accesses and mappings seen in the trace.  
4. Optionally launch a longer-running program and inspect its maps after startup:  
   **sleep 60 &**  
   **cat /proc/\<PID\>/maps**  
   Use this to compare startup-related concepts with steady-state memory layout.

## **Expected outcomes** 

Students should conclude that program startup involves a sequence of kernel and runtime actions: the executable is invoked, libraries are located, files are opened, memory regions are mapped, and execution begins. They should see that dynamic linking is a concrete runtime process, not a purely compile-time idea. The result is a deeper, integrated understanding of program loading, binary dependencies, and memory layout in Linux.

