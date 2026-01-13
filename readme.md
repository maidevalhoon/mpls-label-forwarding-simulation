# MPLS Network Simulation (Assignment 5)

## Project Overview

This project implements a **basic MPLS (Multi‑Protocol Label Switching) network simulator** in **C++**. It models routers, routing tables, label assignment, and packet forwarding based on **Forwarding Equivalence Classes (FECs)**. The simulator demonstrates how packets traverse an MPLS-enabled network using labels instead of traditional IP lookups.

This project was developed as part of **CS342 – Computer Networks / Lab Assignment 5**.

---

## Key Concepts Implemented

* MPLS label assignment
* Forwarding Equivalence Class (FEC)
* Routing table construction
* Label Switching Router (LSR) behaviour
* End‑to‑end packet forwarding simulation

---

## Project Structure

```
.
├── mpls_sim.cpp          # Main source file (MPLS simulation)
├── output.txt            # Sample output of the program
├── Group_50_Assignment_5_report.pdf   # Detailed project report
└── README.md             # Project documentation
```

---

## Prerequisites

Make sure you have the following installed:

* **g++ compiler** (version 7 or higher)
* Linux / macOS / WSL environment

## How to Compile and Run

### Step 1: Compile the program

```bash
g++ mpls_sim.cpp -o mpls_sim
```

### Step 2: Run the executable

```bash
./mpls_sim
```

---

## Output

* The program prints routing tables for each router.
* It shows how labels are assigned and used for packet forwarding.
* A sample run output is provided in **`output.txt`** for reference.

---

## Example Output (Snippet)

```
Routing Table for Router R0:
Destination | Next Hop | Total Cost
...
```

---

## Notes

* The network topology is **hardcoded** in the source file for simplicity.
* No external libraries are used beyond the C++ STL.

---

## Authors

**Group 50**
CS342 – Assignment 5

---
