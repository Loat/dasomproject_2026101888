#ifndef BACKGROUNDS_H
#define BACKGROUNDS_H

#include "raylib.h"
#include <string>
#include <cmath>

using namespace std;

enum SceneType
{
    SCENE_MEADOW,
    SCENE_CAVE,
    SCENE_NIGHT_SEA,
    SCENE_SUNSET,
    SCENE_VOLCANO,
    SCENE_STORM_SEA,
    SCENE_HIGH_SKY,
    SCENE_TEMPLE,
    SCENE_DISTORTION,
    SCENE_FIRE_DESERT,
    SCENE_THUNDER,
    SCENE_ICE_CAVE,
    SCENE_GLOW_FOREST,
    SCENE_RED_RUINS,
    SCENE_MOSS_ROCKS,
    SCENE_SUN_ALTAR,
    SCENE_MOON_NIGHT,
    SCENE_SPACE,
    SCENE_FOG_FOREST,
    SCENE_RED_NIGHT,
    SCENE_JUNGLE,
    SCENE_CITY,
    SCENE_CRYSTAL_CAVE
};

enum ParticleType
{
    PARTICLE_NONE,
    PARTICLE_SNOW,
    PARTICLE_RAIN,
    PARTICLE_EMBER,
    PARTICLE_STAR,
    PARTICLE_MOTE,
    PARTICLE_ASH,
    PARTICLE_CELL,
    PARTICLE_FOG
};

struct SceneTheme
{
    SceneType scene;
    Color skyTop;
    Color skyBottom;
    Color groundTop;
    Color groundBottom;
    Color accent;
    ParticleType particle;
    Color particleColor;
    bool grass;
    Color grassBack;
    Color grassFront;
};

inline float SceneRand(int i, int k)
{
    float v = sinf(i * 12.9898f + k * 78.233f) * 43758.5453f;
    return v - floorf(v);
}

inline Color MixColor(Color a, Color b, float t)
{
    Color c;
    c.r = (unsigned char)(a.r + ((int)b.r - (int)a.r) * t);
    c.g = (unsigned char)(a.g + ((int)b.g - (int)a.g) * t);
    c.b = (unsigned char)(a.b + ((int)b.b - (int)a.b) * t);
    c.a = 255;
    return c;
}

inline float WrapValue(float value, float range)
{
    float v = fmodf(value, range);
    if (v < 0)
    {
        v += range;
    }
    return v;
}

inline void SceneTri(Vector2 a, Vector2 b, Vector2 c, Color color)
{
    float cross = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    if (cross > 0)
    {
        DrawTriangle(a, c, b, color);
    }
    else
    {
        DrawTriangle(a, b, c, color);
    }
}

inline void SceneGlow(Vector2 center, float radius, Color color, float strength)
{
    for (int k = 16; k >= 1; k--)
    {
        DrawCircleV(center, radius * k / 16.0f, Fade(color, 0.035f * strength));
    }
}

inline Color C(int r, int g, int b)
{
    Color c = { (unsigned char)r, (unsigned char)g, (unsigned char)b, 255 };
    return c;
}

inline SceneTheme MakeTheme(SceneType scene, Color skyTop, Color skyBottom, Color groundTop, Color groundBottom,
                            Color accent, ParticleType particle, Color particleColor)
{
    SceneTheme t;
    t.scene = scene;
    t.skyTop = skyTop;
    t.skyBottom = skyBottom;
    t.groundTop = groundTop;
    t.groundBottom = groundBottom;
    t.accent = accent;
    t.particle = particle;
    t.particleColor = particleColor;
    t.grass = false;
    t.grassBack = C(98, 172, 92);
    t.grassFront = C(70, 140, 70);
    return t;
}

inline SceneTheme WithGrass(SceneTheme t, Color back, Color front)
{
    t.grass = true;
    t.grassBack = back;
    t.grassFront = front;
    return t;
}

