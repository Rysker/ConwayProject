#include "GUIPanel.h"
#include "IConwayAlgorithm.h"
#include <cstring>

GUIPanel::GUIPanel(AppState& state, IAppControls& appControls): state_(state), appControls_(appControls), menuWidth_(250.0f), currentImplIndex_(0)
{
    std::strncpy(filenameBuffer_, "save_state.txt", 128);
    filenameBuffer_[127] = '\0';
    implementations_.push_back("Wersja sekwencyjna");
    implementations_.push_back("Wersja OpenMP");
}

char* GUIPanel::getFilenameBufferPtr()
{
    return filenameBuffer_;
}

void GUIPanel::draw(std::map<std::string, std::unique_ptr<ICommand>>& commands)
{
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - menuWidth_, 0));
    ImGui::SetNextWindowSize(ImVec2(menuWidth_, ImGui::GetIO().DisplaySize.y));

    ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    ImGui::Text("Implementation:");

    if (ImGui::Combo("##impl_combo", &currentImplIndex_, implementations_.data(), static_cast<int>(implementations_.size())))
        appControls_.changeImplementation(currentImplIndex_);

    ImGui::TextDisabled("Active: %s", implementations_.at(currentImplIndex_));
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("Simulation");
    ImGui::Separator();

    if (state_.isPaused)
    {
        if (ImGui::Button("Start", ImVec2(menuWidth_ - 15, 30)))
            commands.at("play")->execute();
    }
    else
    {
        if (ImGui::Button("Pause", ImVec2(menuWidth_ - 15, 30)))
            commands.at("pause")->execute();
    }

    if (ImGui::Button("Clear board", ImVec2(menuWidth_ - 15, 25)))
        commands.at("clear")->execute();

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("Speed (s/generation): %.2f", state_.generationTimeSec);
    ImGui::SliderFloat("##time", &state_.generationTimeSec, 0.01f, 2.0f);

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("Tools (only when paused)");
    ImGui::Separator();
    ImGui::RadioButton("Brush", reinterpret_cast<int*>(&state_.currentTool), static_cast<int>(TOOL::BRUSH));
    ImGui::SameLine();
    ImGui::RadioButton("Eraser", reinterpret_cast<int*>(&state_.currentTool), static_cast<int>(TOOL::ERASER));
    ImGui::SliderInt("Brush size", &state_.brushSize, 1, 20);

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("File");
    ImGui::Separator();

    ImGui::InputText("File name", filenameBuffer_, 128);

    if (ImGui::Button("Save state", ImVec2(menuWidth_ - 15, 25)))
        commands.at("save")->execute();

    if (ImGui::Button("Load state", ImVec2(menuWidth_ - 15, 25)))
        commands.at("load")->execute();

    ImGui::End();
}