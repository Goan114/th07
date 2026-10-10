// Generated from purple THPrac (MIT), TH07 source sha256 b5be6dc698f574992ab49696c5c0d8ad756af93bf0760be1504ee9e958bee1b0.
#pragma once
#include <imgui.h>
#include <deque>
#include <vector>
#include <string>
#include <cstdio>
#include <cmath>
#include <cstdint>
namespace PracticeRuntime {
struct KeyRectStyle {
    bool separated = true;
    bool show_aps = true;
    uint32_t fill_color_press = 0xFFFFFFFF;
    uint32_t fill_color_release = 0xFFFFFFFF;
    uint32_t border_color_press = 0xFFFF4444;
    uint32_t border_color_release = 0xFFFFCCCC;
    uint32_t text_color_press = 0xFFFFFFFF;
    uint32_t text_color_release = 0xFFFFFFFF;
    ImVec2 padding = { 0.05f, 0.05f };
    ImVec2 size = { 34.0f, 34.0f };
    int type = 2;
};
struct PurpleKeyMonitor {
enum THKey {
    Key_Up,
    Key_Down,
    Key_Left,
    Key_Right,
    Key_Z,
    Key_X,
    Key_C,
    Key_D,
    Key_Ctrl,
    Key_Shift,
    END
};

inline static const std::string THKeyNames[] = { "up", "down", "left", "right", "Z", "X", "C", "D", "Ctrl", "Shift" };
bool g_keys_down[END] = { 0 };
uint32_t g_key_mask[END] = { 0 };





 bool g_record_key_aps=false;int g_aps_cur=0;std::vector<int> g_recorded_aps;std::vector<uint16_t> g_recorded_keys;std::deque<uint32_t> keys_per_sec;
void record(int ver, uint32_t cur_key)
{
    switch (ver) {
    default:
    case 6:
    case 7:
    case 8:
    case 10:
        g_key_mask[Key_Shift] = 0x4;
        g_key_mask[Key_Z] = 0x1;
        g_key_mask[Key_X] = 0x2;
        g_key_mask[Key_Ctrl] = 0x100;
        g_key_mask[Key_Up] = 0x10;
        g_key_mask[Key_Down] = 0x20;
        g_key_mask[Key_Left] = 0x40;
        g_key_mask[Key_Right] = 0x80;
        break;
    case 11:
    case 12:
        g_key_mask[Key_Shift] = 0x8;
        g_key_mask[Key_Z] = 0x1;
        g_key_mask[Key_X] = 0x2;
        g_key_mask[Key_Ctrl] = 0x200;
        g_key_mask[Key_Up] = 0x10;
        g_key_mask[Key_Down] = 0x20;
        g_key_mask[Key_Left] = 0x40;
        g_key_mask[Key_Right] = 0x80;
        break;
    case 128:
        g_key_mask[Key_Shift] = 0x8;
        g_key_mask[Key_Z] = 0x1;
        g_key_mask[Key_X] = 0x2;
        g_key_mask[Key_C] = 0x200;
        g_key_mask[Key_Up] = 0x10;
        g_key_mask[Key_Down] = 0x20;
        g_key_mask[Key_Left] = 0x40;
        g_key_mask[Key_Right] = 0x80;
        break;
    case 13:
    case 16:
        g_key_mask[Key_Shift] = 0x8;
        g_key_mask[Key_Z] = 0x1;
        g_key_mask[Key_X] = 0x2;
        g_key_mask[Key_C] = 0xA00;
        g_key_mask[Key_Ctrl] = 0x200;
        g_key_mask[Key_Up] = 0x10;
        g_key_mask[Key_Down] = 0x20;
        g_key_mask[Key_Left] = 0x40;
        g_key_mask[Key_Right] = 0x80;
        break;
    case 14:
    case 15:
    case 17:
        g_key_mask[Key_Shift] = 0x8;
        g_key_mask[Key_Z] = 0x1;
        g_key_mask[Key_X] = 0x2;
        g_key_mask[Key_Ctrl] = 0x200;
        g_key_mask[Key_Up] = 0x10;
        g_key_mask[Key_Down] = 0x20;
        g_key_mask[Key_Left] = 0x40;
        g_key_mask[Key_Right] = 0x80;
        break;
    case 18:
        g_key_mask[Key_Shift] = 0x8;
        g_key_mask[Key_Z] = 0x1;
        g_key_mask[Key_X] = 0x2;
        g_key_mask[Key_C] = 0x400;
        g_key_mask[Key_D] = 0x800;
        g_key_mask[Key_Up] = 0x10;
        g_key_mask[Key_Down] = 0x20;
        g_key_mask[Key_Left] = 0x40;
        g_key_mask[Key_Right] = 0x80;
        break;
    case 20:
        g_key_mask[Key_Shift] = 0x8;
        g_key_mask[Key_Z] = 0x1;
        g_key_mask[Key_X] = 0x4;
        g_key_mask[Key_Up] = 0x10;
        g_key_mask[Key_Down] = 0x20;
        g_key_mask[Key_Left] = 0x40;
        g_key_mask[Key_Right] = 0x80;
        break;
    }
    for (int i = 0; i < END; i++) {
        if (g_key_mask[i])
            g_keys_down[i] = ((cur_key & g_key_mask[i]) == g_key_mask[i]);
    }
    uint32_t key_cur = 0;
    for (int i = 0; i < END; i++) {
        if (g_keys_down[i])
            key_cur |= 1 << i;
    } // not use zun's keycode



    uint32_t key_last = 0;
    while (keys_per_sec.size() >= 60) {
        key_last = keys_per_sec.front();
        keys_per_sec.pop_front();
    }
    keys_per_sec.push_back(key_cur);
    g_aps_cur = 0;
    for (auto key : keys_per_sec) {
        if (key != key_last) {
            uint32_t diff = key ^ key_last;
            uint32_t diff_cnt = 0;
            for (int i = 0; i < END; i++) {
                if (diff & (1 << i))
                    diff_cnt++;
            }
            g_aps_cur += diff_cnt;
            key_last = key;
        }
    }
    if (g_record_key_aps){
        g_recorded_aps.push_back(g_aps_cur);
        g_recorded_keys.push_back((uint16_t)key_cur);
    }
}

