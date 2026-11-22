#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>

#include "../common/Player.h"
#include "../common/Upgrade.h"
#include "../common/Shop.h"
#include "../common/Legendary.h"
#include "../common/Worker.h"
#include "../common/WorkerManager.h"
#include "../common/GameCore.h"

using namespace std;

const string SAVE_FILE = "save.txt";

class Game
{
private:
    GameCore core;
    time_t lastTime;

    int readInt()
    {
        string line;
        if (!getline(cin, line))
        {
            save();
            exit(0);
        }

        if (line.empty() || line.length() > 9)
        {
            return -1;
        }
        for (size_t i = 0; i < line.length(); i++)
        {
            if (line[i] < '0' || line[i] > '9')
            {
                return -1;
            }
        }
        return stoi(line);
    }

    void save()
    {
        if (!core.saveToFile(SAVE_FILE))
        {
            cout << "[경고] 저장에 실패했습니다." << endl;
        }
    }

    void showMenu()
    {
        const Legendary& target = core.getLegendary(core.getTargetIndex());

        cout << endl;
        cout << "========== 이로치 타이쿤 ==========" << endl;
        cout << " 목표 : " << target.getName()
             << " (이번 사냥 " << core.getHuntDraws() << "회)" << endl;
        cout << " 도감 : " << core.getCaughtCount() << " / " << core.getLegendaryCount() << endl;
        if (core.getWorkers().getTotalHired() > 0)
        {
            cout << " 알바 : " << core.getWorkers().getTotalHired() << "명 근무 중 (초당 "
                 << fixed << setprecision(2) << core.getWorkers().getDrawsPerSecond() << "회)" << endl;
        }
        cout << "-----------------------------------" << endl;
        cout << " 1. 뽑기 1회" << endl;
        cout << " 2. 연속 뽑기 (티켓 구매)" << endl;
        cout << " 3. 업그레이드 상점" << endl;
        cout << " 4. 현재 상태 보기" << endl;
        cout << " 5. 도감 / 목표 변경" << endl;
        cout << " 6. 알바 고용소" << endl;
        cout << " 7. 저장 후 종료" << endl;
        cout << "===================================" << endl;
        cout << "선택 >> ";
    }

    void showLegendaryList()
    {
        cout << endl;
        cout << "========== 전설 포켓몬 도감 (" << core.getCaughtCount()
             << " / " << core.getLegendaryCount() << ") ==========" << endl;

        for (int i = 0; i < core.getLegendaryCount(); i++)
        {
            const Legendary& l = core.getLegendary(i);

            cout << " " << (i + 1) << ". " << l.getName();
            if (l.isCaught())
            {
                cout << "  [수집 완료 - " << l.getCatchDraws() << "회 만에]";
            }
            else if (i == core.getTargetIndex())
            {
                cout << "  [사냥 중]";
            }
            cout << endl;
        }
    }

    bool chooseTarget()
    {
        while (true)
        {
            showLegendaryList();
            if (core.isHunting())
            {
                cout << " 0. 돌아가기" << endl;
            }
            else
            {
                cout << " 0. 저장 후 종료" << endl;
            }
            cout << "사냥할 전설 번호 >> ";

            int choice = readInt();
            if (choice == 0)
            {
                return core.isHunting();
            }
            if (choice < 1 || choice > core.getLegendaryCount())
            {
                cout << "잘못된 입력입니다." << endl;
                continue;
            }

            string message;
            bool ok = core.selectTarget(choice - 1, message);
            cout << message << endl;
            if (ok)
            {
                return true;
            }
        }
    }

    void doSingleDraw()
    {
        string subject = core.getLegendary(core.getTargetIndex()).getNameWithSubject();

        if (core.singleDraw())
        {
            showCatch();
        }
        else
        {
            const Player& player = core.getPlayer();
            cout << "야생 " << subject << " 나타났다! 평범한 색이다... (+"
                 << player.getGpPerDraw() << " GP, 보유 " << player.getGp() << " GP)" << endl;
        }
    }

