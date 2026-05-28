#include "banking.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void initBankingSystem() {
    printMenu();
}

void printMenu() {
    int choice = -1;
    while (choice != 0) {
        printf("\n=== BANKING SYSTEM ===\n1. Register\n2. Deposit\n3. Withdrawal\n4. Transfer\n5. Insurance\n6. Search\n7. List all accounts\n0. Exit\n");
        if (readInt("Choose operation: ", &choice) && choice != 0) {
            handleMenuChoice(choice);
        }
    }
}

void handleMenuChoice(int choice) {
    if (choice == 1) registerAccount();
    else if (choice == 2) executeDeposit();
    else if (choice == 3) executeWithdrawal();
    else if (choice == 4) executeTransfer();
    else if (choice == 5) executeInsurance();
    else if (choice == 6) searchMenu();
    else if (choice == 7) listAllAccounts();
    else printf("Error: Invalid choice.\n");
}

FILE* openDatabase() {
    FILE* fp = fopen("db.dat", "rb+");
    if (fp == NULL) fp = fopen("db.dat", "wb+");
    return fp;
}

FILE* openInsuranceDatabase() {
    FILE* fp = fopen("insurance.dat", "rb+");
    if (fp == NULL) fp = fopen("insurance.dat", "wb+");
    return fp;
}

bool writeAccount(Account acc) {
    FILE* fp = openDatabase();
    if (fp == NULL) return false;
    long offset = (acc.id - 1) * sizeof(Account);
    fseek(fp, offset, SEEK_SET);
    if (fwrite(&acc, sizeof(Account), 1, fp) != 1) {
        printf("Error: Failed to write to database.\n");
        fclose(fp); return false;
    }
    fclose(fp); return true;
}

Account readAccount(int id) {
    Account acc = {0};
    FILE* fp = openDatabase();
    if (fp != NULL) {
        long offset = (id - 1) * sizeof(Account);
        fseek(fp, offset, SEEK_SET);
        if (fread(&acc, sizeof(Account), 1, fp) != 1) acc.isActive = false;
        fclose(fp);
    }
    return acc;
}

int getNextFreeId() {
    int id = 1; Account acc;
    FILE* fp = openDatabase();
    if (fp == NULL) return 1;
    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (!acc.isActive) break;
        id++;
    }
    fclose(fp); return id;
}

bool isNameValid(const char* str) {
    if (strlen(str) == 0) return false;
    for (int i = 0; str[i] != '\0'; i++) {
        if (isdigit((unsigned char)str[i])) return false;
    }
    return true;
}

bool isPeselValid(const char* str) {
    for (int i = 0; i < PESEL_LEN; i++) {
        if (!isdigit((unsigned char)str[i])) return false;
    }
    return true;
}

bool isRegistrationValid(const char* str) {
    int len = strlen(str);
    if (len < 4 || len > 16) return false;
    for (int i = 0; i < len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (!isalnum(c) && c != ' ' && c != '-') return false;
    }
    return true;
}

