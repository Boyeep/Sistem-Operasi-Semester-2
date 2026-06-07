## **Build a Complete System Hardware Profile**

### **Motivation**

One of the first responsibilities of a Linux administrator or systems student is to understand what physical machine Linux is running on. Before tuning performance, diagnosing slowness, planning upgrades, or troubleshooting unsupported hardware, the administrator must be able to identify the complete hardware profile of the system. Linux provides several tools that gather hardware information from different sources: firmware tables, kernel-detected devices, PCI buses, and system inventory utilities. This practical develops the important skill of building a baseline system profile, which becomes a reference point for future troubleshooting and comparison. Students will learn that no single tool shows everything perfectly. Instead, good system inspection requires combining several outputs and understanding how they complement one another. This activity forms the foundation for all later tasks related to CPU, memory, storage, cache, networking, and device management.

### **Tools**

`uname`, `hostnamectl`, `lshw`, `dmidecode`, `inxi` (optional), `cat /etc/os-release`

### **Detailed Activity**

1. Identify the operating system and kernel:

   uname \-a  
   cat /etc/os-release  
   hostnamectl

2. Generate a system hardware summary:

   sudo lshw \-short

3. Extract detailed firmware and board information:

    sudo dmidecode \-t system \-t baseboard \-t bios

4. Compare the manufacturer, product name, serial information, BIOS vendor, and architecture.

5. Write a one-page hardware profile that includes:

   * system vendor  
   * product/model  
   * motherboard/baseboard  
   * BIOS version/date  
   * kernel version  
   * architecture type  
   * whether the machine is physical, virtual, or ambiguous

### **Expected Outcomes** 

Students should produce a **consolidated hardware profile** of the machine. They should be able to explain which information came from firmware tables (`dmidecode`), which came from kernel/hardware probing (`lshw`), and which described the OS/runtime environment (`uname`, `hostnamectl`). A strong outcome includes recognizing whether the system is a laptop, desktop, server, or VM. Students should also notice that some fields may be empty, hidden, or generic on virtual machines. This teaches an important lesson: Linux visibility depends on both actual hardware and what firmware/hypervisors expose to the OS.

## **Investigate CPU Architecture and Processing Topology**

### **Motivation**

The CPU is the central execution resource managed by the operating system. To understand scheduling, thread placement, performance tuning, or virtualization limits, students must be able to interpret CPU topology correctly. Modern CPUs are not simple single-core processors; they include multiple sockets, physical cores, logical processors, simultaneous multithreading, variable frequency features, and architecture-specific capabilities. Linux exposes this information through tools such as `lscpu` and `/proc/cpuinfo`, but students often confuse concepts like core, thread, socket, and vCPU. This exercise helps students move beyond simply reading numbers and toward understanding what those numbers mean for system behavior. By connecting CPU topology to Linux scheduling and workload placement, students gain insight into why systems behave differently under load and why some applications scale better than others depending on processor design and available logical CPUs.

### **Tools**

`lscpu`, `cat /proc/cpuinfo`, `nproc`, `grep`

### **Detailed Activity**

1. Display structured CPU information:

    lscpu

2. Count logical processors:

   nproc  
   grep \-c ^processor /proc/cpuinfo

3. Inspect physical/core relationships:

    cat /proc/cpuinfo | less

4. Record these fields:

   * architecture  
   * CPU op-mode  
   * sockets  
   * cores per socket  
   * threads per core  
   * model name  
   * vendor ID  
   * flags

5. Verify whether:

   * `logical CPUs = sockets × cores per socket × threads per core`

6. Explain the difference between:

   * processor  
   * core  
   * thread  
   * socket

### **Expected Outcomes** 

Students should be able to produce a **CPU topology map** and verify the relationship between physical and logical execution units. They should explain how Linux sees schedulable CPUs and how hyperthreading or SMT changes CPU counts without increasing physical cores proportionally. A good report will distinguish hardware topology from OS-visible scheduling entities. Students should also notice that `/proc/cpuinfo` is repetitive because it presents per-logical-CPU information, whereas `lscpu` summarizes topology more clearly. This exercise should lead to the realization that proper CPU analysis requires both raw detail and structured summary.

