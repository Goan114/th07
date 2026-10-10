// Generated from purple THPrac (MIT), TH07 source sha256 b5be6dc698f574992ab49696c5c0d8ad756af93bf0760be1504ee9e958bee1b0.
#pragma once
#include <imgui.h>
#include <string>
namespace PracticeRuntime {
        inline bool PurpleHotkeyWidget(const char* mKeyText, const char* text, bool mStatus)
        {
            std::string realText;
            if (mStatus) {
                realText = std::string("[") + mKeyText + ": " + text + "]";
                ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 1.0f, 0.0f, 1.0f });
            } else {
                realText = std::string(mKeyText) + ": " + text;
            }

            auto cursor = ImGui::GetCursorPos();
            ImGui::TextUnformatted(realText.c_str());
            ImGui::SetCursorPos(cursor);

            if (mStatus)
                ImGui::PopStyleColor();

            if (ImGui::InvisibleButton(mKeyText, ImGui::CalcTextSize(realText.c_str())))
                return true;
            else
                return false;
        }
inline void DrawPurpleHotkey(const char* key, const char* text, bool& status) {
    if (PurpleHotkeyWidget(key, text, status)) status = !status;
}
inline constexpr const char* purple_THPRAC_INGAMEINFO[3]{"更多游戏信息","in game info","in game info"};
inline constexpr const char* purple_TH_ENEMY_MUTEKI[3]{"敌人无敌","Enemy Invincible","敵無敵"};
inline constexpr const char* purple_TH_ADV_DISABLE_X_KEY[3]{"禁用X键","disable X key","disable X key"};
inline constexpr const char* purple_TH_ADV_DISABLE_SHIFT_KEY[3]{"禁用Shift键","disable shift key","disable shift key"};
inline constexpr const char* purple_TH_ADV_DISABLE_Z_KEY[3]{"禁用Z键","disable Z key","disable Z key"};
inline constexpr const char* purple_TH_ADV_DISABLE_C_KEY_SAMETIME[3]{"禁用X同时禁用C键","disable C key at the same time when disable X","disable C key at the same time when disable X"};
inline constexpr const char* purple_TH_ADV_FORCE_SHIFT_KEY[3]{"始终按住Shift键","force press shift key","force press shift key"};
inline constexpr const char* purple_THPRAC_INFLIVES_MAP[3]{"将锁残修改为不疮","map inf.lives to no continue","map inf.lives to no continue"};
}