inline SceneTheme GetSceneTheme(const string& key)
{
    if (key == "mewtwo")
        return MakeTheme(SCENE_CAVE, C(34, 24, 52), C(78, 58, 108), C(74, 62, 92), C(40, 32, 54),
                         C(190, 120, 255), PARTICLE_MOTE, C(200, 150, 255));
    if (key == "lugia")
        return MakeTheme(SCENE_NIGHT_SEA, C(14, 24, 56), C(52, 96, 140), C(34, 78, 120), C(14, 40, 74),
                         C(220, 235, 255), PARTICLE_MOTE, C(220, 240, 255));
    if (key == "hooh")
        return WithGrass(MakeTheme(SCENE_SUNSET, C(110, 80, 160), C(255, 176, 110), C(196, 160, 86), C(150, 110, 60),
                                   C(255, 150, 60), PARTICLE_MOTE, C(255, 210, 110)),
                         C(196, 150, 70), C(160, 110, 50));
    if (key == "groudon")
        return MakeTheme(SCENE_VOLCANO, C(56, 22, 24), C(196, 78, 42), C(74, 46, 40), C(38, 24, 22),
                         C(255, 120, 30), PARTICLE_EMBER, C(255, 150, 50));
    if (key == "kyogre")
        return MakeTheme(SCENE_STORM_SEA, C(36, 44, 62), C(96, 112, 132), C(30, 66, 104), C(12, 34, 64),
                         C(200, 220, 240), PARTICLE_RAIN, C(190, 210, 235));
    if (key == "rayquaza")
        return MakeTheme(SCENE_HIGH_SKY, C(24, 48, 120), C(136, 186, 238), C(236, 242, 252), C(196, 212, 236),
                         C(255, 255, 255), PARTICLE_NONE, C(255, 255, 255));
    if (key == "dialga")
        return MakeTheme(SCENE_TEMPLE, C(36, 64, 116), C(150, 190, 222), C(186, 192, 210), C(132, 140, 166),
                         C(80, 150, 230), PARTICLE_MOTE, C(140, 200, 255));
    if (key == "palkia")
        return MakeTheme(SCENE_TEMPLE, C(86, 48, 100), C(222, 170, 204), C(204, 186, 206), C(150, 128, 156),
                         C(230, 110, 170), PARTICLE_MOTE, C(255, 170, 220));
    if (key == "giratina")
        return MakeTheme(SCENE_DISTORTION, C(24, 16, 34), C(88, 56, 90), C(58, 44, 66), C(30, 22, 36),
                         C(200, 60, 80), PARTICLE_MOTE, C(220, 90, 110));
    if (key == "reshiram")
        return MakeTheme(SCENE_FIRE_DESERT, C(250, 222, 196), C(255, 186, 140), C(236, 204, 158), C(206, 164, 112),
                         C(255, 250, 235), PARTICLE_EMBER, C(255, 140, 60));
    if (key == "zekrom")
        return MakeTheme(SCENE_THUNDER, C(22, 26, 46), C(62, 72, 102), C(52, 56, 70), C(28, 30, 40),
                         C(140, 220, 255), PARTICLE_RAIN, C(170, 190, 220));
    if (key == "kyurem")
        return MakeTheme(SCENE_ICE_CAVE, C(30, 56, 86), C(120, 170, 204), C(206, 226, 240), C(150, 182, 210),
                         C(180, 235, 255), PARTICLE_SNOW, C(255, 255, 255));
    if (key == "xerneas")
        return WithGrass(MakeTheme(SCENE_GLOW_FOREST, C(30, 60, 84), C(110, 168, 150), C(70, 120, 96), C(40, 80, 64),
                                   C(255, 255, 220), PARTICLE_MOTE, C(255, 255, 255)),
                         C(80, 150, 110), C(50, 110, 80));
    if (key == "yveltal")
        return MakeTheme(SCENE_RED_RUINS, C(40, 10, 16), C(156, 44, 44), C(64, 32, 32), C(30, 14, 16),
                         C(255, 90, 60), PARTICLE_ASH, C(120, 100, 100));
    if (key == "zygarde")
        return WithGrass(MakeTheme(SCENE_MOSS_ROCKS, C(52, 82, 64), C(156, 194, 156), C(86, 116, 80), C(52, 76, 52),
                                   C(120, 230, 90), PARTICLE_CELL, C(120, 240, 100)),
                         C(80, 140, 70), C(50, 104, 50));
    if (key == "solgaleo")
        return MakeTheme(SCENE_SUN_ALTAR, C(255, 196, 90), C(255, 240, 196), C(226, 200, 160), C(184, 150, 110),
                         C(255, 170, 40), PARTICLE_MOTE, C(255, 220, 120));
    if (key == "lunala")
        return MakeTheme(SCENE_MOON_NIGHT, C(12, 12, 42), C(64, 52, 116), C(58, 52, 92), C(30, 26, 54),
                         C(240, 236, 200), PARTICLE_STAR, C(255, 255, 230));
    if (key == "necrozma")
        return MakeTheme(SCENE_SPACE, C(4, 4, 18), C(34, 22, 64), C(44, 38, 66), C(20, 16, 34),
                         C(255, 255, 255), PARTICLE_STAR, C(255, 255, 255));
    if (key == "zacian")
        return WithGrass(MakeTheme(SCENE_FOG_FOREST, C(112, 132, 168), C(204, 214, 230), C(92, 120, 120), C(58, 84, 88),
                                   C(120, 170, 230), PARTICLE_FOG, C(255, 255, 255)),
                         C(86, 136, 130), C(56, 100, 100));
    if (key == "zamazenta")
        return WithGrass(MakeTheme(SCENE_FOG_FOREST, C(128, 120, 156), C(214, 206, 222), C(100, 116, 110), C(64, 80, 80),
                                   C(230, 110, 130), PARTICLE_FOG, C(255, 255, 255)),
                         C(90, 130, 116), C(60, 96, 90));
    if (key == "eternatus")
        return MakeTheme(SCENE_RED_NIGHT, C(20, 6, 30), C(126, 24, 64), C(44, 24, 46), C(22, 12, 26),
                         C(255, 60, 120), PARTICLE_EMBER, C(255, 80, 140));
    if (key == "koraidon")
        return WithGrass(MakeTheme(SCENE_JUNGLE, C(250, 170, 110), C(255, 226, 172), C(190, 104, 70), C(146, 70, 48),
                                   C(180, 90, 60), PARTICLE_MOTE, C(255, 236, 160)),
                         C(80, 150, 60), C(50, 110, 40));
    if (key == "miraidon")
        return MakeTheme(SCENE_CITY, C(10, 8, 40), C(64, 30, 104), C(24, 16, 54), C(10, 8, 26),
                         C(70, 230, 255), PARTICLE_STAR, C(200, 220, 255));
    if (key == "terapagos")
        return MakeTheme(SCENE_CRYSTAL_CAVE, C(18, 30, 54), C(56, 92, 130), C(54, 76, 104), C(26, 38, 58),
                         C(120, 240, 230), PARTICLE_MOTE, C(180, 255, 250));

    return WithGrass(MakeTheme(SCENE_MEADOW, C(150, 200, 240), C(216, 237, 207), C(190, 226, 176), C(160, 206, 146),
                               C(255, 255, 255), PARTICLE_NONE, C(255, 255, 255)),
                     C(98, 172, 92), C(70, 140, 70));
}

inline void SceneGrassTuft(float x, float y, float h, Color color, float shake, float sway)
{
    for (int k = -1; k <= 1; k++)
    {
        float baseX = x + k * 9.0f;
        float height = h - (k == 0 ? 0.0f : 10.0f);
        Vector2 top = { baseX + k * 6.0f + shake + sway, y - height };
        Vector2 left = { baseX - 7.0f, y };
        Vector2 right = { baseX + 7.0f, y };
        SceneTri(top, left, right, color);
    }
}

inline void SceneCloud(float x, float y, float size, Color color)
{
    DrawCircleV(Vector2{ x, y }, size, color);
    DrawCircleV(Vector2{ x - size * 0.9f, y + size * 0.25f }, size * 0.7f, color);
    DrawCircleV(Vector2{ x + size * 0.9f, y + size * 0.2f }, size * 0.75f, color);
    DrawCircleV(Vector2{ x + size * 0.3f, y - size * 0.45f }, size * 0.65f, color);
}

inline void SceneCloudRow(Rectangle f, float y, float size, Color color, float t, float speed, int count, int seed)
{
    for (int i = 0; i < count; i++)
    {
        float range = f.width + size * 4;
        float x = f.x - size * 2 + WrapValue(SceneRand(i, seed) * range + t * speed, range);
        float yy = y + (SceneRand(i, seed + 1) - 0.5f) * size;
        SceneCloud(x, yy, size * (0.7f + SceneRand(i, seed + 2) * 0.6f), color);
    }
}

inline void SceneWaves(Rectangle f, float y, float amp, float speed, float t, Color color)
{
    float step = 8.0f;
    for (float x = f.x; x < f.x + f.width; x += step)
    {
        float y1 = y + sinf(x * 0.03f + t * speed) * amp;
        float y2 = y + sinf((x + step) * 0.03f + t * speed) * amp;
        float bottom = f.y + f.height;
        SceneTri(Vector2{ x, y1 }, Vector2{ x, bottom }, Vector2{ x + step, y2 }, color);
        SceneTri(Vector2{ x + step, y2 }, Vector2{ x, bottom }, Vector2{ x + step, bottom }, color);
    }
}