## **Examine CPU Flags, Virtualization Support, and Instruction Capabilities**

### **Motivation**

Not all CPUs support the same instruction sets or hardware features. When installing hypervisors, running containers, enabling cryptographic acceleration, compiling optimized binaries, or deploying scientific workloads, administrators must know what the processor is capable of. Linux exposes CPU features using flags such as `vmx`, `svm`, `avx`, `aes`, and others. These capabilities directly affect performance, compatibility, and available workloads. Students often assume that if a CPU is modern, every feature is available, but Linux may reveal that firmware settings disable virtualization, or that a VM hides some instruction flags from the guest OS. This practical teaches students how to inspect CPU feature exposure and interpret what those features mean operationally. By connecting processor flags to real system functions such as virtualization, encryption, and vector processing, students will understand that hardware discovery is not only about identifying devices but also about understanding functional capability.

### **Tools**

`lscpu`, `grep`, `egrep`, `/proc/cpuinfo`

### **Detailed Activity**

1. Inspect CPU flags from `lscpu`:

    lscpu | less

2. Extract flags from `/proc/cpuinfo`:

    grep \-m1 '^flags' /proc/cpuinfo

3. Search for important capabilities:

   egrep \-o 'vmx|svm|aes|avx|avx2|sse4\_1|sse4\_2' /proc/cpuinfo | sort | uniq

4. Determine:

   * Intel virtualization support (`vmx`) or AMD virtualization support (`svm`)  
   * encryption support (`aes`)  
   * vector/instruction extensions (`sse`, `avx`, `avx2`)  
5. Write a short interpretation of what workloads benefit from each feature.

### **Expected Outcomes** 

Students should identify which **instruction and virtualization features** are available on the machine and connect them to practical uses. For example, `vmx` or `svm` suggests CPU support for hardware virtualization, while `aes` implies hardware acceleration for AES encryption. AVX-related flags indicate improved support for numerical, multimedia, and scientific workloads. Students should also understand that Linux reports what is exposed to the running OS, which may differ in a virtualized guest. The expected learning outcome is not only listing flags but translating them into system capability and deployment implications.

## **Analyze CPU Cache Hierarchy and Its Relationship to Performance**

### **Motivation**

Cache memory is one of the most important but least visible hardware resources influencing system performance. Many students focus only on RAM size or CPU frequency, yet real-world performance often depends heavily on cache hierarchy: L1, L2, and L3. Linux provides ways to inspect cache structure and sharing relationships between CPUs, allowing students to understand how the processor reduces memory access latency. This matters greatly for performance-sensitive software, multi-threaded workloads, database systems, and CPU-bound applications. In addition, cache sharing patterns can influence contention between threads. This exercise helps students connect hardware architecture to software execution by examining the cache layout that Linux reports. It develops the ability to interpret how CPUs are organized internally and why two processors with similar clock speeds may perform very differently. Students will also learn that cache information may come from structured tools and the sysfs interface, reinforcing Linux’s role as a hardware abstraction layer.

### **Tools**

`lscpu`, `lscpu -C`, `/sys/devices/system/cpu`, `getconf`

### **Detailed Activity**

1. Show cache summary:

   lscpu  
   lscpu \-C

2. Explore sysfs cache details:

    ls /sys/devices/system/cpu/cpu0/cache/

3. Inspect each cache index:

    for i in /sys/devices/system/cpu/cpu0/cache/index\*; do  
       echo "== $i \==";  
       cat $i/level $i/type $i/size $i/coherency\_line\_size $i/shared\_cpu\_list;  
   done

4. Record:

   * cache levels  
   * cache type (Data, Instruction, Unified)  
   * size of each level  
   * which CPUs share the cache

5. Explain why L1 is smaller and faster, while L3 is larger and often shared.

### **Expected Outcomes** 

Students should produce a **cache hierarchy description** showing L1/L2/L3 sizes, types, and sharing patterns. They should explain that Linux exposes the cache as part of CPU topology and that cache is a key intermediary between registers and main memory. A strong outcome includes recognizing that L1 is usually private and very fast, while L3 is typically shared among multiple cores. Students should also appreciate that shared cache can improve data reuse but may become a contention point. The real goal is for them to connect hardware structure to observable performance behavior.

