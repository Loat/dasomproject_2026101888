#ifndef GAMECORE_H
#define GAMECORE_H

#include <string>
#include <vector>
#include <random>
#include <fstream>
#include "Player.h"
#include "Upgrade.h"
#include "Shop.h"
#include "Legendary.h"
#include "Worker.h"
#include "WorkerManager.h"

using namespace std;

class GameCore
{
public:
    static const int TICKET_COUNT = 3;
    static const int COLLECT_BONUS_GP = 2;

private:
    Player player;
    Shop baseShop;
    Shop shop;
    vector<Legendary> legendaries;
    WorkerManager workers;
    mt19937 engine;

    int targetIndex;
    long long huntDraws;
    int lastCaughtIndex;

    int ticketDraws[TICKET_COUNT];
    int ticketPrices[TICKET_COUNT];

    void setupUpgrades(Shop& target)
    {
        target.addUpgrade(new GenerationUpgrade(100));
        target.addUpgrade(new ShinyCharm(500));

        int gpPrices[5]    = { 50, 150, 400, 1000, 2500 };
        int gpIncreases[5] = {  1,   2,   4,    8,   16 };

        for (int i = 0; i < GpUpgrade::MAX_STEP; i++)
        {
            target.addUpgrade(new GpUpgrade(i + 1, gpPrices[i], gpIncreases[i]));
        }
    }

    void setupLegendaries()
    {
        legendaries.push_back(Legendary("뮤츠", "mewtwo"));
        legendaries.push_back(Legendary("루기아", "lugia"));
        legendaries.push_back(Legendary("칠색조", "hooh"));
        legendaries.push_back(Legendary("그란돈", "groudon"));
        legendaries.push_back(Legendary("가이오가", "kyogre"));
        legendaries.push_back(Legendary("레쿠쟈", "rayquaza"));
        legendaries.push_back(Legendary("디아루가", "dialga"));
        legendaries.push_back(Legendary("펄기아", "palkia"));
        legendaries.push_back(Legendary("기라티나", "giratina"));
        legendaries.push_back(Legendary("레시라무", "reshiram"));
        legendaries.push_back(Legendary("제크로무", "zekrom"));
        legendaries.push_back(Legendary("큐레무", "kyurem"));
        legendaries.push_back(Legendary("제르네아스", "xerneas"));
        legendaries.push_back(Legendary("이벨타르", "yveltal"));
        legendaries.push_back(Legendary("지가르데", "zygarde"));
        legendaries.push_back(Legendary("솔가레오", "solgaleo"));
        legendaries.push_back(Legendary("루나아라", "lunala"));
        legendaries.push_back(Legendary("네크로즈마", "necrozma"));
        legendaries.push_back(Legendary("자시안", "zacian"));
        legendaries.push_back(Legendary("자마젠타", "zamazenta"));
        legendaries.push_back(Legendary("무한다이노", "eternatus"));
        legendaries.push_back(Legendary("코라이돈", "koraidon"));
        legendaries.push_back(Legendary("미라이돈", "miraidon"));
        legendaries.push_back(Legendary("테라파고스", "terapagos"));
    }

    void setupWorkers()
    {
        workers.addWorker(new ShortsKid());
        workers.addWorker(new EliteTrainer());
        workers.addWorker(new VeteranTrainer());
    }

    int findLegendary(const string& imageKey) const
    {
        for (size_t i = 0; i < legendaries.size(); i++)
        {
            if (legendaries[i].getImageKey() == imageKey)
            {
                return (int)i;
            }
        }
        return -1;
    }

    bool rollShiny()
    {
        uniform_int_distribution<int> dist(1, player.getShinyDenominator());
        int number = dist(engine);
        return number <= player.getShinyNumerator();
    }

    void catchTarget()
    {
        legendaries[targetIndex].setCaught(huntDraws);
        player.addGpPerDraw(COLLECT_BONUS_GP);

        lastCaughtIndex = targetIndex;
        targetIndex = -1;
        huntDraws = 0;
    }

