# Laporan Hands-on 2

## Identitas Praktikum

| Item | Keterangan |
| --- | --- |
| Mata kuliah | Sistem Operasi |
| Judul praktikum | Hands-on 2 - OS Overview and Process Management |
| Nama | - |
| NIM | - |
| Kelas | - |
| Tanggal praktikum | 21 April 2026 |
| Sistem yang digunakan | Ubuntu pada VMware Virtual Platform, kernel `6.17.0-19-generic`, arsitektur `x86_64` |

## Catatan

Laporan ini disusun dalam format naratif seperti `Laporan.md` dan disiapkan agar mudah dipindahkan ke dokumen Word maupun PDF. Bagian screenshot sengaja menggunakan penanda teks "Taruh gambar ini di sini" agar gambar dapat ditempatkan ulang dengan lebih fleksibel pada proses finalisasi dokumen.

## Pendahuluan

Hands-on 2 berfokus pada observasi proses, resource yang dipegang proses, pemetaan memori, struktur executable, serta perilaku scheduling di Linux. Laporan ini menyusun hasil pengamatan dari tool seperti `ps`, `top`, `pstree`, `lsof`, `pidstat`, `pmap`, `ldd`, `readelf`, `objdump`, `taskset`, `chrt`, dan `strace` ke dalam bentuk naratif agar hubungan antara output command dan konsep inti sistem operasi terlihat lebih jelas. Seluruh hasil utama sudah dirangkum per section, sehingga tahap finalisasi yang tersisa terutama adalah penempatan screenshot pada dokumen Word atau PDF.

---

## Section 1 - Observasi Lanskap Proses dengan `ps`, `top`, dan `/proc`

Section 1 membandingkan tiga sudut pandang terhadap proses Linux, yaitu snapshot statis melalui `ps`, tampilan dinamis melalui `top`, dan metadata kernel melalui direktori `/proc`. Tiga proses yang menjadi acuan adalah `bash` sebagai shell aktif, `vmtoolsd` sebagai daemon, dan `gnome-shell` sebagai proses user yang paling menonjol pada snapshot pengamatan.

### Ringkasan Pengamatan

| Komponen | Hasil Pengamatan |
| --- | --- |
| Snapshot `ps` teratas | Pada snapshot terlihat `gnome-shell` PID `2230` sebagai proses user paling menonjol (`1.4% CPU`, `7.5% MEM`), diikuti `kworker/1:2-eve` PID `169`, `tracker-miner-f` PID `2821`, `gjs` PID `8091`, dan `vmtoolsd` PID `3255` dengan pemakaian CPU sekitar `0.2-0.3%`. |
| Proses shell yang diamati | `bash`, PID `3650` |
| Proses daemon yang diamati | `vmtoolsd`, PID `3255` |
| Proses CPU aktif yang diamati | `gnome-shell`, PID `2230`; pada tampilan `top`, urutan proses berubah dinamis dan `top` sendiri sempat muncul saat aktif digunakan. |
| Perbedaan utama `ps` vs `top` | `ps` memberi snapshot statis yang rapi dan mudah dibaca ulang, sedangkan `top` menampilkan perubahan CPU dan memori secara live sehingga urutan proses dapat berubah dari detik ke detik. |
| Informasi penting dari `/proc/PID/status` | Untuk `gnome-shell` PID `2230`, file `status` menunjukkan `State: S (sleeping)`, `PPid: 1999`, `Threads: 19`, `VmSize: 3933820 kB`, `VmRSS: 299152 kB`, serta capability efektif bernilai `0`. |
| Informasi penting dari `/proc/PID/stat` | File `stat` untuk PID `2230` menunjukkan state `R` pada saat dibaca, `PPID 1999`, `priority 20`, `nice 0`, serta akumulasi waktu user/system yang menandakan proses GUI ini aktif berjalan. |
| Informasi dari `/proc/PID/cmdline` | `/usr/bin/gnome-shell` |

### Kesimpulan Section 1

Section 1 menegaskan bahwa `ps` cocok untuk snapshot yang terstruktur, `top` lebih cocok untuk observasi real-time, sedangkan `/proc` memberikan detail mentah dari kernel. Kombinasi PID, PPID, state, nice, dan priority memberi gambaran proses dari level ringkasan sampai metadata kernel yang paling rinci.

