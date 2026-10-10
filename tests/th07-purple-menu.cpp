#include "PurpleHotkey.hpp"
#include <imgui_internal.h>
#include <cassert>
#include <cstdio>

int main()
{
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(640, 480);
    io.DeltaTime = 1.0f / 60.0f;
    io.IniFilename = nullptr;
    io.Fonts->AddFontDefault();
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    bool status = false;
    ImVec2 target;
    for (int frame = 0; frame < 12; ++frame)
    {
        io.MousePos = frame >= 2 ? target : ImVec2(-100, -100);
        io.MouseDown[0] = frame == 2 || frame == 7;
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::Begin("Mod Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        const int colors = GImGui->ColorStack.Size;
        const int flags = GImGui->ItemFlagsStack.Size;
        PracticeRuntime::DrawPurpleHotkey("U", "Enemy Invincible", status);
        assert(GImGui->ColorStack.Size == colors);
        assert(GImGui->ItemFlagsStack.Size == flags);
        const auto lo = ImGui::GetItemRectMin(), hi = ImGui::GetItemRectMax();
        target = ImVec2((lo.x + hi.x) * .5f, (lo.y + hi.y) * .5f);
        assert(hi.x > lo.x && hi.y > lo.y);
        if (frame == 3) assert(status);
        if (frame == 8) assert(!status);
        ImGui::End();
        ImGui::Render();
        assert(ImGui::GetDrawData()->Valid);
    }
    ImGui::DestroyContext();
    std::puts("TH07 native purple hotkey: cold open, click, toggle, reopen stacks PASS");
}