    bool drawOnce(bool giveGp)
    {
        if (giveGp)
        {
            player.addGp(player.getGpPerDraw());
        }
        player.addDrawCount();
        huntDraws++;

        if (rollShiny())
        {
            catchTarget();
            return true;
        }
        return false;
    }

public:
    GameCore()
    {
        targetIndex = -1;
        huntDraws = 0;
        lastCaughtIndex = -1;

        random_device rd;
        engine.seed(rd());

        ticketDraws[0] = 50;    ticketPrices[0] = 100;
        ticketDraws[1] = 200;   ticketPrices[1] = 350;
        ticketDraws[2] = 1000;  ticketPrices[2] = 1500;

        setupUpgrades(baseShop);
        Shop tempShop(baseShop);
        shop = tempShop;

        setupLegendaries();
        setupWorkers();
    }

    const Player& getPlayer() const { return player; }
    const Shop& getShop() const { return shop; }
    const WorkerManager& getWorkers() const { return workers; }

    int getTicketDraws(int index) const { return ticketDraws[index]; }
    int getTicketPrice(int index) const { return ticketPrices[index]; }

    int getLegendaryCount() const { return (int)legendaries.size(); }
    const Legendary& getLegendary(int index) const { return legendaries[index]; }

    bool isHunting() const { return targetIndex != -1; }
    int getTargetIndex() const { return targetIndex; }
    long long getHuntDraws() const { return huntDraws; }
    int getLastCaughtIndex() const { return lastCaughtIndex; }

    int getCaughtCount() const
    {
        int count = 0;
        for (size_t i = 0; i < legendaries.size(); i++)
        {
            if (legendaries[i].isCaught())
            {
                count++;
            }
        }
        return count;
    }

    bool isAllCaught() const
    {
        return getCaughtCount() == getLegendaryCount();
    }

    bool selectTarget(int index, string& message)
    {
        if (index < 0 || index >= (int)legendaries.size())
        {
            message = "존재하지 않는 전설입니다.";
            return false;
        }
        if (legendaries[index].isCaught())
        {
            message = "이미 수집한 전설입니다.";
            return false;
        }

        if (index != targetIndex)
        {
            targetIndex = index;
            huntDraws = 0;
        }
        message = legendaries[index].getName() + " 이로치 사냥을 시작합니다!";
        return true;
    }

    bool singleDraw()
    {
        if (!isHunting())
        {
            return false;
        }
        return drawOnce(true);
    }

    int useTicket(int index, string& message)
    {
        if (!isHunting())
        {
            message = "먼저 사냥할 전설을 선택하세요.";
            return -1;
        }
        if (index < 0 || index >= TICKET_COUNT)
        {
            message = "존재하지 않는 티켓입니다.";
            return -1;
        }
        if (!player.spendGp(ticketPrices[index]))
        {
            message = "GP가 부족합니다! (필요: " + to_string(ticketPrices[index])
                    + " GP, 보유: " + to_string(player.getGp()) + " GP)";
            return -1;
        }

        string targetName = legendaries[targetIndex].getName();
        int done = 0;
        bool caught = false;

        for (int i = 0; i < ticketDraws[index]; i++)
        {
            done++;
            if (drawOnce(false))
            {
                caught = true;
                break;
            }
        }

        if (caught)
        {
            message = to_string(done) + "번째 뽑기에서 이로치 " + targetName + " 발견!";
        }
        else
        {
            message = to_string(done) + "회 뽑기 완료! 이로치는 나오지 않았습니다.";
        }
        return done;
    }

    bool hireWorker(int index, string& message)
    {
        return workers.hire(index, player, message);
    }

    int updateWorkers(double seconds, long long& gpEarned)
    {
        gpEarned = 0;
        if (!isHunting() || seconds <= 0)
        {
            return 0;
        }

        int draws = workers.work(seconds);
        long long gpBefore = player.getGp();
        int done = 0;

        for (int i = 0; i < draws; i++)
        {
            done++;
            if (drawOnce(true))
            {
                break;
            }
        }

        gpEarned = player.getGp() - gpBefore;
        return done;
    }

    bool buyUpgrade(int realIndex, string& message)
    {
        return shop.buy(realIndex, player, message);
    }

