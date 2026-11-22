#include "raylib.h"

#include <string>
#include <vector>
#include <cmath>
#include <cstdio>

#include "../common/Player.h"
#include "../common/Upgrade.h"
#include "../common/Shop.h"
#include "../common/Legendary.h"
#include "../common/Worker.h"
#include "../common/WorkerManager.h"
#include "../common/GameCore.h"
#include "Backgrounds.h"

using namespace std;

const string SAVE_FILE = "save.txt";

const int SCREEN_W = 1100;
const int SCREEN_H = 750;

const Color COL_BG        = { 243, 245, 249, 255 };
const Color COL_PANEL     = { 255, 255, 255, 255 };
const Color COL_CARD      = { 248, 249, 252, 255 };
const Color COL_CARD_DARK = { 226, 229, 236, 255 };
const Color COL_CARD_GOLD = { 255, 250, 234, 255 };
const Color COL_BORDER    = { 222, 226, 234, 255 };
const Color COL_TEXT      = {  38,  43,  54, 255 };
const Color COL_SUB       = { 120, 127, 142, 255 };
const Color COL_ACCENT    = {  72, 104, 230, 255 };
const Color COL_GOLD      = { 232, 172,  40, 255 };
const Color COL_GOLD_TEXT = { 196, 136,  10, 255 };
const Color COL_GOOD      = {  46, 160, 100, 255 };
const Color COL_BAD       = { 214,  72,  72, 255 };
const Color COL_DISABLED  = { 214, 218, 226, 255 };
const Color TINT_UNCAUGHT = { 118, 121, 134, 255 };
const Color TINT_HOVER    = { 170, 173, 184, 255 };

Font gFont;
bool gHoverButton = false;
bool gInputBlocked = false;

void DrawK(const string& text, float x, float y, float size, Color color)
{
    DrawTextEx(gFont, text.c_str(), Vector2{ x, y }, size, 0.0f, color);
}

float MeasureK(const string& text, float size)
{
    return MeasureTextEx(gFont, text.c_str(), size, 0.0f).x;
}

void DrawKCenter(const string& text, Rectangle area, float size, Color color)
{
    Vector2 m = MeasureTextEx(gFont, text.c_str(), size, 0.0f);
    DrawK(text, area.x + (area.width - m.x) / 2.0f, area.y + (area.height - m.y) / 2.0f, size, color);
}

void DrawKRight(const string& text, float rightX, float y, float size, Color color)
{
    DrawK(text, rightX - MeasureK(text, size), y, size, color);
}

int DrawKWrapped(const string& text, float x, float y, float maxWidth, float size, Color color)
{
    vector<string> words;
    string word;
    for (size_t i = 0; i < text.size(); i++)
    {
        if (text[i] == ' ')
        {
            words.push_back(word);
            word = "";
        }
        else
        {
            word += text[i];
        }
    }
    words.push_back(word);

    string line;
    int lineCount = 0;
    for (size_t i = 0; i < words.size(); i++)
    {
        string test = line.empty() ? words[i] : line + " " + words[i];
        if (!line.empty() && MeasureK(test, size) > maxWidth)
        {
            DrawK(line, x, y + lineCount * (size + 4), size, color);
            lineCount++;
            line = words[i];
        }
        else
        {
            line = test;
        }
    }
    if (!line.empty())
    {
        DrawK(line, x, y + lineCount * (size + 4), size, color);
        lineCount++;
    }
    return lineCount;
}

void DrawPanel(Rectangle r, Color fill, Color border, float roundness)
{
    DrawRectangleRounded(r, roundness, 12, border);
    Rectangle inner = { r.x + 1, r.y + 1, r.width - 2, r.height - 2 };
    DrawRectangleRounded(inner, roundness, 12, fill);
}

void DrawPanelThick(Rectangle r, Color fill, Color border, float roundness, float thick)
{
    DrawRectangleRounded(r, roundness, 12, border);
    Rectangle inner = { r.x + thick, r.y + thick, r.width - thick * 2, r.height - thick * 2 };
    DrawRectangleRounded(inner, roundness, 12, fill);
}

bool IsHovered(Rectangle r)
{
    return !gInputBlocked && CheckCollisionPointRec(GetMousePosition(), r);
}

bool DrawButton(Rectangle r, const string& text, bool enabled, Color color, float textSize)
{
    bool hover = IsHovered(r);

    Color fill = color;
    Color textColor = WHITE;
    if (!enabled)
    {
        fill = COL_DISABLED;
        textColor = COL_SUB;
    }
    else if (hover && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        fill = ColorBrightness(color, -0.25f);
    }
    else if (hover)
    {
        fill = ColorBrightness(color, -0.12f);
    }

    if (enabled && hover)
    {
        gHoverButton = true;
    }

    DrawRectangleRounded(r, 0.3f, 10, fill);
    DrawKCenter(text, r, textSize, textColor);

    return enabled && hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

string WithComma(long long number)
{
    string digits = to_string(number);
    string result;
    int count = 0;
    for (int i = (int)digits.size() - 1; i >= 0; i--)
    {
        result.insert(result.begin(), digits[i]);
        count++;
        if (count % 3 == 0 && i > 0 && digits[i - 1] != '-')
        {
            result.insert(result.begin(), ',');
        }
    }
    return result;
}

string PercentText(double percent)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.4f%%", percent);
    return string(buffer);
}

string OneDecimal(double value)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.1f", value);
    return string(buffer);
}

