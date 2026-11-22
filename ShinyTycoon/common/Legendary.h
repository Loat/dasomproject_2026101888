#ifndef LEGENDARY_H
#define LEGENDARY_H

#include <string>

using namespace std;

class Legendary
{
private:
    string name;
    string imageKey;
    bool caught;
    long long catchDraws;

    static bool hasFinalConsonant(const string& text)
    {
        if (text.size() < 3)
        {
            return false;
        }

        unsigned char b1 = text[text.size() - 3];
        unsigned char b2 = text[text.size() - 2];
        unsigned char b3 = text[text.size() - 1];
        int code = ((b1 & 0x0F) << 12) | ((b2 & 0x3F) << 6) | (b3 & 0x3F);

        if (code < 0xAC00 || code > 0xD7A3)
        {
            return false;
        }
        return (code - 0xAC00) % 28 != 0;
    }

public:
    Legendary(const string& name, const string& imageKey)
    {
        this->name = name;
        this->imageKey = imageKey;
        caught = false;
        catchDraws = 0;
    }

    string getName() const { return name; }
    string getImageKey() const { return imageKey; }
    bool isCaught() const { return caught; }
    long long getCatchDraws() const { return catchDraws; }

    string getNameWithSubject() const
    {
        if (hasFinalConsonant(name))
        {
            return name + "이";
        }
        return name + "가";
    }

    void setCaught(long long draws)
    {
        caught = true;
        catchDraws = draws;
    }
};

#endif
