#ifndef WORKERMANAGER_H
#define WORKERMANAGER_H

#include <string>
#include <vector>
#include "Player.h"
#include "Worker.h"

using namespace std;

class WorkerManager
{
private:
    vector<Worker*> workers;

    void clearWorkers()
    {
        for (size_t i = 0; i < workers.size(); i++)
        {
            delete workers[i];
        }
        workers.clear();
    }

public:
    WorkerManager() {}

    ~WorkerManager()
    {
        clearWorkers();
    }

    WorkerManager(const WorkerManager& other)
    {
        for (size_t i = 0; i < other.workers.size(); i++)
        {
            workers.push_back(other.workers[i]->clone());
        }
    }

    WorkerManager& operator=(const WorkerManager& other)
    {
        if (this == &other)
        {
            return *this;
        }

        clearWorkers();
        for (size_t i = 0; i < other.workers.size(); i++)
        {
            workers.push_back(other.workers[i]->clone());
        }
        return *this;
    }

    void addWorker(Worker* worker)
    {
        workers.push_back(worker);
    }

    int getCount() const
    {
        return (int)workers.size();
    }

    const Worker* getWorker(int index) const
    {
        if (index < 0 || index >= (int)workers.size())
        {
            return nullptr;
        }
        return workers[index];
    }

    int getTotalHired() const
    {
        int total = 0;
        for (size_t i = 0; i < workers.size(); i++)
        {
            total += workers[i]->getCount();
        }
        return total;
    }

    double getDrawsPerSecond() const
    {
        double total = 0.0;
        for (size_t i = 0; i < workers.size(); i++)
        {
            total += workers[i]->getDrawsPerSecond();
        }
        return total;
    }

    int work(double seconds)
    {
        int total = 0;
        for (size_t i = 0; i < workers.size(); i++)
        {
            total += workers[i]->work(seconds);
        }
        return total;
    }

    bool hire(int index, Player& player, string& message)
    {
        if (index < 0 || index >= (int)workers.size())
        {
            message = "존재하지 않는 알바입니다.";
            return false;
        }

        Worker* w = workers[index];

        if (w->isMaxed())
        {
            message = w->getName() + "는 최대 " + to_string(Worker::MAX_COUNT) + "명까지 고용할 수 있습니다.";
            return false;
        }

        int price = w->getPrice();
        if (!player.spendGp(price))
        {
            message = "GP가 부족합니다! (필요: " + to_string(price)
                    + " GP, 보유: " + to_string(player.getGp()) + " GP)";
            return false;
        }

        w->hire();
        message = "[" + w->getName() + "] 고용 완료! (" + to_string(w->getCount()) + "명 근무 중)";
        return true;
    }

    void setHiredCount(int index, int value)
    {
        if (index < 0 || index >= (int)workers.size())
        {
            return;
        }
        if (value < 0)
        {
            value = 0;
        }
        if (value > Worker::MAX_COUNT)
        {
            value = Worker::MAX_COUNT;
        }
        workers[index]->setCount(value);
    }
};

#endif
