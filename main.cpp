#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Commodity;
class Store;


string readWholeLine() {
    string input;
    cin.get();
    getline(cin, input);
    return input;
}

bool isNum(string& str) {
    for (int i = 0; i < str.size(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

class Commodity {
protected:
    int price;
    string description;
    string commodityName;

public:
    Commodity(int price, string commodityName, string description) {
        this->price = price;
        this->commodityName = commodityName;
        this->description = description;
    }

    void detail() {
        cout << commodityName << endl;
        cout << "price: " << price << endl;
        cout << "description: " << description << endl;
        cout << "----------------------------" << endl;
    }

    string getName() {
        return commodityName;
    }
};

class CommodityList {
private:
    vector<Commodity> commodities;

public:
    void showCommoditiesDetail() {
        for (int i = 0; i < commodities.size(); i++) {
            cout << i + 1 << ". ";
            commodities[i].detail();
        }
    }

    void showCommoditiesName() {
        for (int i = 0; i < commodities.size(); i++) {
            cout << i + 1 << ". " << commodities[i].getName() << endl;
        }
    }

    bool empty() {
        return commodities.empty();
    }

    int size() {
        return (int)commodities.size();
    }

    Commodity get(int index) {
        return commodities[index];
    }

    void add(Commodity* newCommodity) {
        commodities.push_back(*newCommodity);
    }

    void remove(int index) {
        commodities.erase(commodities.begin() + index);
    }
};

class ShoppingCart {
private:
    vector<Commodity> content;

public:
    void push(Commodity entry) {
        content.push_back(entry);
    }
};

class Store {
private:
    enum UMode {USER, MANAGER} userStatus;
    enum SMode {OPENING, DECIDING, SHOPPING, CART_CHECKING, CHECK_OUT, MANAGING, CLOSE} storeStatus;
    CommodityList commodityList;
    ShoppingCart cart;

    bool isValidNum(string& str) {
        if (isNum(str) && stoi(str) <= 0)
            return false;
        return true;
    }

    int numberInput() {
        string input;
        cin >> input;
        while (!isValidNum(input)) {
            cout << "Please input again your input is NOT an integer or is lower than or equal to 0:" << endl;
            cin >> input;
        }
        return stoi(input);
    }

    int inputCheck(string input, int maxChoiceLen) {
        // Change input to the general integer
        int choice = 0;
        for (int i = 0; i < input.size(); i++) {
            if (isdigit(input[i])) {
                choice = choice * 10 + (input[i] - '0');
            } else {
                return -1;
            }
        }

        return (choice <= maxChoiceLen && choice >= 0)? choice : -1;
    }

    int getInput(int maxChoiceLen) {
        string input;

        cin >> input;
        int choice = inputCheck(input, maxChoiceLen);
        while (choice == -1) {
            cout << "your input is wrong, please input again:" << endl;
            cin >> input;
            choice = inputCheck(input, maxChoiceLen);
        }

        return choice;
    }

    void commodityInput() {
        string name, detail;
        int price;

        cout << "Please input the commodity name:" << endl;
        name = readWholeLine();
        cout << "Please input the commodity price:" << endl;
        price = numberInput();
        cout << "Pease input the detail of the commodity:" << endl;
        detail = readWholeLine();

        Commodity* newCom = new Commodity(price, name, detail);
        commodityList.add(newCom);
    }

    void deleteCommodity() {
        if (commodityList.empty()) {
            cout << "No commodity inside the store" << endl;
            return;
        }

        int choice;
        cout << "There are existing commodity in our store:" << endl;
        commodityList.showCommoditiesName();
        cout << "Or type 0 to regret" << endl
             << "Which one do you want to delete?" << endl;

        choice = getInput(commodityList.size());

        if (choice != 0) {
            commodityList.remove(choice - 1);
        }
    }

    void showCommodity() {
        if (commodityList.empty()) {
            cout << "No commodity inside the store" << endl;
            return;
        }

        cout << "Here are all commodity in our store:" << endl;
        commodityList.showCommoditiesDetail();
        cout << endl;
    }

    UMode askMode() {
        string input;

        cout << "Are you a general user or a manager?" << endl
             << "1. general user, 2. manager" << endl;

        int choice = getInput(2);

        return (choice == 1) ? UMode::USER : UMode::MANAGER;
    }

    void decideService() {
        string input;

        cout << "Below are our service:" << endl
             << "1. Buy the commodity you want" << endl
             << "2. Check your shopping cart" << endl
             << "3. Check out" << endl
             << "You may choose what you need:" << endl;

        int choice = getInput(3);

        if (choice == 1) {
            storeStatus = SMode::SHOPPING;
        } else if (choice == 2) {
            storeStatus = SMode::CART_CHECKING;
        } else if (choice == 3) {
            storeStatus = SMode::CHECK_OUT;
        }
    }

    void chooseCommodity() {
        string input;
//        cout << "Here are our commodity:" << endl;
//        commodityList.showCommoditiesDetail();
        showCommodity();
        cout << "Or input 0 to exit shopping" << endl;

        int choice = getInput(commodityList.size());

        // Push the commodity into shopping cart here
        if (choice == 0) {
            storeStatus = SMode::DECIDING;
        } else {
            // May be some bug here, test later
            cart.push(commodityList.get(choice));
        }
    }

    void managerInterface() {
        cout << "Here are some manager services you can use:" << endl
             << "1. Add new commodity" << endl
             << "2. Delete commodity from the commodity list" << endl
             << "3. Show all existing commodity" << endl
             << "Or type 0 to exit manager mode" << endl
             << "Which action do you need?" << endl;

        int choice = getInput(3);

        if (choice == 1) {
            commodityInput();
        } else if (choice == 2) {
            deleteCommodity();
        } else if (choice == 3) {
            showCommodity();
        } else if (choice == 0) {
            storeStatus = SMode::OPENING;
        }
    }

    void userInterface() {
        if (storeStatus == SMode::OPENING) {
            userStatus = askMode();
            if (userStatus == UMode::USER) {
                storeStatus = SMode::DECIDING;
            } else {
                storeStatus = SMode::MANAGING;
            }
        } else if (storeStatus == SMode::DECIDING) {
            decideService();
        } else if (storeStatus == SMode::SHOPPING) {
            chooseCommodity();
        } else if (storeStatus == SMode::CART_CHECKING) {
            cout << "cart" << endl;
        } else if (storeStatus == SMode::CHECK_OUT) {
            cout << "check out" << endl;
        } else if (storeStatus == SMode::MANAGING) {
            managerInterface();
        } else if (storeStatus == SMode::CLOSE) {
            cout << "closing" << endl;
        }
    }

public:
    Store() {
        userStatus = UMode::USER;
        storeStatus = SMode::CLOSE;
    }

    void open() {
        storeStatus = SMode::OPENING;
        while (storeStatus != SMode::CLOSE) {
            userInterface();
        }
    }
};


int main() {
    Store csStore;
    csStore.open();
//    int test;
//    string testStr;
//    cin >> test;
//    cin.get();
//    getline(cin, testStr);
//    cout << testStr;
    return 0;
}