    void doMultiDraw()
    {
        cout << "========= 연속 뽑기 티켓 =========" << endl;
        cout << " 보유 GP : " << core.getPlayer().getGp() << " GP" << endl;
        cout << "----------------------------------" << endl;
        for (int i = 0; i < GameCore::TICKET_COUNT; i++)
        {
            cout << " " << (i + 1) << ". " << core.getTicketDraws(i) << "회 연속 뽑기권 ("
                 << core.getTicketPrice(i) << " GP)" << endl;
        }
        cout << " 0. 돌아가기" << endl;
        cout << "==================================" << endl;
        cout << "선택 >> ";

        int choice = readInt();
        if (choice == 0)
        {
            return;
        }
        if (choice < 1 || choice > GameCore::TICKET_COUNT)
        {
            cout << "잘못된 입력입니다." << endl;
            return;
        }

        string message;
        int done = core.useTicket(choice - 1, message);
        cout << message << endl;

        if (done == -1)
        {
            return;
        }

        if (!core.isHunting())
        {
            showCatch();
        }
        else
        {
            cout << "(연속 뽑기는 GP를 얻지 못합니다)" << endl;
        }
    }

    void showShopList()
    {
        const Player& player = core.getPlayer();
        const Shop& shop = core.getShop();

        cout << "============= 업그레이드 상점 =============" << endl;
        cout << " 보유 GP : " << player.getGp() << " GP" << endl;
        cout << "-------------------------------------------" << endl;

        int number = 1;
        for (int i = 0; i < shop.getCount(); i++)
        {
            const Upgrade* u = shop.getUpgrade(i);

            if (!u->isVisible(player))
            {
                continue;
            }

            cout << " " << number << ". " << u->getName()
                 << " (" << u->getPrice() << " GP)" << endl;
            cout << "     " << u->getDescription() << endl;

            if (u->isPurchased())
            {
                cout << "     [구매 완료]" << endl;
            }
            else if (!u->isAvailable(player))
            {
                cout << "     [잠김] " << u->getUnavailableReason(player) << endl;
            }
            number++;
        }
        cout << " 0. 돌아가기" << endl;
        cout << "===========================================" << endl;
    }

    void openShop()
    {
        while (true)
        {
            showShopList();
            cout << "구매할 번호 >> ";
            int choice = readInt();

            if (choice == 0)
            {
                return;
            }
            if (choice < 0 || choice > core.getShop().getVisibleCount(core.getPlayer()))
            {
                cout << "잘못된 입력입니다." << endl;
                continue;
            }

            int realIndex = core.getShop().getRealIndex(choice - 1, core.getPlayer());
            string message;
            core.buyUpgrade(realIndex, message);
            cout << message << endl;
            cout << endl;
        }
    }

    void settleWorkers()
    {
        time_t now = time(nullptr);
        double seconds = difftime(now, lastTime);
        lastTime = now;

        if (seconds <= 0 || !core.isHunting())
        {
            return;
        }

        long long gpEarned = 0;
        int done = core.updateWorkers(seconds, gpEarned);
        if (done == 0)
        {
            return;
        }

        cout << endl;
        cout << "[알바] 그동안 알바가 " << done << "회 뽑았습니다. (+" << gpEarned << " GP)" << endl;

        if (!core.isHunting())
        {
            cout << "[알바] 알바가 이로치를 발견했습니다!" << endl;
            showCatch();
        }
    }

    void openWorkerOffice()
    {
        while (true)
        {
            const WorkerManager& workers = core.getWorkers();

            cout << "============== 알바 고용소 ==============" << endl;
            cout << " 보유 GP : " << core.getPlayer().getGp() << " GP" << endl;
            cout << " 알바는 지금 사냥 중인 전설을 대신 뽑아줍니다. (GP도 얻음)" << endl;
            cout << " 현재 " << workers.getTotalHired() << "명 근무 중, 초당 "
                 << fixed << setprecision(2) << workers.getDrawsPerSecond() << "회" << endl;
            cout << "-----------------------------------------" << endl;

            for (int i = 0; i < workers.getCount(); i++)
            {
                const Worker* w = workers.getWorker(i);

                cout << " " << (i + 1) << ". " << w->getName()
                     << " (" << w->getCount() << " / " << Worker::MAX_COUNT << "명)";
                if (w->isMaxed())
                {
                    cout << " [최대 고용]" << endl;
                }
                else
                {
                    cout << " - 고용 " << w->getPrice() << " GP" << endl;
                }
                cout << "     " << w->getDescription() << endl;
            }
            cout << " 0. 돌아가기" << endl;
            cout << "=========================================" << endl;
            cout << "고용할 번호 >> ";

            int choice = readInt();
            if (choice == 0)
            {
                return;
            }
            if (choice < 1 || choice > workers.getCount())
            {
                cout << "잘못된 입력입니다." << endl;
                continue;
            }

            string message;
            core.hireWorker(choice - 1, message);
            cout << message << endl;
            cout << endl;
        }
    }