inline void SceneCave(Rectangle f, float hy, Color wall, Color stal)
{
    float W = f.width;
    SceneTri(Vector2{ f.x, f.y }, Vector2{ f.x, hy + 30 }, Vector2{ f.x + W * 0.20f, f.y }, wall);
    SceneTri(Vector2{ f.x, f.y + f.height * 0.3f }, Vector2{ f.x, hy + 30 }, Vector2{ f.x + W * 0.12f, hy + 30 }, wall);
    SceneTri(Vector2{ f.x + W, f.y }, Vector2{ f.x + W * 0.80f, f.y }, Vector2{ f.x + W, hy + 30 }, wall);
    SceneTri(Vector2{ f.x + W, f.y + f.height * 0.3f }, Vector2{ f.x + W * 0.88f, hy + 30 }, Vector2{ f.x + W, hy + 30 }, wall);

    for (int i = 0; i < 9; i++)
    {
        float x = f.x + (i + 0.5f) * W / 9.0f + (SceneRand(i, 11) - 0.5f) * 24;
        float w = 18 + SceneRand(i, 12) * 26;
        float len = f.height * (0.08f + SceneRand(i, 13) * 0.18f);
        SceneTri(Vector2{ x - w / 2, f.y }, Vector2{ x, f.y + len }, Vector2{ x + w / 2, f.y }, stal);
    }

    for (int i = 0; i < 6; i++)
    {
        float side = (i % 2 == 0) ? 0.06f + SceneRand(i, 14) * 0.18f : 0.76f + SceneRand(i, 14) * 0.18f;
        float x = f.x + W * side;
        float w = 14 + SceneRand(i, 15) * 18;
        float h = 20 + SceneRand(i, 16) * 40;
        SceneTri(Vector2{ x - w / 2, hy + 6 }, Vector2{ x, hy + 6 - h }, Vector2{ x + w / 2, hy + 6 }, stal);
    }
}

inline void ScenePillar(float x, float baseY, float w, float h, Color color, Color stripe, bool broken)
{
    DrawRectangle((int)(x - w / 2), (int)(baseY - h), (int)w, (int)h, color);
    DrawRectangle((int)(x - w / 2 - 5), (int)(baseY - 8), (int)(w + 10), 8, ColorBrightness(color, -0.15f));
    DrawRectangle((int)(x - w * 0.2f), (int)(baseY - h), (int)(w * 0.12f), (int)h, Fade(stripe, 0.5f));

    if (broken)
    {
        SceneTri(Vector2{ x - w / 2, baseY - h }, Vector2{ x - w / 2, baseY - h - 14 }, Vector2{ x + w * 0.1f, baseY - h }, color);
        SceneTri(Vector2{ x + w * 0.1f, baseY - h }, Vector2{ x + w / 2, baseY - h - 6 }, Vector2{ x + w / 2, baseY - h }, color);
    }
    else
    {
        DrawRectangle((int)(x - w / 2 - 6), (int)(baseY - h - 10), (int)(w + 12), 10, ColorBrightness(color, -0.1f));
    }
}

inline void SceneTree(float x, float baseY, float h, Color trunk, Color leaf)
{
    DrawRectangle((int)(x - h * 0.05f), (int)(baseY - h), (int)(h * 0.1f), (int)h, trunk);
    DrawCircleV(Vector2{ x, baseY - h }, h * 0.32f, leaf);
    DrawCircleV(Vector2{ x - h * 0.24f, baseY - h * 0.8f }, h * 0.24f, leaf);
    DrawCircleV(Vector2{ x + h * 0.24f, baseY - h * 0.82f }, h * 0.25f, leaf);
}

inline void SceneCrystal(float x, float baseY, float w, float h, Color color)
{
    DrawRectangle((int)(x - w / 2), (int)(baseY - h), (int)w, (int)h, color);
    SceneTri(Vector2{ x - w / 2, baseY - h }, Vector2{ x, baseY - h - w * 0.9f }, Vector2{ x + w / 2, baseY - h }, color);
    DrawRectangle((int)(x - w / 2), (int)(baseY - h), (int)(w * 0.3f), (int)h, Fade(WHITE, 0.25f));
}

inline void SceneBolt(float x, float top, float bottom, int seed, Color color)
{
    Vector2 prev = { x, top };
    int steps = 7;
    for (int i = 1; i <= steps; i++)
    {
        float y = top + (bottom - top) * i / steps;
        float nx = x + (SceneRand(seed, i) - 0.5f) * 50;
        Vector2 next = { nx, y };
        DrawLineEx(prev, next, 4.0f, Fade(color, 0.5f));
        DrawLineEx(prev, next, 2.0f, WHITE);
        prev = next;
    }
}