## **Compare Physical Memory Information from Firmware and the Running Kernel**

### **Motivation**

Memory is reported differently depending on whether the information comes from hardware inventory data or the live kernel. Firmware-based tools such as `dmidecode` tell us what memory modules are installed physically, while the running kernel reports what memory is usable, reserved, cached, free, or swapped. Students often misunderstand why the total RAM reported by the kernel may not exactly match the installed memory capacity. This practical addresses that confusion by comparing physical module information with runtime memory accounting. Understanding this distinction is critical for hardware planning, upgrades, troubleshooting missing memory, and explaining reserved regions for firmware or integrated graphics. It also teaches students that Linux is not merely a consumer of RAM, but the active manager of memory resources. By learning to compare static hardware data and dynamic kernel data, students gain a more mature understanding of how the operating system interprets and allocates system memory.

### **Tools**

`dmidecode`, `free`, `/proc/meminfo`, `lshw`

### **Detailed Activity**

1. Inspect installed memory modules:

    sudo dmidecode \-t memory

2. Extract a cleaner memory summary:

    sudo lshw \-class memory

3. Check runtime memory totals:

   free \-h  
   cat /proc/meminfo | less

4. Record:  
   * number of DIMM slots  
   * module sizes  
   * total installed memory  
   * memory speed (if available)  
   * total memory seen by kernel

5. Compare installed vs usable memory and propose reasons for differences.

### **Expected Outcomes** 

Students should produce a **comparison between installed physical memory and kernel-visible memory**. They should explain that `dmidecode` reflects firmware inventory of RAM modules, while `free` and `/proc/meminfo` reflect the running OS perspective. Small discrepancies may result from reserved memory regions, hardware mappings, integrated GPU allocation, or virtualization behavior. The expected outcome is not merely identifying RAM size, but demonstrating that Linux resource management involves translating installed hardware into usable, managed memory for applications and kernel functions.

## **Interpreting Linux Memory Utilization, Buffers, Cache, and Available Memory**

### **Motivation**

A common beginner mistake is to look at the “used” memory value and conclude that the system is running out of RAM. In Linux, memory management is more sophisticated: unused RAM is often used for caching and buffering to improve performance, and much of that memory can be reclaimed when applications need it. This practical is important because it teaches students to read Linux memory statistics correctly rather than making incorrect operational judgments. Understanding concepts such as buffers, page cache, reclaimable memory, and available memory is essential for system monitoring, troubleshooting, and capacity planning. Students will learn that Linux tries to maximize memory usefulness, not maximize memory emptiness. This exercise develops the ability to distinguish healthy cache use from real memory pressure, which is a crucial skill for administrators, DevOps engineers, and performance analysts working with production systems.

### **Tools**

`free`, `vmstat`, `/proc/meminfo`, `sar` (optional)

### **Detailed Activity**

1. Observe memory summary:

   free \-h  
   free \-h \-w

2. Examine detailed memory counters:

    grep \-E 'MemTotal|MemFree|MemAvailable|Buffers|Cached|SwapTotal|SwapFree' /proc/meminfo

3. Monitor memory activity:

    vmstat 1 5

4. Explain each of these fields:

   * used  
   * free  
   * shared  
   * buffers  
   * cache  
   * available

5. Write a conclusion: is the system under memory pressure or just using RAM efficiently?

### **Expected Outcomes** 

Students should demonstrate an accurate **interpretation of Linux memory usage**. They should state that cached or buffered memory is not “wasted” and can often be reclaimed. A good answer will focus on `MemAvailable` rather than only `MemFree`, explaining that Linux intentionally uses spare RAM to improve performance. Students should also correlate `vmstat` output with the memory summary, particularly swap activity and run queue behavior if present. The expected result is a shift from simplistic “used vs free” thinking to kernel-aware memory interpretation.

## **Map Storage Devices, Partitions, Filesystems, and Mount Points**

### **Motivation**

