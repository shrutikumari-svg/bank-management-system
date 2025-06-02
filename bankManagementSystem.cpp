#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;

class Account {
private:
    long accountNumber;
    string firstname;
    string lastname;
    double balance;
    static long nextaccountnumber;
public:
    Account() : accountNumber(0), balance(0.0) {}
    Account(string fn, string ln, double bal);

    void Deposit(float amount);
    void Withdraw(float amount);
    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();
    void setFirstname(string fn);
    void setLastname(string ln);
    void setBalance(double bal);

    long getAccountNumber() const;
    string getFirstname() const;
    string getLastname() const;
    double getBalance() const;

    friend ofstream &operator<<(ofstream &ofs, const Account &acc);
    friend ifstream &operator>>(ifstream &ifs, Account &acc);
    friend ostream &operator<<(ostream &os, const Account &acc);
};

class Bank {
private:
    map<long, Account> accounts;

public:
    Bank();
    Account OpenAccount(string fn, string ln, float bal);
    Account BalanceEnquiry(long accountNumber);
    Account Deposit(long accountNumber, float amount);
    Account Withdraw(long accountNumber, float amount);
    void CloseAccount(long accountNumber);
    void DisplayAllAccounts();
    ~Bank();
};

int main() {
    Bank b;
    Account acc;
    int choice;
    long accountNumber;
    string fn, ln;
    float amount;
    float balance;
    cout << "***Banking System***" << endl;
    do {
        cout << endl << "Select One Option: " << endl;
        cout << "1. Open Account" << endl;
        cout << "2. Balance Enquiry" << endl;
        cout << "3. Deposit" << endl;
        cout << "4. Withdraw" << endl;
        cout << "5. Close Account" << endl;
        cout << "6. Display All Accounts" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Enter First Name: ";
                cin >> fn;
                cout << "Enter Last Name: ";
                cin >> ln;
                cout << "Enter Initial Balance: ";
                cin >> balance;
                acc = b.OpenAccount(fn, ln, balance);
                cout << endl << "Congratulations! Account is created" << endl;
                cout << "Account Number: " << acc.getAccountNumber() << endl;
                cout << acc;
                break;
            case 2:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                acc = b.BalanceEnquiry(accountNumber);
                if (acc.getAccountNumber() != 0) {
                    cout << endl << "Account Details: " << endl;
                    cout << acc;
                } else {
                    cout << "Account not found!" << endl;
                }
                break;
            case 3:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Amount to Deposit: ";
                cin >> amount;
                acc = b.Deposit(accountNumber, amount);
                if (acc.getAccountNumber() != 0) {
                    cout << endl << "Amount is deposited" << endl;
                    cout << acc;
                } else {
                    cout << "Account not found!" << endl;
                }
                break;
            case 4:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Amount to Withdraw: ";
                cin >> amount;
                acc = b.Withdraw(accountNumber, amount);
                if (acc.getAccountNumber() != 0) {
                    cout << endl << "Amount Withdrawn" << endl;
                    cout << acc;
                } else {
                    cout << "Account not found or insufficient balance!" << endl;
                }
                break;
            case 5:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                b.CloseAccount(accountNumber);
                cout << endl << "Account is Closed" << endl;
                break;
            case 6:
                b.DisplayAllAccounts();
                break;
            case 7:
                break;
            default:
                cout << "\nEnter correct choice" << endl;
                exit(0);
        }
    } while (choice != 7);
    return 0;
}

// Account implementation
Account::Account(string fn, string ln, double bal) : firstname(fn), lastname(ln), balance(bal) {
    accountNumber = ++nextaccountnumber;
}

void Account::Deposit(float amount) {
    balance += amount;
}

void Account::Withdraw(float amount) {
    if (balance >= amount) {
        balance -= amount;
    } else {
        cout << "Insufficient Balance" << endl;
    }
}

void Account::setLastAccountNumber(long accountNumber) {
    nextaccountnumber = accountNumber;
}

long Account::getLastAccountNumber() {
    return nextaccountnumber;
}

void Account::setFirstname(string fn) {
    firstname = fn;
}

void Account::setLastname(string ln) {
    lastname = ln;
}

void Account::setBalance(double bal) {
    balance = bal;
}

long Account::getAccountNumber() const {
    return accountNumber;
}

string Account::getFirstname() const {
    return firstname;
}

string Account::getLastname() const {
    return lastname;
}

double Account::getBalance() const {
    return balance;
}

ofstream &operator<<(ofstream &ofs, const Account &acc) {
    ofs << acc.accountNumber << endl;
    ofs << acc.firstname << endl;
    ofs << acc.lastname << endl;
    ofs << acc.balance << endl;
    return ofs;
}

ifstream &operator>>(ifstream &ifs, Account &acc) {
    ifs >> acc.accountNumber;
    ifs.ignore();
    getline(ifs, acc.firstname);
    getline(ifs, acc.lastname);
    ifs >> acc.balance;
    return ifs;
}

ostream &operator<<(ostream &os, const Account &acc) {
    os << "First Name: " << acc.firstname << endl;
    os << "Last Name: " << acc.lastname << endl;
    os << "Account Number: " << acc.accountNumber << endl;
    os << "Balance: $" << acc.balance << endl;
    return os;
}

long Account::nextaccountnumber = 0;

// Bank implementation
Bank::Bank() {
    Account account;
    ifstream infile("Bank.data");
    if (!infile) {
        cout << "Error in Opening! File Not Found!!" << endl;
        return;
    }
    while (infile >> account) {
        accounts[account.getAccountNumber()] = account;
        if (account.getAccountNumber() > Account::getLastAccountNumber()) {
            Account::setLastAccountNumber(account.getAccountNumber());
        }
    }
    infile.close();
    Account::setLastAccountNumber(Account::getLastAccountNumber() + 1);
}

Account Bank::OpenAccount(string fn, string ln, float bal) {
    ofstream outfile("Bank.data", ios::app);
    Account account(fn, ln, bal);
    accounts[account.getAccountNumber()] = account;
    for (const auto &pair : accounts) {
        outfile << pair.second;
    }
    outfile.close();
    return account;
}

Account Bank::BalanceEnquiry(long accountNumber) {
    auto itr = accounts.find(accountNumber);
    if (itr != accounts.end()) {
        return itr->second;
    }
    return Account(); // Return an empty account object if not found
}

Account Bank::Deposit(long accountNumber, float amount) {
    auto itr = accounts.find(accountNumber);
    if (itr != accounts.end()) {
        itr->second.Deposit(amount);
        return itr->second;
    }
    return Account(); // Return an empty account object if not found
}

Account Bank::Withdraw(long accountNumber, float amount) {
    auto itr = accounts.find(accountNumber);
    if (itr != accounts.end()) {
        itr->second.Withdraw(amount);
        return itr->second;
    }
    return Account(); // Return an empty account object if not found
}

void Bank::CloseAccount(long accountNumber) {
    auto itr = accounts.find(accountNumber);
    if (itr != accounts.end()) {
        cout << "Account Deleted" << endl << itr->second;
        accounts.erase(accountNumber);
        ofstream outfile("Bank.data", ios::trunc);
        for (const auto &pair : accounts) {
            outfile << pair.second;
        }
        outfile.close();
    } else {
        cout << "Account not found!" << endl;
    }
}

void Bank::DisplayAllAccounts() {
    for (const auto &pair : accounts) {
        cout << "Account " << pair.first << endl << pair.second << endl;
    }
}

Bank::~Bank() {
    ofstream outfile("Bank.data", ios::trunc);
    for (const auto &pair : accounts) {
        outfile << pair.second;
    }
    outfile.close();
}