### Bukti Screenshot

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/01-section1-ps-top-1.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/01-section1-ps-top-2.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/02-section1-proc-details-1.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/02-section1-proc-details-2.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/02-section1-proc-details-3.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/02-section1-proc-details-4.png`

---

## Section 2 - Relasi Parent-Child dan Pembuatan Proses

Section 2 berfokus pada hubungan antarproses, terutama parent-child relationship, process group, dan session. Analisis dilakukan menggunakan `pstree -p`, `ps -eo pid,ppid,pgid,sid,comm --forest`, lalu dilanjutkan dengan membuat background job menggunakan `sleep 300 &` dan `jobs -l`. Pada bagian ini, dokumentasikan bagaimana shell bertindak sebagai parent, bagaimana nilai `PPID`, `PGID`, dan `SID` terkait dengan job control, serta apa yang terjadi setelah proses dihentikan dengan `kill`.

### Ringkasan Pengamatan

| Komponen | Hasil Pengamatan |
| --- | --- |
| Hasil utama `pstree -p` | `pstree -p` memperlihatkan `systemd(1)` sebagai akar tree, lalu service seperti `NetworkManager`, `dbus-daemon`, `gdm3`, dan sesi GNOME bercabang di bawahnya. |
| Hasil utama `ps --forest` | `ps --forest` menampilkan kernel thread di bawah PID `2` dan memperlihatkan relasi parent-child melalui indentasi hirarkis, sehingga proses sistem dan user dapat dilihat dalam satu pohon. |
| PID shell pembuat proses | `3650` (`bash`) |
| PID background process `sleep 300` | `20987` |
| PPID proses `sleep` | `3650`, sehingga jelas bahwa proses `sleep` dibuat oleh shell `bash` yang sedang aktif. |
| PGID dan SID proses `sleep` | `PGID 20987` dan `SID 3650`; artinya `sleep` mempunyai process group sendiri, tetapi masih berada dalam session shell yang sama. |
| Kondisi setelah `kill PID` | Setelah `kill`, shell menampilkan status `Terminated` dan proses `sleep` tidak lagi muncul, sehingga child process telah direap dengan benar oleh shell. |

### Kesimpulan Section 2

Section 2 memperlihatkan bahwa proses di Linux tidak berdiri sendiri, melainkan muncul dalam struktur keluarga dan konteks job control tertentu. Hasil pengamatan menunjukkan bahwa shell bukan hanya interpreter command, tetapi juga parent process yang membuat, mengelola, dan mereap child process.

### Bukti Screenshot

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/03-section2-pstree-forest-1.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/03-section2-pstree-forest-2.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/04-section2-jobs-kill.png`

---

## Section 3 - Inspeksi Open Resources dengan `lsof`

Section 3 membahas hubungan antara proses dan resource yang sedang dibuka, seperti current working directory, executable image, shared libraries, terminal device, file biasa, dan socket. Dari hasil `lsof` terlihat bahwa sebuah proses dapat tampak tenang dari sisi CPU, tetapi tetap memegang resource kernel penting yang menentukan interaksi proses dengan sistem.

### Ringkasan Pengamatan

| Komponen | Hasil Pengamatan |
| --- | --- |
| Temuan umum dari `lsof | head -40` | Kolom `COMMAND` menunjukkan nama proses, `PID` nomor proses, `USER` pemilik, `FD` jenis descriptor seperti `cwd`, `txt`, `0u`, `1u`, `2u`, `TYPE` jenis objek, dan `NAME` path resource yang sedang dibuka. |
| PID proses yang dianalisis | `bash`, PID `3650` |
| `cwd` | `/home/ubuntu/Desktop` |
| `txt` | `/usr/bin/bash` |
| `0u`, `1u`, `2u` | Ketiganya mengarah ke `/dev/pts/0`, yaitu stdin, stdout, dan stderr dari terminal interaktif yang sedang dipakai. |
| File uji yang dibuka | `/home/ubuntu/Desktop/handson2-open-file.txt` dibuka oleh proses `tail` PID `21113`. |
| Resource jaringan dari `lsof -i` | Resource jaringan terlihat pada `python3` PID `21114` yang sedang `LISTEN` pada `TCP *:8000`. |

### Kesimpulan Section 3

Section 3 menunjukkan bahwa banyak resource Linux direpresentasikan sebagai file descriptor atau objek mirip file. Melalui `lsof`, hubungan antara proses dengan file, direktori, terminal, shared library, dan koneksi jaringan dapat ditelusuri dengan sangat jelas.