Linux manages block devices through a layered structure that includes physical devices, partitions, logical volumes, filesystems, and mount points. Students need to understand this hierarchy to troubleshoot boot issues, missing disks, storage planning, filesystem errors, and resource allocation problems. Many beginners can identify a disk name like `/dev/sda` or `/dev/nvme0n1`, but cannot explain how it relates to partitions, UUIDs, filesystems, and mounted directories. This practical helps students build that mental model. It also reinforces the principle that Linux resource management is not limited to CPU and memory; storage devices are equally important resources whose structure and state must be understood. By combining outputs from multiple utilities, students learn to see storage as an integrated stack rather than separate disconnected commands. This skill is foundational for administrators, especially when working with servers, cloud instances, or systems that mix SSD, HDD, and removable media.

### **Tools**

`lsblk`, `blkid`, `df`, `mount`, `findmnt`

### **Detailed Activity**

1. Display block device tree:

   lsblk  
   lsblk \-f

2. Show UUID and filesystem metadata:

    sudo blkid

3. Check mounted filesystems:

   df \-hT  
   findmnt  
   mount | less

4. Identify:

   * disk device names  
   * partition names  
   * filesystem types  
   * mount points  
   * root filesystem device

5. Draw a storage map from physical disk to mounted directory.

### **Expected Outcomes**

Students should create a **clear mapping of storage layers**, showing how disks are partitioned and mounted into the Linux directory tree. They should explain that `lsblk` shows device relationships, `blkid` shows persistent identifiers and filesystem signatures, and `df`/`findmnt` show active filesystem usage. A strong outcome includes identifying the root filesystem and explaining why mount points matter more to users than raw device names. Students should leave this exercise with a functional understanding of how Linux presents storage resources to the system.

## **Inspect PCI Devices and Kernel-Visible Bus Architecture**

### **Motivation**

Most major internal hardware devices in a modern Linux system—network interfaces, GPUs, storage controllers, USB controllers, audio devices—are connected through the PCI/PCIe subsystem. Understanding this bus architecture is essential for diagnosing driver issues, unsupported hardware, and missing devices. Linux uses PCI enumeration to detect hardware at boot and expose it to user space. Students often see only the high-level device names but do not understand that many device classes are discovered through PCI scanning. This practical teaches students to inspect the system from the bus perspective, which is closer to how the kernel itself sees hardware. That viewpoint is extremely useful in real troubleshooting, especially when a device exists physically but is not functioning correctly. By studying PCI devices, class codes, vendor IDs, and drivers, students will see how Linux hardware management depends on the relationship between physical buses and loaded kernel modules.

### **Tools**

`lspci`, `lspci -k`, `lshw`

### **Detailed Activity**

1. List PCI devices:

    lspci

2. Show detailed class and driver information:

    lspci \-k

3. For verbose information on a selected device:

    sudo lspci \-vv

4. Identify at least five PCI devices and classify them, such as:

   * VGA/GPU  
   * Ethernet controller  
   * SATA/NVMe controller  
   * USB controller  
   * audio device

5. For one device, record the kernel driver in use and any available kernel modules.

### **Expected Outcomes**

Students should produce a **PCI device inventory** and demonstrate that many internal resources are exposed through the PCI subsystem. They should explain the difference between device identification and driver binding, showing that Linux must both detect a device and associate it with a suitable driver. The expected outcome includes recognizing that `lspci -k` is especially valuable because it links hardware devices to kernel modules. Students should understand that hardware management in Linux is inseparable from driver support

## **Examine Network Interface Hardware and Link Characteristics**

### **Motivation**

Network interfaces are both physical hardware resources and logical operating system resources. To troubleshoot connectivity, speed negotiation, duplex mismatch, link failures, or incorrect device selection, students need to inspect interfaces from both perspectives. Linux provides commands that show interface names and addresses, but deeper hardware and link-level details require tools such as `ethtool`. This practical helps students understand the relationship between the NIC hardware, its driver, and the active network link. It is particularly valuable because network issues may come from physical negotiation problems rather than IP configuration alone. Students also learn that interface naming in Linux has evolved, often reflecting predictable naming based on hardware topology. By combining interface discovery, driver information, and link diagnostics, students gain a realistic understanding of how Linux handles networking resources at the device level.