inline void SceneParticles(const SceneTheme& th, Rectangle f, float hy, float t, bool backLayer)
{
    Color col = th.particleColor;

    if (th.particle == PARTICLE_STAR)
    {
        if (!backLayer) return;
        for (int i = 0; i < 70; i++)
        {
            float x = f.x + SceneRand(i, 1) * f.width;
            float y = f.y + SceneRand(i, 2) * (hy - f.y) * 0.95f;
            float twinkle = 0.3f + 0.7f * fabsf(sinf(t * (0.8f + SceneRand(i, 3) * 2.0f) + i));
            DrawCircleV(Vector2{ x, y }, 0.8f + SceneRand(i, 4) * 1.4f, Fade(col, twinkle));
        }
        return;
    }

    if (backLayer) return;

    if (th.particle == PARTICLE_SNOW)
    {
        for (int i = 0; i < 70; i++)
        {
            float x = f.x + WrapValue(SceneRand(i, 1) * f.width + sinf(t * 0.7f + i) * 12, f.width);
            float y = f.y + WrapValue(SceneRand(i, 2) * f.height + t * (18 + SceneRand(i, 3) * 22), f.height);
            DrawCircleV(Vector2{ x, y }, 1.2f + SceneRand(i, 4) * 1.8f, Fade(col, 0.85f));
        }
    }
    else if (th.particle == PARTICLE_RAIN)
    {
        for (int i = 0; i < 90; i++)
        {
            float x = f.x + WrapValue(SceneRand(i, 1) * f.width - t * 60, f.width);
            float y = f.y - 20 + WrapValue(SceneRand(i, 2) * (f.height + 20) + t * (420 + SceneRand(i, 3) * 120), f.height + 20);
            DrawLineEx(Vector2{ x, y }, Vector2{ x - 5, y + 16 }, 1.3f, Fade(col, 0.55f));
        }
    }
    else if (th.particle == PARTICLE_EMBER)
    {
        for (int i = 0; i < 40; i++)
        {
            float rise = WrapValue(SceneRand(i, 2) * f.height + t * (25 + SceneRand(i, 3) * 45), f.height);
            float y = f.y + f.height - rise;
            float x = f.x + SceneRand(i, 1) * f.width + sinf(t * 1.5f + i) * 8;
            float alpha = 1.0f - rise / f.height;
            DrawCircleV(Vector2{ x, y }, 1.5f + SceneRand(i, 4) * 2.0f, Fade(col, alpha * 0.9f));
        }
    }
    else if (th.particle == PARTICLE_MOTE)
    {
        for (int i = 0; i < 28; i++)
        {
            float rise = WrapValue(SceneRand(i, 2) * f.height + t * (8 + SceneRand(i, 3) * 14), f.height);
            float y = f.y + f.height - rise;
            float x = f.x + SceneRand(i, 1) * f.width + sinf(t * 0.6f + i) * 14;
            float twinkle = 0.4f + 0.6f * fabsf(sinf(t * 1.7f + i * 0.9f));
            Color c = col;
            if (th.scene == SCENE_GLOW_FOREST)
            {
                c = ColorFromHSV((float)((i * 47) % 360), 0.45f, 1.0f);
            }
            DrawCircleV(Vector2{ x, y }, 7.0f, Fade(c, 0.12f * twinkle));
            DrawCircleV(Vector2{ x, y }, 2.2f, Fade(c, 0.85f * twinkle));
        }
    }
    else if (th.particle == PARTICLE_ASH)
    {
        for (int i = 0; i < 50; i++)
        {
            float x = f.x + WrapValue(SceneRand(i, 1) * f.width - t * 12 + sinf(t + i) * 6, f.width);
            float y = f.y + WrapValue(SceneRand(i, 2) * f.height + t * (14 + SceneRand(i, 3) * 14), f.height);
            float s = 1.5f + SceneRand(i, 4) * 2.0f;
            DrawRectangleV(Vector2{ x, y }, Vector2{ s, s }, Fade(col, 0.7f));
        }
    }
    else if (th.particle == PARTICLE_CELL)
    {
        for (int i = 0; i < 26; i++)
        {
            float rise = WrapValue(SceneRand(i, 2) * f.height + t * (10 + SceneRand(i, 3) * 12), f.height);
            float y = f.y + f.height - rise;
            float x = f.x + SceneRand(i, 1) * f.width + sinf(t * 0.8f + i) * 10;
            float twinkle = 0.4f + 0.6f * fabsf(sinf(t * 2.0f + i));
            DrawPoly(Vector2{ x, y }, 6, 3.0f + SceneRand(i, 4) * 2.5f, t * 30 + i * 20, Fade(col, 0.75f * twinkle));
        }
    }
    else if (th.particle == PARTICLE_FOG)
    {
        for (int i = 0; i < 7; i++)
        {
            float range = f.width + 360;
            float x = f.x - 180 + WrapValue(SceneRand(i, 1) * range + t * (10 + i * 3), range);
            float y = hy - 30 + i * 16;
            DrawEllipse((int)x, (int)y, 170 + SceneRand(i, 2) * 60, 16 + SceneRand(i, 3) * 8, Fade(col, 0.16f));
        }
    }
}