bool DrawTab(Rectangle r, const string& text, bool selected)
{
    bool hover = IsHovered(r);
    Color fill = COL_CARD;
    Color textColor = COL_SUB;
    if (selected)
    {
        fill = COL_ACCENT;
        textColor = WHITE;
    }
    else if (hover)
    {
        fill = Color{ 230, 234, 244, 255 };
        gHoverButton = true;
    }
    DrawRectangleRounded(r, 0.3f, 10, fill);
    DrawKCenter(text, r, 17, textColor);
    return !selected && hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void DrawSparkle(float x, float y, float size, Color color)
{
    DrawLineEx(Vector2{ x - size, y }, Vector2{ x + size, y }, 3.0f, color);
    DrawLineEx(Vector2{ x, y - size }, Vector2{ x, y + size }, 3.0f, color);
    float s = size * 0.45f;
    DrawLineEx(Vector2{ x - s, y - s }, Vector2{ x + s, y + s }, 2.0f, color);
    DrawLineEx(Vector2{ x - s, y + s }, Vector2{ x + s, y - s }, 2.0f, color);
}

void DrawSparkleRing(float cx, float cy, float radius, float time)
{
    for (int i = 0; i < 6; i++)
    {
        float angle = time * 1.2f + i * (PI / 3.0f);
        float size = 7.0f + 3.0f * sinf(time * 5.0f + i);
        DrawSparkle(cx + cosf(angle) * radius, cy + sinf(angle) * radius * 0.8f, size, COL_GOLD);
    }
}

void DrawCreature(float cx, float cy, float scale, bool shiny, float time)
{
    Color body  = shiny ? Color{ 250, 206,  72, 255 } : Color{ 122, 190, 176, 255 };
    Color dark  = shiny ? Color{ 214, 150,  30, 255 } : Color{  78, 146, 134, 255 };
    Color belly = shiny ? Color{ 255, 236, 170, 255 } : Color{ 200, 234, 224, 255 };
    Color eye   = { 40, 44, 56, 255 };

    cy += sinf(time * 3.0f) * 3.0f * scale;

    DrawCircleV(Vector2{ cx - 34 * scale, cy - 40 * scale }, 17 * scale, dark);
    DrawCircleV(Vector2{ cx + 34 * scale, cy - 40 * scale }, 17 * scale, dark);
    DrawCircleV(Vector2{ cx, cy }, 50 * scale, body);
    DrawEllipse((int)cx, (int)(cy + 18 * scale), 30 * scale, 22 * scale, belly);
    DrawCircleV(Vector2{ cx - 17 * scale, cy - 10 * scale }, 6 * scale, eye);
    DrawCircleV(Vector2{ cx + 17 * scale, cy - 10 * scale }, 6 * scale, eye);
}

void DrawTextureFit(Texture2D texture, Rectangle box, Color tint)
{
    float scaleX = box.width / texture.width;
    float scaleY = box.height / texture.height;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    float w = texture.width * scale;
    float h = texture.height * scale;
    Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
    Rectangle dest = { box.x + (box.width - w) / 2.0f, box.y + (box.height - h) / 2.0f, w, h };
    DrawTexturePro(texture, source, dest, Vector2{ 0, 0 }, 0.0f, tint);
}

Rectangle ScaleRect(Rectangle r, float scale)
{
    float w = r.width * scale;
    float h = r.height * scale;
    return Rectangle{ r.x + (r.width - w) / 2.0f, r.y + (r.height - h) / 2.0f, w, h };
}

Font LoadKoreanFont(int size)
{
    vector<int> codepoints;
    for (int c = 32; c <= 126; c++)
    {
        codepoints.push_back(c);
    }
    for (int c = 0xAC00; c <= 0xD7A3; c++)
    {
        codepoints.push_back(c);
    }
    codepoints.push_back(0x00B7);
    codepoints.push_back(0x2026);

    const char* paths[] = {
        "font.ttf",
        "C:/Windows/Fonts/malgun.ttf",
        "/usr/share/fonts/truetype/nanum/NanumGothic.ttf",
        "/System/Library/Fonts/AppleSDGothicNeo.ttc"
    };

    for (int i = 0; i < 4; i++)
    {
        if (FileExists(paths[i]))
        {
            Font font = LoadFontEx(paths[i], size, codepoints.data(), (int)codepoints.size());
            if (font.texture.id != 0)
            {
                SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
                return font;
            }
        }
    }

    TraceLog(LOG_WARNING, "한글 글꼴을 찾지 못했습니다. font.ttf를 실행 폴더에 넣어주세요.");
    return GetFontDefault();
}

string FindImageFolder()
{
    vector<string> candidates;
    candidates.push_back("images/");
    candidates.push_back("../images/");
    candidates.push_back("../../images/");
    candidates.push_back(string(GetApplicationDirectory()) + "images/");

    string source = __FILE__;
    size_t slash = source.find_last_of("/\\");
    if (slash != string::npos)
    {
        candidates.push_back(source.substr(0, slash + 1) + "../images/");
    }

    for (size_t i = 0; i < candidates.size(); i++)
    {
        string test = candidates[i] + "dialga_normal.png";
        if (FileExists(test.c_str()))
        {
            return candidates[i];
        }
    }
    return "";
}

Texture2D LoadPokemonTexture(const string& path)
{
    Texture2D texture = { 0 };
    if (!FileExists(path.c_str()))
    {
        return texture;
    }

    texture = LoadTexture(path.c_str());
    if (texture.id != 0)
    {
        GenTextureMipmaps(&texture);
        SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);
    }
    return texture;
}

struct LogLine
{
    string text;
    Color color;
};

enum ScreenType
{
    SCREEN_SELECT,
    SCREEN_HUNT
};

class GuiGame
{
private:
    GameCore core;

    vector<Texture2D> normalImages;
    vector<Texture2D> shinyImages;

    vector<LogLine> logs;

    Rectangle fieldRect;
    Rectangle drawButtonRect;

