# AntiVirus Lab B (SPL)

A lightweight C-based virus detector and neutralizer built for the Systems Programming Lab assignment.

The program loads virus signatures from a binary signatures file, scans a suspected file with a naive byte-by-byte matching algorithm, and can neutralize detected signatures by patching their first byte to `RET` (`0xC3`).

---

## Features

- Load virus signatures from `VIRL` (little-endian) and `VIRB` (big-endian) files
- Store signatures in a dynamic linked list
- Print all loaded signatures in a readable format
- Detect viruses in suspected files (up to 10KB read buffer)
- Automatically fix detected viruses with file patching
- Menu-driven interface with clean memory deallocation on exit

---

## Menu

When running, the program provides:

1. Load signatures  
2. Print signatures  
3. Detect viruses  
4. Fix file  
5. AI analysis of file (stub)  
6. Quit

---

## Build

```bash
make clean
make
```

This builds the executable:

```bash
./AntiVirus
```

---

## Run With Valgrind

```bash
valgrind --leak-check=full --show-leak-kinds=all ./AntiVirus
```

Recommended: choose option `6` (`Quit`) to trigger full cleanup and verify leak-free execution.

---

## Typical Workflow

1. Run program
2. Select `1` and provide signatures file path
3. Select `3` and provide suspected file path to detect viruses
4. Select `4` and provide suspected file path to neutralize viruses
5. Select `6` to quit and free resources

---

## Project Files

- `AntiVirus.c` - full implementation
- `Makefile` - build rules

If your course checker requires lowercase, provide `makefile` in submission zip as well.

---

## Notes

- This project is for educational purposes (binary parsing, linked lists, memory management, and file patching).
- The detection algorithm is intentionally simple and not suitable for real-world malware analysis.