### Bukti Screenshot

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/05-section3-lsof-overview.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/06-section3-lsof-file-network.png`

---

## Section 4 - Menemukan Proses Pemilik Port atau File

Section 4 berfokus pada troubleshooting resource conflict menggunakan `ss`, `netstat`, dan `fuser`. Pengamatan ini menunjukkan bagaimana proses pemilik listening port maupun file tertentu dapat diidentifikasi kembali dengan cepat dari sisi jaringan maupun file reference.

### Ringkasan Pengamatan

| Komponen | Hasil Pengamatan |
| --- | --- |
| Port yang dipilih untuk dianalisis | Port `631` dan `53` terlihat jelas pada listener lokal, sedangkan port uji buatan yang paling mudah dipetakan adalah `8000`. |
| Hasil `ss -tulpn` | `ss` menampilkan listener UDP/TCP pada `5353`, `53`, `631`, dan beberapa port dinamis; yang paling mudah dikenali pada screenshot adalah `127.0.0.1:631`, `[::1]:631`, `127.0.0.54:53`, dan `127.0.0.53%lo:53`. |
| Hasil `netstat -tulpn` | `netstat` tidak tersedia pada sistem ini, sehingga inspeksi socket lebih praktis dilakukan dengan `ss`. |
| Pemilik listening port | Pada screenshot `ss`, kolom `Process` tidak terisi, tetapi pada pengamatan jaringan yang lebih terkontrol terlihat `python3` PID `21114` sebagai pemilik `TCP *:8000 (LISTEN)`. |
| File yang diuji dengan `fuser` | `/home/ubuntu/Desktop/handson2-fuser.txt` |
| Hasil `fuser "$HOME/Desktop/handson2-fuser.txt"` | PID `20576` |
| Hasil `fuser -v "$HOME/Desktop/handson2-fuser.txt"` | File sedang direferensikan oleh user `ubuntu` melalui proses `tail` PID `20576` dengan mode akses `f....`, yang menunjukkan file masih terbuka aktif. |

### Kesimpulan Section 4

Section 4 menegaskan bahwa konflik port dan file pada Linux dapat ditelusuri kembali ke proses pemilik resource. Pada sistem ini, `ss` menjadi tool utama untuk inspeksi socket, sedangkan `fuser` sangat praktis untuk mengetahui proses yang masih mereferensikan file atau mount point.

### Bukti Screenshot

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/07-section4-ss-netstat.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/08-section4-fuser.png`

---

## Section 5 - Mengukur Aktivitas CPU dan I/O per Proses dengan `pidstat`

Section 5 menggunakan `pidstat` untuk melihat perilaku proses dari waktu ke waktu, bukan hanya snapshot sesaat. Perbandingan antara proses CPU-bound dan proses yang lebih dominan pada I/O memperlihatkan bahwa pola beban kerja terlihat jauh lebih jelas ketika diamati dalam beberapa interval.

### Ringkasan Pengamatan

| Komponen | Hasil Pengamatan |
| --- | --- |
| Proses CPU-bound yang dipilih | `yes`, PID `20604` |
| Ringkasan `pidstat -p $pid 1 10` | Proses `yes` secara konsisten memakai sekitar `98-100% CPU` pada hampir semua interval, dengan rata-rata `98.80% CPU` dan dominasi waktu user dibanding system. |
| Ringkasan `pidstat -r -p $pid 1 10` | `minflt/s` dan `majflt/s` sama-sama `0.00`; `VSZ` sekitar `16964`, `RSS` sekitar `1916 kB`, dan `%MEM` sekitar `0.05`, sehingga proses CPU-bound ini nyaris tidak menunjukkan aktivitas paging. |
| Proses I/O yang diamati | `bash` PID `3650` saat menjalankan `dd`, dengan `tracker-miner-f` PID `2821` sebagai pembanding proses latar. |
| Ringkasan `pidstat -d 1 10` | `bash` menulis sangat besar saat `dd` berjalan, sempat mencapai sekitar `231985.84 kB_wr/s`, sedangkan `tracker-miner-f` hanya menunjukkan I/O lebih kecil sekitar `7.08 kB_rd/s` dan `148.67 kB_wr/s` pada awal pengamatan. |
| Perbedaan CPU-bound vs I/O-bound | Proses CPU-bound tampak penuh di kolom `%CPU` tetapi hampir tidak melakukan I/O, sedangkan proses I/O-bound menonjol di `kB_rd/s` dan `kB_wr/s` walaupun tidak selalu memakai CPU setinggi `yes`. |