    ScreenType screen;
    bool hasEncounter;
    string encounterTitle;
    string encounterSub;

    bool catchPopup;
    bool finalPopup;

    float shakeTimer;
    float popTimer;
    float holdTimer;
    float flashTimer;
    float time;

    bool unsavedDraws;
    float autoSaveTimer;
    bool quit;

    int shopTab;
    float manualDrawTimer;
    int workerDrawsThisSecond;
    long long workerGpThisSecond;
    int workerDrawsLastSecond;
    long long workerGpLastSecond;
    float workerSecondTimer;

    void addLog(const string& text, Color color)
    {
        LogLine line;
        line.text = text;
        line.color = color;
        logs.push_back(line);
        if (logs.size() > 4)
        {
            logs.erase(logs.begin());
        }
    }

    void saveGame()
    {
        if (!core.saveToFile(SAVE_FILE))
        {
            addLog("[경고] 저장에 실패했습니다.", COL_BAD);
        }
        unsavedDraws = false;
        autoSaveTimer = 0.0f;
    }

    bool isPopupOpen() const
    {
        return catchPopup || finalPopup || flashTimer > 0.0f;
    }

    void loadImages()
    {
        string folder = FindImageFolder();
        if (folder.empty())
        {
            addLog("images 폴더를 찾지 못해 기본 도형으로 표시합니다.", COL_BAD);
        }

        for (int i = 0; i < core.getLegendaryCount(); i++)
        {
            Texture2D normal = { 0 };
            Texture2D shiny = { 0 };
            if (!folder.empty())
            {
                string key = core.getLegendary(i).getImageKey();
                normal = LoadPokemonTexture(folder + key + "_normal.png");
                shiny = LoadPokemonTexture(folder + key + "_shiny.png");
            }
            normalImages.push_back(normal);
            shinyImages.push_back(shiny);
        }
    }

    void drawPokemon(int index, Rectangle box, bool shiny, Color tint)
    {
        Texture2D texture = shiny ? shinyImages[index] : normalImages[index];
        if (texture.id != 0)
        {
            DrawTextureFit(texture, box, tint);
        }
        else
        {
            float scale = box.height / 150.0f;
            DrawCreature(box.x + box.width / 2.0f, box.y + box.height / 2.0f, scale, shiny, time);
        }
    }

    void startHunt(int index)
    {
        string message;
        if (core.selectTarget(index, message))
        {
            addLog(message, COL_ACCENT);
            screen = SCREEN_HUNT;
            hasEncounter = false;
            saveGame();
        }
        else
        {
            addLog(message, COL_BAD);
        }
    }

    void doDraw()
    {
        if (!core.isHunting() || isPopupOpen())
        {
            return;
        }

        string subject = core.getLegendary(core.getTargetIndex()).getNameWithSubject();
        bool caught = core.singleDraw();
        manualDrawTimer = 0.0f;

        hasEncounter = true;
        shakeTimer = 0.3f;
        popTimer = 0.18f;
        unsavedDraws = true;

        if (caught)
        {
            onCaught();
        }
        else
        {
            encounterTitle = "야생 " + subject + " 나타났다!";
            encounterSub = "평범한 색이다... (+" + to_string(core.getPlayer().getGpPerDraw()) + " GP)";
        }
    }

    void doTicket(int index)
    {
        string name = core.getLegendary(core.getTargetIndex()).getName();
        string message;
        int done = core.useTicket(index, message);
        manualDrawTimer = 0.0f;
        if (done == -1)
        {
            addLog(message, COL_BAD);
            return;
        }

        hasEncounter = true;
        shakeTimer = 0.3f;
        popTimer = 0.18f;

        if (!core.isHunting())
        {
            addLog(message, COL_GOLD_TEXT);
            onCaught();
        }
        else
        {
            addLog(message + " (GP 획득 없음)", COL_TEXT);
            encounterTitle = name + " " + WithComma(done) + "마리 연속 조우!";
            encounterSub = "모두 평범한 색이었다... (연속 뽑기는 GP를 얻지 못해요)";
            saveGame();
        }
    }

    void updateWorkers(float dt)
    {
        if (!core.isHunting() || isPopupOpen())
        {
            return;
        }

        string name = core.getLegendary(core.getTargetIndex()).getName();
        long long gpEarned = 0;
        int done = core.updateWorkers(dt, gpEarned);

        workerDrawsThisSecond += done;
        workerGpThisSecond += gpEarned;
        workerSecondTimer += dt;
        if (workerSecondTimer >= 1.0f)
        {
            workerDrawsLastSecond = workerDrawsThisSecond;
            workerGpLastSecond = workerGpThisSecond;
            workerDrawsThisSecond = 0;
            workerGpThisSecond = 0;
            workerSecondTimer = 0.0f;
        }

        if (done == 0)
        {
            return;
        }
        unsavedDraws = true;

        if (!core.isHunting())
        {
            addLog("알바가 이로치를 발견했습니다!", COL_GOLD_TEXT);
            hasEncounter = true;
            onCaught();
            return;
        }

        if (manualDrawTimer > 1.5f)
        {
            hasEncounter = true;
            if (shakeTimer < 0.12f)
            {
                shakeTimer = 0.12f;
            }
            encounterTitle = "알바 트레이너들이 " + name + "의 이로치를 찾는 중!";
            encounterSub = "알바 뽑기 초당 " + to_string(workerDrawsLastSecond) + "회 (+" + WithComma(workerGpLastSecond) + " GP)";
        }
    }

