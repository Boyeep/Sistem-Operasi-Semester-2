# Explanation Pre-Test Sistem Operasi

!Caution (Pilihan Jawaban Belum Tentu Benar)

## Soal 1
![Soal 1](./Soal%201.png)

Penjelasan: Inode menyimpan metadata file seperti nomor inode, mode/permission, dan user ID. Pada materi dasar sistem file, `Device Identifier` biasanya bukan informasi inti yang dimaksud sebagai metadata inode biasa, sehingga itulah pilihan yang dianggap bukan bagian utama dari isi inode.

## Soal 2
![Soal 2](./Soal%202.png)

Penjelasan: `vim` paling cocok karena editor ini berbasis keyboard, punya mode insert dan command, bisa menjalankan perintah shell, dan sangat mudah diperluas.

## Soal 3
![Soal 3](./Soal%203.png)

Penjelasan: Saat mengetik perintah di terminal, kita berinteraksi dengan `shell`. Shell menerima input pengguna lalu meneruskannya ke kernel bila perlu.

## Soal 4
![Soal 4](./Soal%204.png)

Penjelasan: `cron` atau `cron jobs` dipakai untuk menjalankan skrip secara otomatis pada waktu yang sudah dijadwalkan.

## Soal 5
![Soal 5](./Soal%205.png)

Penjelasan: Message queue umumnya memakai prinsip `FIFO` atau `First In First Out`, artinya pesan yang masuk lebih dulu akan diproses lebih dulu.

## Soal 6
![Soal 6](./Soal%206.png)

Penjelasan: Output yang muncul adalah `Dimana kita? pwd` karena `dmn='pwd'` memakai tanda petik tunggal, jadi isi variabel adalah teks literal `pwd`, bukan hasil eksekusi perintah `pwd`.

## Soal 7
![Soal 7](./Soal%207.png)

Penjelasan: `IPC` adalah singkatan dari `Interprocess Communication`, yaitu mekanisme komunikasi antar proses.

## Soal 8
![Soal 8](./Soal%208.png)

Penjelasan: `top -H` menampilkan thread yang sedang berjalan. Opsi `-H` membuat `top` menampilkan level thread, bukan hanya proses.

## Soal 9
![Soal 9](./Soal%209.png)

Penjelasan: `Thread` bukan kategori IPC. IPC biasanya mencakup pipe, message queue, shared memory, dan semaphore.

## Soal 10
![Soal 10](./Soal%2010.png)

Penjelasan: `pstree` menampilkan proses dalam bentuk hierarki pohon, sedangkan `ps` menampilkan daftar proses biasa. Jadi perbedaan utamanya ada pada bentuk penyajiannya.

## Soal 11
![Soal 11](./Soal%2011.png)

Penjelasan: `awk` digunakan untuk mengambil record tertentu dari file lalu memprosesnya. Tool ini memang dirancang untuk pemrosesan teks berbasis kolom dan pola.

## Soal 12
![Soal 12](./Soal%2012.png)

Penjelasan: Akses elemen array Bash memakai bentuk `${kelas_sisop[2]}`. Sintaks ini mengambil isi indeks ke-2 dari array.

## Soal 13
![Soal 13](./Soal%2013.png)

Penjelasan: `UID` menunjukkan identitas user yang menjalankan proses. Jadi UID dipakai untuk mengetahui proses itu berjalan atas nama user siapa.

## Soal 14
![Soal 14](./Soal%2014.png)

Penjelasan: `poll()` dipakai untuk menunggu file descriptor siap melakukan operasi I/O. Fungsi ini umum pada asynchronous atau event-driven programming.

## Soal 15
![Soal 15](./Soal%2015.png)

Penjelasan: `clear` bukan fungsi callback utama yang disediakan FUSE. Fungsi seperti `getattr`, `write`, `unlink`, dan `mkdir` memang ada dalam operasi filesystem FUSE.

## Soal 16
![Soal 16](./Soal%2016.png)

Penjelasan: Fungsi utama `VFS` adalah menyediakan antarmuka umum antara system call dan berbagai implementasi filesystem yang sebenarnya.

## Soal 17
![Soal 17](./Soal%2017.png)

Penjelasan: `umask(0)` mengosongkan file mode creation mask agar daemon tidak dibatasi mask lama dan dapat mengatur permission file baru dengan jelas.

