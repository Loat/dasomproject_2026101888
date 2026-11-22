#ifndef WORKER_H
#define WORKER_H

#include <string>

using namespace std;

class Worker
{
public:
    static const int MAX_COUNT = 10;

private:
    string name;
    int basePrice;
    int count;
    double timer;

public:
    Worker(const string& name, int basePrice)
    {
        this->name = name;
        this->basePrice = basePrice;
        count = 0;
        timer = 0.0;
    }

    virtual ~Worker() {}

    string getName() const { return name; }
    int getCount() const { return count; }
    bool isMaxed() const { return count >= MAX_COUNT; }

    int getPrice() const
    {
        return basePrice + basePrice * count / 2;
    }

    void hire() { count++; }
    void setCount(int value) { count = value; }

    double getDrawsPerSecond() const
    {
        if (count == 0)
        {
            return 0.0;
        }
        return count / getInterval();
    }

    int work(double seconds)
    {
        if (count == 0)
        {
            return 0;
        }

        timer += seconds * count;

        int draws = (int)(timer / getInterval());
        timer -= draws * getInterval();
        return draws;
    }

    virtual double getInterval() const = 0;

    virtual string getDescription() const = 0;

    virtual Worker* clone() const = 0;
};

class ShortsKid : public Worker
{
public:
    ShortsKid() : Worker("반바지 꼬마", 200) {}

    double getInterval() const override
    {
        return 3.0;
    }

    string getDescription() const override
    {
        return "3초에 1번 뽑기. 반바지는 언제나 짧은 게 좋다!";
    }

    Worker* clone() const override
    {
        return new ShortsKid(*this);
    }
};

class EliteTrainer : public Worker
{
public:
    EliteTrainer() : Worker("엘리트 트레이너", 2000) {}

    double getInterval() const override
    {
        return 1.0;
    }

    string getDescription() const override
    {
        return "1초에 1번 뽑기. 실력 좋은 트레이너입니다.";
    }

    Worker* clone() const override
    {
        return new EliteTrainer(*this);
    }
};

class VeteranTrainer : public Worker
{
public:
    VeteranTrainer() : Worker("베테랑 트레이너", 15000) {}

    double getInterval() const override
    {
        return 1.0 / 3.0;
    }

    string getDescription() const override
    {
        return "1초에 3번 뽑기. 경험이 풍부한 베테랑입니다.";
    }

    Worker* clone() const override
    {
        return new VeteranTrainer(*this);
    }
};

#endif
