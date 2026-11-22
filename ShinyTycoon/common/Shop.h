#ifndef SHOP_H
#define SHOP_H

#include <string>
#include <vector>
#include "Player.h"
#include "Upgrade.h"

using namespace std;

class Shop
{
private:
    vector<Upgrade*> upgrades;

    void clearUpgrades()
    {
        for (size_t i = 0; i < upgrades.size(); i++)
        {
            delete upgrades[i];
        }
        upgrades.clear();
    }

public:
    Shop() {}

    ~Shop()
    {
        clearUpgrades();
    }

    Shop(const Shop& other)
    {
        for (size_t i = 0; i < other.upgrades.size(); i++)
        {
            upgrades.push_back(other.upgrades[i]->clone());
        }
    }

    Shop& operator=(const Shop& other)
    {
        if (this == &other)
        {
            return *this;
        }

        clearUpgrades();
        for (size_t i = 0; i < other.upgrades.size(); i++)
        {
            upgrades.push_back(other.upgrades[i]->clone());
        }
        return *this;
    }

    void addUpgrade(Upgrade* upgrade)
    {
        upgrades.push_back(upgrade);
    }

    int getCount() const
    {
        return (int)upgrades.size();
    }

    const Upgrade* getUpgrade(int index) const
    {
        if (index < 0 || index >= (int)upgrades.size())
        {
            return nullptr;
        }
        return upgrades[index];
    }

    int getVisibleCount(const Player& player) const
    {
        int count = 0;
        for (size_t i = 0; i < upgrades.size(); i++)
        {
            if (upgrades[i]->isVisible(player))
            {
                count++;
            }
        }
        return count;
    }

    int getRealIndex(int visibleIndex, const Player& player) const
    {
        int count = 0;
        for (size_t i = 0; i < upgrades.size(); i++)
        {
            if (upgrades[i]->isVisible(player))
            {
                if (count == visibleIndex)
                {
                    return (int)i;
                }
                count++;
            }
        }
        return -1;
    }

    bool buy(int index, Player& player, string& message)
    {
        if (index < 0 || index >= (int)upgrades.size())
        {
            message = "존재하지 않는 업그레이드입니다.";
            return false;
        }

        Upgrade* u = upgrades[index];

        if (!u->isAvailable(player))
        {
            message = "구매할 수 없습니다: " + u->getUnavailableReason(player);
            return false;
        }

        if (!player.spendGp(u->getPrice()))
        {
            message = "GP가 부족합니다! (필요: " + to_string(u->getPrice())
                    + " GP, 보유: " + to_string(player.getGp()) + " GP)";
            return false;
        }

        u->applyEffect(player);
        u->setPurchased();
        player.addPurchasedName(u->getName());

        message = "[" + u->getName() + "] 구매 완료! " + u->getDescription();
        return true;
    }

    bool restorePurchase(int index, Player& player)
    {
        if (index < 0 || index >= (int)upgrades.size())
        {
            return false;
        }

        Upgrade* u = upgrades[index];
        if (!u->isAvailable(player))
        {
            return false;
        }

        u->applyEffect(player);
        u->setPurchased();
        player.addPurchasedName(u->getName());
        return true;
    }
};

#endif
