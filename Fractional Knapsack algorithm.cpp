// Compile with command option '-std=c++11'

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

#define MAX_CAPACITY 40

struct Item {
    string name;
    double weight;
    double value;

    double valuePerWeight() const {
        return value / weight;
    }
};

struct Bag {
    vector<Item> items;
    int maxCapacity;

    double totalWeight() const {
        double totalWeight = 0;
        for (const auto& item : items) {
            totalWeight += item.weight;
        }
        return totalWeight;
    }

    double totalValue() const {
        double totalValue = 0;
        for (const auto& item : items) {
            totalValue += item.value;
        }
        return totalValue;
    }
};

Bag calculateOptimalBag(vector<Item>& items) {
    Bag bag;
    bag.maxCapacity = MAX_CAPACITY;

    sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.valuePerWeight() > b.valuePerWeight();
    });

    for (auto it = items.begin(); it != items.end(); ) {
        if (bag.totalWeight() + it->weight <= bag.maxCapacity) {
            bag.items.push_back(*it);
            it = items.erase(it);
        } else {
            break;
        }
    }

    double remainingCapacity = bag.maxCapacity - bag.totalWeight();
    if (!items.empty() && remainingCapacity > 0) {
        bag.items.push_back({items.front().name, remainingCapacity, remainingCapacity * items.front().valuePerWeight()});
    }

    return bag;
}

void displayBagContents(const Bag& bag) {
    printf("Goods\tWeight of goods in knapsack\tValue of goods in knapsack\n");
    for (const auto& item : bag.items) {
        printf("%-5s\t%-.2f\t\t\t%-.2f\n", item.name.c_str(), item.weight, item.value);
    }
    printf("Total \t%-.2f\t\t\t%-.2f\n", bag.totalWeight(), bag.totalValue());
}

int main() {
    // Define items directly instead of reading from file
    vector<Item> items = {
        {"주석", 50, 5},
        {"백금", 10, 60},
        {"은", 25, 10},
        {"금", 15, 75}
    };

    clock_t startTime = clock();
    Bag bag = calculateOptimalBag(items);
    clock_t endTime = clock();

    displayBagContents(bag);
    printf("Execution Time (ms): %f\n", static_cast<double>(endTime - startTime));

    return 0;
}
