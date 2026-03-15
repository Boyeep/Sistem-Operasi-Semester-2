# Practice

This folder contains small practice programs based on the module topics:

- Introduction to command line and navigation
- Shell scripting
- Cron jobs
- AWK

These examples are written for Linux or WSL. If you open this repository from Windows, run the scripts in:

- WSL Ubuntu
- Git Bash
- Any Linux terminal

## Folder Structure

- `shell/` - basic shell scripting examples
- `cron/` - cron job examples and a simple scheduled script
- `awk/` - AWK practice files and sample data
- `tasks/` - assignment questions and sample answers

## How To Run

Give execute permission first:

```bash
chmod +x Practice/shell/*.sh
chmod +x Practice/cron/*.sh
```

Run an example:

```bash
bash Practice/shell/01_simple_script.sh
```

Run a script with input:

```bash
bash Practice/shell/03_input_output.sh
```

Run an AWK example:

```bash
awk -f Practice/awk/01_print_fields.awk Practice/awk/students.txt
```

## Topic Map

### 1. Introduction

- Command and navigation:
  - `pwd`
  - `ls`
  - `cd`
  - `mkdir`
  - `cp`
  - `mv`
  - `rm`
- User and permission concept:
  - `whoami`
  - `id`
  - `ls -l`
  - `chmod`
- Text editor in CLI:
  - `nano`
  - `vim`

### 2. Shell Scripting

- `01_simple_script.sh` - simple shell script
- `02_variables.sh` - variables
- `03_input_output.sh` - input and output
- `04_quoting.sh` - single quotes, double quotes, escaping
- `05_operators.sh` - arithmetic and comparison operators
- `06_conditional.sh` - if, elif, else
- `07_looping.sh` - for and while loops
- `08_function.sh` - functions
- `09_file_tools.sh` - combines commands into one useful mini program
- `10_arguments.sh` - command line arguments
- `11_case_menu.sh` - `case` statement example
- `12_file_check.sh` - check whether a file or directory exists
- `13_mini_project.sh` - mini project that combines input, condition, loop, and function

### 3. Cron Jobs

- `backup_logs.sh` - a script that can be scheduled with cron
- `crontab_examples.txt` - ready-to-study cron expressions
- `cleanup_temp.sh` - example cleanup script for scheduled execution

### 4. AWK

- `students.txt` - sample data
- `01_print_fields.awk` - print selected fields
- `02_begin_end.awk` - use `BEGIN` and `END`
- `03_filter_and_average.awk` - filter data and calculate average
- `04_department_count.awk` - count students per department
- `05_pass_fail.awk` - label pass and fail status

## Suggested Practice Order

1. Read the shell scripts from top to bottom.
2. Run each script and change some values.
3. Study the cron expressions.
4. Run the AWK programs using `students.txt`.
5. Modify one script and create your own version.

## Extra Run Examples

Command line arguments:

```bash
bash Practice/shell/10_arguments.sh Lina 90
```

Check a file or directory:

```bash
bash Practice/shell/12_file_check.sh Practice/awk/students.txt
```

Run the mini project:

```bash
bash Practice/shell/13_mini_project.sh
```

Count departments with AWK:

```bash
awk -f Practice/awk/04_department_count.awk Practice/awk/students.txt
```

## Notes

- These files are intentionally simple so they are easy to explain in class or submit as practice.
- You can extend them later with command line arguments, file input, and more validation.

## Tasks

- Read [tasks/questions.md](./tasks/questions.md) for 10 short exercises.
- Sample solutions are available in `tasks/answers/`.
