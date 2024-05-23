#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <stdexcept>

// Класс представления провайдера услуг
class Provider {
public:
    Provider(const std::string& name, const std::string& billType)
        : name(name), billType(billType) {}

    std::string getName() const {
        return name;
    }

    std::string getBillType() const {
        return billType;
    }

private:
    std::string name;
    std::string billType;
};

// Базовый класс представления счета за коммунальные услуги
class Bill {
public:
    Bill(const std::shared_ptr<Provider>& provider, double amount)
        : provider(provider), amount(amount) {}

    virtual void display() const {
        std::cout << "Provider: " << provider->getName() << " | Amount: $" << amount;
    }

    double getAmount() const {
        return amount;
    }

    virtual ~Bill() = default;

protected:
    std::shared_ptr<Provider> provider;
    double amount;
};

// Класс представления счета за электроэнергию
class ElectricityBill : public Bill {
public:
    ElectricityBill(const std::shared_ptr<Provider>& provider, double amount)
        : Bill(provider, amount) {}

    void display() const override {
        std::cout << "Electricity ";
        Bill::display();
        std::cout << std::endl;
    }
};

// Класс представления счета за воду
class WaterBill : public Bill {
public:
    WaterBill(const std::shared_ptr<Provider>& provider, double amount)
        : Bill(provider, amount) {}

    void display() const override {
        std::cout << "Water ";
        Bill::display();
        std::cout << std::endl;
    }
};

// Класс представления счета за газ
class GasBill : public Bill {
public:
    GasBill(const std::shared_ptr<Provider>& provider, double amount)
        : Bill(provider, amount) {}

    void display() const override {
        std::cout << "Gas ";
        Bill::display();
        std::cout << std::endl;
    }
};

// Шаблонный класс представления платежа
template <typename T>
class Payment {
public:
    Payment(T amount, const std::string& date)
        : amount(amount), date(date) {}

    void display() const {
        std::cout << "Payment Amount: $" << amount << " | Date: " << date << std::endl;
    }

private:
    T amount;
    std::string date;
};

// Класс представления пользователя
class User {
public:
    User(const std::string& name) : name(name) {}

    std::string getName() const {
        return name;
    }

    void addBill(const std::shared_ptr<Bill>& bill) {
        bills.push_back(bill);
    }

    void removeBill(size_t index) {
        if (index < bills.size()) {
            bills.erase(bills.begin() + index);
            std::cout << "Bill removed successfully!" << std::endl;
        }
        else {
            throw std::runtime_error("Invalid Bill Index");
        }
    }

    template <typename T>
    void addPayment(const std::shared_ptr<Payment<T>>& payment) {
        payments.push_back(payment);
    }

    void displayBills() const {
        std::cout << "Bills for user: " << name << std::endl;
        for (size_t i = 0; i < bills.size(); ++i) {
            std::cout << i << ". ";
            bills[i]->display();
        }
    }

    void displayPayments() const {
        std::cout << "Payments for user: " << name << std::endl;
        for (const auto& payment : payments) {
            payment->display();
        }
    }

private:
    std::string name;
    std::vector<std::shared_ptr<Bill>> bills;
    std::vector<std::shared_ptr<Payment<double>>> payments;
};

// Класс для управления пользователями
class UserManager {
public:
    void addUser(const std::string& name) {
        users[userId++] = std::make_shared<User>(name);
        std::cout << "User added successfully!" << std::endl;
    }

    void removeUser(int id) {
        if (users.erase(id)) {
            std::cout << "User removed successfully!" << std::endl;
        }
        else {
            throw std::runtime_error("Invalid User ID");
        }
    }

    std::shared_ptr<User> getUser(int id) const {
        if (users.find(id) != users.end()) {
            return users.at(id);
        }
        throw std::runtime_error("Invalid User ID");
    }

    void displayUsers() const {
        for (const auto& userEntry : users) {
            std::cout << "ID: " << userEntry.first << " | " << userEntry.second->getName() << std::endl;
        }
    }

private:
    std::unordered_map<int, std::shared_ptr<User>> users;
    static int userId;
};