    void onCaught()
    {
        const Legendary& caught = core.getLegendary(core.getLastCaughtIndex());

        encounterTitle = "반짝반짝! 이로치 " + caught.getNameWithSubject() + " 나타났다!";
        encounterSub = "";
        addLog("이로치 " + caught.getName() + " 수집! (" + WithComma(caught.getCatchDraws()) + "회 만에)", COL_GOLD_TEXT);
        addLog("수집 보상 : 뽑기당 GP +" + to_string(GameCore::COLLECT_BONUS_GP), COL_GOOD);

        flashTimer = 0.6f;
        catchPopup = true;
        holdTimer = 1.0f;
        saveGame();
    }

    void drawTopBar()
    {
        const Player& p = core.getPlayer();

        DrawRectangle(0, 0, SCREEN_W, 72, COL_PANEL);
        DrawRectangle(0, 72, SCREEN_W, 1, COL_BORDER);
        DrawSparkle(38, 36, 10, COL_GOLD);
        DrawK("이로치 타이쿤", 58, 20, 30, COL_TEXT);

        string labels[5] = { "보유 GP", "이로치 확률", "뽑기당 GP", "총 뽑기", "도감" };
        string values[5];
        values[0] = WithComma(p.getGp());
        values[1] = to_string(p.getShinyNumerator()) + "/" + to_string(p.getShinyDenominator())
                  + "  (" + PercentText(p.getShinyPercent()) + ")";
        values[2] = "+" + to_string(p.getGpPerDraw());
        values[3] = WithComma(p.getTotalDraws()) + "회";
        values[4] = to_string(core.getCaughtCount()) + " / " + to_string(core.getLegendaryCount());

        float widths[5] = { 140, 220, 100, 130, 110 };
        float x = SCREEN_W - 24;
        for (int i = 4; i >= 0; i--)
        {
            x -= widths[i];
            Rectangle chip = { x, 12, widths[i] - 10, 48 };
            DrawPanel(chip, COL_CARD, COL_BORDER, 0.3f);
            DrawK(labels[i], chip.x + 12, chip.y + 5, 14, COL_SUB);

            Color valueColor = COL_TEXT;
            if (i == 0)
            {
                valueColor = COL_ACCENT;
            }
            else if (i == 4)
            {
                valueColor = COL_GOLD_TEXT;
            }
            DrawK(values[i], chip.x + 12, chip.y + 22, 20, valueColor);
        }
    }

    void drawSelectScreen()
    {
        Rectangle panel = { 24, 92, 1052, 634 };
        DrawPanel(panel, COL_PANEL, COL_BORDER, 0.03f);

        float x = panel.x + 16;
        float w = panel.width - 32;

        if (core.isAllCaught())
        {
            DrawK("모든 전설의 이로치를 수집했습니다!", x + 8, panel.y + 18, 24, COL_GOLD_TEXT);
            DrawK("처음부터 다시 하려면 save.txt 파일을 지우세요.", x + 8, panel.y + 52, 15, COL_SUB);
        }
        else
        {
            DrawK("어떤 전설의 이로치를 노릴까요?", x + 8, panel.y + 18, 24, COL_TEXT);
            DrawK("카드를 눌러 사냥할 전설을 고르세요. 수집할 때마다 뽑기당 GP +"
                  + to_string(GameCore::COLLECT_BONUS_GP), x + 8, panel.y + 52, 15, COL_SUB);
        }

        float rightX = panel.x + panel.width - 24;
        if (core.isHunting())
        {
            Rectangle back = { rightX - 170, panel.y + 20, 170, 38 };
            if (DrawButton(back, "사냥으로 돌아가기", true, COL_ACCENT, 17))
            {
                screen = SCREEN_HUNT;
            }
            rightX -= 190;
        }
        DrawKRight("도감 " + to_string(core.getCaughtCount()) + " / " + to_string(core.getLegendaryCount()),
                   rightX, panel.y + 26, 22, COL_GOLD_TEXT);

        const int COLS = 6;
        const float GAP = 12;
        float cardW = (w - GAP * (COLS - 1)) / COLS;
        float cardH = 124;
        float startY = panel.y + 84;

        for (int i = 0; i < core.getLegendaryCount(); i++)
        {
            int col = i % COLS;
            int row = i / COLS;
            Rectangle card = { x + col * (cardW + GAP), startY + row * (cardH + GAP), cardW, cardH };
            drawLegendaryCard(i, card);
        }
    }

