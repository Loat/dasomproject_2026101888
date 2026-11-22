#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class Player
{
private:
    long long gp;
    int gpPerDraw;
    int shinyNumerator;
    int shinyDenominator;
    int gpLevel;
    long long totalDraws;
    vector<string> purchasedList;

public:
    Player()
    {
        gp = 0;
        gpPerDraw = 1;
        shinyNumerator = 1;
        shinyDenominator = 8192;
        gpLevel = 0;
        totalDraws = 0;
    }

    long long getGp() const { return gp; }
    int getGpPerDraw() const { return gpPerDraw; }
    int getShinyNumerator() const { return shinyNumerator; }
    int getShinyDenominator() const { return shinyDenominator; }
    int getGpLevel() const { return gpLevel; }
    long long getTotalDraws() const { return totalDraws; }
    const vector<string>& getPurchasedList() const { return purchasedList; }

    double getShinyPercent() const
    {
        return (double)shinyNumerator / shinyDenominator * 100.0;
    }

    void addGp(long long amount) { gp += amount; }

    bool spendGp(long long amount)
    {
        if (gp < amount)
        {
            return false;
        }
        gp -= amount;
        return true;
    }

    void addDrawCount() { totalDraws++; }
    void setTotalDraws(long long value) { totalDraws = value; }
    void setShinyDenominator(int value) { shinyDenominator = value; }
    void multiplyShinyNumerator(int times) { shinyNumerator *= times; }
    void addGpPerDraw(int amount) { gpPerDraw += amount; }
    void increaseGpLevel() { gpLevel++; }
    void addPurchasedName(const string& name) { purchasedList.push_back(name); }

    friend ostream& operator<<(ostream& os, const Player& p)
    {
        os << "========== 현재 상태 ==========" << endl;
        os << " 보유 GP        : " << p.gp << " GP" << endl;
        os << " 뽑기당 GP 획득 : " << p.gpPerDraw << " GP" << endl;
        os << " 이로치 확률    : " << p.shinyNumerator << "/" << p.shinyDenominator
           << " (" << fixed << setprecision(4) << p.getShinyPercent() << "%)" << endl;
        os << " 총 뽑기 횟수   : " << p.totalDraws << "회" << endl;
        os << " 구매한 업그레이드:";
        if (p.purchasedList.empty())
        {
            os << " 없음" << endl;
        }
        else
        {
            os << endl;
            for (size_t i = 0; i < p.purchasedList.size(); i++)
            {
                os << "   - " << p.purchasedList[i] << endl;
            }
        }
        os << "===============================";
        return os;
    }
};

#endif