### Kesimpulan Section 5

Section 5 memperlihatkan bahwa `pidstat` berguna untuk mengamati pola runtime secara temporal. Proses CPU-bound cenderung dominan pada metrik CPU, sedangkan proses I/O-bound lebih terlihat pada kolom aktivitas disk atau fault tertentu.

### Bukti Screenshot

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/09-section5-pidstat-cpu.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/10-section5-pidstat-memory-io.png`

---

## Section 6 - Memory Pressure

Section 6 mengamati cara Linux melaporkan memori pada kondisi normal dan ketika diberi beban sedang. Kombinasi `free`, `vmstat`, dan `/proc/meminfo` memperlihatkan hubungan antara kapasitas memori, cache, swap, dan aktivitas CPU saat sistem menerima beban tambahan.

### Ringkasan Pengamatan

| Komponen | Hasil Pengamatan |
| --- | --- |
| `MemTotal` | `3960608 kB` |
| `MemFree` | `563704 kB` |
| `MemAvailable` | `2455588 kB` |
| `Buffers` | `56572 kB` |
| `Cached` | `2013540 kB` |
| `SwapTotal` | `3959804 kB` |
| `SwapFree` | `3959684 kB` |
| Ringkasan `vmstat` kondisi awal | Pada kondisi awal, `r` umumnya `0`, `si/so` tetap `0`, `bi/bo` kecil dan sesekali muncul, sedangkan CPU didominasi `id 97-98%` dengan `us/sy` sekitar `1-2%`. |
| Ringkasan `vmstat` setelah diberi beban | Saat proses alokasi memori dijalankan, runnable process sempat naik sampai `r=3`, `us/sy` melonjak pada interval awal (sekitar `us 27`, `sy 33`, `id 40`), tetapi `si/so` tetap `0` dan memori tersedia masih sekitar `2.3 GiB`. |
| Interpretasi memory pressure | Sistem masih relatif longgar; beban tambahan memicu aktivitas CPU dan perubahan free/cache, tetapi belum menunjukkan gejala swap in/out atau tekanan memori berat. |

### Kesimpulan Section 6

Section 6 menekankan bahwa memori Linux tidak boleh dinilai hanya dari angka `free` saja. Pada pengamatan ini, `MemAvailable` lebih representatif untuk kapasitas yang masih dapat dipakai, sedangkan aktivitas swap dan paging menjadi indikator utama apakah tekanan memori benar-benar terjadi.

### Bukti Screenshot

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/11-section6-memory-baseline.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/12-section6-memory-load.png`

---

## Section 7 - Peta Memori Proses dengan `pmap` dan `/proc/PID/maps`

Section 7 menghubungkan konsep address space dengan peta memori proses yang nyata. Dari hasil `pmap` dan `/proc/PID/maps`, struktur executable mapping, shared library, heap, stack, anonymous mapping, dan special mapping dapat diamati langsung pada sebuah proses sederhana.

### Ringkasan Pengamatan

| Komponen | Hasil Pengamatan |
| --- | --- |
| Proses yang dianalisis | `sleep 300`, PID `20640` |
| Ringkasan `pmap $pid` | `pmap` menunjukkan total pemetaan sekitar `16968K` dengan region executable kecil untuk `sleep` dan beberapa mapping library pendukung. |
| Ringkasan `pmap -x $pid` | `pmap -x` menampilkan total sekitar `16968 kB`, `RSS` sekitar `2152 kB`, dan `Dirty` sekitar `108 kB`. |
| Heap | Region `[heap]` terlihat pada rentang `5b781ac47000-5b781ac68000`. |
| Stack | Region `[stack]` terlihat pada rentang `7fffa22a6000-7fffa22c7000`. |
| Shared libraries | Library yang terlihat antara lain `libc.so.6`, `ld-linux-x86-64.so.2`, `locale-archive`, dan `coreutils.mo`. |
| Anonymous mapping | Ada beberapa region `[ anon ]` dengan mode `rw---` dan `r-x--` yang menunjukkan mapping anonim tambahan. |
| Special mapping | Mapping khusus yang tampak meliputi `[vvar]`, `[vclock]`, `[vdso]`, dan `[vsyscall]`. |

### Kesimpulan Section 7