    void drawLegendaryCard(int index, Rectangle card)
    {
        const Legendary& l = core.getLegendary(index);
        bool caught = l.isCaught();
        bool isTarget = (index == core.getTargetIndex());
        bool hover = IsHovered(card);
        bool selectable = !caught;

        if (caught)
        {
            DrawPanelThick(card, COL_CARD_GOLD, COL_GOLD, 0.12f, 2.5f);
        }
        else if (isTarget || (hover && selectable))
        {
            DrawPanelThick(card, COL_CARD, COL_ACCENT, 0.12f, 2.0f);
        }
        else
        {
            DrawPanel(card, COL_CARD_DARK, COL_BORDER, 0.12f);
        }

        Rectangle imageBox = { card.x + 8, card.y + 6, card.width - 16, 76 };
        if (caught)
        {
            drawPokemon(index, imageBox, true, WHITE);
        }
        else if (hover || isTarget)
        {
            drawPokemon(index, imageBox, false, TINT_HOVER);
        }
        else
        {
            drawPokemon(index, imageBox, false, TINT_UNCAUGHT);
        }

        DrawKCenter(l.getName(), Rectangle{ card.x, card.y + 82, card.width, 22 }, 17,
                    caught ? COL_TEXT : COL_SUB);

        string status;
        Color statusColor = COL_SUB;
        if (caught)
        {
            status = "수집 완료 · " + WithComma(l.getCatchDraws()) + "회";
            statusColor = COL_GOLD_TEXT;
        }
        else if (isTarget)
        {
            status = "사냥 중 · " + WithComma(core.getHuntDraws()) + "회";
            statusColor = COL_ACCENT;
        }
        else if (hover)
        {
            status = "눌러서 사냥 시작";
            statusColor = COL_ACCENT;
        }
        else
        {
            status = "미수집";
        }
        DrawKCenter(status, Rectangle{ card.x, card.y + 102, card.width, 18 }, 13, statusColor);

        if (selectable && hover)
        {
            gHoverButton = true;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                startHunt(index);
            }
        }
    }

    void drawField()
    {
        Rectangle panel = { 24, 92, 620, 380 };
        DrawPanel(panel, COL_PANEL, COL_BORDER, 0.05f);

        int index = core.isHunting() ? core.getTargetIndex() : core.getLastCaughtIndex();
        if (index == -1)
        {
            return;
        }
        const Legendary& l = core.getLegendary(index);
        SceneTheme theme = GetSceneTheme(l.getImageKey());
        float shake = shakeTimer / 0.3f;

        DrawSceneBack(theme, fieldRect, time);
        DrawSceneBackGrass(theme, fieldRect, time, shake);

        if (hasEncounter)
        {
            float scale = 1.0f - (popTimer / 0.18f) * 0.2f;
            float bob = sinf(time * 2.5f) * 3.0f;
            Rectangle box = { fieldRect.x + fieldRect.width / 2.0f - 130, fieldRect.y + 112 + bob, 260, 220 };
            box = ScaleRect(box, scale);

            DrawSceneSpotlight(theme, Vector2{ box.x + box.width / 2, box.y + box.height / 2 }, 160);
            DrawEllipse((int)(box.x + box.width / 2), (int)(fieldRect.y + 318), 90, 12, Fade(BLACK, 0.18f));
            drawPokemon(index, box, !core.isHunting(), WHITE);
            if (!core.isHunting())
            {
                DrawSparkleRing(box.x + box.width / 2, box.y + box.height / 2, 135, time);
            }
        }
        else
        {
            Rectangle box = { fieldRect.x + fieldRect.width / 2.0f - 130, fieldRect.y + 112, 260, 220 };
            drawPokemon(index, box, false, Color{ 200, 204, 216, 90 });
        }

        DrawSceneFront(theme, fieldRect, time, shake);

        float nameWidth = MeasureK(l.getName(), 22);
        string huntText = core.isHunting() ? "이번 사냥 " + WithComma(core.getHuntDraws()) + "회" : "수집 완료";
        float infoWidth = nameWidth + 20 + MeasureK(huntText, 15) + 24;
        DrawRectangleRounded(Rectangle{ fieldRect.x + 10, fieldRect.y + 10, infoWidth, 50 }, 0.3f, 8, Fade(WHITE, 0.85f));
        DrawK("목표", fieldRect.x + 22, fieldRect.y + 13, 13, COL_SUB);
        DrawK(l.getName(), fieldRect.x + 22, fieldRect.y + 29, 22, COL_TEXT);
        DrawK(huntText, fieldRect.x + 30 + nameWidth, fieldRect.y + 35, 15, COL_SUB);

        Rectangle changeButton = { fieldRect.x + fieldRect.width - 122, fieldRect.y + 14, 108, 34 };
        if (DrawButton(changeButton, "목표 변경", true, COL_SUB, 16))
        {
            screen = SCREEN_SELECT;
        }

        string title;
        string sub;
        Color titleColor = core.isHunting() ? COL_TEXT : COL_GOLD_TEXT;
        if (hasEncounter)
        {
            title = encounterTitle;
            sub = encounterSub;
        }
        else
        {
            title = l.getName() + "의 이로치를 찾아 뽑기를 시작하세요!";
            titleColor = COL_SUB;
        }

        float titleWidth = MeasureK(title, 19);
        if (!sub.empty() && MeasureK(sub, 14) > titleWidth)
        {
            titleWidth = MeasureK(sub, 14);
        }
        float pillH = sub.empty() ? 32.0f : 52.0f;
        Rectangle pill = { fieldRect.x + (fieldRect.width - titleWidth) / 2 - 18, fieldRect.y + 66, titleWidth + 36, pillH };
        DrawRectangleRounded(pill, 0.4f, 8, Fade(WHITE, 0.85f));
        DrawKCenter(title, Rectangle{ pill.x, pill.y + 4, pill.width, 24 }, 19, titleColor);
        if (!sub.empty())
        {
            DrawKCenter(sub, Rectangle{ pill.x, pill.y + 28, pill.width, 20 }, 14, COL_SUB);
        }
    }

    void drawDrawControls()
    {
        bool enabled = core.isHunting() && !isPopupOpen();
        bool clicked = DrawButton(drawButtonRect, "뽑기", enabled, COL_ACCENT, 26);

        bool mouseHeld = enabled && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsHovered(drawButtonRect);
        bool keyPressed = enabled && IsKeyPressed(KEY_SPACE);
        bool keyHeld = enabled && IsKeyDown(KEY_SPACE);

        if (clicked || keyPressed)
        {
            doDraw();
            holdTimer = 0.35f;
        }
        else if (mouseHeld || keyHeld)
        {
            holdTimer -= GetFrameTime();
            if (holdTimer <= 0.0f)
            {
                doDraw();
                holdTimer = 0.07f;
            }
        }

        float tx = drawButtonRect.x + drawButtonRect.width + 20;
        DrawK("1회당 +" + to_string(core.getPlayer().getGpPerDraw()) + " GP", tx, drawButtonRect.y + 2, 20, COL_ACCENT);
        DrawK("Space 키나 버튼을 누르고 있으면 계속 뽑아요", tx, drawButtonRect.y + 28, 15, COL_SUB);

        const WorkerManager& workers = core.getWorkers();
        if (workers.getTotalHired() > 0)
        {
            string text = "알바 " + to_string(workers.getTotalHired()) + "명 근무 중 · 초당 "
                        + OneDecimal(workers.getDrawsPerSecond()) + "회 자동 뽑기";
            DrawK(text, tx, drawButtonRect.y + 47, 15, core.isHunting() ? COL_GOOD : COL_SUB);
        }
    }

    void drawLog()
    {
        Rectangle panel = { 24, 576, 620, 150 };
        DrawPanel(panel, COL_PANEL, COL_BORDER, 0.08f);
        DrawK("기록", panel.x + 18, panel.y + 12, 16, COL_SUB);

        if (logs.empty())
        {
            DrawK("아직 기록이 없습니다.", panel.x + 18, panel.y + 42, 17, COL_DISABLED);
        }
        for (size_t i = 0; i < logs.size(); i++)
        {
            DrawK(logs[i].text, panel.x + 18, panel.y + 40 + i * 26, 17, logs[i].color);
        }
    }

    void drawShop()
    {
        const Player& p = core.getPlayer();
        const Shop& shop = core.getShop();

        Rectangle panel = { 664, 92, 412, 634 };
        DrawPanel(panel, COL_PANEL, COL_BORDER, 0.04f);

        float x = panel.x + 16;
        float w = panel.width - 32;
        float y = panel.y + 16;

        Rectangle tab1 = { x, y - 4, (w - 8) / 2, 38 };
        Rectangle tab2 = { x + (w - 8) / 2 + 8, y - 4, (w - 8) / 2, 38 };
        if (DrawTab(tab1, "업그레이드", shopTab == 0))
        {
            shopTab = 0;
        }
        if (DrawTab(tab2, "알바 (" + to_string(core.getWorkers().getTotalHired()) + "명)", shopTab == 1))
        {
            shopTab = 1;
        }
        y += 44;

        if (shopTab == 1)
        {
            drawWorkerTab(x, y, w);
            return;
        }

        int visibleCount = shop.getVisibleCount(p);
        for (int i = 0; i < visibleCount; i++)
        {
            int realIndex = shop.getRealIndex(i, p);
            const Upgrade* u = shop.getUpgrade(realIndex);
            if (u == nullptr)
            {
                break;
            }

            Rectangle card = { x, y, w, 112 };
            DrawPanel(card, COL_CARD, COL_BORDER, 0.12f);

            DrawK(u->getName(), card.x + 14, card.y + 12, 19, COL_TEXT);
            DrawKRight(WithComma(u->getPrice()) + " GP", card.x + card.width - 14, card.y + 12, 19, COL_ACCENT);
            DrawKWrapped(u->getDescription(), card.x + 14, card.y + 40, card.width - 28, 15, COL_SUB);

            Rectangle button = { card.x + card.width - 112, card.y + 76, 98, 28 };

            if (u->isPurchased())
            {
                DrawKRight("구매 완료", card.x + card.width - 18, card.y + 81, 16, COL_GOOD);
            }
            else if (!u->isAvailable(p))
            {
                DrawK("잠김 : " + u->getUnavailableReason(p), card.x + 14, card.y + 82, 14, COL_BAD);
            }
            else
            {
                float ratio = (float)p.getGp() / u->getPrice();
                if (ratio > 1.0f)
                {
                    ratio = 1.0f;
                }
                Rectangle bar = { card.x + 14, card.y + 86, card.width - 150, 8 };
                DrawRectangleRounded(bar, 1.0f, 8, COL_DISABLED);
                Rectangle filled = { bar.x, bar.y, bar.width * ratio, bar.height };
                if (filled.width > 2)
                {
                    DrawRectangleRounded(filled, 1.0f, 8, ratio >= 1.0f ? COL_GOOD : COL_ACCENT);
                }

                bool enoughGp = p.getGp() >= u->getPrice();
                if (DrawButton(button, enoughGp ? "구매" : "GP 부족", enoughGp, COL_ACCENT, 16))
                {
                    string message;
                    bool ok = core.buyUpgrade(realIndex, message);
                    addLog(message, ok ? COL_GOOD : COL_BAD);
                    saveGame();
                    break;
                }
            }
            y += 122;
        }

        y = panel.y + 60 + 3 * 122 + 6;
        DrawRectangle((int)x, (int)y - 8, (int)w, 1, COL_BORDER);
        DrawK("연속 뽑기 티켓", x, y + 4, 22, COL_TEXT);
        DrawK("연속 뽑기 중에는 GP를 얻지 못해요", x, y + 34, 14, COL_SUB);
        y += 62;

        for (int i = 0; i < GameCore::TICKET_COUNT; i++)
        {
            int draws = core.getTicketDraws(i);
            int price = core.getTicketPrice(i);

            Rectangle row = { x, y, w, 38 };
            DrawPanel(row, COL_CARD, COL_BORDER, 0.3f);
            DrawK(WithComma(draws) + "회 연속 뽑기", row.x + 14, row.y + 9, 18, COL_TEXT);

            Rectangle button = { row.x + row.width - 124, row.y + 5, 116, 28 };
            bool canBuy = core.isHunting() && p.getGp() >= price;
            if (DrawButton(button, WithComma(price) + " GP", canBuy, COL_ACCENT, 16))
            {
                doTicket(i);
            }
            y += 44;
        }
    }

    void drawWorkerTab(float x, float y, float w)
    {
        const Player& p = core.getPlayer();
        const WorkerManager& workers = core.getWorkers();

        for (int i = 0; i < workers.getCount(); i++)
        {
            const Worker* worker = workers.getWorker(i);

            Rectangle card = { x, y, w, 128 };
            DrawPanel(card, COL_CARD, COL_BORDER, 0.12f);

            DrawK(worker->getName(), card.x + 14, card.y + 12, 19, COL_TEXT);
            DrawKRight(to_string(worker->getCount()) + " / " + to_string(Worker::MAX_COUNT) + "명",
                       card.x + card.width - 14, card.y + 12, 18, COL_ACCENT);
            DrawKWrapped(worker->getDescription(), card.x + 14, card.y + 40, card.width - 28, 15, COL_SUB);

            for (int k = 0; k < Worker::MAX_COUNT; k++)
            {
                float cx = card.x + 22 + k * 20;
                float cy = card.y + 100;
                if (k < worker->getCount())
                {
                    if (core.isHunting())
                    {
                        cy -= fabsf(sinf(time * 6.0f + k * 0.7f + i)) * 4.0f;
                    }
                    DrawCircleV(Vector2{ cx, cy }, 7, COL_ACCENT);
                }
                else
                {
                    DrawCircleV(Vector2{ cx, cy }, 7, COL_DISABLED);
                }
            }

            Rectangle button = { card.x + card.width - 132, card.y + 86, 118, 30 };
            if (worker->isMaxed())
            {
                DrawButton(button, "최대 고용", false, COL_ACCENT, 15);
            }
            else
            {
                bool canHire = p.getGp() >= worker->getPrice();
                if (DrawButton(button, "고용 " + WithComma(worker->getPrice()) + " GP", canHire, COL_GOOD, 15))
                {
                    string message;
                    bool ok = core.hireWorker(i, message);
                    addLog(message, ok ? COL_GOOD : COL_BAD);
                    saveGame();
                }
            }
            y += 138;
        }

        Rectangle box = { x, y + 4, w, 132 };
        DrawPanel(box, COL_PANEL, COL_BORDER, 0.12f);
        double rate = workers.getDrawsPerSecond();
        DrawK("근무 중 " + to_string(workers.getTotalHired()) + "명", box.x + 14, box.y + 12, 19, COL_TEXT);
        DrawK("초당 " + OneDecimal(rate) + "회 자동 뽑기", box.x + 14, box.y + 42, 16, COL_GOOD);
        DrawK("예상 수입 : 초당 약 +" + WithComma((long long)(rate * p.getGpPerDraw())) + " GP",
              box.x + 14, box.y + 66, 16, COL_ACCENT);
        DrawKWrapped("알바는 지금 사냥 중인 전설을 대신 뽑아요. 목표가 없으면 쉬어요.",
                     box.x + 14, box.y + 94, box.width - 28, 14, COL_SUB);
    }

    void drawCatchPopup()
    {
        int index = core.getLastCaughtIndex();
        const Legendary& l = core.getLegendary(index);

        DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.55f));

        Rectangle box = { SCREEN_W / 2.0f - 290, SCREEN_H / 2.0f - 265, 580, 530 };
        DrawPanelThick(box, COL_PANEL, COL_GOLD, 0.06f, 3.0f);

        DrawKCenter("반짝반짝! 이로치 " + l.getName() + " 수집!", Rectangle{ box.x, box.y + 24, box.width, 40 }, 28, COL_GOLD_TEXT);

        SceneTheme theme = GetSceneTheme(l.getImageKey());
        Rectangle sceneRect = { box.x + 30, box.y + 76, box.width - 60, 262 };
        DrawSceneBack(theme, sceneRect, time);
        Rectangle imageBox = { box.x + box.width / 2.0f - 130, box.y + 92, 260, 230 };
        DrawSceneSpotlight(theme, Vector2{ imageBox.x + imageBox.width / 2, imageBox.y + imageBox.height / 2 }, 160);
        DrawEllipse((int)(imageBox.x + imageBox.width / 2), (int)(imageBox.y + imageBox.height - 12), 90, 12, Fade(BLACK, 0.18f));
        drawPokemon(index, imageBox, true, WHITE);
        DrawSceneFront(theme, sceneRect, time, 0.0f);
        BeginScissorMode((int)sceneRect.x, (int)sceneRect.y, (int)sceneRect.width, (int)sceneRect.height);
        DrawSparkleRing(imageBox.x + imageBox.width / 2, imageBox.y + imageBox.height / 2, 140, time);
        EndScissorMode();

        string lines[3];
        lines[0] = "이번 사냥 " + WithComma(l.getCatchDraws()) + "회 만에 만났습니다";
        lines[1] = "수집 보상 : 뽑기당 GP +" + to_string(GameCore::COLLECT_BONUS_GP)
                 + "  (현재 +" + to_string(core.getPlayer().getGpPerDraw()) + ")";
        lines[2] = "도감 " + to_string(core.getCaughtCount()) + " / " + to_string(core.getLegendaryCount());

        for (int i = 0; i < 3; i++)
        {
            DrawKCenter(lines[i], Rectangle{ box.x, box.y + 348 + i * 32.0f, box.width, 28 }, 19,
                        i == 1 ? COL_GOOD : COL_TEXT);
        }

        Rectangle button = { box.x + box.width / 2.0f - 100, box.y + 462, 200, 44 };
        if (DrawButton(button, "도감으로", true, COL_GOLD, 20))
        {
            catchPopup = false;
            hasEncounter = false;
            screen = SCREEN_SELECT;
            if (core.isAllCaught())
            {
                finalPopup = true;
            }
        }
    }

    void drawFinalPopup()
    {
        const Player& p = core.getPlayer();

        DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.55f));

        Rectangle box = { SCREEN_W / 2.0f - 330, SCREEN_H / 2.0f - 290, 660, 580 };
        DrawPanelThick(box, COL_PANEL, COL_GOLD, 0.05f, 3.0f);

        DrawKCenter("모든 전설 포켓몬의 이로치를 수집했다!", Rectangle{ box.x, box.y + 24, box.width, 40 }, 28, COL_GOLD_TEXT);

        const int COLS = 8;
        float size = 70;
        float gridX = box.x + (box.width - COLS * size) / 2.0f;
        float gridY = box.y + 84;
        for (int i = 0; i < core.getLegendaryCount(); i++)
        {
            Rectangle cell = { gridX + (i % COLS) * size, gridY + (i / COLS) * size, size, size };
            drawPokemon(i, ScaleRect(cell, 0.92f), true, WHITE);
        }

        string lines[3];
        lines[0] = "총 뽑기 횟수 : " + WithComma(p.getTotalDraws()) + "회";
        lines[1] = "최종 보유 GP : " + WithComma(p.getGp()) + " GP";
        lines[2] = "최종 이로치 확률 : " + to_string(p.getShinyNumerator()) + "/"
                 + to_string(p.getShinyDenominator()) + " (" + PercentText(p.getShinyPercent()) + ")";
        for (int i = 0; i < 3; i++)
        {
            DrawKCenter(lines[i], Rectangle{ box.x, box.y + 316 + i * 32.0f, box.width, 28 }, 19, COL_TEXT);
        }
        DrawKCenter("축하합니다! 게임을 클리어했습니다.", Rectangle{ box.x, box.y + 424, box.width, 24 }, 17, COL_SUB);

        Rectangle viewButton = { box.x + box.width / 2.0f - 210, box.y + 486, 200, 46 };
        Rectangle quitButton = { box.x + box.width / 2.0f + 10, box.y + 486, 200, 46 };
        if (DrawButton(viewButton, "도감 구경하기", true, COL_ACCENT, 19))
        {
            finalPopup = false;
        }
        if (DrawButton(quitButton, "게임 종료", true, COL_GOLD, 19))
        {
            quit = true;
        }
    }