int UserManager::userId = 1;

// Класс для управления провайдерами услуг
class ProviderManager {
public:
    void addProvider(const std::string& name, const std::string& billType) {
        providers[providerId++] = std::make_shared<Provider>(name, billType);
        std::cout << "Provider added successfully!" << std::endl;
    }

    void removeProvider(int id) {
        if (providers.erase(id)) {
            std::cout << "Provider removed successfully!" << std::endl;
        }
        else {
            throw std::runtime_error("Invalid Provider ID");
        }
    }

    std::shared_ptr<Provider> getProvider(int id) const {
        if (providers.find(id) != providers.end()) {
            return providers.at(id);
        }
        throw std::runtime_error("Invalid Provider ID");
    }

    void displayProviders() const {
        for (const auto& providerEntry : providers) {
            std::cout << "ID: " << providerEntry.first << " | " << providerEntry.second->getName()
                << " | Bill Type: " << providerEntry.second->getBillType() << std::endl;
        }
    }

private:
    std::unordered_map<int, std::shared_ptr<Provider>> providers;
    static int providerId;
};

int ProviderManager::providerId = 1;

// Класс для управления счетами
class BillManager {
public:
    void createBill(const std::shared_ptr<User>& user, const std::shared_ptr<Provider>& provider, double amount) {
        std::shared_ptr<Bill> bill;
        if (provider->getBillType() == "electricity") {
            bill = std::make_shared<ElectricityBill>(provider, amount);
        }
        else if (provider->getBillType() == "water") {
            bill = std::make_shared<WaterBill>(provider, amount);
        }
        else if (provider->getBillType() == "gas") {
            bill = std::make_shared<GasBill>(provider, amount);
        }
        else {
            throw std::runtime_error("Invalid bill type!");
        }
        user->addBill(bill);
        std::cout << "Bill created successfully!" << std::endl;
    }
};

// Класс представления администратора
class Admin {
public:
    Admin(UserManager& userManager, ProviderManager& providerManager, BillManager& billManager)
        : userManager(userManager), providerManager(providerManager), billManager(billManager) {}

    void addProvider(const std::string& name, const std::string& billType) {
        providerManager.addProvider(name, billType);
    }

    void removeProvider(int id) {
        providerManager.removeProvider(id);
    }

    void addUser(const std::string& name) {
        userManager.addUser(name);
    }

    void removeUser(int id) {
        userManager.removeUser(id);
    }

    void createBill(const std::shared_ptr<User>& user, const std::shared_ptr<Provider>& provider, double amount) {
        billManager.createBill(user, provider, amount);
    }

    void removeBill(const std::shared_ptr<User>& user, size_t billIndex) {
        user->removeBill(billIndex);
    }

    void displayProviders() const {
        providerManager.displayProviders();
    }

    void displayUsers() const {
        userManager.displayUsers();
    }

    UserManager& getUserManager() {
        return userManager;
    }

    ProviderManager& getProviderManager() {
        return providerManager;
    }

private:
    UserManager& userManager;
    ProviderManager& providerManager;
    BillManager& billManager;
};

// Класс для отображения и обработки меню
class Menu {
public:
    Menu(Admin& admin) : admin(admin) {}

    void showMenu() const {
        std::cout << "1. Add Provider\n";
        std::cout << "2. Remove Provider\n";
        std::cout << "3. Add User\n";
        std::cout << "4. Remove User\n";
        std::cout << "5. Create Bill\n";
        std::cout << "6. Remove Bill\n";
        std::cout << "7. Display Bills for a User\n";
        std::cout << "8. Add Payment\n";
        std::cout << "9. Display Payments for a User\n";
        std::cout << "10. Exit\n";
        std::cout << "Enter your choice: ";
    }

