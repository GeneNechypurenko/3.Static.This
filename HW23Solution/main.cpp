#include <iostream>
#include <cstring>
#include <windows.h>

using namespace std;

/*
==========================================================================================================================================
                                    ДОМАШНЄ ЗАВДАННЯ 23. Статичні члени, покажчик this. Клас Account
==========================================================================================================================================
*/

// Рівень 1:
class Account
{
private:
    static int count;   // 1. Чому count і totalMoney статичні, а balance — ні?
    static int nextId;
    static double totalMoney;   // 1. Чому count і totalMoney статичні, а balance — ні?

    const int id;
    char* owner;
    double balance;    // 1. Чому count і totalMoney статичні, а balance — ні?

public:

    Account(const char* owner, double balance) : id(nextId++), owner(nullptr), balance(0)
    {
        setOwner(owner);

        if (balance > 0)
        {
            this->balance = balance;
        }

        count++;
        totalMoney += this->balance;

        cout << "  [+] Рахунок #" << id << " (" << this->owner << ")" << endl;
    }

    Account(const char* owner) : Account(owner, 0)  {}
    Account() : Account("Без імені", 0)  {}

    ~Account()
    {
        cout << "  [-] Рахунок #" << id << " (" << owner << ")" << endl;
        totalMoney -= balance;
        count--;
        delete[] owner;
    }

    void setOwner(const char* owner)    // 2. Навіщо в setOwner потрібен this-> ? Що станеться, якщо його прибрати ?
    {
        delete[] this->owner;
        int length = strlen(owner) + 1;
        this->owner = new char[length];
        strcpy_s(this->owner, length, owner);
    }

    double getBalance()
    {
        return balance;
    }

    int getId()
    {
        return id;
    }

    // Рівень 2:
    Account& deposit(double amount)     // 5. (Рівень 2) Чому deposit повертає Account&, а не Account?
    {
        if (amount > 0)
        {
            balance += amount;
            totalMoney += amount;
        }
        return *this;
    }

    Account& withdraw(double amount)
    {
        if (amount > 0 && amount <= balance)
        {
            balance -= amount;
            totalMoney -= amount;
        }
        else
        {
            cout << "  Відмова: недостатньо коштів" << endl;
        }
        return *this;
    }

    bool transfer(Account& to, double amount)
    {
        // 4. (Рівень 2) Що станеться, якщо в transfer прибрати перевірку if (this == &to)? Чи зміниться totalMoney після переказу? Чому?
        if (this == &to)    
        {
            cout << "  Відмова: переказ самому собі" << endl;
            return false;
        }

        if (amount <= 0 || amount > balance)
        {
            cout << "  Відмова: некоректна сума" << endl;
            return false;
        }

        balance -= amount;
        to.balance += amount;

        return true;
    }

    void print()
    {
        cout << "  #" << id << " " << owner << ": " << balance << " грн" << endl;
    }

    static int getCount()
    {
        return count;
    }

    static double getTotalMoney()
    {
        return totalMoney;
    }
};

int Account::count = 0;
int Account::nextId = 1001;
double Account::totalMoney = 0;

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "========== РІВЕНЬ 1 ==========" << endl;

    //   3. Чому getCount() можна викликати ще до створення будь-якого рахунку?
    cout << "Рахунків до початку: " << Account::getCount() << endl;

    Account a("Олена", 1000); 
    Account b("Ігор");
    Account c;

    a.deposit(500);
    a.withdraw(300);
    b.deposit(200);
    b.withdraw(1000);

    a.print();
    b.print();
    c.print();
    cout << "Рахунків: " << Account::getCount()
        << ", усього грошей: " << Account::getTotalMoney() << " грн" << endl;

    cout << endl << "========== РІВЕНЬ 2 ==========" << endl;

    a.deposit(500).withdraw(200).print();

    cout << "Успішний переказ 300 грн з a на b:" << endl;
    if (a.transfer(b, 300))
    {
        cout << "  Переказ виконано" << endl;
    }
    cout << "Переказ самому собі:" << endl;
    a.transfer(a, 100);
    cout << "Переказ завеликої суми:" << endl;
    b.transfer(c, 99999);

    a.print();
    b.print();
    cout << "Усього грошей після переказу: " << Account::getTotalMoney()
        << " грн (не змінилося)" << endl;

    cout << endl << "========== РІВЕНЬ 3 ==========" << endl;

    int countBefore = Account::getCount();
    double moneyBefore = Account::getTotalMoney();

    int n;
    cout << "Кількість рахунків у масиві: ";
    cin >> n;
    if (n < 1)
    {
        n = 1;
    }

    // Рівень 3:
    // 1. Для кожного елемента викликається конструктор за замовчуванням
    Account* accounts = new Account[n];

    // 2. Поповнюємо рахунки різними сумами: 100, 200, 300 ...
    for (int i = 0; i < n; i++)
    {
        accounts[i].deposit((i + 1) * 100.0);
    }

    // 3. Пошук рахунку з найбільшим балансом
    int maxIndex = 0;
    for (int i = 1; i < n; i++)
    {
        if (accounts[i].getBalance() > accounts[maxIndex].getBalance())
        {
            maxIndex = i;
        }
    }
    cout << "Найбільший баланс:" << endl;
    accounts[maxIndex].print();
    cout << "Рахунків з масивом: " << Account::getCount() << endl;

    // 4. delete[] викликає деструктор КОЖНОГО елемента (у зворотному порядку)
    delete[] accounts;

    cout << "Після delete[]: рахунків " << Account::getCount()
        << " (було " << countBefore << "), грошей " << Account::getTotalMoney()
        << " (було " << moneyBefore << ")" << endl;
    
    return 0;
}   // деструктори c, b, a — у зворотному порядку

/*

  1. Чому count і totalMoney статичні, а balance — ні?
  2. Навіщо в setOwner потрібен this->? Що станеться, якщо його прибрати?
  3. Чому getCount() можна викликати ще до створення будь-якого рахунку?
  4. (Рівень 2) Що станеться, якщо в transfer прибрати перевірку if(this == &to)? Чи зміниться totalMoney після переказу? Чому?
  5. (Рівень 2) Чому deposit повертає Account&, а не Account?

  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1. 

  2. 

  3. 

  4. 

  5. 
*/
