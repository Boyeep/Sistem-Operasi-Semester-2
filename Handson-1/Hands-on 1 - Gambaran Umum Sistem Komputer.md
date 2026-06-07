# Hands-on 1 - Gambaran Umum Sistem Komputer

## Tujuan Umum

Praktikum ini membantu mahasiswa memahami bagaimana Linux mengenali, menampilkan, dan mengelola sumber daya perangkat keras. Fokus utamanya bukan hanya menjalankan perintah, tetapi juga membandingkan hasil dari beberapa alat, lalu menyusun laporan yang rapi dan masuk akal.

Praktikum sebaiknya dijalankan di:

- Linux asli
- WSL
- VM Linux

Jika dijalankan di WSL atau VM, tuliskan pada laporan bahwa sebagian informasi perangkat keras bisa terlihat tidak lengkap, generik, atau virtual.

---

## 1. Membangun Profil Perangkat Keras Sistem Secara Lengkap

### Tujuan

Mahasiswa mampu mengidentifikasi identitas sistem, vendor perangkat, motherboard, BIOS, kernel, dan arsitektur mesin.

### Tools

- `uname`
- `hostnamectl`
- `lshw`
- `dmidecode`
- `inxi` (opsional)
- `cat /etc/os-release`

### Langkah

1. Identifikasi sistem operasi dan kernel:

```bash
uname -a
cat /etc/os-release
hostnamectl
```

2. Tampilkan ringkasan perangkat keras:

```bash
sudo lshw -short
```

3. Ambil informasi firmware dan motherboard:

```bash
sudo dmidecode -t system -t baseboard -t bios
```

4. Bandingkan informasi berikut:

- pabrikan sistem
- nama produk atau model
- serial number jika ada
- vendor BIOS
- arsitektur mesin

### Output yang Diharapkan

Buat profil perangkat keras satu halaman yang memuat:

- vendor sistem
- model produk
- motherboard atau baseboard
- versi dan tanggal BIOS
- versi kernel
- jenis arsitektur
- kesimpulan apakah mesin fisik, virtual, atau ambigu

---

## 2. Menyelidiki Arsitektur CPU dan Topologi Pemrosesan

### Tujuan

Mahasiswa memahami perbedaan antara socket, core, thread, dan logical CPU.

### Tools

- `lscpu`
- `cat /proc/cpuinfo`
- `nproc`
- `grep`

### Langkah

1. Tampilkan informasi CPU terstruktur:

```bash
lscpu
```

2. Hitung jumlah logical processor:

```bash
nproc
grep -c ^processor /proc/cpuinfo
```

3. Lihat hubungan core dan processor:

```bash
cat /proc/cpuinfo | less
```

4. Catat informasi berikut:

- architecture
- CPU op-mode
- socket
- cores per socket
- threads per core
- model name
- vendor ID
- flags

5. Verifikasi apakah:

```text
logical CPUs = sockets x cores per socket x threads per core
```

6. Jelaskan perbedaan:

- processor
- core
- thread
- socket

### Output yang Diharapkan

Buat peta topologi CPU dan jelaskan bagaimana Linux melihat CPU yang dapat dijadwalkan.

---

## 3. Memeriksa Flag CPU, Dukungan Virtualisasi, dan Kemampuan Instruksi

### Tujuan

Mahasiswa mampu mengaitkan flag CPU dengan kemampuan nyata seperti virtualisasi, enkripsi, dan pemrosesan vektor.

### Tools

- `lscpu`
- `grep`
- `egrep`
- `/proc/cpuinfo`

### Langkah

1. Lihat flag CPU dari `lscpu`:

```bash
lscpu | less
```

2. Ambil baris flag dari `/proc/cpuinfo`:

```bash
grep -m1 '^flags' /proc/cpuinfo
```

3. Cari fitur penting:

```bash
egrep -o 'vmx|svm|aes|avx|avx2|sse4_1|sse4_2' /proc/cpuinfo | sort | uniq
```

4. Tentukan:

- dukungan virtualisasi Intel (`vmx`) atau AMD (`svm`)
- dukungan enkripsi (`aes`)
- dukungan instruksi vektor (`sse`, `avx`, `avx2`)

5. Tulis penjelasan singkat beban kerja apa yang terbantu oleh fitur-fitur tersebut.

### Output yang Diharapkan

Buat daftar fitur CPU dan arti praktisnya bagi sistem.

---

## 4. Menganalisis Hirarki Cache CPU dan Hubungannya dengan Kinerja

### Tujuan

Mahasiswa memahami cache L1, L2, dan L3 serta relasi cache terhadap performa sistem.

### Tools

- `lscpu`
- `lscpu -C`
- `/sys/devices/system/cpu`
- `getconf`

### Langkah

1. Tampilkan ringkasan cache:

```bash
lscpu
lscpu -C
```

2. Lihat struktur cache CPU 0:

```bash
ls /sys/devices/system/cpu/cpu0/cache/
```

3. Periksa setiap indeks cache:

```bash
for i in /sys/devices/system/cpu/cpu0/cache/index*; do
  echo "== $i ==";
  cat $i/level $i/type $i/size $i/coherency_line_size $i/shared_cpu_list;
done
```

4. Catat:

- level cache
- tipe cache
- ukuran cache
- CPU yang berbagi cache

5. Jelaskan mengapa L1 lebih kecil dan cepat, sedangkan L3 lebih besar dan umumnya dibagi bersama.

### Output yang Diharapkan

Buat deskripsi hirarki cache CPU beserta pola pembagiannya.

---

## 5. Membandingkan Informasi Memori Fisik dari Firmware dan Kernel

### Tujuan

Mahasiswa memahami perbedaan antara memori yang terpasang secara fisik dan memori yang terlihat oleh kernel saat sistem berjalan.

### Tools

- `dmidecode`
- `free`
- `/proc/meminfo`
- `lshw`

### Langkah

1. Periksa modul RAM yang terpasang:

```bash
sudo dmidecode -t memory
```

2. Ambil ringkasan memori yang lebih rapi:

```bash
sudo lshw -class memory
```

3. Periksa total memori saat runtime:

```bash
free -h
cat /proc/meminfo | less
```

4. Catat:

- jumlah slot DIMM
- ukuran modul
- total memori terpasang
- kecepatan memori jika tersedia
- total memori yang terlihat oleh kernel

5. Bandingkan memori terpasang dengan memori yang dapat dipakai, lalu jelaskan kemungkinan penyebab selisih.

### Output yang Diharapkan

Buat perbandingan antara memori fisik dan memori yang digunakan kernel.

---

## 6. Menafsirkan Penggunaan Memori Linux, Buffer, Cache, dan Available Memory

### Tujuan

Mahasiswa mampu membaca statistik memori Linux dengan benar dan tidak salah menafsirkan nilai "used".

### Tools

- `free`
- `vmstat`
- `/proc/meminfo`
- `sar` (opsional)

### Langkah

1. Lihat ringkasan memori:

```bash
free -h
free -h -w
```

2. Ambil field penting dari `/proc/meminfo`:

```bash
grep -E 'MemTotal|MemFree|MemAvailable|Buffers|Cached|SwapTotal|SwapFree' /proc/meminfo
```

3. Pantau aktivitas memori:

```bash
vmstat 1 5
```

4. Jelaskan arti field berikut:

- used
- free
- shared
- buffers
- cache
- available

5. Tulis kesimpulan: apakah sistem sedang benar-benar tertekan secara memori atau hanya memanfaatkan RAM dengan efisien.

### Output yang Diharapkan

Buat interpretasi yang benar tentang kondisi memori sistem.

---

## 7. Memetakan Perangkat Penyimpanan, Partisi, Filesystem, dan Mount Point

### Tujuan

Mahasiswa memahami hubungan antara disk fisik, partisi, filesystem, dan titik mount.

### Tools

- `lsblk`
- `blkid`
- `df`
- `mount`
- `findmnt`

### Langkah

