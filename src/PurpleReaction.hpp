// Generated from purple THPrac (MIT), TH07 source sha256 b5be6dc698f574992ab49696c5c0d8ad756af93bf0760be1504ee9e958bee1b0.
#pragma once
#include "PurpleHotkey.hpp"
#include <SDL3/SDL.h>
#include <functional>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>
namespace PracticeRuntime {
struct PurpleCounter { int64_t QuadPart=0; };
inline void purple_counter_frequency(PurpleCounter* p){p->QuadPart=1000000000;}
inline void purple_counter_now(PurpleCounter* p){p->QuadPart=SDL_GetTicksNS();}
inline std::function<unsigned()> purple_random_generator(unsigned lo,unsigned hi){return std::bind(std::uniform_int_distribution<unsigned>(lo,hi),std::mt19937(unsigned(SDL_GetTicksNS())));}
class THGuiTestReactionTest {
    enum TestState {
        NOT_BEGIN,
        WAIT_TIME,
        WAIT_TIME_PRESSED,
        REACT_TIME,
        TOO_EARLY,
        SHOW_RES,
    };
    enum TestType {
        PRESS,
        RELEASE
    };

public:
    THGuiTestReactionTest();
    bool GuiUpdate(bool ingame, unsigned buttons, int locale);
    void Reset();

private:
    PurpleCounter mTimeFreq;
    PurpleCounter mPressTime;
    PurpleCounter mWaitTime;
    unsigned mPreviousButtons=0;
    int mFrameCount;

    TestState mTestState;
    TestType mTestType;
    int mTestTime;
    bool mShowProgressBar;