    void handleChoice(int choice) {
        try {
            switch (choice) {
            case 1:
                handleAddProvider();
                break;
            case 2:
                handleRemoveProvider();
                break;
            case 3:
                handleAddUser();
                break;
            case 4:
                handleRemoveUser();
                break;
            case 5:
                handleCreateBill();
                break;
            case 6:
                handleRemoveBill();
                break;
            case 7:
                handleDisplayBills();
                break;
            case 8:
                handleAddPayment();
                break;
            case 9:
                handleDisplayPayments();
                break;
            case 10:
                std::cout << "Exiting program." << std::endl;
                exit(0);
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

private:
    void handleAddProvider() {
        std::string providerName, billType;
        std::cout << "Enter provider name: ";
        std::cin.ignore(); // Игнорируем предыдущий ввод
        std::getline(std::cin, providerName);
        std::cout << "Enter bill type (electricity/water/gas): ";
        std::getline(std::cin, billType);
        admin.addProvider(providerName, billType);
    }

    void handleRemoveProvider() {
        int providerId;
        std::cout << "Enter Provider ID to remove: ";
        std::cin >> providerId;
        admin.removeProvider(providerId);
    }

    void handleAddUser() {
        std::string userName;
        std::cout << "Enter user name: ";
        std::cin.ignore(); // Игнорируем предыдущий ввод
        std::getline(std::cin, userName);
        admin.addUser(userName);
    }

    void handleRemoveUser() {
        int userId;
        std::cout << "Enter User ID to remove: ";
        std::cin >> userId;
        admin.removeUser(userId);
    }

    void handleCreateBill() {
        int selectedProviderId, selectedUserId;
        double amount;

        std::cout << "Select Provider ID from the list below:" << std::endl;
        admin.displayProviders();
        std::cin >> selectedProviderId;

        auto provider = admin.getProviderManager().getProvider(selectedProviderId);

        std::cout << "Select User ID from the list below:" << std::endl;
        admin.displayUsers();
        std::cin >> selectedUserId;

        auto user = admin.getUserManager().getUser(selectedUserId);

        std::cout << "Enter bill amount: ";
        std::cin >> amount;

        admin.createBill(user, provider, amount);
    }

    void handleRemoveBill() {
        int userId;
        size_t billIndex;

        std::cout << "Select User ID to remove a bill from:" << std::endl;
        admin.displayUsers();
        std::cin >> userId;

        auto user = admin.getUserManager().getUser(userId);

        user->displayBills();

        std::cout << "Enter Bill Index to remove: ";
        std::cin >> billIndex;

        admin.removeBill(user, billIndex);
    }

    void handleDisplayBills() {
        int selectedUserId;
        std::cout << "Select User ID to display bills:" << std::endl;
        admin.displayUsers();
        std::cin >> selectedUserId;

        auto user = admin.getUserManager().getUser(selectedUserId);

        user->displayBills();
    }

    void handleAddPayment() {
        int selectedUserId;
        double amount;
        std::string date;

        std::cout << "Select User ID to add payment:" << std::endl;
        admin.displayUsers();
        std::cin >> selectedUserId;

        auto user = admin.getUserManager().getUser(selectedUserId);

        std::cout << "Enter payment amount: ";
        std::cin >> amount;

        std::cout << "Enter payment date (YYYY-MM-DD): ";
        std::cin.ignore(); // Игнорируем предыдущий ввод
        std::getline(std::cin, date);

        auto payment = std::make_shared<Payment<double>>(amount, date);
        user->addPayment(payment);
        std::cout << "Payment added successfully!" << std::endl;
    }

    void handleDisplayPayments() {
        int selectedUserId;
        std::cout << "Select User ID to display payments:" << std::endl;
        admin.displayUsers();
        std::cin >> selectedUserId;

        auto user = admin.getUserManager().getUser(selectedUserId);

        user->displayPayments();
    }

    Admin& admin;
};

int main() {
    UserManager userManager;
    ProviderManager providerManager;
    BillManager billManager;
    Admin admin(userManager, providerManager, billManager);
    Menu menu(admin);

    int choice;
    while (true) {
        menu.showMenu();
        std::cin >> choice;
        menu.handleChoice(choice);
    }

    return 0;
}