### **Tools**

`ip`, `ethtool`, `lshw -class network`, `lspci -k`

### **Detailed Activity**

1. List network interfaces:

    ip link show  
   ip addr show

2. Inspect hardware details:

    sudo lshw \-class network

3. Check NIC/link parameters for one active interface:

    sudo ethtool \<interface\>  
   sudo ethtool \-i \<interface\>

4. Record:

   * interface name  
   * MAC address  
   * driver  
   * link detected or not  
   * speed  
   * duplex  
   * port type

5. Explain how the NIC appears in both the PCI view and network-interface view.

### **Expected Outcomes**

Students should produce a **network hardware profile** for at least one interface. They should explain that `ip` shows the logical interface layer, while `ethtool` reveals physical link characteristics and driver details. They should also connect the network interface back to `lspci` or `lshw`, showing how one hardware device appears through different Linux abstractions. The expected outcome is a layered understanding of networking: hardware device, driver, link negotiation, and OS-visible interface state.

## **Investigate NUMA, CPU Locality, and Resource Placement**

### **Motivation**

On many laptops and small desktops, CPU and memory appear uniform. However, on larger servers and high-performance systems, memory access can depend on locality, especially in NUMA (Non-Uniform Memory Access) architectures. Linux exposes NUMA topology so that administrators and performance engineers can understand how processors and memory nodes are organized. This matters because accessing local memory is typically faster than remote memory attached to another NUMA node. Students working toward systems, cloud, or performance engineering roles should understand that hardware resources are not always equally close to every CPU. This practical introduces the concept of locality and shows how Linux exposes it. Even if the system is not NUMA-capable, the exercise is still valuable because students learn how to test for it and interpret the absence of NUMA complexity. It broadens their understanding of modern multiprocessor systems and resource-aware scheduling.

### **Tools**

`lscpu`, `numactl --hardware`, `lstopo` or `hwloc-ls` (if available)

### **Detailed Activity**

1. Check whether NUMA information exists:

    lscpu | grep \-i numa

2. If installed, inspect NUMA nodes:

    numactl \--hardware

3. If available, visualize topology:

    lstopo

4. Record:  
   * number of NUMA nodes  
   * CPUs in each node  
   * memory size per node

5. Explain why locality matters for databases, virtualization, and multi-threaded workloads.

### **Expected Outcomes** 

Students should determine whether the system is **NUMA-aware or UMA-like** and explain the implications. On a non-NUMA machine, they should note the simpler topology and explain that memory appears more uniform. On a NUMA-capable machine, they should identify node boundaries and relate them to CPU placement and memory access behavior. The expected learning is not merely detecting NUMA, but understanding that Linux may schedule workloads on hardware with non-uniform access costs.

## **Correlate Kernel Boot Messages with Detected Hardware**

### **Motivation**

Hardware detection is not only visible through summary tools; it is also visible in the kernel’s boot and runtime messages. The `dmesg` log often reveals how the kernel initialized CPUs, memory regions, storage controllers, network drivers, USB devices, ACPI tables, and firmware interactions. For troubleshooting, this is one of the most valuable sources of truth because it shows the sequence of hardware discovery and any errors or warnings encountered. Students should learn to use `dmesg` not as a random stream of text, but as an interpretable record of Linux bringing hardware resources online. This practical encourages temporal thinking: not just what hardware exists, but how Linux discovered it, configured it, and whether any problems occurred during initialization. This is especially important in diagnosing missing drivers, failed link training, memory reservations, or firmware compatibility issues.

### **Tools**

`dmesg`, `journalctl -k` (optional), `grep`

### **Detailed Activity**

1. Inspect the kernel ring buffer:

    dmesg | less

2. Search for important hardware categories:

    dmesg | grep \-Ei 'cpu|memory|pci|usb|eth|nvme|sata|acpi'

3. Identify messages related to:

   * CPU initialization

   * memory detection/reservation

   * PCI enumeration

   * storage controller detection

   * NIC driver loading

4. Select three hardware-related messages and explain what stage of initialization they represent.

### **Expected Outcomes** 