public:
    GuiGame()
    {
        fieldRect = Rectangle{ 40, 108, 588, 348 };
        drawButtonRect = Rectangle{ 24, 488, 240, 64 };

        hasEncounter = false;
        catchPopup = false;
        finalPopup = false;
        shakeTimer = 0.0f;
        popTimer = 0.0f;
        holdTimer = 0.0f;
        flashTimer = 0.0f;
        time = 0.0f;
        unsavedDraws = false;
        autoSaveTimer = 0.0f;
        quit = false;

        shopTab = 0;
        manualDrawTimer = 100.0f;
        workerDrawsThisSecond = 0;
        workerGpThisSecond = 0;
        workerDrawsLastSecond = 0;
        workerGpLastSecond = 0;
        workerSecondTimer = 0.0f;

        if (core.loadFromFile(SAVE_FILE))
        {
            addLog("저장된 기록을 불러왔습니다.", COL_SUB);
        }
        else
        {
            addLog("이로치 타이쿤에 오신 것을 환영합니다!", COL_TEXT);
        }

        loadImages();

        screen = core.isHunting() ? SCREEN_HUNT : SCREEN_SELECT;
    }

    ~GuiGame()
    {
        for (size_t i = 0; i < normalImages.size(); i++)
        {
            if (normalImages[i].id != 0) UnloadTexture(normalImages[i]);
            if (shinyImages[i].id != 0) UnloadTexture(shinyImages[i]);
        }
    }

    GuiGame(const GuiGame&) = delete;
    GuiGame& operator=(const GuiGame&) = delete;

    void run()
    {
        while (!WindowShouldClose() && !quit)
        {
            float dt = GetFrameTime();
            time += dt;
            if (shakeTimer > 0.0f) shakeTimer -= dt;
            if (popTimer > 0.0f)   popTimer -= dt;
            if (flashTimer > 0.0f) flashTimer -= dt;
            if (shakeTimer < 0.0f) shakeTimer = 0.0f;
            if (popTimer < 0.0f)   popTimer = 0.0f;
            if (flashTimer < 0.0f) flashTimer = 0.0f;

            manualDrawTimer += dt;
            updateWorkers(dt);

            if (unsavedDraws)
            {
                autoSaveTimer += dt;
                if (autoSaveTimer >= 3.0f)
                {
                    saveGame();
                }
            }

            gHoverButton = false;
            gInputBlocked = isPopupOpen();

            BeginDrawing();
            ClearBackground(COL_BG);

            drawTopBar();
            if (screen == SCREEN_SELECT)
            {
                drawSelectScreen();
            }
            else
            {
                drawField();
                drawDrawControls();
                drawLog();
                drawShop();
            }

            if (flashTimer > 0.0f)
            {
                DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(WHITE, flashTimer / 0.6f));
            }
            else
            {
                gInputBlocked = false;
                if (catchPopup)
                {
                    drawCatchPopup();
                }
                else if (finalPopup)
                {
                    drawFinalPopup();
                }
            }

            EndDrawing();

            SetMouseCursor(gHoverButton ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);
        }

        saveGame();
    }
};

int main()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(SCREEN_W, SCREEN_H, "이로치 타이쿤");
    SetTargetFPS(60);

    BeginDrawing();
    ClearBackground(COL_BG);
    DrawText("Loading...", SCREEN_W / 2 - 60, SCREEN_H / 2 - 10, 24, COL_SUB);
    EndDrawing();

    gFont = LoadKoreanFont(32);

    {
        GuiGame game;
        game.run();
    }

    UnloadFont(gFont);
    CloseWindow();
    return 0;
}
