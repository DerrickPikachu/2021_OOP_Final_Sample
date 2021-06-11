#include <iostream>
#include <vector>

using namespace std;

class Commodity;
class Store;

class Commodity {
protected:
    int price;
    string description;
    string commodityName;

public:
    void detail() {
        cout << commodityName << endl;
        cout << "price: " << price << endl;
        cout << "description: " << description << endl;
        cout << "----------------------------" << endl;
    }
};

class CommodityList {
private:
    vector<Commodity> commodities;

public:
    void showCommodities() {
        for (int i = 0; i < commodities.size(); i++) {
            cout << i << ". ";
            commodities[i].detail();
        }
    }

    int size() {
        return (int)commodities.size();
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

        return (choice <= maxChoiceLen)? choice : -1;
    }

    int getInput(int maxChoiceLen) {
        string input;

        cin >> input;
        int choice = inputCheck(input, maxChoiceLen);
        while (choice == -1) {
            cout << "your input is wrong, please input again: ";
            cin >> input;
            choice = inputCheck(input, maxChoiceLen);
        }

        return choice;
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
             << "You may choose what you need: ";

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
        cout << "Here are our commodity:" << endl;
        commodityList.showCommodities();

        cout << "Or input 0 to exit shopping" << endl;
        cin >> input;
        int choice = inputCheck(input, commodityList.size());
        while (choice == -1) {
            cout << "your input is wrong, please input again: ";
            cin >> input;
            choice = inputCheck(input, commodityList.size());
        }

        // Push the commodity into shopping cart here
        if (choice == 0) {
            storeStatus = SMode::CLOSE; //***
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
        } else if (storeStatus == SMode::MANAGING) {
            cout << "manage" << endl;
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
    return 0;
}