inline void DrawSceneBack(const SceneTheme& th, Rectangle f, float t)
{
    float W = f.width;
    float H = f.height;
    float hy = f.y + H * 0.66f;

    BeginScissorMode((int)f.x, (int)f.y, (int)f.width, (int)f.height);

    DrawRectangleGradientV((int)f.x, (int)f.y, (int)W, (int)(hy - f.y) + 1, th.skyTop, th.skyBottom);
    DrawRectangleGradientV((int)f.x, (int)hy, (int)W, (int)(f.y + H - hy) + 1, th.groundTop, th.groundBottom);

    SceneParticles(th, f, hy, t, true);

    switch (th.scene)
    {
    case SCENE_MEADOW:
        DrawEllipse((int)(f.x + W * 0.25f), (int)(hy + 6), W * 0.36f, 46, MixColor(th.groundTop, th.skyBottom, 0.35f));
        DrawEllipse((int)(f.x + W * 0.78f), (int)(hy + 10), W * 0.32f, 40, MixColor(th.groundTop, th.skyBottom, 0.2f));
        SceneCloudRow(f, f.y + H * 0.22f, 22, Fade(WHITE, 0.8f), t, 6, 3, 30);
        break;

    case SCENE_CAVE:
        for (int i = 0; i < 5; i++)
        {
            float x = f.x + W * (0.15f + 0.18f * i);
            float y = f.y + H * (0.35f + 0.15f * SceneRand(i, 5)) + sinf(t + i) * 6;
            SceneGlow(Vector2{ x, y }, 30, th.accent, 1.0f);
        }
        SceneCave(f, hy, C(24, 16, 38), C(52, 40, 76));
        break;

    case SCENE_ICE_CAVE:
        SceneGlow(Vector2{ f.x + W * 0.5f, f.y + H * 0.35f }, 160, th.accent, 0.8f);
        SceneCave(f, hy, C(60, 100, 140), C(170, 215, 240));
        for (int i = 0; i < 7; i++)
        {
            float x = f.x + W * (0.05f + 0.14f * i) + (SceneRand(i, 6) - 0.5f) * 30;
            SceneCrystal(x, hy + 10, 12 + SceneRand(i, 7) * 10, 10 + SceneRand(i, 8) * 30, Fade(C(210, 240, 255), 0.85f));
        }
        break;

    case SCENE_CRYSTAL_CAVE:
        SceneCave(f, hy, C(14, 22, 40), C(36, 56, 84));
        for (int i = 0; i < 10; i++)
        {
            float side = (i % 2 == 0) ? 0.04f + SceneRand(i, 6) * 0.26f : 0.70f + SceneRand(i, 6) * 0.26f;
            float x = f.x + W * side;
            float h = 30 + SceneRand(i, 7) * 70;
            Color c = ColorFromHSV((float)((i * 61) % 360), 0.45f, 1.0f);
            float pulse = 0.6f + 0.4f * sinf(t * 1.5f + i);
            SceneGlow(Vector2{ x, hy - h * 0.5f }, h * 0.7f, c, pulse);
            SceneCrystal(x, hy + 12, 12 + SceneRand(i, 8) * 12, h, Fade(c, 0.9f));
        }
        for (int i = 0; i < 6; i++)
        {
            DrawPolyLines(Vector2{ f.x + W * (0.2f + 0.12f * i), hy + 40 + (i % 2) * 22 }, 6, 22, 0, Fade(th.accent, 0.35f));
        }
        break;

    case SCENE_NIGHT_SEA:
        SceneGlow(Vector2{ f.x + W * 0.82f, f.y + H * 0.2f }, 60, th.accent, 0.8f);
        DrawCircleV(Vector2{ f.x + W * 0.82f, f.y + H * 0.2f }, 16, C(235, 240, 255));
        SceneCloudRow(f, f.y + H * 0.3f, 26, Fade(C(30, 44, 80), 0.8f), t, 10, 4, 40);
        SceneTri(Vector2{ f.x + W * 0.04f, hy }, Vector2{ f.x + W * 0.12f, hy - 50 }, Vector2{ f.x + W * 0.22f, hy }, C(20, 30, 50));
        SceneTri(Vector2{ f.x + W * 0.14f, hy }, Vector2{ f.x + W * 0.2f, hy - 30 }, Vector2{ f.x + W * 0.28f, hy }, C(26, 38, 60));
        for (int i = 0; i < 8; i++)
        {
            float y = hy + 12 + i * 14;
            float x = f.x + WrapValue(SceneRand(i, 9) * W + t * 15, W);
            DrawRectangle((int)x, (int)y, 40 + (int)(SceneRand(i, 10) * 50), 2, Fade(th.accent, 0.25f));
        }
        break;

    case SCENE_STORM_SEA:
        SceneCloudRow(f, f.y + H * 0.08f, 44, C(46, 54, 72), t, 14, 6, 50);
        SceneCloudRow(f, f.y + H * 0.22f, 30, C(64, 74, 94), t, 22, 5, 60);
        break;

    case SCENE_SUNSET:
        SceneGlow(Vector2{ f.x + W * 0.22f, hy - 6 }, 110, th.accent, 1.2f);
        DrawCircleV(Vector2{ f.x + W * 0.22f, hy - 6 }, 32, C(255, 214, 140));
        {
            Color rainbow[6] = { C(255, 90, 90), C(255, 170, 60), C(255, 235, 90), C(110, 210, 110), C(90, 160, 255), C(160, 110, 230) };
            for (int i = 0; i < 6; i++)
            {
                float r = H * 0.58f - i * 7;
                DrawRing(Vector2{ f.x + W * 0.66f, hy + 30 }, r - 7, r, 180, 360, 48, Fade(rainbow[i], 0.28f));
            }
        }
        for (int i = 0; i < 5; i++)
        {
            float w = 54 - i * 9;
            float y = hy - 22 - i * 24;
            float x = f.x + W * 0.88f;
            DrawRectangle((int)(x - w / 2 + 4), (int)y, (int)(w - 8), 22, C(96, 60, 92));
            SceneTri(Vector2{ x - w / 2 - 6, y + 2 }, Vector2{ x, y - 8 }, Vector2{ x + w / 2 + 6, y + 2 }, C(80, 46, 78));
        }
        SceneCloudRow(f, f.y + H * 0.18f, 18, Fade(C(255, 200, 200), 0.6f), t, 8, 4, 70);
        break;

    case SCENE_VOLCANO:
    {
        float cx = f.x + W * 0.5f;
        float topY = hy - H * 0.48f;
        SceneTri(Vector2{ cx - W * 0.42f, hy }, Vector2{ cx - 34, topY }, Vector2{ cx + 34, topY }, C(84, 46, 40));
        SceneTri(Vector2{ cx - W * 0.42f, hy }, Vector2{ cx + 34, topY }, Vector2{ cx + W * 0.42f, hy }, C(84, 46, 40));
        float pulse = 0.7f + 0.3f * sinf(t * 2.0f);
        SceneGlow(Vector2{ cx, topY }, 80, th.accent, pulse * 1.4f);
        DrawLineEx(Vector2{ cx - 10, topY }, Vector2{ cx - 40, topY + 70 }, 5, Fade(th.accent, 0.9f));
        DrawLineEx(Vector2{ cx - 40, topY + 70 }, Vector2{ cx - 30, hy }, 5, Fade(th.accent, 0.8f));
        DrawLineEx(Vector2{ cx + 12, topY }, Vector2{ cx + 50, topY + 90 }, 4, Fade(th.accent, 0.8f));
        for (int i = 0; i < 4; i++)
        {
            float rise = WrapValue(t * 18 + i * 30, 120);
            DrawCircleV(Vector2{ cx + sinf(t * 0.5f + i) * 20, topY - rise }, 18 + rise * 0.25f, Fade(C(60, 50, 50), 0.35f * (1 - rise / 120)));
        }
        for (int i = 0; i < 6; i++)
        {
            float x = f.x + W * (0.08f + 0.16f * i);
            float y = hy + 20 + SceneRand(i, 3) * (f.y + H - hy - 30);
            DrawLineEx(Vector2{ x, y }, Vector2{ x + 30, y + 8 }, 3, Fade(th.accent, pulse));
            DrawLineEx(Vector2{ x + 30, y + 8 }, Vector2{ x + 50, y + 2 }, 2, Fade(th.accent, pulse));
        }
        break;
    }

    case SCENE_HIGH_SKY:
        for (int i = 0; i < 25; i++)
        {
            DrawCircleV(Vector2{ f.x + SceneRand(i, 1) * W, f.y + SceneRand(i, 2) * H * 0.25f }, 1.2f,
                        Fade(WHITE, 0.4f + 0.4f * sinf(t + i)));
        }
        SceneCloudRow(f, f.y + H * 0.38f, 20, Fade(WHITE, 0.55f), t, 18, 4, 80);
        SceneCloudRow(f, f.y + H * 0.52f, 28, Fade(WHITE, 0.75f), t, 30, 4, 90);
        for (int i = 0; i < 12; i++)
        {
            float x = f.x + WrapValue(i * W / 11.0f + t * 6, W + 60) - 30;
            DrawCircleV(Vector2{ x, hy + 4 }, 26 + SceneRand(i, 3) * 12, th.groundTop);
        }
        break;

    case SCENE_TEMPLE:
        SceneGlow(Vector2{ f.x + W * 0.5f, f.y + H * 0.18f }, 70, th.accent, 0.9f + 0.3f * sinf(t * 1.5f));
        ScenePillar(f.x + W * 0.33f, hy, 22, H * 0.36f, C(206, 208, 222), th.accent, true);
        ScenePillar(f.x + W * 0.67f, hy, 22, H * 0.42f, C(206, 208, 222), th.accent, false);
        ScenePillar(f.x + W * 0.08f, hy + 6, 34, H * 0.58f, C(226, 228, 240), th.accent, false);
        ScenePillar(f.x + W * 0.22f, hy + 6, 30, H * 0.34f, C(226, 228, 240), th.accent, true);
        ScenePillar(f.x + W * 0.80f, hy + 6, 30, H * 0.44f, C(226, 228, 240), th.accent, true);
        ScenePillar(f.x + W * 0.93f, hy + 6, 34, H * 0.60f, C(226, 228, 240), th.accent, false);
        for (int i = 1; i <= 4; i++)
        {
            float y = hy + i * i * 7.0f;
            DrawRectangle((int)f.x, (int)y, (int)W, 1, Fade(BLACK, 0.12f));
        }
        for (int i = -4; i <= 4; i++)
        {
            DrawLineEx(Vector2{ f.x + W * 0.5f + i * 30, hy }, Vector2{ f.x + W * 0.5f + i * 150, f.y + H }, 1, Fade(BLACK, 0.10f));
        }
        break;

    case SCENE_DISTORTION:
        for (int k = 0; k < 6; k++)
        {
            float r = 50 + k * 28;
            float start = t * 20 * ((k % 2 == 0) ? 1 : -1) + k * 50;
            DrawRing(Vector2{ f.x + W * 0.5f, f.y + H * 0.32f }, r, r + 3, start, start + 140, 32, Fade(th.accent, 0.25f));
        }
        for (int i = 0; i < 6; i++)
        {
            float x = f.x + W * (0.08f + 0.17f * i);
            float y = f.y + H * (0.15f + 0.3f * SceneRand(i, 4)) + sinf(t * 0.8f + i) * 6;
            float w = 30 + SceneRand(i, 5) * 40;
            bool upsideDown = (i % 2 == 1);
            Color rock = C(70, 52, 76);
            if (!upsideDown)
            {
                DrawRectangle((int)(x - w / 2), (int)y, (int)w, 8, C(110, 70, 100));
                SceneTri(Vector2{ x - w / 2, y + 8 }, Vector2{ x, y + 8 + w * 0.6f }, Vector2{ x + w / 2, y + 8 }, rock);
            }
            else
            {
                DrawRectangle((int)(x - w / 2), (int)y, (int)w, 8, C(110, 70, 100));
                SceneTri(Vector2{ x - w / 2, y }, Vector2{ x, y - w * 0.6f }, Vector2{ x + w / 2, y }, rock);
            }
        }
        break;

    case SCENE_FIRE_DESERT:
        SceneGlow(Vector2{ f.x + W * 0.78f, f.y + H * 0.22f }, 90, th.accent, 1.4f);
        DrawCircleV(Vector2{ f.x + W * 0.78f, f.y + H * 0.22f }, 26, C(255, 252, 240));
        DrawEllipse((int)(f.x + W * 0.2f), (int)(hy + 8), W * 0.32f, 34, C(240, 214, 170));
        DrawEllipse((int)(f.x + W * 0.7f), (int)(hy + 12), W * 0.38f, 30, C(230, 196, 150));
        DrawEllipse((int)(f.x + W * 0.45f), (int)(hy + 40), W * 0.4f, 26, C(242, 210, 164));
        break;

    case SCENE_THUNDER:
    {
        SceneCloudRow(f, f.y + H * 0.06f, 46, C(30, 34, 54), t, 12, 6, 100);
        SceneCloudRow(f, f.y + H * 0.2f, 32, C(44, 50, 74), t, 20, 5, 110);
        int strike = (int)(t / 2.6f);
        float phase = WrapValue(t, 2.6f);
        if (phase < 0.2f)
        {
            float x = f.x + W * (0.15f + 0.7f * SceneRand(strike, 1));
            SceneBolt(x, f.y + H * 0.15f, hy - 10, strike, th.accent);
        }
        for (int i = 0; i < 7; i++)
        {
            float x = f.x + W * (i / 6.0f);
            SceneTri(Vector2{ x - 60, hy + 2 }, Vector2{ x, hy - 24 - SceneRand(i, 2) * 30 }, Vector2{ x + 60, hy + 2 }, C(36, 40, 54));
        }
        break;
    }

    case SCENE_GLOW_FOREST:
        for (int i = 0; i < 5; i++)
        {
            float x = f.x + W * (0.1f + 0.2f * i);
            SceneTri(Vector2{ x, f.y }, Vector2{ x - 40, hy }, Vector2{ x + 30, hy }, Fade(th.accent, 0.06f));
        }
        for (int i = 0; i < 7; i++)
        {
            SceneTree(f.x + W * (0.05f + 0.15f * i), hy + 4, 90 + SceneRand(i, 1) * 50, C(50, 90, 90), C(60, 120, 110));
        }
        SceneTree(f.x + W * 0.06f, hy + 30, 170, C(30, 56, 56), C(38, 84, 76));
        SceneTree(f.x + W * 0.94f, hy + 30, 180, C(30, 56, 56), C(38, 84, 76));
        break;

    case SCENE_RED_RUINS:
        SceneGlow(Vector2{ f.x + W * 0.3f, hy - 30 }, 100, th.accent, 1.0f);
        DrawCircleV(Vector2{ f.x + W * 0.3f, hy - 30 }, 34, C(200, 70, 60));
        ScenePillar(f.x + W * 0.1f, hy + 4, 26, H * 0.3f, C(30, 12, 16), C(30, 12, 16), true);
        ScenePillar(f.x + W * 0.82f, hy + 4, 28, H * 0.42f, C(30, 12, 16), C(30, 12, 16), true);
        ScenePillar(f.x + W * 0.92f, hy + 4, 22, H * 0.2f, C(30, 12, 16), C(30, 12, 16), true);
        for (int i = 0; i < 2; i++)
        {
            float x = f.x + W * (0.62f + 0.1f * i);
            float h = 70 + i * 20;
            DrawLineEx(Vector2{ x, hy }, Vector2{ x, hy - h }, 4, C(26, 10, 14));
            DrawLineEx(Vector2{ x, hy - h * 0.6f }, Vector2{ x - 22, hy - h * 0.9f }, 3, C(26, 10, 14));
            DrawLineEx(Vector2{ x, hy - h * 0.75f }, Vector2{ x + 18, hy - h * 1.05f }, 3, C(26, 10, 14));
        }
        break;

    case SCENE_MOSS_ROCKS:
        for (int i = 0; i < 5; i++)
        {
            float x = f.x + W * (0.05f + 0.22f * i);
            float w = 90 + SceneRand(i, 1) * 60;
            float h = 50 + SceneRand(i, 2) * 70;
            Rectangle rock = { x - w / 2, hy + 10 - h, w, h };
            DrawRectangleRounded(rock, 0.5f, 10, C(104, 112, 108));
            DrawEllipse((int)x, (int)(hy + 12 - h), w * 0.48f, 12, C(80, 150, 70));
        }
        break;

    case SCENE_SUN_ALTAR:
    {
        Vector2 sun = { f.x + W * 0.5f, f.y + H * 0.32f };
        for (int i = 0; i < 12; i++)
        {
            float a = t * 0.3f + i * (PI / 6.0f);
            Vector2 p1 = { sun.x + cosf(a - 0.08f) * 260, sun.y + sinf(a - 0.08f) * 260 };
            Vector2 p2 = { sun.x + cosf(a + 0.08f) * 260, sun.y + sinf(a + 0.08f) * 260 };
            SceneTri(sun, p1, p2, Fade(WHITE, 0.18f));
        }
        SceneGlow(sun, 120, th.accent, 1.3f);
        DrawCircleV(sun, 46, C(255, 236, 170));
        for (int i = 0; i < 3; i++)
        {
            float w = W * (0.62f - i * 0.14f);
            DrawRectangle((int)(f.x + (W - w) / 2), (int)(hy - 10 - i * 12), (int)w, 12, MixColor(C(214, 186, 140), C(250, 230, 190), i / 3.0f));
        }
        ScenePillar(f.x + W * 0.1f, hy + 6, 28, H * 0.42f, C(236, 214, 170), th.accent, false);
        ScenePillar(f.x + W * 0.9f, hy + 6, 28, H * 0.42f, C(236, 214, 170), th.accent, false);
        break;
    }

    case SCENE_MOON_NIGHT:
    {
        Vector2 moon = { f.x + W * 0.5f, f.y + H * 0.3f };
        SceneGlow(moon, 120, C(180, 170, 255), 1.1f);
        DrawCircleV(moon, 52, th.accent);
        DrawCircleV(Vector2{ moon.x - 16, moon.y - 10 }, 10, Fade(C(200, 196, 160), 0.6f));
        DrawCircleV(Vector2{ moon.x + 18, moon.y + 14 }, 7, Fade(C(200, 196, 160), 0.6f));
        DrawCircleV(Vector2{ moon.x + 10, moon.y - 22 }, 5, Fade(C(200, 196, 160), 0.6f));
        for (int i = 0; i < 3; i++)
        {
            float w = W * (0.62f - i * 0.14f);
            DrawRectangle((int)(f.x + (W - w) / 2), (int)(hy - 10 - i * 12), (int)w, 12, MixColor(C(52, 46, 90), C(80, 72, 126), i / 3.0f));
        }
        ScenePillar(f.x + W * 0.1f, hy + 6, 28, H * 0.42f, C(66, 60, 104), C(200, 190, 255), false);
        ScenePillar(f.x + W * 0.9f, hy + 6, 28, H * 0.42f, C(66, 60, 104), C(200, 190, 255), false);
        break;
    }

    case SCENE_SPACE:
    {
        Color prism[5] = { C(255, 90, 90), C(255, 220, 80), C(90, 230, 120), C(80, 180, 255), C(200, 110, 255) };
        Vector2 top = { f.x + W * 0.5f, f.y - 20 };
        for (int i = 0; i < 5; i++)
        {
            float a = -0.6f + i * 0.3f + sinf(t * 0.4f) * 0.05f;
            Vector2 p1 = { top.x + sinf(a - 0.06f) * 500, top.y + cosf(a - 0.06f) * 500 };
            Vector2 p2 = { top.x + sinf(a + 0.06f) * 500, top.y + cosf(a + 0.06f) * 500 };
            SceneTri(top, p1, p2, Fade(prism[i], 0.12f));
        }
        DrawCircleV(Vector2{ f.x + W * 0.16f, f.y + H * 0.24f }, 26, C(110, 80, 160));
        DrawEllipseLines((int)(f.x + W * 0.16f), (int)(f.y + H * 0.24f), 44, 9, Fade(C(200, 180, 255), 0.7f));
        DrawCircleV(Vector2{ f.x + W * 0.86f, f.y + H * 0.14f }, 10, C(90, 130, 190));
        for (int i = 0; i < 9; i++)
        {
            float x = f.x + W * (0.04f + 0.115f * i);
            float h = 10 + SceneRand(i, 7) * 26;
            SceneTri(Vector2{ x - 8, hy + 6 }, Vector2{ x, hy + 6 - h }, Vector2{ x + 8, hy + 6 }, Fade(prism[i % 5], 0.5f));
        }
        break;
    }

    case SCENE_FOG_FOREST:
        for (int i = 0; i < 9; i++)
        {
            float x = f.x + W * (0.03f + 0.12f * i) + SceneRand(i, 1) * 20;
            DrawRectangle((int)x, (int)f.y, 14 + (int)(SceneRand(i, 2) * 8), (int)(hy - f.y + 10), Fade(C(80, 96, 120), 0.45f));
        }
        for (int i = 0; i < 5; i++)
        {
            float x = f.x + W * (0.02f + 0.24f * i) + SceneRand(i, 3) * 30;
            if (x > f.x + W * 0.35f && x < f.x + W * 0.65f) continue;
            DrawRectangle((int)x, (int)f.y, 26 + (int)(SceneRand(i, 4) * 12), (int)(hy - f.y + 24), C(48, 58, 74));
        }
        DrawRectangleGradientV((int)f.x, (int)f.y, (int)W, 40, C(40, 52, 66), Fade(C(40, 52, 66), 0.0f));
        break;

    case SCENE_RED_NIGHT:
    {
        float pulse = 0.8f + 0.2f * sinf(t * 1.2f);
        SceneGlow(Vector2{ f.x + W * 0.5f, f.y + H * 0.2f }, 180, th.accent, pulse);
        for (int k = 0; k < 4; k++)
        {
            float r = 60 + k * 30;
            float start = t * 25 * ((k % 2 == 0) ? 1 : -1) + k * 70;
            DrawRing(Vector2{ f.x + W * 0.5f, f.y + H * 0.2f }, r, r + 4, start, start + 120, 32, Fade(th.accent, 0.3f));
        }
        for (int i = 0; i < 6; i++)
        {
            float w = 70 - i * 10;
            DrawRectangle((int)(f.x + W * 0.84f - w / 2), (int)(hy - 30 - i * 28), (int)w, 30, C(36, 16, 40));
            DrawRectangle((int)(f.x + W * 0.84f - 3), (int)(hy - 22 - i * 28), 6, 6, Fade(th.accent, pulse));
        }
        for (int i = 0; i < 6; i++)
        {
            float x = f.x + W * (0.02f + 0.11f * i);
            float h = 20 + SceneRand(i, 4) * 40;
            DrawRectangle((int)x, (int)(hy - h), 46, (int)h + 2, C(30, 14, 34));
        }
        break;
    }

    case SCENE_JUNGLE:
    {
        for (int i = 0; i < 3; i++)
        {
            float x = f.x + W * (0.15f + 0.35f * i);
            float w = 110 + SceneRand(i, 1) * 60;
            float h = 50 + SceneRand(i, 2) * 50;
            Color c = MixColor(C(200, 110, 80), th.skyBottom, 0.35f);
            DrawRectangle((int)(x - w / 2 + 16), (int)(hy - h), (int)(w - 32), (int)h, c);
            SceneTri(Vector2{ x - w / 2, hy }, Vector2{ x - w / 2 + 16, hy - h }, Vector2{ x - w / 2 + 16, hy }, c);
            SceneTri(Vector2{ x + w / 2 - 16, hy }, Vector2{ x + w / 2 - 16, hy - h }, Vector2{ x + w / 2, hy }, c);
        }
        for (int i = 0; i < 4; i++)
        {
            float sway = sinf(t * 1.2f + i) * 4;
            Vector2 baseL = { f.x - 10, f.y + 20 + i * 26.0f };
            Vector2 tipL = { f.x + 90 + i * 15 + sway, f.y + 60 + i * 30 };
            SceneTri(baseL, Vector2{ (baseL.x + tipL.x) / 2, (baseL.y + tipL.y) / 2 - 16 }, tipL, C(50, 120, 50));
            SceneTri(baseL, tipL, Vector2{ (baseL.x + tipL.x) / 2, (baseL.y + tipL.y) / 2 + 14 }, C(40, 100, 40));

            Vector2 baseR = { f.x + W + 10, f.y + 10 + i * 28.0f };
            Vector2 tipR = { f.x + W - 90 - i * 15 + sway, f.y + 50 + i * 32 };
            SceneTri(baseR, Vector2{ (baseR.x + tipR.x) / 2, (baseR.y + tipR.y) / 2 - 16 }, tipR, C(50, 120, 50));
            SceneTri(baseR, tipR, Vector2{ (baseR.x + tipR.x) / 2, (baseR.y + tipR.y) / 2 + 14 }, C(40, 100, 40));
        }
        break;
    }

    case SCENE_CITY:
    {
        for (int i = 0; i < 12; i++)
        {
            float x = f.x + i * W / 12.0f;
            float w = W / 12.0f - 4;
            float h = 50 + SceneRand(i, 1) * 110;
            DrawRectangle((int)x, (int)(hy - h), (int)w, (int)h + 2, C(26, 20, 62));
            DrawRectangle((int)x, (int)(hy - h), (int)w, 2, Fade((i % 2 == 0) ? th.accent : C(255, 80, 220), 0.9f));
            for (int wy = 0; wy < (int)(h / 14) - 1; wy++)
            {
                for (int wx = 0; wx < 3; wx++)
                {
                    float on = sinf(t * 0.7f + i * 3.1f + wy * 1.7f + wx);
                    if (on > 0.2f)
                    {
                        Color c = ((i + wy) % 3 == 0) ? C(255, 80, 220) : th.accent;
                        DrawRectangle((int)(x + 5 + wx * (w - 10) / 3), (int)(hy - h + 8 + wy * 14), 5, 5, Fade(c, 0.7f));
                    }
                }
            }
        }
        for (int i = 1; i <= 5; i++)
        {
            float y = hy + i * i * 4.5f;
            DrawRectangle((int)f.x, (int)y, (int)W, 1, Fade(C(255, 80, 220), 0.5f));
        }
        for (int i = -6; i <= 6; i++)
        {
            DrawLineEx(Vector2{ f.x + W * 0.5f + i * 20, hy }, Vector2{ f.x + W * 0.5f + i * 120, f.y + H }, 1, Fade(th.accent, 0.4f));
        }
        break;
    }
    }

    EndScissorMode();
}

