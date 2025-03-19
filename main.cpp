#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<iomanip>
using namespace std;

class Item {
public:
    string name;
    int quantity;
    int initial_quantity;
    float cost_price;
    float selling_price;
    int total_sold = 0;
    float total_profit = 0;

    void display() {
        cout << left << setw(15) << name
             << setw(15) << quantity
             << setw(15) << cost_price
             << setw(15) << selling_price << endl;
    }
};

class Shop {
private:
    map<string, Item> inventory;
    float total_investment = 0;
    float total_profit = 0;
    float total_revenue = 0;

public:
    void loadItems() {
        ifstream file("inventory.txt");
        if (!file) {
            cout << "Inventory file not found. Creating one..." << endl;
            ofstream newFile("inventory.txt");
            newFile.close();
            return;
        }

        string header;
        getline(file, header); // Skip the header line

        while (!file.eof()) {
            Item item;
            file >> item.name >> item.quantity >> item.cost_price >> item.selling_price;
            if (!file.fail()) {
                item.initial_quantity = item.quantity;
                inventory[item.name] = item;
                total_investment += (item.cost_price * item.quantity);
            }
        }
        file.close();
    }

    void displayItems() {
        cout << left << setw(15) << "Item Name"
             << setw(15) << "Quantity"
             << setw(15) << "Cost Price"
             << setw(15) << "Selling Price" << endl;
        cout << "-----------------------------------------------------------" << endl;
        for (auto i : inventory) {
            i.second.display();
        }
    }

    void sellItems() {
        string itemName;
        int qty;
        char choice;
        do {
            cout << "Enter Item Name: ";
            cin >> itemName;
            cout << "Enter Quantity: ";
            cin >> qty;
            if (inventory.find(itemName) != inventory.end()) {
                if (inventory[itemName].quantity >= qty) {
                    inventory[itemName].quantity -= qty;
                    inventory[itemName].total_sold += qty;
                    float revenue = inventory[itemName].selling_price * qty;
                    float profit = (inventory[itemName].selling_price - inventory[itemName].cost_price) * qty;
                    inventory[itemName].total_profit += profit;
                    total_revenue += revenue;
                    total_profit += profit;

                    cout << "\nItem Sold Successfully!\n";
                    updateInventory();
                    updateProfitFile();
                } else {
                    cout << "Stock is not available! Only " << inventory[itemName].quantity << " are in stock.\n";
                }
            } else {
                cout << "Item not found!\n";
            }

            cout << "Do you want to buy more items? (y/n): ";
            cin >> choice;
        } while (choice == 'y' || choice == 'Y');
    }

    void updateInventory() {
        ofstream file("inventory.txt");
        file << left << setw(15) << "Item Name"
             << setw(15) << "Quantity"
             << setw(15) << "Cost Price"
             << setw(15) << "Selling Price" << endl;
        for (auto i : inventory) {
            file << left << setw(15) << i.second.name
                 << setw(15) << i.second.quantity
                 << setw(15) << i.second.cost_price
                 << setw(15) << i.second.selling_price << endl;
        }
        file.close();
    }

    void updateProfitFile() {
        ofstream file("profit_loss.txt");
        file << "Total Investment: Rs." << total_investment << endl;
        file << "Total Revenue: Rs." << total_revenue << endl;
        file << "Total Profit: Rs." << total_profit << endl;
        file << "\nItems Sold:\n";
        file << left << setw(15) << "Item Name" << setw(15) << "Bought" << setw(15) << "Sold" << setw(15) << "Profit" << endl;
        file << "--------------------------------------------------------------" << endl;
        for (auto i : inventory) {
            file << left << setw(15) << i.second.name 
                 << setw(15) << i.second.initial_quantity 
                 << setw(15) << i.second.total_sold 
                 << setw(15) << i.second.total_profit << endl;
        }
        file << "\nItems that need restocking:\n";
        for (auto i : inventory) {
            if (i.second.quantity == 0) {
                file << i.second.name << " needs to be restocked!" << endl;
            }
        }
        file.close();
    }
};

int main() {
    Shop shop;
    shop.loadItems();

    int choice;
    do {
        cout << "\n--- Grocery Shop Management ---" << endl;
        cout << "1. Show Items" << endl;
        cout << "2. Sell Items" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            shop.displayItems();
        }
        else if (choice == 2) {
            shop.sellItems();
        }
        else if (choice == 3) {
            cout << "Exiting...\n";
        }
        else {
            cout << "Invalid Choice!" << endl;
        }
    } while (choice != 3);

    return 0;
}