    void showStatus()
    {
        cout << core.getPlayer() << endl;
        cout << " 도감 : " << core.getCaughtCount() << " / " << core.getLegendaryCount() << endl;

        const WorkerManager& workers = core.getWorkers();
        cout << " 알바 : " << workers.getTotalHired() << "명";
        for (int i = 0; i < workers.getCount(); i++)
        {
            cout << " / " << workers.getWorker(i)->getName() << " " << workers.getWorker(i)->getCount() << "명";
        }
        cout << endl;
    }

    void showCatch()
    {
        const Legendary& caught = core.getLegendary(core.getLastCaughtIndex());

        cout << endl;
        cout << "*****************************************" << endl;
        cout << "  ✦ 반짝반짝! 이로치 " << caught.getNameWithSubject() << " 나타났다! ✦" << endl;
        cout << "*****************************************" << endl;
        cout << " 이로치 " << caught.getName() << " 수집 성공!" << endl;
        cout << " 이번 사냥 뽑기 횟수 : " << caught.getCatchDraws() << "회" << endl;
        cout << " 수집 보상 : 뽑기당 GP +" << GameCore::COLLECT_BONUS_GP
             << " (현재 뽑기당 " << core.getPlayer().getGpPerDraw() << " GP)" << endl;
        cout << " 도감 : " << core.getCaughtCount() << " / " << core.getLegendaryCount() << endl;
    }

    void showFinalEnding()
    {
        cout << endl;
        cout << "*****************************************" << endl;
        cout << "  ✦ 모든 전설 포켓몬의 이로치를 수집했다! ✦" << endl;
        cout << "*****************************************" << endl;
        cout << core.getPlayer() << endl;
        cout << "축하합니다! 게임을 클리어했습니다." << endl;
        cout << "(처음부터 다시 하려면 " << SAVE_FILE << " 파일을 지우세요)" << endl;
    }

public:
    void run()
    {
        cout << "이로치 타이쿤에 오신 것을 환영합니다!" << endl;
        cout << "전설 포켓몬을 골라 GP를 모으고, 이로치를 수집하세요." << endl;

        if (core.loadFromFile(SAVE_FILE))
        {
            cout << "저장된 기록을 불러왔습니다. (도감 " << core.getCaughtCount()
                 << " / " << core.getLegendaryCount() << ")" << endl;
        }
        else
        {
            cout << "새 게임을 시작합니다." << endl;
        }

        lastTime = time(nullptr);

        while (true)
        {
            settleWorkers();

            if (core.isAllCaught())
            {
                showFinalEnding();
                save();
                return;
            }

            if (!core.isHunting())
            {
                if (!chooseTarget())
                {
                    save();
                    cout << "저장했습니다. 안녕히 가세요!" << endl;
                    return;
                }
                save();
                continue;
            }

            showMenu();
            int choice = readInt();

            settleWorkers();
            if (!core.isHunting())
            {
                continue;
            }

            if (choice == 1)
            {
                doSingleDraw();
            }
            else if (choice == 2)
            {
                doMultiDraw();
            }
            else if (choice == 3)
            {
                openShop();
            }
            else if (choice == 4)
            {
                showStatus();
            }
            else if (choice == 5)
            {
                chooseTarget();
            }
            else if (choice == 6)
            {
                openWorkerOffice();
            }
            else if (choice == 7)
            {
                save();
                cout << "저장했습니다. 안녕히 가세요!" << endl;
                return;
            }
            else
            {
                cout << "잘못된 입력입니다. 1~7 사이의 숫자를 입력해주세요." << endl;
            }

            save();
        }
    }
};

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    Game game;
    game.run();

    return 0;
}
