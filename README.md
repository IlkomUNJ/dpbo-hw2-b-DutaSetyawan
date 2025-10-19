[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/uAfN8jpt)
Name: Duta Setyawan

NIM : 13113624047

# E-Shop — HW DPBO

An interactive C++ console e-shop demo used for the DPBO assignment. It implements buyers, sellers, a simple bank, inventory management, cart & orders, and CSV persistence.

---

## Quick overview
- Language: C++ (C++20)
- Build: Meson + Ninja
- Scope: local console app, CSV-based persistence, single-process (no networking)

Due: October 20th, 23:55 AM — make sure you fill the Identity/Collaborators sections below before submission.

---

## Quick start (Windows PowerShell)
Ensure Meson and Ninja are installed (MSYS2 MinGW recommended).

```powershell
meson setup builddir
meson compile -C builddir
& "$(Resolve-Path builddir\my_app.exe)"
```

Tip: run from a terminal so you can interact with the menu.

---

## Features implemented
1. User registration & login for Buyers and Sellers.
2. Bank account per user: deposit, withdraw, and transaction history.
3. Upgrade Buyer → Seller flow (creates store metadata).
4. Seller inventory: add, remove, and view items.
5. Shopping cart, checkout, order creation, and payment flow (buyer pays seller).
6. Persistence: save/load Buyers, Sellers, Inventory, Orders to CSV (files in repo root).
7. Robust CSV parsing (safe numeric parsing to avoid crashes on malformed rows).
8. Developer reset option to clear saved data and memory for fresh testing.
9. Refactor: UI code moved to `ui.cpp`/`ui.h` and persistence logic to `persistence.cpp`/`persistence.h` for clarity.

---

##  Example happy path (what to try)
1. Run the app.
2. Register a new user (fill name, address, phone, email).
3. (Optional) Create seller account in the registration flow.
4. As a buyer: browse a store, add items to cart, checkout, then pay from your bank balance.
5. As a seller: add inventory, view paid orders.

The app prints Order IDs and status so you can confirm the flow.

---

##  Data files (location)
Files are created/updated in the project root:
- `data_buyers.csv`
- `data_sellers.csv`
- `data_inventory.csv`
- `data_orders.csv`

If you need to start from a clean slate, delete these files and use the "Reset All Data (DEV)" menu option.


# Collaborators (The person you learned from)
1. ..........

# Students (the person you taught to complete the assignments)
1. ...........

# AI Assistance
AI Tools: Github
Paid AI Plugin (state none, if not subscribed): none

Total Prompts used (int estimate): 12

Dominant prompt used for (seek solution / implement ideas) : Teach me how to ... and please make an example so i can get the picture of the idea