inline void DrawSceneFront(const SceneTheme& th, Rectangle f, float t, float shake)
{
    float W = f.width;
    float H = f.height;
    float hy = f.y + H * 0.66f;

    BeginScissorMode((int)f.x, (int)f.y, (int)f.width, (int)f.height);

    if (th.grass)
    {
        for (int i = 0; i < 12; i++)
        {
            float x = f.x + 30 + i * (W - 60) / 11.0f + (float)((i * 29) % 13 - 6);
            float y = f.y + H - 10 - (float)((i * 7) % 5);
            float h = 36.0f + (float)((i * 31) % 17);
            float s = sinf(t * 45.0f + i * 1.3f) * 7.0f * shake;
            SceneGrassTuft(x, y, h, th.grassFront, s, sinf(t * 1.5f + i * 1.3f) * 2.0f);
        }
    }

    if (th.scene == SCENE_NIGHT_SEA)
    {
        SceneWaves(f, f.y + H - 26, 5, 1.6f + shake * 6, t, Fade(C(40, 90, 140), 0.85f));
        SceneWaves(f, f.y + H - 12, 4, 2.2f + shake * 6, t + 1.3f, Fade(C(70, 130, 180), 0.8f));
    }
    else if (th.scene == SCENE_STORM_SEA)
    {
        SceneWaves(f, f.y + H - 34, 9, 2.0f + shake * 6, t, Fade(C(30, 76, 120), 0.9f));
        SceneWaves(f, f.y + H - 16, 7, 2.8f + shake * 6, t + 2.0f, Fade(C(60, 110, 160), 0.85f));
    }
    else if (th.scene == SCENE_HIGH_SKY)
    {
        SceneCloudRow(f, f.y + H - 8, 26, Fade(WHITE, 0.9f), t, 26, 5, 120);
    }

    SceneParticles(th, f, hy, t, false);

    if (th.scene == SCENE_THUNDER)
    {
        float phase = WrapValue(t, 2.6f);
        if (phase < 0.2f)
        {
            DrawRectangle((int)f.x, (int)f.y, (int)W, (int)H, Fade(WHITE, 0.25f * (1.0f - phase / 0.2f)));
        }
    }

    EndScissorMode();
}

inline void DrawSceneSpotlight(const SceneTheme& th, Vector2 center, float radius)
{
    int brightness = ((int)th.skyBottom.r + (int)th.skyBottom.g + (int)th.skyBottom.b) / 3;
    if (brightness < 115)
    {
        SceneGlow(center, radius, WHITE, 0.55f);
        SceneGlow(center, radius * 0.6f, th.accent, 0.35f);
    }
}

inline void DrawSceneBackGrass(const SceneTheme& th, Rectangle f, float t, float shake)
{
    if (!th.grass)
    {
        return;
    }
    BeginScissorMode((int)f.x, (int)f.y, (int)f.width, (int)f.height);
    for (int i = 0; i < 16; i++)
    {
        float x = f.x + 20 + i * (f.width - 40) / 15.0f + (float)((i * 37) % 11 - 5);
        float y = f.y + f.height * 0.72f + (float)((i * 13) % 7);
        float h = 26.0f + (float)((i * 53) % 17);
        float s = sinf(t * 45.0f + i * 0.7f) * 7.0f * shake;
        SceneGrassTuft(x, y, h, th.grassBack, s, sinf(t * 1.5f + i * 0.7f) * 2.0f);
    }
    EndScissorMode();
}

#endif