## Soal 18
![Soal 18](./Soal%2018.png)

Penjelasan: `ps aux` dipakai untuk melihat proses yang sedang berjalan secara detail, termasuk user, PID, CPU, dan penggunaan memori.

## Soal 19
![Soal 19](./Soal%2019.png)

Penjelasan: Cara memastikan hanya satu thread mengakses resource pada satu waktu adalah `mutual exclusion`, biasanya dengan mutex atau lock.

## Soal 20
![Soal 20](./Soal%2020.png)

Penjelasan: Perintah `kill` menghentikan atau mengendalikan proses dengan mengirimkan signal tertentu, misalnya `SIGTERM` atau `SIGKILL`.

## Soal 21
![Soal 21](./Soal%2021.png)

Penjelasan: Kelemahan pipe yang paling umum adalah komunikasi dasarnya satu arah. Untuk dua arah biasanya dibutuhkan dua pipe atau mekanisme lain.

## Soal 22
![Soal 22](./Soal%2022.png)

Penjelasan: Secara teknis `Ctrl + Z` mengirim `SIGTSTP`, yaitu signal untuk suspend dari terminal. Jika mengikuti opsi pada gambar, `SIGSTOP` adalah pilihan yang paling mendekati, tetapi nama signal yang tepat adalah `SIGTSTP`.

## Soal 23
![Soal 23](./Soal%2023.png)

Penjelasan: `SIGSTART` bukan signal standar Unix/Linux untuk menghentikan proses. Signal yang umum dipakai justru `SIGTERM`, `SIGKILL`, `SIGINT`, dan sebagainya.

## Soal 24
![Soal 24](./Soal%2024.png)

Penjelasan: Operator `>>` digunakan untuk menambahkan isi ke akhir file tanpa menimpa isi lama. Kalau memakai `>`, isi file akan diganti.

## Soal 25
![Soal 25](./Soal%2025.png)

Penjelasan: Library thread yang umum dipakai di C pada sistem Unix/Linux adalah `pthread.h`, bagian dari POSIX Threads.

## Soal 26
![Soal 26](./Soal%2026.png)

Penjelasan: Fungsi `join` dipakai untuk menunggu thread lain selesai. Jadi thread pemanggil akan sinkron dengan thread yang ditunggu.

## Soal 27
![Soal 27](./Soal%2027.png)

Penjelasan: `fork()` membuat proses baru yang merupakan salinan dari proses pemanggil. Setelah itu parent dan child berjalan sebagai dua proses berbeda.

## Soal 28
![Soal 28](./Soal%2028.png)

Penjelasan: `getattr` pada FUSE dipakai untuk mengambil atribut file, misalnya mode, ukuran, link count, dan waktu akses.

## Soal 29
![Soal 29](./Soal%2029.png)

Penjelasan: File descriptor standar ditutup agar daemon lepas dari terminal dan tidak memakai resource terminal, sehingga bisa berjalan di background dengan benar.

## Soal 30
![Soal 30](./Soal%2030.png)

Penjelasan: `su` adalah command untuk berpindah user, termasuk menjadi superuser. Karena itu `su` sering dipakai untuk masuk sebagai `root`.

## Soal 31
![Soal 31](./Soal%2031.png)

Penjelasan: Perintah Bash untuk menduplikasi file adalah `cp`, singkatan dari copy.

## Soal 32
![Soal 32](./Soal%2032.png)

Penjelasan: `Multiprocess` berarti membuat banyak proses, sedangkan `multithreading` berarti membuat banyak thread di dalam satu proses yang sama.

## Soal 33
![Soal 33](./Soal%2033.png)

Penjelasan: Untuk unmount FUSE digunakan `fusermount -u [direktori]`. Itu adalah perintah standar untuk melepas mount point FUSE.

## Soal 34
![Soal 34](./Soal%2034.png)

Penjelasan: `OpenFS` bukan nama filesystem umum yang lazim seperti `ext3`, `ext4`, `NTFS`, atau `exFAT`, sehingga itulah pilihan pengecualiannya.

## Soal 35
![Soal 35](./Soal%2035.png)

Penjelasan: Dalam Bash, sintaks untuk mengambil input dari keyboard adalah `read`. Perintah ini menyimpan input pengguna ke variabel shell.