Section 7 menunjukkan bahwa memori proses Linux terdiri atas banyak region virtual, bukan satu blok tunggal. `pmap` memberi ringkasan yang nyaman dibaca, sementara `/proc/PID/maps` memberikan detail mentah yang lebih dekat ke sudut pandang kernel.

### Bukti Screenshot

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/13-section7-pmap-x.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/14-section7-proc-maps.png`

---

## Section 8 - Shared Libraries dan Dynamic Linking dengan `ldd`

Section 8 mengamati dependensi shared library dari executable Linux menggunakan `ldd`. Perbandingan antara `/bin/ls`, `/bin/bash`, dan `/proc/PID/maps` memperlihatkan bagaimana dynamic loader dan library bersama muncul kembali pada proses yang sedang berjalan.

### Ringkasan Pengamatan

| Komponen | Hasil Pengamatan |
| --- | --- |
| Ringkasan `ldd /bin/ls` | `ls` bergantung pada `linux-vdso.so.1`, `libselinux.so.1`, `libc.so.6`, `libpcre2-8.so.0`, dan loader `/lib64/ld-linux-x86-64.so.2`. |
| Ringkasan `ldd /bin/bash` | `bash` bergantung pada `linux-vdso.so.1`, `libtinfo.so.6`, `libc.so.6`, dan loader `/lib64/ld-linux-x86-64.so.2`. |
| Dynamic loader yang muncul | `/lib64/ld-linux-x86-64.so.2` |
| Proses runtime yang dibandingkan | `bash`, PID `3650` |
| Kecocokan library di `/proc/PID/maps` | Pada `/proc/3650/maps` terlihat `/usr/bin/bash` bersama `libc.so.6`, `libcap.so.2.66`, dan `libtinfo.so.6.4`, yang konsisten dengan dependensi `bash` pada hasil `ldd`. |
| Interpretasi dynamic linking | Dynamic linking membuat executable memuat library yang dibutuhkan saat startup, sehingga program tidak perlu menyertakan seluruh kode dependensinya secara statis. |

### Kesimpulan Section 8

Section 8 menegaskan bahwa banyak executable Linux tidak bersifat mandiri, melainkan bergantung pada shared library yang diselesaikan saat runtime. Hubungan antara daftar dependensi `ldd`, dynamic loader, dan mapping library pada proses aktif terlihat konsisten sepanjang pengamatan.

### Bukti Screenshot

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/15-section8-ldd-ls-bash.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/16-section8-maps-library-match.png`

---

## Section 9 - Struktur File Eksekusi dengan `readelf`

Section 9 mempelajari struktur ELF pada executable Linux menggunakan `readelf -h`, `readelf -S`, `readelf -l`, dan `readelf -d`. Hasilnya memperlihatkan perbedaan yang jelas antara header umum, section header, program header, dan dynamic information dalam satu file executable.

### Ringkasan Pengamatan

| Komponen | Hasil Pengamatan |
| --- | --- |
| ELF class | `ELF64` |
| Machine type | `Advanced Micro Devices X86-64` |
| Entry point | `0x6d30` |
| Section penting | `.text` menyimpan instruction utama, `.data` dan `.bss` menyimpan data writable, `.rodata` menyimpan data read-only, dan `.dynsym` menyimpan simbol yang dipakai saat dynamic linking. |
| Program header penting | Terlihat beberapa segment `LOAD` untuk area `R`, `R E`, dan `RW`, serta `INTERP` yang menunjuk ke loader dinamis. |
| Dynamic information | Dynamic section mencantumkan `NEEDED` library `[libselinux.so.1]` dan `[libc.so.6]`, bersama tag seperti `INIT`, `FINI`, `GNU_HASH`, `STRTAB`, `SYMTAB`, `RELA`, dan `FLAGS_1 = NOW PIE`. |
| Interpretasi umum | Section menggambarkan organisasi isi file untuk linking dan analisis, sedangkan segment/program header adalah blueprint yang benar-benar dipakai loader saat memetakan file ke memori. |

### Kesimpulan Section 9

Section 9 menunjukkan bahwa file ELF memuat metadata terstruktur untuk proses loading dan linking. Section lebih berguna untuk sudut pandang linking dan organisasi isi file, sedangkan segment lebih dekat dengan kebutuhan runtime ketika executable dimuat ke memori.

