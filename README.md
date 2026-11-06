# Local History Oral Archive Index

Console-based C++ application (MCA, CAP455, Phase 1 - CA1) that keeps an in-memory catalogue of oral history
recordings, each with an 8-step archive indexing checklist.

Student: Vishal Singh | Reg. No: 12620661 | Roll No: RD1P2635B40

## Build and run
    g++ -std=c++17 -o oral_archive src/main.cpp
    ./oral_archive

## Features
Add record, view all, view details, view indexing checklist, search by keyword/narrator, total records, exit.
Records are stored only in memory (array of 30 objects); nothing is saved after exit.
