#ifndef UPGRADE_H
#define UPGRADE_H

#include <string>
#include "Player.h"

using namespace std;

class Upgrade
{
private:
    string name;
    int price;
    bool purchased;

public:
    Upgrade(const string& name, int price)
    {
        this->name = name;
        this->price = price;
        purchased = false;
    }

    virtual ~Upgrade() {}

    string getName() const { return name; }
    int getPrice() const { return price; }
    bool isPurchased() const { return purchased; }
    void setPurchased() { purchased = true; }

    virtual string getDescription() const = 0;

    virtual void applyEffect(Player& player) = 0;

    virtual bool isVisible(const Player& player) const
    {
        return true;
    }

    virtual bool isAvailable(const Player& player) const
    {
        return !purchased;
    }

    virtual string getUnavailableReason(const Player& player) const
    {
        if (purchased)
        {
            return "이미 구매한 업그레이드입니다.";
        }
        return "";
    }

    virtual Upgrade* clone() const = 0;
};

class GenerationUpgrade : public Upgrade
{
public:
    GenerationUpgrade(int price) : Upgrade("세대 업그레이드", price) {}

    string getDescription() const override
    {
        return "이로치 확률의 분모를 8192에서 4096으로 낮춥니다.";
    }

    void applyEffect(Player& player) override
    {
        player.setShinyDenominator(4096);
    }

    Upgrade* clone() const override
    {
        return new GenerationUpgrade(*this);
    }
};

class ShinyCharm : public Upgrade
{
public:
    ShinyCharm(int price) : Upgrade("빛나는 부적", price) {}

    string getDescription() const override
    {
        return "이로치 확률이 3배가 됩니다.";
    }

    void applyEffect(Player& player) override
    {
        player.multiplyShinyNumerator(3);
    }

    Upgrade* clone() const override
    {
        return new ShinyCharm(*this);
    }
};

class GpUpgrade : public Upgrade
{
public:
    static const int MAX_STEP = 5;

private:
    int step;
    int gpIncrease;

public:
    GpUpgrade(int step, int price, int gpIncrease)
        : Upgrade("GP 획득량 업그레이드 " + to_string(step) + "단계", price)
    {
        this->step = step;
        this->gpIncrease = gpIncrease;
    }

    string getDescription() const override
    {
        return "뽑기 1회당 GP가 +" + to_string(gpIncrease) + " 증가합니다.";
    }

    void applyEffect(Player& player) override
    {
        player.addGpPerDraw(gpIncrease);
        player.increaseGpLevel();
    }

    bool isVisible(const Player& player) const override
    {
        if (step == player.getGpLevel() + 1)
        {
            return true;
        }
        if (step == MAX_STEP && isPurchased())
        {
            return true;
        }
        return false;
    }

    bool isAvailable(const Player& player) const override
    {
        if (isPurchased())
        {
            return false;
        }
        return player.getGpLevel() == step - 1;
    }

    string getUnavailableReason(const Player& player) const override
    {
        if (isPurchased())
        {
            return "이미 구매한 업그레이드입니다.";
        }
        return to_string(step - 1) + "단계를 먼저 구매해야 합니다.";
    }

    Upgrade* clone() const override
    {
        return new GpUpgrade(*this);
    }
};

#endif
