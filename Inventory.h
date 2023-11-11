/*
* Inventory functions that will be used for items.
* By: Prasanna Joshi
*/
#pragma once

#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>
#include <algorithm>
#include "InventoryItem.h"

class Inventory
{
public:
    Inventory();
    void AddItem(const std::string& item);
    void RemoveItem(const std::string& item);
    const std::vector<std::string>& GetItems() const;

private:
    std::vector<std::string> mItems;
};

#endif