#ifndef BANKING_H
#define BANKING_H

#include <stdbool.h>
#include <stdio.h>

#define MAX_STR 50
#define PESEL_LEN 11
#define BUF_SIZE 64
#define DYN_BUF_SIZE 256
#define CONFIRM_BUF 16
#define MAX_TRANSACTION 10000000.0f

typedef struct {
    int id;
    char name[MAX_STR];
    char surname[MAX_STR];
    char address[MAX_STR];
    char pesel[MAX_STR];
    float balance;
    bool isActive;
} Account;

typedef struct {
    int userId;
    char registrationNo[MAX_STR];
    float amount;
    bool isActive;
} Insurance;

void initBankingSystem();
void printMenu();
void handleMenuChoice(int choice);

void registerAccount();
bool fillAccountDetails(Account* acc);

void executeDeposit();
void executeWithdrawal();
void executeTransfer();
bool validateTransfer(Account a1, Account a2, int id1, int id2);

void executeInsurance();
void searchMenu();
void performSearch(int type, int idTerm, const char* term);
bool matchesSearch(Account acc, int type, int idTerm, const char* term);

void listAllAccounts();

bool isNameValid(const char* str);
bool isPeselValid(const char* str);
bool isRegistrationValid(const char* str);
bool isAddressValid(const char* str);
bool confirmOperation();
void toLowerCase(char* str);

void clearInputBuffer();
bool readString(const char* message, char* buffer, int maxLen);
bool readFloat(const char* message, float* outVal);
bool readInt(const char* message, int* outVal);

FILE* openDatabase();
FILE* openInsuranceDatabase();
bool writeAccount(Account acc);
Account readAccount(int id);
int getNextFreeId();

bool writeInsurance(Insurance ins);
void printUserInsurances(int userId);
void printAccountInfo(Account acc);

#endif