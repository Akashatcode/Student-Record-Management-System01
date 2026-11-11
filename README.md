# Student Record Management System (C)

A menu-driven console application written in C to store and manage student records using structures and binary file handling.

## Features

- Add a student record
- View all student records
- Search by student ID
- Update an existing record
- Delete a record
- Store records between program runs

## Student fields

Each record contains a student ID, name, age, course, and marks.

## Requirements

- A C compiler such as GCC
- Windows, Linux, or macOS terminal

## Compile

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c student.c -o student
```

## Run

Linux/macOS:

```bash
./student
```

Windows (MinGW):

```bash
student.exe
```

The program creates `students.dat` in the current working directory when the first record is added. This file is intentionally excluded from Git because it may contain personal information.

## Project structure

```text
Student-Record-Management-System/
├── main.c
├── student.c
├── student.h
├── README.md
├── .gitignore
└── docs/
    └── project-abstract.md
```

## Concepts demonstrated

- Structures
- Functions and header files
- File handling (`fopen`, `fread`, `fwrite`, `fseek`)
- Input validation
- Searching, updating, and deleting records

## Notes

This is an educational, local console project. It does not include user authentication, encryption, or a multi-user database. Do not store real sensitive student information in a public repository.

## Author

Add your name here.