bool isAddressValid(const char* str) {
    return strlen(str) > 0;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

bool readString(const char* message, char* buffer, int maxLen) {
    char dyn[DYN_BUF_SIZE];
    printf("%s", message);
    if (fgets(dyn, sizeof(dyn), stdin) != NULL) {
        if (strchr(dyn, '\n') == NULL) clearInputBuffer();
        dyn[strcspn(dyn, "\n")] = '\0';
        if (strlen(dyn) >= maxLen - 1) {
            printf("Error: Input too long!\n"); return false;
        }
        strcpy(buffer, dyn); return true;
    }
    return false;
}

bool readFloat(const char* message, float* outVal) {
    char buf[BUF_SIZE]; char *endptr;
    if (!readString(message, buf, BUF_SIZE)) return false;
    for (int i = 0; buf[i] != '\0'; i++) {
        if (!isdigit((unsigned char)buf[i]) && buf[i] != '.') {
            printf("Error: Incorrect sign!\n"); return false;
        }
    }
    *outVal = strtof(buf, &endptr);
    if (*endptr != '\0') { printf("Error: Invalid format!\n"); return false; }
    if (*outVal > MAX_TRANSACTION) { printf("Error: Transaction limit exceeded!\n"); return false; }
    return true;
}

bool readInt(const char* message, int* outVal) {
    char buf[BUF_SIZE]; char *endptr;
    if (!readString(message, buf, BUF_SIZE)) return false;
    for (int i = 0; buf[i] != '\0'; i++) {
        if (!isdigit((unsigned char)buf[i])) {
            printf("Error: Incorrect sign!\n"); return false;
        }
    }
    *outVal = (int)strtol(buf, &endptr, 10);
    if (*endptr != '\0') return false;
    return true;
}

bool confirmOperation() {
    char buf[CONFIRM_BUF];
    printf("Confirm operation? (Y/N): ");
    if (fgets(buf, sizeof(buf), stdin) != NULL) {
        if (buf[0] == 'Y' || buf[0] == 'y') return true;
    }
    printf("Operation cancelled.\n");
    return false;
}

bool fillAccountDetails(Account* acc) {
    if (!readString("Enter Name: ", acc->name, MAX_STR)) return false;
    if (!isNameValid(acc->name)) { printf("Error: Incorrect sign!\n"); return false; }
    if (!readString("Enter Surname: ", acc->surname, MAX_STR)) return false;
    if (!isNameValid(acc->surname)) { printf("Error: Incorrect sign!\n"); return false; }
    if (!readString("Enter Address: ", acc->address, MAX_STR)) return false;
    if (!isAddressValid(acc->address)) { printf("Error: Incorrect sign!\n"); return false; }
    if (!readString("Enter PESEL: ", acc->pesel, MAX_STR)) return false;
    if (strlen(acc->pesel) != PESEL_LEN) { printf("Error: Incorrect length!\n"); return false; }
    if (!isPeselValid(acc->pesel)) { printf("Error: Incorrect sign!\n"); return false; }
    return true;
}

void registerAccount() {
    Account acc = {0};
    acc.id = getNextFreeId();
    if (!fillAccountDetails(&acc)) return;
    acc.isActive = true;
    if (writeAccount(acc)) {
        printf("Account registered! ID: %d\n", acc.id);
    }
}

void executeDeposit() {
    int id; float amount;
    if (!readInt("Enter Account ID: ", &id)) return;
    Account acc = readAccount(id);
    if (!acc.isActive || id <= 0) { printf("Error: Account not found!\n"); return; }
    if (!readFloat("Amount to deposit: ", &amount)) return;
    if (amount <= 0.0f) { printf("Error: Invalid amount!\n"); return; }
    if (!confirmOperation()) return;
    acc.balance += amount;
    if (writeAccount(acc)) printf("New balance: %.2f zl\n", acc.balance);
}

void executeWithdrawal() {
    int id; float amount;
    if (!readInt("Enter Account ID: ", &id)) return;
    Account acc = readAccount(id);
    if (!acc.isActive || id <= 0) { printf("Error: Account not found!\n"); return; }
    if (!readFloat("Amount to withdraw: ", &amount)) return;
    if (amount <= 0.0f) { printf("Error: Invalid amount!\n"); return; }
    if (acc.balance < amount) { printf("Error: Insufficient funds!\n"); return; }
    if (!confirmOperation()) return;
    acc.balance -= amount;
    if (writeAccount(acc)) printf("New balance: %.2f zl\n", acc.balance);
}

bool validateTransfer(Account a1, Account a2, int id1, int id2) {
    if (!a1.isActive || !a2.isActive || id1 <= 0 || id2 <= 0) {
        printf("Error: Account not found!\n"); return false;
    }
    if (id1 == id2) {
        printf("Error: Cannot transfer to yourself!\n"); return false;
    }
    return true;
}

void executeTransfer() {
    int idFrom, idTo; float amount;
    if (!readInt("From ID: ", &idFrom)) return;
    Account accFrom = readAccount(idFrom);
    if (!readInt("To ID: ", &idTo)) return;
    Account accTo = readAccount(idTo);
    if (!validateTransfer(accFrom, accTo, idFrom, idTo)) return;
    if (!readFloat("Amount to transfer: ", &amount)) return;
    if (amount <= 0.0f) { printf("Error: Invalid amount!\n"); return; }
    if (accFrom.balance < amount) { printf("Error: Insufficient funds!\n"); return; }
    if (!confirmOperation()) return;
    accFrom.balance -= amount; accTo.balance += amount;
    writeAccount(accFrom); writeAccount(accTo);
    printf("Transfer complete!\n");
}

bool writeInsurance(Insurance ins) {
    FILE* fp = openInsuranceDatabase();
    if (fp == NULL) return false;
    fseek(fp, 0, SEEK_END);
    if (fwrite(&ins, sizeof(Insurance), 1, fp) != 1) {
        printf("Error: Failed to write insurance.\n");
        fclose(fp); return false;
    }
    fclose(fp); return true;
}

void printUserInsurances(int userId) {
    Insurance ins; bool hasIns = false;
    FILE* fp = openInsuranceDatabase();
    if (fp == NULL) return;
    while (fread(&ins, sizeof(Insurance), 1, fp) == 1) {
        if (ins.isActive && ins.userId == userId) {
            if (!hasIns) { printf("Insurances:\n"); hasIns = true; }
            printf("- Reg No: %s, Yearly: %.2f zl\n", ins.registrationNo, ins.amount);
        }
    }
    fclose(fp);
}

void executeInsurance() {
    int id; float amount;
    if (!readInt("Enter Account ID: ", &id)) return;
    Account acc = readAccount(id);
    if (!acc.isActive || id <= 0) { printf("Error: Account not found!\n"); return; }
    Insurance ins = {0}; ins.userId = id;
    if (!readString("Reg Number: ", ins.registrationNo, MAX_STR)) return;
    if (!isRegistrationValid(ins.registrationNo)) { printf("Error: Invalid registration number!\n"); return; }
    if (!readFloat("Yearly payment: ", &amount)) return;
    if (amount <= 0.0f) { printf("Error: Invalid amount!\n"); return; }
    if (acc.balance < amount) { printf("Error: Insufficient funds!\n"); return; }
    if (!confirmOperation()) return;
    acc.balance -= amount; ins.isActive = true; ins.amount = amount;
    if (writeInsurance(ins)) { writeAccount(acc); printf("Insurance registered!\n"); }
}

void printAccountInfo(Account acc) {
    printf("\n--- Account Info ---\nID: %d\nName: %s %s\n", acc.id, acc.name, acc.surname);
    printf("Address: %s\nPESEL: %s\nBalance: %.2f zl\n", acc.address, acc.pesel, acc.balance);
    printUserInsurances(acc.id);
    printf("--------------------\n");
}

bool matchesSearch(Account acc, int type, int idTerm, const char* term) {
    if (type == 1 && acc.id == idTerm) return true;
    char aName[MAX_STR], aSur[MAX_STR], aAddr[MAX_STR], tLow[MAX_STR];
    strcpy(aName, acc.name); strcpy(aSur, acc.surname); strcpy(aAddr, acc.address);
    toLowerCase(aName); toLowerCase(aSur); toLowerCase(aAddr);
    strcpy(tLow, term); toLowerCase(tLow);
    if (type == 2 && strstr(aName, tLow) != NULL) return true;
    if (type == 3 && strstr(aSur, tLow) != NULL) return true;
    if (type == 4 && strstr(aAddr, tLow) != NULL) return true;
    if (type == 5 && strcmp(acc.pesel, term) == 0) return true;
    return false;
}

void performSearch(int type, int idTerm, const char* term) {
    FILE* fp = openDatabase();
    if (fp == NULL) return;
    Account acc; bool found = false;
    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (acc.isActive && matchesSearch(acc, type, idTerm, term)) {
            printAccountInfo(acc); found = true;
        }
    }
    fclose(fp);
    if (!found) printf("No matching accounts found.\n");
}

void searchMenu() {
    printf("\n--- Search By ---\n1. ID\n2. Name\n3. Surname\n4. Address\n5. PESEL\n");
    int type, idTerm = -1; char term[MAX_STR] = "";
    if (!readInt("Choose field: ", &type) || type < 1 || type > 5) {
        printf("Error: Invalid choice.\n"); return;
    }
    if (type == 1) {
        if (!readInt("Enter ID: ", &idTerm)) return;
    } else {
        if (!readString("Enter term: ", term, MAX_STR)) return;
    }
    performSearch(type, idTerm, term);
}

void listAllAccounts() {
    FILE* fp = openDatabase();
    if (fp == NULL) { printf("Database is empty.\n"); return; }
    Account acc; bool found = false;
    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (acc.isActive) { printAccountInfo(acc); found = true; }
    }
    fclose(fp);
    if (!found) printf("No accounts found.\n");
}