 void clear_record(){g_recorded_aps.clear();g_recorded_keys.clear();}
 std::string csv() const {std::string out="frame,aps";for(const auto& key:THKeyNames)out+=","+key;out+="\n";
  for(size_t j=0;j<g_recorded_aps.size();++j){out+=std::to_string(j+1)+","+std::to_string(g_recorded_aps[j]);for(int i=0;i<END;++i)out+=(g_recorded_keys[j]&(1<<i))?",O":",-";out+="\n";}return out;}
};
void static KeysRect(ImDrawList* p, ImVec2 pos, ImVec2 size, float border_size, const KeyRectStyle& style, bool is_pressed, const char* text, bool disabled = false)
{
    uint32_t fill_color = is_pressed ? style.fill_color_press : style.fill_color_release;
    uint32_t border_color = is_pressed ? style.border_color_press : style.border_color_release;
    uint32_t text_color = is_pressed ? style.text_color_press : style.text_color_release;
    if (disabled){
        fill_color = 0xFFAAAAAA;
        border_color = style.border_color_release;
        text_color = 0xFF555555;
    }
    ImVec2 real_pos1 = { pos.x + style.padding.x * size.x, pos.y + style.padding.y * size.y };
    ImVec2 real_pos2 = { real_pos1.x + size.x * (1.0f - 2.0f * style.padding.x), real_pos1.y + +size.y * (1.0f - 2.0f * style.padding.y) };
    switch (style.type) {
    case 3: {
        ImVec2 real_pos_m = { (real_pos1.x + real_pos2.x) * 0.5f, (real_pos1.y + real_pos2.y) * 0.5f };
        ImVec2 sz_ratio = { size.x * (1.0f - 2.0f * style.padding.x), size.y * (1.0f - 2.0f * style.padding.y) };
        constexpr int n_heart = 15;
        static ImVec2 points_heart_u[n_heart + 1] = { { -1, -1 } };
        static ImVec2 points_heart_d[n_heart + 1] = { { -1, -1 } };
        static ImVec2 points_heart_buffer[n_heart][8];
        static ImVec2 points_heart_buffer2[n_heart * 2];
        if (points_heart_u[0].x == -1) { // not init
            for (int i = 0; i <= n_heart; i++) {
                float t = -1.57079f + (float)i / n_heart * 3.1415926f;
                points_heart_u[i] = { 16.0f * powf(sinf(t), 3.0f), 13.0f * cosf(t) - 5.0f * cos(2.0f * t) - 2.0f * cos(3.0f * t) - cosf(4.0f * t) };
                t = 1.57079f + (float)i / n_heart * 3.1415926f;
                points_heart_d[i] = { 16.0f * powf(sinf(t), 3.0f), 13.0f * cosf(t) - 5.0f * cos(2.0f * t) - 2.0f * cos(3.0f * t) - cosf(4.0f * t) };
                points_heart_u[i].x /= 28.0f;
                points_heart_d[i].x /= 28.0f;
                points_heart_u[i].y /= -28.0f;
                points_heart_d[i].y /= -28.0f;
            }
        }
        for (int i = 0; i < n_heart; i++) {
            points_heart_buffer[i][0] = points_heart_u[i];
            points_heart_buffer[i][1] = points_heart_u[i + 1];
            points_heart_buffer[i][2] = { points_heart_u[i + 1].x, points_heart_u[0].y };
            points_heart_buffer[i][3] = { points_heart_u[i].x, points_heart_u[0].y };
            points_heart_buffer[i][4] = points_heart_d[i];
            points_heart_buffer[i][5] = points_heart_d[i + 1];
            points_heart_buffer[i][6] = { points_heart_d[i + 1].x, points_heart_d[0].y };
            points_heart_buffer[i][7] = { points_heart_d[i].x, points_heart_d[0].y };
            for (int j = 0; j < 8; j++) {
                points_heart_buffer[i][j] = { points_heart_buffer[i][j].x * sz_ratio.x + real_pos_m.x,
                    points_heart_buffer[i][j].y * sz_ratio.y + real_pos_m.y };
            }
            points_heart_buffer2[i] = points_heart_buffer[i][0];
            points_heart_buffer2[i + n_heart] = points_heart_buffer[i][4];
        }
        for (int i = 0; i < n_heart; i++) {
            auto pts = points_heart_buffer[i];
            p->AddQuadFilled(pts[0], pts[1], pts[2], pts[3], fill_color);
            p->AddQuadFilled(pts[4], pts[5], pts[6], pts[7], fill_color);
            p->AddLine(pts[1], pts[2], fill_color, 3.0f);
            p->AddLine(pts[5], pts[6], fill_color, 3.0f);
        }
        p->AddLine(points_heart_buffer[0][0], points_heart_buffer[n_heart - 1][1], fill_color, 2.0f);
        p->AddPolyline(points_heart_buffer2, n_heart * 2, border_color, ImDrawFlags_Closed, border_size);
        break;
    }

    case 2: {
        float corner = (1.0f - 2.0f * style.padding.y) * size.y * 0.25f;
        ImVec2 points[] = {
            real_pos1,
            { real_pos2.x - corner, real_pos1.y },
            { real_pos2.x, real_pos1.y + corner },
            real_pos2,
            { real_pos1.x + corner, real_pos2.y },
            { real_pos1.x, real_pos2.y - corner },
        };
        p->AddTriangleFilled(points[0], points[4], points[5], fill_color);
        p->AddTriangleFilled(points[1], points[2], points[3], fill_color);
        p->AddQuadFilled(points[0], points[1], points[3], points[4], fill_color);
        p->AddLine(points[0], points[4], fill_color, 2.0f);
        p->AddLine(points[1], points[3], fill_color, 2.0f);
        p->AddPolyline(points, 6, border_color, ImDrawFlags_Closed, border_size);
        break;
    }
    case 1:
        p->AddRectFilled(real_pos1, real_pos2, fill_color, 0.15f * size.y);
        p->AddRect(real_pos1, real_pos2, border_color, 0.15f * size.y, 0, border_size);
        break;
    default:
    case 0:
        p->AddRectFilled(real_pos1, real_pos2, fill_color);
        p->AddRect(real_pos1, real_pos2, border_color, 0.0f, 0, border_size);
        break;
    }
    ImVec2 textSz = ImGui::CalcTextSize(text);
    ImVec2 textPos = {
        (real_pos1.x + real_pos2.x) * 0.5f - textSz.x * 0.5f,
        (real_pos1.y + real_pos2.y) * 0.5f - textSz.y * 0.5f
    };
    p->AddText(textPos, text_color, text);
}

void KeysHUD(int ver, ImVec2 render_pos_arrow, ImVec2 render_pos_key, const KeyRectStyle& style, const PurpleKeyMonitor& monitor, bool align_right_arrow=true, bool align_right_key=false)
{
    const auto& g_keys_down=monitor.g_keys_down;
    const auto& g_key_mask=monitor.g_key_mask;
    const auto g_aps_cur=monitor.g_aps_cur;
    ImGuiIO& io = ImGui::GetIO();
    float iratio_x = io.DisplaySize.x / 1280.0f;
    float iratio_y = io.DisplaySize.y / 960.0f;
    ImVec2 size = { style.size.x * iratio_x,
        style.size.y * iratio_y };
    render_pos_arrow.x *= iratio_x;
    render_pos_arrow.y *= iratio_y;
    render_pos_key.x *= iratio_y;
    render_pos_key.y *= iratio_y;
    float border_size = ceilf(style.size.x * iratio_x / 32.0f*3.0f);
    auto p = ImGui::GetOverlayDrawList();

    if (align_right_arrow){
         render_pos_arrow.x -= size.x * 3.5f;
    }
    if (!style.separated) {
        render_pos_key.x = render_pos_arrow.x - size.x * 4.0f;
    } else {
        if (align_right_key)
            render_pos_key.x -= size.x * 4.0f;
    }
    p->AddRectFilled({ render_pos_arrow.x, render_pos_arrow.y }, { render_pos_arrow.x + size.x * 3.5f, render_pos_arrow.y + size.y * 2.5f }, 0xCC000000);
    p->AddRectFilled({ render_pos_key.x, render_pos_key.y }, { render_pos_key.x + size.x * 4.0f, render_pos_key.y + size.y * 2.5f }, 0xCC000000);
    render_pos_arrow.x += size.x * 0.25f;
    render_pos_arrow.y += size.y * 0.25f;
    render_pos_key.x += size.x * 0.25f;
    render_pos_key.y += size.y * 0.25f;
    render_pos_key.x = roundf(render_pos_key.x);//align to pixel
    render_pos_arrow.x = roundf(render_pos_arrow.x);
    { // arrows
        ImVec2 render_pos_orig = render_pos_arrow;
        render_pos_arrow.x += size.x;
        KeysRect(p, render_pos_arrow, size, border_size, style, g_keys_down[PurpleKeyMonitor::Key_Up], "↑");
        render_pos_arrow.y += size.y;
        render_pos_arrow.x = render_pos_orig.x;
        KeysRect(p, render_pos_arrow, size, border_size, style, g_keys_down[PurpleKeyMonitor::Key_Left], "←");
        render_pos_arrow.x += size.x;
        KeysRect(p, render_pos_arrow, size, border_size, style, g_keys_down[PurpleKeyMonitor::Key_Down], "↓");
        render_pos_arrow.x += size.x;
        KeysRect(p, render_pos_arrow, size, border_size, style, g_keys_down[PurpleKeyMonitor::Key_Right], "→");
    }
    { // keys
        ImVec2 render_pos_orig = render_pos_key;
        KeysRect(p, render_pos_key, { size.x * 1.5f, size.y }, border_size, style, g_keys_down[PurpleKeyMonitor::Key_Shift], "Δ");
        render_pos_key.x += size.x * 1.5f;
        KeysRect(p, render_pos_key, size, border_size, style, g_keys_down[PurpleKeyMonitor::Key_Z], "Z");
        render_pos_key.x += size.x;
        KeysRect(p, render_pos_key, size, border_size, style, g_keys_down[PurpleKeyMonitor::Key_X], "X");
        render_pos_key.y += size.y;
        render_pos_key.x = render_pos_orig.x;
        KeysRect(p, render_pos_key, size, border_size, style, g_keys_down[PurpleKeyMonitor::Key_Ctrl], "Σ", g_key_mask[PurpleKeyMonitor::Key_Ctrl] == 0);
        render_pos_key.x += size.x;
        KeysRect(p, render_pos_key, size, border_size, style, g_keys_down[PurpleKeyMonitor::Key_C], "C", g_key_mask[PurpleKeyMonitor::Key_C] == 0);
        render_pos_key.x += size.x;
        KeysRect(p, render_pos_key, size, border_size, style, g_keys_down[PurpleKeyMonitor::Key_D], "D", g_key_mask[PurpleKeyMonitor::Key_D] == 0);
        render_pos_key = render_pos_orig;
    }
    if (style.show_aps) {
        char aps_buffer[64];std::snprintf(aps_buffer,sizeof(aps_buffer),"aps: %3d/(60frame)",g_aps_cur);std::string aps_text=aps_buffer;
        auto sz = ImGui::CalcTextSize(aps_text.c_str());
        if (ver != 128) {
            render_pos_key.x -= size.x * 0.25f;
            render_pos_key.y -= size.y * 0.25f;
            p->AddRectFilled({ render_pos_key.x - sz.x, render_pos_key.y }, { render_pos_key.x, render_pos_key.y + sz.y }, 0xCC000000);
            p->AddText({ render_pos_key.x - sz.x, render_pos_key.y }, 0xFFFFFFFF, aps_text.c_str());
        } else {
            render_pos_key.x += size.x * 7.25f;
            render_pos_key.y += size.y * 2.25f;
            p->AddRectFilled({ render_pos_key.x - sz.x, render_pos_key.y }, { render_pos_key.x, render_pos_key.y + sz.y }, 0xCC000000);
            p->AddText({ render_pos_key.x - sz.x, render_pos_key.y }, 0xFFFFFFFF, aps_text.c_str());
        }
    }
}


}