### Bukti Screenshot

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/17-section9-readelf-header-sections.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/18-section9-readelf-program-dynamic.png`

---

## Section 10 - Disassembly Machine Code dengan `objdump`

Section 10 memperlihatkan bahwa executable Linux pada akhirnya berisi instruction stream yang dapat didisassembly. Dari `objdump`, struktur section dan potongan assembly awal dapat dibaca sebagai representasi langsung dari machine code yang akan dieksekusi CPU.

### Ringkasan Pengamatan

| Komponen | Hasil Pengamatan |
| --- | --- |
| Ringkasan `objdump -h /bin/ls` | `objdump -h` menampilkan section penting seperti `.interp`, `.dynsym`, `.dynstr`, `.rela.dyn`, `.rela.plt`, `.init`, `.plt`, `.plt.sec`, `.text`, dan `.fini` lengkap dengan alamat/VMA masing-masing. |
| Fungsi / area kode yang diamati | Potongan yang terlihat berfokus pada area code seperti `.init`, `.plt`, dan `.text`; nama fungsi spesifik belum terlihat jelas pada screenshot. |
| Temuan dari `objdump -d /bin/ls` | Output awal memperlihatkan bahwa byte code pada section executable dapat diterjemahkan menjadi instruksi assembly, sehingga binary benar-benar berisi instruction stream yang siap dieksekusi. |
| Temuan tambahan dari `objdump -x /bin/ls` | Pada screenshot ini tidak ada tangkapan `objdump -x` terpisah; fokus observasi berada pada section dan awal disassembly. |
| Hubungan dengan `.text` | Section `.text` adalah lokasi utama instruksi program, dan `objdump -d` membaca byte dari area executable itu lalu menampilkannya dalam bentuk assembly. |

### Kesimpulan Section 10

Section 10 menghubungkan konsep binary dengan machine instruction yang benar-benar dieksekusi CPU. Section metadata dan hasil disassembly saling melengkapi untuk menjelaskan bagaimana code disimpan di dalam executable.

### Bukti Screenshot

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/19-section10-objdump.png`

---

## Section 11 - Relasi ELF Segment ke Runtime Memory Layout

Section 11 mengintegrasikan hasil `readelf -l` dan `/proc/PID/maps` pada executable yang sama, yaitu `/bin/sleep`. Dengan cara ini, relasi antara segment ELF di disk dan mapping virtual saat runtime dapat dilihat secara langsung.

### Ringkasan Pengamatan

| Komponen | Hasil Pengamatan |
| --- | --- |
| Executable yang dipilih | `/bin/sleep` |
| Ringkasan loadable segment | `readelf -l` menunjukkan ELF bertipe `DYN (PIE)` dengan `13` program header, beberapa `LOAD` segment berizin `R`, `R E`, `R`, dan `RW`, serta `INTERP` yang meminta `/lib64/ld-linux-x86-64.so.2`. |
| PID proses runtime | PID proses `sleep 300` tidak terbaca jelas pada screenshot, tetapi mapping yang dianalisis berasal dari proses `sleep` yang baru dijalankan pada section ini. |
| Region di `/proc/PID/maps` yang sesuai dengan executable | Region `/usr/bin/sleep` muncul berurutan sebagai mapping `r--p`, `r-xp`, dan `rw-p`, sesuai dengan segment baca, eksekusi, dan writable yang diharapkan dari file ELF. |
| Region tambahan saat runtime | Selain executable utama, terlihat `heap`, `libc.so.6`, `ld-linux-x86-64.so.2`, `locale-archive`, `[stack]`, `[vvar]`, `[vclock]`, `[vdso]`, dan `[vsyscall]`. |
| Interpretasi transformasi disk ke memori | Loader menerjemahkan segment dalam file ELF menjadi mapping virtual yang terpisah, lalu menambahkan library bersama dan region runtime lain sehingga address space akhir lebih kaya daripada isi file utama saja. |

### Kesimpulan Section 11

Section 11 menekankan bahwa binary di disk bertindak sebagai blueprint, sedangkan address space akhir proses dibentuk oleh loader, shared library, kernel, dan runtime support. Relasi segment ELF dengan mapping runtime terlihat jelas, tetapi tetap disertai region tambahan yang tidak berasal langsung dari file executable utama.

