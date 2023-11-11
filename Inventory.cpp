/*
* Inventory functions that will be used for items.
* By: Prasanna Joshi
*/
#include "Inventory.h"

Inventory::Inventory()
    : mItems()
{
}

void Inventory::AddItem(const std::string& item)
{
    mItems.push_back(item);
}

void Inventory::RemoveItem(const std::string& item)
{
    auto iter = std::find(mItems.begin(), mItems.end(), item);
    if (iter != mItems.end())
    {
        mItems.erase(iter);
    }
}

const std::vector<std::string>& Inventory::GetItems() const
{
    return mItems;
}