    int mCurTest;
    std::function<unsigned int(void)> mRndSeedGen;
    std::vector<float> mResults;
    std::vector<float> mFrameCounts;
};
inline THGuiTestReactionTest::THGuiTestReactionTest()
{
    purple_counter_frequency(&mTimeFreq);
    mTestState = NOT_BEGIN;
    mTestType = PRESS;
    mTestTime = 5;
    mRndSeedGen = purple_random_generator(1200u, 2800u);
    // mRndSeedGen = purple_random_generator(0u, 1u);
    mShowProgressBar = false;
    mCurTest = 0;
}
inline bool THGuiTestReactionTest::GuiUpdate(bool ingame, unsigned buttons, int locale)
{
    const auto pressedButtons = buttons & ~mPreviousButtons;
    mPreviousButtons = buttons;
    PurpleCounter curTime;
    purple_counter_now(&curTime);

    ImVec2 colorBtnSz = ImVec2(ImGui::GetWindowWidth()*0.5f, ImGui::GetTextLineHeight() * 10.0f);
    ImVec2 colorBtnPos = ImVec2(ImGui::GetWindowWidth()*0.25f, ImGui::GetTextLineHeight() * 2.0f);
    unsigned colorBtnFlag = ImGuiColorEditFlags_::ImGuiColorEditFlags_NoTooltip;
    ImVec4 color_BeforeTimeReact = ImVec4(1, 0.2, 0.2, 1);
    ImVec4 color_AfterTimeReact = ImVec4(0, 1, 0.2, 1);
    ImVec4 color_WaitPress = ImVec4(1, 1, 0.2, 1);

    bool isKeyPressed = (buttons & 0xf0) != 0;
    if (!ingame){
        if (ImGui::Button(purple_THPRAC_BACK[locale])) {
            Reset();
            return false;
        }
    }
    switch (mTestState)
    {
    case NOT_BEGIN:
    {
        if (!ingame) {
            ImGui::SameLine();
            ImGui::TextUnformatted(purple_THPRAC_TOOLS_REACTION_TEST[locale]);
            ImGui::Separator();
        }

        ImGui::RadioButton(purple_THPRAC_TOOLS_REACTION_TEST_DOWN[locale], (int*)&mTestType, PRESS);
        ImGui::SameLine();
        ImGui::RadioButton(purple_THPRAC_TOOLS_REACTION_TEST_UP[locale], (int*)&mTestType, RELEASE);

        ImGui::Checkbox(purple_THPRAC_TOOLS_REACTION_SHOW_PROC_BAR[locale], &mShowProgressBar);

        ImGui::DragInt(purple_THPRAC_TOOLS_REACTION_TEST_TIME[locale], &mTestTime, 1.0f, 1, 20);
        ImGui::NewLine();
        if (ImGui::Button(purple_THPRAC_TOOLS_REACTION_TEST_BEGIN[locale]) || (pressedButtons & 1) || (pressedButtons & 4)) {
            // in win 7, due to the initialize sequence, purple_counter_frequency might get 0 value at the beginning,
            // so query it every time before test begin to avoid this problem
            purple_counter_frequency(&mTimeFreq);
            mTestState = WAIT_TIME;
            mCurTest = 1;
            mFrameCount = 0;
            if (mTestType == PRESS) {
                mWaitTime.QuadPart = static_cast<int64_t>(mRndSeedGen() / 1000.0 * (double)mTimeFreq.QuadPart);
                mPressTime.QuadPart = mWaitTime.QuadPart + curTime.QuadPart;
            }
            mResults = {};
        }
        ImGui::InvisibleButton("inv", colorBtnSz);
    } break;

    case TOO_EARLY: {
        ImGui::TextUnformatted(purple_THPRAC_TOOLS_REACTION_TEST_TOO_EARLY[locale]);
        if (ImGui::Button(purple_THPRAC_TOOLS_REACTION_TEST_NEXT_TEST[locale]) || (pressedButtons & 1) || (pressedButtons & 4)) // press z(90)/shift(16)
        {
            mFrameCount = 0;
            mTestState = WAIT_TIME;
            mWaitTime.QuadPart = static_cast<int64_t>(mRndSeedGen() / 1000.0 * (double)mTimeFreq.QuadPart);
            mPressTime.QuadPart = mWaitTime.QuadPart + curTime.QuadPart;
        }
        ImGui::InvisibleButton("inv", colorBtnSz);
    } break;
    case SHOW_RES: {
        if (mResults.size() != 0 && mFrameCounts.size() != 0)
        {
            ImGui::Text("%s(%d): %.1f ms (%.1f frame)(framecount: %d)", purple_THPRAC_TOOLS_REACTION_TEST_RESULT[locale], mCurTest, mResults[mResults.size() - 1], mResults[mResults.size() - 1] / 16.66667f,(int)mFrameCounts[mFrameCounts.size()-1]);
            if (mCurTest < mTestTime)
                ImGui::InvisibleButton("inv", colorBtnSz);
        }else
            ImGui::Text("%s(%d): ?????", purple_THPRAC_TOOLS_REACTION_TEST_RESULT[locale], mCurTest);
        if (mCurTest == mTestTime)
        {
            float avg = 0.0f;
            float maxv = 60.0f;
            float minv = 0.0f;
            float avg_frameCnt = 0.0f;
            float maxfcnt = 10.0f;
            float minfcnt = 0.0f;
            for (auto x : mResults) {
                avg += fabsf(x);
                maxv = std::max(x, maxv);
                minv = std::min(x, minv);
            }
            for (auto x : mFrameCounts) {
                avg_frameCnt += fabsf(x);
                maxfcnt = std::max(x, maxfcnt);
                minfcnt = std::min(x, minfcnt);
            }
            avg /= (float)mTestTime;
            avg_frameCnt /= (float)mTestTime;
            ImGui::Separator();
            ImGui::Text("%s: %s, %s", purple_THPRAC_TOOLS_REACTION_MODE[locale], mTestType == PRESS ? purple_THPRAC_TOOLS_REACTION_MODE_DOWN[locale] : purple_THPRAC_TOOLS_REACTION_MODE_UP[locale],
                mShowProgressBar ? purple_THPRAC_TOOLS_REACTION_MODE_PROGRESSBAR[locale] : purple_THPRAC_TOOLS_REACTION_MODE_NORMAL[locale]
            );
            ImGui::Text("%s: %.1f ms (%.1f frame)(framecount: %.1f)", purple_THPRAC_TOOLS_REACTION_TEST_RESULT_AVG[locale], avg, avg / 16.66667f, avg_frameCnt);

            ImGui::PlotHistogram(purple_THPRAC_TOOLS_REACTION_TEST_RESULT[locale], &mResults[0], mTestTime, 0, purple_THPRAC_TOOLS_REACTION_TEST_RESULT[locale], minv-10.0f, maxv+20.0f, ImVec2(0, 200.0));

            ImGui::PlotHistogram((std::string(purple_THPRAC_TOOLS_REACTION_TEST_RESULT[locale])+"(framecount)").c_str(), &mFrameCounts[0], mTestTime, 0, (std::string(purple_THPRAC_TOOLS_REACTION_TEST_RESULT[locale])+"(framecount)").c_str(), minfcnt-1.0f, maxfcnt + 10.0f, ImVec2(0, 200.0));
            if (ImGui::Button(purple_THPRAC_TOOLS_REACTION_TEST_NEXT_TEST[locale]) || (pressedButtons & 1) || (pressedButtons & 4)) {
                mTestState = NOT_BEGIN;
            }
        } else {
            if (ImGui::Button(purple_THPRAC_TOOLS_REACTION_TEST_NEXT_TEST[locale]) || (pressedButtons & 1) || (pressedButtons & 4)) // press z
            {
                mCurTest++;
                mTestState = WAIT_TIME;
                mFrameCount = 0;
                mWaitTime.QuadPart = static_cast<int64_t>(mRndSeedGen() / 1000.0 * (double)mTimeFreq.QuadPart);
                mPressTime.QuadPart = mWaitTime.QuadPart + curTime.QuadPart;
            }
        }
    } break;

    case REACT_TIME:
    case WAIT_TIME:
    case WAIT_TIME_PRESSED:
    {
        if (mTestType == PRESS)
        {
            if (mTestState == WAIT_TIME && curTime.QuadPart >= mPressTime.QuadPart){
                mTestState = REACT_TIME;
                mFrameCount = 0;
            }
            ImGui::Text("test %d", mCurTest);
            mFrameCount++;
            if (mTestState == WAIT_TIME) {
                if (mShowProgressBar) {
                    ImGui::ProgressBar(std::clamp(std::fabs(static_cast<float>(((double)(mPressTime.QuadPart - curTime.QuadPart)) / ((double)mWaitTime.QuadPart))), 0.0f, 1.0f), ImVec2(0, 0), "waiting...");
                }
                ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + colorBtnPos.x, ImGui::GetCursorPosY() + colorBtnPos.y));
                ImGui::ColorButton("color", color_BeforeTimeReact, colorBtnFlag, colorBtnSz);
                if (isKeyPressed){
                    if (!mShowProgressBar){
                        mTestState = TOO_EARLY;
                    } else {
                        // allow negative reaction time
                        float curTestReactionTimeMs = static_cast<float>(((double)(curTime.QuadPart - mPressTime.QuadPart)) / ((double)mTimeFreq.QuadPart) * 1000.0);
                        mResults.push_back(curTestReactionTimeMs);
                        mFrameCounts.push_back(ceilf(fabsf(curTestReactionTimeMs))/16.6667f);
                        mTestState = SHOW_RES;
                    }
                }
            } else {
                if (mShowProgressBar) {
                    ImGui::ProgressBar(0.0f, ImVec2(0, 0), "press...");
                }
                ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + colorBtnPos.x, ImGui::GetCursorPosY() + colorBtnPos.y));
                ImGui::ColorButton("color", color_AfterTimeReact, colorBtnFlag, colorBtnSz);
                if (isKeyPressed){
                    float curTestReactionTimeMs = static_cast<float>(((double)(curTime.QuadPart - mPressTime.QuadPart)) / ((double)mTimeFreq.QuadPart) * 1000.0);
                    mResults.push_back(curTestReactionTimeMs);
                    mFrameCounts.push_back(static_cast<float>(mFrameCount));
                    mTestState = SHOW_RES;
                }
            }
        }else{
            if (mTestState == WAIT_TIME_PRESSED && curTime.QuadPart >= mPressTime.QuadPart) {
                mTestState = REACT_TIME;
                mFrameCount = 0;
            }
            mFrameCount++;
            ImGui::Text("test %d", mCurTest);
            if (mTestState == WAIT_TIME) {
                if (mShowProgressBar) {
                    ImGui::ProgressBar(1.0f, ImVec2(0, 0), "waiting...");
                }
                ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + colorBtnPos.x, ImGui::GetCursorPosY() + colorBtnPos.y));
                ImGui::ColorButton("color", color_WaitPress, colorBtnFlag, colorBtnSz);
                if (isKeyPressed) {
                    mWaitTime.QuadPart = static_cast<int64_t>(mRndSeedGen() / 1000.0 * (double)mTimeFreq.QuadPart);
                    mPressTime.QuadPart = mWaitTime.QuadPart + curTime.QuadPart;
                    mTestState = WAIT_TIME_PRESSED;
                }
            } else if(mTestState == REACT_TIME){
                if (mShowProgressBar) {
                    ImGui::ProgressBar(0.0f, ImVec2(0, 0), "press...");
                }
                ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + colorBtnPos.x, ImGui::GetCursorPosY() + colorBtnPos.y));
                ImGui::ColorButton("color", color_AfterTimeReact, colorBtnFlag, colorBtnSz);
                if (!isKeyPressed) {
                    float curTestReactionTimeMs = static_cast<float>(((double)(curTime.QuadPart - mPressTime.QuadPart)) / ((double)mTimeFreq.QuadPart) * 1000.0);
                    mResults.push_back(curTestReactionTimeMs);
                    mFrameCounts.push_back(static_cast<float>(mFrameCount));
                    mTestState = SHOW_RES;
                }
            }
            else if (mTestState == WAIT_TIME_PRESSED){
                if (mShowProgressBar) {
                    ImGui::ProgressBar(std::clamp(static_cast<float>(std::abs(((double)(mPressTime.QuadPart - curTime.QuadPart)) / ((double)mWaitTime.QuadPart))), 0.0f, 1.0f), ImVec2(0, 0), "waiting...");
                }
                ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + colorBtnPos.x, ImGui::GetCursorPosY() + colorBtnPos.y));
                ImGui::ColorButton("color", color_BeforeTimeReact, colorBtnFlag, colorBtnSz);
                if (!isKeyPressed) {
                    if (!mShowProgressBar) {
                        mTestState = TOO_EARLY;
                    } else {
                        float curTestReactionTimeMs =  static_cast<float>(((double)(curTime.QuadPart - mPressTime.QuadPart)) / ((double)mTimeFreq.QuadPart) * 1000.0);
                        mResults.push_back(curTestReactionTimeMs);
                        mFrameCounts.push_back(ceilf(fabsf(curTestReactionTimeMs)) / 16.6667f);
                        mTestState = SHOW_RES;
                    }
                }
            }
        }
    }
    break;
    }
    return true;
}

inline void THGuiTestReactionTest::Reset()
{
    purple_counter_frequency(&mTimeFreq);
    mTestState = NOT_BEGIN;
    mTestType = PRESS;
    mTestTime = 5;
    mRndSeedGen = purple_random_generator(1200u, 3500u);
    mShowProgressBar = false;
    mCurTest = 0;
    mResults = {};
    mFrameCounts = {};
}



}
