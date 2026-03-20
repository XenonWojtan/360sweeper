#include "StdAfx.h"
#include "globals.h"
#include "fontprops.h"

// -------------------------------------------------------
// CreateFontProps
// -------------------------------------------------------
void CreateFontProps()
{
    spacing     = 10.0f;
    letterWidth = 20.0f;
	fontScale = 1.0f;
	fontBgR = 118;
	fontBgG = 118;
	fontBgB = 118;
    std::string path = "game:\\" + GAME_ROOT_PATH + "\\packprops.txt";
    std::ofstream outFile(path.c_str(), std::ios::trunc);
    if (outFile.is_open()) {
        outFile << "spacing="     << FontPropsToString(spacing)     << "\n";
        outFile << "letterWidth=" << FontPropsToString(letterWidth) << "\n";
		outFile << "fontScale=" << FontPropsToString(fontScale) << "\n";
		outFile << "bgColor=" << FontPropsToString(fontBgR) << "," << FontPropsToString(fontBgG) << "," << FontPropsToString(fontBgB) << "\n";
        outFile.flush();
        outFile.close();
    }
}


// -------------------------------------------------------
// LoadFontProps
// -------------------------------------------------------
void LoadFontProps()
{
    std::string path = "game:\\" + GAME_ROOT_PATH + "\\packprops.txt";
    std::ifstream inFile(path.c_str());
    std::string line;

    if (inFile.is_open()) {
        while (std::getline(inFile, line)) {
            if (!line.empty() && line[line.size() - 1] == '\r')
                line.erase(line.size() - 1);

            size_t eqPos = line.find('=');
            if (eqPos == std::string::npos) continue;

            std::string key   = line.substr(0, eqPos);
            std::string value = line.substr(eqPos + 1);

            if (key == "spacing") {
                float val = (float)atof(value.c_str());
                if (val < 1.0f)  val = 1.0f;
                if (val > 64.0f) val = 64.0f;
                spacing = val;
                OutputDebugString("FontProps Applied: spacing set.\n");
            }
            else if (key == "letterWidth") {
                float val = (float)atof(value.c_str());
                if (val < 1.0f)  val = 1.0f;
                if (val > 64.0f) val = 64.0f;
                letterWidth = val;
                OutputDebugString("FontProps Applied: letterWidth set.\n");
            }
			else if (key == "fontScale") {
				float val = (float)atof(value.c_str());
				if (val < 0.1f) val = 0.1f;
				if (val > 8.0f) val = 8.0f;
				fontScale = val;
				OutputDebugString("FontProps Applied: fontScale set.\n");
						}
						else if (key == "bgColor") {
				int r = 118, g = 118, b = 118;
				size_t c1 = value.find(',');
				if (c1 != std::string::npos) {
					size_t c2 = value.find(',', c1 + 1);
					if (c2 != std::string::npos) {
						r = atoi(value.substr(0, c1).c_str());
						g = atoi(value.substr(c1 + 1, c2 - c1 - 1).c_str());
						b = atoi(value.substr(c2 + 1).c_str());
					}
				}
				if (r < 0) r = 0; if (r > 255) r = 255;
				if (g < 0) g = 0; if (g > 255) g = 255;
				if (b < 0) b = 0; if (b > 255) b = 255;
				fontBgR = r; fontBgG = g; fontBgB = b;
				OutputDebugString("FontProps Applied: bgColor set.\n");
			}
        }
        inFile.close();
        OutputDebugString("SUCCESS: packprops.txt loaded.\n");
    }
    else {
        OutputDebugString("WARNING: packprops.txt not found. Using defaults.\n");
    }
}

// -------------------------------------------------------
// InitializeFontProps
// -------------------------------------------------------
void InitializeFontProps()
{
    std::string path = "game:\\" + GAME_ROOT_PATH + "\\packprops.txt";
    std::ifstream fileCheck(path.c_str());

    if (fileCheck.is_open()) {
        fileCheck.close();
        OutputDebugString("packprops.txt found! Loading...\n");
        LoadFontProps();
    }
    else {
        OutputDebugString("packprops.txt NOT found! Creating defaults...\n");
        CreateFontProps();
    }
}