### Bukti Screenshot

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/20-section11-readelf-vs-maps.png`

---

## Section 12 - Perilaku Scheduling dan Niceness

Section 12 mengamati pengaruh niceness terhadap pembagian CPU ketika dua proses CPU-bound bersaing. Dua proses `yes` dipakai untuk melihat bagaimana perubahan nice memengaruhi priority metadata dan perilaku scheduler pada sistem dua CPU logis.

### Ringkasan Pengamatan

| Komponen | Hasil Pengamatan |
| --- | --- |
| PID proses CPU-bound 1 | `20699` |
| PID proses CPU-bound 2 | `20700` |
| Nilai `NI` dan `PRI` sebelum `renice` | Keduanya memiliki `NI 0` dan `PRI 19` sebelum dilakukan perubahan niceness. |
| Perubahan setelah `renice 10 -p $pid1` | Proses `20699` berubah menjadi `NI 10` dan `PRI 9`, sedangkan proses `20700` tetap `NI 0` dan `PRI 19`. |
| Pengamatan dari `top` | Pada `top`, kedua proses `yes` tetap dominan di CPU; proses `20700` sempat sekitar `98.3% CPU`, sedangkan proses `20699` yang sudah direnice masih sekitar `94.7% CPU`. |
| Interpretasi efek niceness | Nice yang lebih besar menurunkan priority metadata dan preference scheduler, tetapi pada sistem dua core kedua proses masih dapat memperoleh porsi CPU tinggi ketika masing-masing bisa berjalan paralel. |

### Kesimpulan Section 12

Section 12 menunjukkan bahwa niceness memengaruhi scheduling preference, terutama ketika proses CPU-bound benar-benar saling bersaing. Hasil observasi tetap konsisten dengan teori bahwa nilai nice yang lebih tinggi berarti prioritas relatif yang lebih rendah, walaupun efeknya tidak selalu dramatis pada sistem multicore ringan.

### Bukti Screenshot

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/21-section12-nice-top-1.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/21-section12-nice-top-2.png`

---

## Section 13 - CPU Affinity dengan `taskset`

Section 13 membahas bagaimana Linux tidak hanya memutuskan kapan proses berjalan, tetapi juga pada CPU mana proses diizinkan berjalan. Perubahan affinity dengan `taskset` memperlihatkan bahwa scheduler tunduk pada batas placement yang ditetapkan untuk suatu proses.

### Ringkasan Pengamatan

| Komponen | Hasil Pengamatan |
| --- | --- |
| Jumlah CPU logis sistem | `2` |
| PID proses yang dibatasi affinity-nya | `20728` |
| Affinity awal | Mask awal `3` dengan daftar CPU `0,1`. |
| Affinity setelah dibatasi ke satu CPU | Setelah `taskset -pc 0`, affinity berubah menjadi daftar CPU `0`. |
| Affinity setelah diperluas | Setelah `taskset -pc 0,1`, affinity kembali menjadi daftar CPU `0,1`. |
| Pengamatan performa / distribusi | Proses berhasil dipaksa hanya berjalan pada CPU `0`, lalu diizinkan kembali ke dua CPU; ini menunjukkan scheduler dibatasi pada penempatan core, bukan pada jumlah waktu CPU secara langsung. |
| Perbedaan affinity vs priority | Affinity menentukan di CPU/core mana proses boleh berjalan, sedangkan priority menentukan preferensi penjadwalan ketika beberapa proses bersaing pada CPU yang tersedia. |

### Kesimpulan Section 13

Section 13 menegaskan bahwa CPU affinity adalah placement constraint, bukan pengganti scheduling priority. Pembatasan CPU secara langsung mengurangi fleksibilitas scheduler dalam menempatkan proses pada core yang tersedia.

