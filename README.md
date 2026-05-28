# Banking System Simulation in C

A comprehensive, terminal-based customer bank account management system implemented in C. Developed as a high-grade university project focusing on robust data structures and strict low-level file storage logic.

## Technical Core: True File-Based Persistence
Unlike typical academic projects that cache records in RAM, this system features **strict file-based persistence**. In compliance with production-like constraints:
* Data caching in memory is strictly forbidden.
* Records are read directly from and updated into the storage files (`db.dat` and `insurance.dat`) dynamically.
* A file update takes place immediately after every single modifying operation to guarantee data integrity and prevent data loss.

## Features
* **Account Creation**: Automatically generates unique account identifiers and securely stores customer personal information (Name, Surname, Address, and PESEL validation).
* **Transaction Engine**: Supports balance operations including deposits, withdrawals, and secure money transfers between two distinct accounts with bounds and overdrawn checking.
* **Car Insurance Module**: Allows clients to register an unlimited number of car insurance policies per account, linking registration numbers and policy pricing dynamically.
* **Search & Filter**: Comprehensive database queries filtering by account number, name, surname, address, or identification number.
* **Safety Context**: Built-in interactive confirmation prompts before any structural modifications to bank records.

## Compilation & Execution
The repository includes a strict `Makefile` using `-Wall`, `-Wextra`, and `-pedantic` flags to ensure compiler compliance and memory safety.

To compile and execute the project, run:
```bash
make && ./bank