Students should produce a **hardware initialization summary** based on kernel messages. They should explain that `dmesg` provides chronological evidence of device discovery, driver loading, firmware interaction, and possible failures. A strong outcome includes identifying both successful detections and warnings, then linking them to devices found in `lspci`, `lsusb`, `lsblk`, or `ip`. This exercise reinforces that Linux hardware management is an active process, not just a static inventory.

## **Assess Storage Health and Device-Specific Capabilities**

### **Motivation**

Discovering that a storage device exists is not enough; students also need to assess its type, health indicators, and device-specific capabilities. Linux supports multiple storage technologies, especially SATA and NVMe, each with different tooling and performance characteristics. In real administration, storage failures often begin as degraded health metrics long before total device failure. This practical introduces health-oriented inspection, helping students understand that managed resources must also be monitored for reliability. The exercise is valuable because it links hardware identity, transport/controller type, and maintenance awareness. It also gives students exposure to the difference between generic block-layer visibility and device-specific management utilities. This is a useful professional skill for anyone responsible for maintaining Linux servers, workstations, or lab machines over time.

### **Tools**

`lsblk`, `smartctl`, `nvme`, `lspci`

### **Detailed Activity**

1. Identify storage devices:

    lsblk \-d \-o NAME,MODEL,SIZE,ROTA,TYPE

2. For SATA/SAS drives, if available:

    sudo smartctl \-a /dev/sdX

3. For NVMe devices, if available:

    sudo nvme list  
   sudo nvme smart-log /dev/nvme0

4. Record:

   * device model  
   * rotational or non-rotational status  
   * health summary  
   * temperature or warning fields if available

5. Relate the storage device back to its controller type seen in `lspci`.

### **Expected Outcomes**

Students should produce a **storage health snapshot** and classify the device technology involved. They should explain that `lsblk` reveals generic block device identity, while `smartctl` and `nvme` provide health- and vendor-level insights. The expected outcome is recognition that Linux resource management includes monitoring the quality and condition of physical resources, not only enumerating them. Students should also understand that tooling differs by storage protocol and device class.

## **Create a Cross-Tool Hardware Validation Report**

### **Motivation**

The most realistic systems skill is not running isolated commands, but validating hardware information across multiple tools and resolving differences. In actual administration, one tool may show missing details, another may rely on firmware, and another may reveal only kernel-visible devices. Good system analysts triangulate information from several sources to reach a reliable conclusion. This capstone practical brings together the entire module by requiring students to build a validated report covering CPU, cache, memory, storage, PCI devices, USB devices, network interfaces, and kernel detection evidence. The purpose is to develop analytical confidence and disciplined documentation habits. Students must move from simple command execution to evidence-based reasoning. This simulates the kind of work done in audits, support escalations, migration planning, and performance investigations, where conclusions must be backed by cross-checked observations rather than a single command output.

### **Tools**

Any combination of:  
 `lscpu`, `lshw`, `lspci`, `dmidecode`, `free`, `/proc/meminfo`, `lsblk`, `blkid`, `lsusb`, `ip`, `ethtool`, `dmesg`

### **Detailed Activity**

1. Create a table with these categories:

   * system identity  
   * CPU topology  
   * CPU flags  
   * cache hierarchy  
   * physical memory  
   * runtime memory  
   * swap  
   * storage map  
   * PCI devices  
   * USB devices  
   * network hardware  
   * kernel-detected events

2. For each category, gather evidence from at least two commands where possible.  
3. Identify at least three examples where one command adds context to another.  
4. Identify at least one inconsistency, ambiguity, or missing field and explain why it may occur.  
5. Submit a structured report with screenshots or terminal captures.

### **Expected Outcomes**

Students should submit a **cross-validated hardware/resource audit report**. The strongest reports will not simply paste outputs, but interpret them. Students should demonstrate that Linux presents hardware through multiple layers: firmware, kernel discovery, bus enumeration, logical interfaces, and filesystem/device abstraction. They should also show critical thinking by discussing missing fields, contradictions, or virtualization effects. The expected final learning outcome is systems literacy: understanding not only what hardware exists, but how Linux knows about it, manages it, and exposes it to users and administrators.