    bool saveToFile(const string& fileName) const
    {
        ofstream fout(fileName.c_str());
        if (!fout)
        {
            return false;
        }

        fout << "SHINY_TYCOON_SAVE 1" << endl;
        fout << "GP " << player.getGp() << endl;
        fout << "TOTAL_DRAWS " << player.getTotalDraws() << endl;

        fout << "UPGRADES " << shop.getCount();
        for (int i = 0; i < shop.getCount(); i++)
        {
            fout << " " << (shop.getUpgrade(i)->isPurchased() ? 1 : 0);
        }
        fout << endl;

        fout << "LEGENDARIES " << legendaries.size() << endl;
        for (size_t i = 0; i < legendaries.size(); i++)
        {
            fout << legendaries[i].getImageKey() << " "
                 << (legendaries[i].isCaught() ? 1 : 0) << " "
                 << legendaries[i].getCatchDraws() << endl;
        }

        if (isHunting())
        {
            fout << "TARGET " << legendaries[targetIndex].getImageKey() << endl;
        }
        else
        {
            fout << "TARGET none" << endl;
        }
        fout << "HUNT_DRAWS " << huntDraws << endl;

        fout << "WORKERS " << workers.getCount();
        for (int i = 0; i < workers.getCount(); i++)
        {
            fout << " " << workers.getWorker(i)->getCount();
        }
        fout << endl;

        return true;
    }

    bool loadFromFile(const string& fileName)
    {
        ifstream fin(fileName.c_str());
        if (!fin)
        {
            return false;
        }

        string label;
        int version = 0;
        long long savedGp = 0;
        long long savedTotalDraws = 0;
        int upgradeCount = 0;
        vector<int> upgradeFlags;
        int legendaryCount = 0;
        vector<string> savedKeys;
        vector<int> savedCaught;
        vector<long long> savedCatchDraws;
        string savedTarget;
        long long savedHuntDraws = 0;

        fin >> label >> version;
        if (label != "SHINY_TYCOON_SAVE")
        {
            return false;
        }

        fin >> label >> savedGp;
        fin >> label >> savedTotalDraws;

        fin >> label >> upgradeCount;
        if (upgradeCount < 0 || upgradeCount > 100)
        {
            return false;
        }
        for (int i = 0; i < upgradeCount; i++)
        {
            int flag = 0;
            fin >> flag;
            upgradeFlags.push_back(flag);
        }

        fin >> label >> legendaryCount;
        if (legendaryCount < 0 || legendaryCount > 1000)
        {
            return false;
        }
        for (int i = 0; i < legendaryCount; i++)
        {
            string key;
            int caughtFlag = 0;
            long long draws = 0;
            fin >> key >> caughtFlag >> draws;
            savedKeys.push_back(key);
            savedCaught.push_back(caughtFlag);
            savedCatchDraws.push_back(draws);
        }

        fin >> label >> savedTarget;
        fin >> label >> savedHuntDraws;

        if (fin.fail())
        {
            return false;
        }

        vector<int> workerCounts;
        if (fin >> label && label == "WORKERS")
        {
            int workerTypes = 0;
            fin >> workerTypes;
            for (int i = 0; i < workerTypes && i < 100; i++)
            {
                int hired = 0;
                fin >> hired;
                workerCounts.push_back(hired);
            }
            if (fin.fail())
            {
                workerCounts.clear();
            }
        }

        player.addGp(savedGp);
        player.setTotalDraws(savedTotalDraws);

        for (int i = 0; i < upgradeCount && i < shop.getCount(); i++)
        {
            if (upgradeFlags[i] == 1)
            {
                shop.restorePurchase(i, player);
            }
        }

        for (int i = 0; i < legendaryCount; i++)
        {
            int index = findLegendary(savedKeys[i]);
            if (index != -1 && savedCaught[i] == 1 && !legendaries[index].isCaught())
            {
                legendaries[index].setCaught(savedCatchDraws[i]);
                player.addGpPerDraw(COLLECT_BONUS_GP);
            }
        }

        int target = findLegendary(savedTarget);
        if (target != -1 && !legendaries[target].isCaught())
        {
            targetIndex = target;
            huntDraws = savedHuntDraws;
        }

        for (size_t i = 0; i < workerCounts.size(); i++)
        {
            workers.setHiredCount((int)i, workerCounts[i]);
        }

        return true;
    }
};

#endif
