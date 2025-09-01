# JerryBoree

A Rick & Morty–themed C program that lets **Rick** (the user) manage and maintain copies of **Jerry** via a simple, menu-driven CLI.

---

## Quick Start

### Build

Use the provided `Makefile` to compile the project into an executable named **`JerryBoree`**:

```bash
make
```

### Run

Execute the program from your shell:

```bash
./JerryBoree
```

### Clean

Optionally remove intermediate object files generated during compilation:

```bash
make clean
```

---

## Overview

This project was developed in two phases to keep the implementation clear and maintainable:

1. **Data Structures** — Implement generic, reusable ADTs in C to support the application.
2. **Application Logic** — Implement the menu, input handling, and overall flow while keeping the system stable and predictable. The program is designed to exit cleanly and free resources even when unexpected errors occur.

All interactions happen in the terminal. The menu guides the user and is self-explanatory, so you can discover the available operations as you go.

---

## Core Data Structures

Because C lacks built-in generics, each ADT is **generic** via function pointers supplied to its constructor:

* `copy` — deep-copies an element
* `free` — releases an element
* `print` — prints an element
* `equals` — compares two elements for equality

This pattern provides type-agnostic containers similar to those in higher-level languages.

### 1) `LinkedList`

A generic singly linked list implementation.
Includes an additional capability to **compare elements partially** (beyond full equality), enabling flexible lookups and removals.

### 2) `KeyValuePair`

A lightweight, generic wrapper for `<key, value>` pairs used throughout the other containers.

### 3) `HashTable`

A generic hash table with a **custom hash function**.
Collisions are handled by **separate chaining**, i.e., an **array of `LinkedList`** buckets.

### 4) `MultiValueHashTable`

A hash table whose values are **linked lists**, allowing multiple values to be mapped to the same key.
Used to support **fast, many-to-one** lookups (e.g., finding all Jerries that share an attribute).

---

## What You Can Do (Menu Highlights)

* **Add Jerries** (the menu prompts for the needed details)
* **Remove Jerries**
* **Rank / manage Jerries**
* **Find the “saddest” Jerry**
* And more…

The menu is designed so that options are easy to follow and learn, and all operations are implemented with efficiency in mind.

---

## Design Notes

* **Generic ADTs**: Reusable containers via function pointers (`copy`, `free`, `print`, `equals`) keep the code modular and testable.
* **Efficiency**: Heavy use of appropriate data structures (hash tables, linked lists) provides fast lookups and updates.
* **Robustness**: Input handling and cleanup paths aim to keep the system stable; resources are freed on exit to avoid leaks.