### Bukti Screenshot

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/22-section13-taskset-affinity.png`

---

## Section 14 - Atribut Real-Time Scheduling dengan `chrt`

Section 14 memperkenalkan bahwa Linux memiliki scheduling class yang lebih kaya daripada sekadar niceness. Perbandingan antara `chrt` dan `ps` menunjukkan policy default proses normal, sekaligus memperlihatkan bahwa perubahan ke policy real-time dibatasi oleh hak akses sistem.

### Ringkasan Pengamatan

| Komponen | Hasil Pengamatan |
| --- | --- |
| Proses normal yang diamati | `bash`, PID `3650` |
| Hasil `chrt -p $$` untuk proses normal | Policy saat ini adalah `SCHED_OTHER` dengan priority `0`. |
| Proses CPU-bound yang diamati | Tidak direkam terpisah pada screenshot; bukti section ini berfokus pada shell normal sebagai contoh policy default. |
| Hasil `ps -o pid,cls,rtprio,pri,ni,cmd -p $$` | `PID 3650`, `CLS TS`, `RTPRIO -`, `PRI 19`, `NI 0`, `CMD bash`. |
| Status percobaan `chrt -f 10 sleep 30` | Gagal dengan pesan `Operation not permitted`. |
| Alasan bila gagal | Pengaturan scheduling real-time memerlukan hak akses lebih tinggi karena dapat mempengaruhi fairness dan respons sistem secara keseluruhan. |
| Perbedaan normal vs real-time scheduling | Proses normal memakai kelas `TS` / `SCHED_OTHER` tanpa prioritas real-time, sedangkan policy real-time seperti `FIFO` diberi kontrol privilege ketat agar tidak mudah mengganggu sistem. |

### Kesimpulan Section 14

Section 14 menunjukkan bahwa Linux memiliki kelas scheduling yang berbeda untuk kebutuhan yang berbeda pula. Real-time policy bersifat lebih sensitif dan biasanya dilindungi oleh mekanisme privilege karena kesalahan konfigurasi dapat mengganggu stabilitas sistem.

### Bukti Screenshot

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/23-section14-chrt-policy.png`

---

## Section 15 - Startup Program dengan `strace`, `time`, dan Process Mapping

Section 15 mengamati startup program sebagai rangkaian event kernel dan runtime, bukan sekadar satu aksi `exec`. Kombinasi `time`, `strace`, `ldd`, dan `readelf -d` memperlihatkan alur pemuatan executable, pencarian library, dan pembentukan runtime environment pada saat program mulai berjalan.

### Ringkasan Pengamatan

| Komponen | Hasil Pengamatan |
| --- | --- |
| Hasil `time ls > /dev/null` | `real 0m0.007s`, `user 0m0.004s`, `sys 0m0.001s` |
| Ringkasan syscall penting dari `ls.trace` | Trace menunjukkan `execve("/usr/bin/ls")`, `brk`, beberapa `mmap`, `access("/etc/ld.so.preload")`, `openat("/etc/ld.so.cache")`, lalu `openat` ke library seperti `libselinux.so.1`, `libc.so.6`, dan `libpcre2-8.so.0`, diikuti `read`, `pread64`, dan `close`. |
| Ringkasan `ldd /bin/ls` | `linux-vdso.so.1`, `libselinux.so.1`, `libc.so.6`, `libpcre2-8.so.0`, dan `/lib64/ld-linux-x86-64.so.2`. |
| Ringkasan `readelf -d /bin/ls` | Dynamic section mencantumkan `NEEDED [libselinux.so.1]` dan `NEEDED [libc.so.6]`, bersama tag seperti `INIT`, `FINI`, `RELA`, `GNU_HASH`, `STRTAB`, `SYMTAB`, dan `FLAGS_1 = NOW PIE`. |
| Proses tambahan yang diamati dengan `/proc/PID/maps` | Tidak diamati terpisah pada screenshot final; fokus bukti berada pada trace startup, dependensi library, dan dynamic section dari `ls`. |
| Interpretasi startup program | Startup program dimulai dari `execve`, dilanjutkan pembacaan cache loader dan shared library melalui `openat`/`mmap`, lalu dependensi yang sama dikonfirmasi kembali oleh `ldd` dan `readelf -d`. |

### Kesimpulan Section 15

Section 15 mengintegrasikan konsep executable, dynamic linking, syscall, dan memory mapping menjadi satu alur startup program. Eksekusi program modern memang melibatkan pemuatan executable, pencarian library, pembukaan file, memory mapping, dan handoff ke runtime sebelum logika utama program berjalan.

### Bukti Screenshot

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/24-section15-time-strace-1.png`

Taruh gambar ini di sini: `Screenshot-Laporan-Handson-2/24-section15-time-strace-2.png`

---

## Penutup

Berdasarkan seluruh pengamatan pada Hands-on 2, Linux menyediakan banyak cara untuk melihat proses, resource, memori, struktur executable, dan kebijakan scheduling dari sudut pandang yang saling melengkapi. Laporan ini sudah dirapikan dalam format naratif agar mudah dipindahkan ke Word atau PDF, dan tahap finalisasi yang tersisa terutama adalah menempatkan screenshot pada posisi yang sudah ditandai serta melengkapi identitas penulis bila diperlukan.