1. Tampilkan struktur block device:

```bash
lsblk
lsblk -f
```

2. Tampilkan UUID dan metadata filesystem:

```bash
sudo blkid
```

3. Lihat filesystem yang sedang ter-mount:

```bash
df -hT
findmnt
mount | less
```

4. Identifikasi:

- nama disk
- nama partisi
- tipe filesystem
- mount point
- device untuk root filesystem

5. Gambar peta dari disk fisik sampai direktori mount.

### Output yang Diharapkan

Buat storage map yang jelas dari perangkat sampai mount point.

---

## 8. Memeriksa Perangkat PCI dan Arsitektur Bus yang Terlihat oleh Kernel

### Tujuan

Mahasiswa memahami bagaimana Linux mendeteksi perangkat internal melalui PCI atau PCIe dan mengaitkannya dengan driver kernel.

### Tools

- `lspci`
- `lspci -k`
- `lshw`

### Langkah

1. Daftar semua perangkat PCI:

```bash
lspci
```

2. Lihat detail driver dan modul kernel:

```bash
lspci -k
```

3. Ambil detail verbose untuk salah satu perangkat:

```bash
sudo lspci -vv
```

4. Identifikasi minimal lima perangkat PCI, misalnya:

- GPU atau VGA
- Ethernet controller
- SATA atau NVMe controller
- USB controller
- audio device

5. Pilih satu perangkat dan catat driver kernel yang sedang digunakan.

### Output yang Diharapkan

Buat inventaris perangkat PCI dan kaitkan dengan driver yang aktif.

---

## 9. Memeriksa Perangkat Jaringan dan Karakteristik Link

### Tujuan

Mahasiswa memahami perbedaan antara tampilan interface jaringan di level OS dan detail perangkat keras di level NIC.

### Tools

- `ip`
- `ethtool`
- `lshw -class network`
- `lspci -k`

### Langkah

1. Tampilkan interface jaringan:

```bash
ip link show
ip addr show
```

2. Lihat detail perangkat jaringan:

```bash
sudo lshw -class network
```

3. Periksa salah satu interface aktif:

```bash
sudo ethtool <interface>
sudo ethtool -i <interface>
```

4. Catat:

- nama interface
- MAC address
- driver
- status link
- speed
- duplex
- port type

5. Jelaskan bagaimana perangkat yang sama muncul di tampilan PCI dan tampilan interface jaringan.

### Output yang Diharapkan

Buat profil perangkat jaringan untuk minimal satu interface aktif.

---

## 10. Menyelidiki NUMA, Lokalitas CPU, dan Penempatan Resource

### Tujuan

Mahasiswa memahami apakah sistem menggunakan NUMA atau tidak, serta mengapa lokalitas memori penting pada sistem besar.

### Tools

- `lscpu`
- `numactl --hardware`
- `lstopo` atau `hwloc-ls` jika tersedia

### Langkah

1. Cek informasi NUMA:

```bash
lscpu | grep -i numa
```

2. Jika tersedia, periksa node NUMA:

```bash
numactl --hardware
```

3. Jika tersedia, visualisasikan topologi:

```bash
lstopo
```

4. Catat:

- jumlah node NUMA
- CPU pada tiap node
- ukuran memori per node

5. Jelaskan mengapa lokalitas penting untuk database, virtualisasi, dan aplikasi multi-thread.

### Output yang Diharapkan

Tentukan apakah sistem bersifat NUMA-aware atau lebih mirip UMA.

---

## 11. Menghubungkan Pesan Boot Kernel dengan Hardware yang Terdeteksi

### Tujuan

Mahasiswa mampu membaca `dmesg` sebagai jejak proses deteksi hardware oleh kernel.

### Tools

- `dmesg`
- `journalctl -k` (opsional)
- `grep`

### Langkah

1. Lihat ring buffer kernel:

```bash
dmesg | less
```

2. Cari kategori penting:

```bash
dmesg | grep -Ei 'cpu|memory|pci|usb|eth|nvme|sata|acpi'
```

3. Identifikasi pesan terkait:

- inisialisasi CPU
- deteksi dan reservasi memori
- enumerasi PCI
- deteksi storage controller
- pemuatan driver NIC

4. Pilih tiga pesan hardware dan jelaskan tahap inisialisasi yang diwakili.

### Output yang Diharapkan

Buat ringkasan proses inisialisasi hardware berdasarkan log kernel.

---

## 12. Menilai Kesehatan Storage dan Kemampuan Khusus Perangkat

### Tujuan

Mahasiswa memahami bahwa keberadaan storage saja tidak cukup, tetapi kondisi dan kesehatannya juga penting.

### Tools

- `lsblk`
- `smartctl`
- `nvme`
- `lspci`

### Langkah

1. Identifikasi storage utama:

```bash
lsblk -d -o NAME,MODEL,SIZE,ROTA,TYPE
```

2. Untuk SATA atau SAS, jika tersedia:

```bash
sudo smartctl -a /dev/sdX
```

3. Untuk NVMe, jika tersedia:

```bash
sudo nvme list
sudo nvme smart-log /dev/nvme0
```

4. Catat:

- model perangkat
- rotational atau non-rotational
- ringkasan health
- temperatur atau warning penting

5. Hubungkan perangkat tersebut dengan controllernya di `lspci`.

### Output yang Diharapkan

Buat snapshot kesehatan storage dan jenis teknologinya.

---

## 13. Menyusun Laporan Validasi Hardware Lintas Tools

### Tujuan

Mahasiswa mampu memvalidasi data perangkat keras dengan beberapa command, bukan hanya mengandalkan satu output.

### Tools

Gunakan kombinasi yang relevan dari:

- `lscpu`
- `lshw`
- `lspci`
- `dmidecode`
- `free`
- `/proc/meminfo`
- `lsblk`
- `blkid`
- `lsusb`
- `ip`
- `ethtool`
- `dmesg`

### Langkah

1. Buat tabel dengan kategori berikut:

- identitas sistem
- topologi CPU
- flag CPU
- hirarki cache
- memori fisik
- memori runtime
- swap
- peta storage
- perangkat PCI
- perangkat USB
- hardware jaringan
- event yang terdeteksi kernel

2. Untuk setiap kategori, kumpulkan bukti dari minimal dua command jika memungkinkan.

3. Tulis minimal tiga contoh ketika satu command menambah konteks dari command lain.

4. Tulis minimal satu inkonsistensi, ambiguitas, atau field yang hilang, lalu jelaskan penyebabnya.

5. Lampirkan screenshot atau capture terminal.

### Output yang Diharapkan

Laporan akhir harus menunjukkan bahwa kamu bisa:

- membaca hardware dari beberapa lapisan Linux
- membandingkan informasi firmware, kernel, dan device probing
- menjelaskan hasil, bukan hanya menyalin output

---

## Format Laporan yang Disarankan

Kamu bisa memakai struktur berikut:

```markdown
# Laporan Hands-on 1

## 1. Identitas Sistem

## 2. Profil Hardware Umum

## 3. CPU dan Topologi

## 4. Flag CPU dan Virtualisasi

## 5. Cache CPU

## 6. Memori Fisik vs Runtime

## 7. Interpretasi Penggunaan Memori

## 8. Storage, Partisi, dan Filesystem

## 9. Perangkat PCI

## 10. Jaringan

## 11. NUMA

## 12. Log Kernel Terkait Hardware

## 13. Kesehatan Storage

## 14. Validasi Lintas Tools

## 15. Kesimpulan
```

---

## Catatan Penting

- Jalankan command di Linux, WSL, atau VM Linux.
- Beberapa command membutuhkan `sudo`.
- Jika ada command yang tidak tersedia, tulis pada laporan bahwa tool belum terpasang.
- Jika sistem berjalan di virtual machine atau WSL, beberapa field seperti serial number, BIOS, atau info motherboard bisa kosong atau generik.
- Yang dinilai bukan hanya output command, tetapi kemampuan menjelaskan hasilnya.

