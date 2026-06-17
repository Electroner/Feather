#include "../feathergui.h"

void FeatherGUI::newImage()
{

    //make buttons, sliders, radiobuttons white
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25F, 0.25F, 0.25F, 1.20F));
    //Make overbutton selection white
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.75F, 0.75F, 0.75F, 1.20F));
    //Make textbox gray
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.25F, 0.25F, 0.25F, 1.20F));

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    //Create a new window to ask for the new image size and the new image name
    if (ImGui::Begin(ICON_FA_IMAGE " New Image", &this->newImagePopUp, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::InputText("Image Name", this->newImageName, 100);
        ImGui::InputInt("Image Width", &this->newImageWidth);
        ImGui::InputInt("Image Height", &this->newImageHeight);
        ImGui::Checkbox("Transparency", &this->newImageTransparency);

        //If the user press the OK button
        if (ImGui::Button("Create Image"))
        {
            MEMORYSTATUSEX status;
            status.dwLength = sizeof(status);
            GlobalMemoryStatusEx(&status);
            //Check if the user has enough memory to create the new image
            if (status.ullAvailPhys > (this->newImageWidth * this->newImageHeight * 4 * 8))
            {
                //check if the image size is bigger than 0
                if (this->newImageWidth > 0 && this->newImageHeight > 0 && this->newImageName[0] != '\0')
                {
                    ImageStr blank;
                    //Create a new image with the given parameters
                    blank.name = this->newImageName;
                    blank.width = this->newImageWidth;
                    blank.height = this->newImageHeight;
                    blank.extension = "png";
                    blank.modified = false;
                    blank.loaded = true;
                    blank.visible = true;
                    blank.opacity = 1.0f;

                    // Always use 4 channels so layers composite correctly
                    blank.channels = 4;
                    blank.data = new unsigned char[blank.width * blank.height * 4];
                    if (this->newImageTransparency)
                    {
                        // Transparent background
                        memset(blank.data, 0, blank.width * blank.height * 4);
                    }
                    else
                    {
                        // Opaque white background
                        memset(blank.data, 255, blank.width * blank.height * 4);
                    }

                    glDisable(GL_DEPTH_TEST);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                    glGenTextures(1, &blank.texture);
                    glBindTexture(GL_TEXTURE_2D, blank.texture);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    #if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
                    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
                    #endif
                    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, blank.width, blank.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, blank.data);

                    // Add as topmost layer and make it active
                    this->workStation.insertImageOnTop(blank);
                    workStation.combineLayers();
                    this->centerImage();
                    this->UpdateImage();

                    *this->newImageName = {};
                    //Close the window
                    this->newImagePopUp = false;
                }
                else
                {
                    this->errorWindowCode = 2; //No name or dimensions error
                }
            }
            else
            {
                this->errorWindowCode = 1; //No memory to create the new image error
            }
        }
        //Show Buttons on the same line
        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            *this->newImageName = {};
            this->newImagePopUp = false;
        }

        ImGui::End();
    }

    //Show Error Windows in case of error
    this->ErrorWindow();

    //Pop style
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}

void FeatherGUI::BuildCanvasResizeDialog()
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25F, 0.25F, 0.25F, 1.0F));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.45F, 0.45F, 0.45F, 1.0F));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.25F, 0.25F, 0.25F, 1.0F));

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::Begin(ICON_FA_CROP " Canvas Size", &this->canvasResizePopUp,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar))
    {

        int curW = this->workStation.getCanvasWidth();
        int curH = this->workStation.getCanvasHeight();
        ImGui::Text("Current:  %d x %d", curW, curH);
        ImGui::Separator();

        ImGui::InputInt("Width", &this->canvasNewWidth);
        ImGui::InputInt("Height", &this->canvasNewHeight);
        if (this->canvasNewWidth < 1) this->canvasNewWidth = 1;
        if (this->canvasNewHeight < 1) this->canvasNewHeight = 1;

        ImGui::Spacing();
        ImGui::Text("Anchor:");

        // 3x3 anchor grid
        const char* labels[3][3] = {
            {ICON_FA_ARROW_UP   " " ICON_FA_ARROW_LEFT,  ICON_FA_ARROW_UP,   ICON_FA_ARROW_UP   " " ICON_FA_ARROW_RIGHT},
            {ICON_FA_ARROW_LEFT,  "Center", ICON_FA_ARROW_RIGHT},
            {ICON_FA_ARROW_DOWN " " ICON_FA_ARROW_LEFT,  ICON_FA_ARROW_DOWN, ICON_FA_ARROW_DOWN " " ICON_FA_ARROW_RIGHT}
        };

        for (int ay = 0; ay < 3; ay++)
        {
            for (int ax = 0; ax < 3; ax++)
            {
                if (ax > 0) ImGui::SameLine(0, 2);
                bool selected = (ax == this->canvasAnchorX && ay == this->canvasAnchorY);
                ImGui::PushID(ay * 3 + ax);
                if (selected)
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.45f, 0.85f, 1.0f));
                if (ImGui::Button("  ##anc", ImVec2(30, 24)))
                {
                    this->canvasAnchorX = ax;
                    this->canvasAnchorY = ay;
                }
                if (selected)
                    ImGui::PopStyleColor();
                ImGui::PopID();
            }
        }

        // Show current anchor as text
        const char* hNames[] = { "Left", "Center", "Right" };
        const char* vNames[] = { "Top",  "Center", "Bottom" };
        ImGui::Text("Position: %s - %s", hNames[this->canvasAnchorX], vNames[this->canvasAnchorY]);

        ImGui::Spacing();
        ImGui::Separator();

        if (ImGui::Button("Apply", ImVec2(80, 0)))
        {
            if (this->workStation.ImagesSize() > 0)
            {
                this->workStation.pushUndoState();
                this->workStation.resizeCanvas(this->canvasNewWidth, this->canvasNewHeight,
                                               this->canvasAnchorX, this->canvasAnchorY);
                this->workStation.combineLayers();
                this->centerImage();
                this->UpdateImage();
                std::cout << "Canvas resized to " << this->canvasNewWidth
                    << " x " << this->canvasNewHeight << std::endl;
            }
            this->canvasResizePopUp = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(80, 0)))
            this->canvasResizePopUp = false;

        ImGui::End();
    }

    ImGui::PopStyleColor(3);
}