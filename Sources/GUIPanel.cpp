#include "GUIPanel.h"
#include "IConwayAlgorithm.h"
#include <cstring>

GUIPanel::GUIPanel(AppState& state, const char* simName): m_state(state), m_simName(simName), m_menuWidth(250.0f)
{
    std::strncpy(m_filenameBuffer, "save_state.txt", 128);
    m_filenameBuffer[127] = '\0';
}

char* GUIPanel::getFilenameBufferPtr()
{
    return m_filenameBuffer;
}

void GUIPanel::draw(std::map<std::string, std::unique_ptr<ICommand>>& commands)
{
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - m_menuWidth, 0));
    ImGui::SetNextWindowSize(ImVec2(m_menuWidth, ImGui::GetIO().DisplaySize.y));

    ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    ImGui::Text("Implementation:");
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", m_simName);

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("Simulation");
    ImGui::Separator();

    if (m_state.isPaused)
    {
        if (ImGui::Button("Start", ImVec2(m_menuWidth - 15, 30)))
            commands.at("play")->execute();
    }
    else
    {
        if (ImGui::Button("Pause", ImVec2(m_menuWidth - 15, 30)))
            commands.at("pause")->execute();
    }

    if (ImGui::Button("Clear board", ImVec2(m_menuWidth - 15, 25)))
        commands.at("clear")->execute();

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("Speed (s/generation): %.2f", m_state.generationTimeSec);
    ImGui::SliderFloat("##time", &m_state.generationTimeSec, 0.01f, 2.0f);

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("Tools (only when paused)");
    ImGui::Separator();
    ImGui::RadioButton("Brush", reinterpret_cast<int*>(&m_state.currentTool), static_cast<int>(TOOL::BRUSH));
    ImGui::SameLine();
    ImGui::RadioButton("Eraser", reinterpret_cast<int*>(&m_state.currentTool), static_cast<int>(TOOL::ERASER));
    ImGui::SliderInt("Brush size", &m_state.brushSize, 1, 20);

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Text("File");
    ImGui::Separator();

    ImGui::InputText("File name", m_filenameBuffer, 128);

    if (ImGui::Button("Save state", ImVec2(m_menuWidth - 15, 25)))
        commands.at("save")->execute();

    if (ImGui::Button("Load state", ImVec2(m_menuWidth - 15, 25)))
        commands.at("load")->execute();

    ImGui::End();
}