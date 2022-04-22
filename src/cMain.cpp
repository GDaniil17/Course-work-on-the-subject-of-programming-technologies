#include "cMain.h"
#include <string>
#include <windows.h>
#include <conio.h>
#include <wx/utils.h> 
#include <iostream> 
#include <unordered_map> //hash
#include <wx/fontdlg.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/colourdata.h>
#include <wx/colordlg.h>
#include <math.h>
#include <wx/wx.h>
#include <wx/textfile.h>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <random>

enum enumsEvt {
	quitEvt, settingsEvt, textEvt, widthEv, heightEv,
	widthLabelEv, heightLabelEv, saveEvt
};

void cMain::saveGameAndExit(wxCommandEvent& evt) {
	if (wxMessageBox(_("Do you want to save the game before exit?"), _("Please confirm"),
		wxICON_QUESTION | wxYES_NO, this) == wxNO) {
		Close(true);
		return;
	}
	wxFile file;
	file.Create(wxT("game.txt"), true);
	std::string ans = std::to_string(height) + " " + std::to_string(width) + "\n";
	std::string cur;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			cur = std::to_string(ceilValues[i * width + j]);
			ans = ans + cur + " ";
		}
		ans += "\n";
	}
	ans += std::to_string(counterValue);
	file.Write(ans);
	file.Close();
	Close(true);
	evt.Skip();
}

void cMain::saveGameFile(wxCommandEvent& evt) {
	wxFile file;
	file.Create(wxT("game.txt"), true);
	std::string ans = std::to_string(height) + " " + std::to_string(width) + "\n";
	std::string cur;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			cur = std::to_string(ceilValues[i * width + j]);
			ans = ans + cur + " ";
		}
		ans += "\n";
	}
	ans += std::to_string(counterValue);
	file.Write(ans);
	file.Close();
	evt.Skip();
}

void cMain::openTextSettings(wxCommandEvent& evt) {
	wxFontDialog* fontDial = new wxFontDialog(this);
	if (fontDial->ShowModal() == wxID_OK) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				ceils[x + width * y]->SetFont(fontDial->GetFontData().GetChosenFont());
			}
		}
	}
}

cMain::cMain(int wid, int hei, bool newSizes) : wxFrame(nullptr, wxID_ANY, "2048 student edition", wxPoint(50, 50), wxSize(720, 720))
{
	resumePrevGame = !newSizes;
	wxFont btnFont(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
	counter = new wxStaticText(this, 500, "0", wxPoint(0, 0), wxDefaultSize);
	counter->SetBackgroundColour(wxColour(255, 255, 255));
	counter->SetFont(btnFont);
	std::ifstream fin("game.txt");
	if (!newSizes && fin.good()) {
		fin.close();
		freopen("game.txt", "r", stdin);
		theGameJustStarted = false;
		std::cin >> hei >> wid;
		height = hei;
		width = wid;
		ceils = new wxButton * [height * width];
		ceilValues.clear();
		ceilValues.assign(height * width, 0);

		int cur_val;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				std::cin >> cur_val;
				ceilValues[i * width + j] = cur_val;
			}
		}

		std::cin >> counterValue;
		counter->SetLabelText(std::to_string(counterValue));
	}
	else {
		theGameJustStarted = true;
		resumePrevGame = false;
		width = wid;
		height = hei;
		ceils = new wxButton * [height * width];
		ceilValues.resize(height * width, 0);
	}
	menubar = new wxMenuBar;
	file = new wxMenu;
	file->Append(textEvt, wxT("&Text Settings"));
	file->Append(saveEvt, wxT("&Save Field"));
	file->Append(quitEvt, wxT("&Quit"));
	menubar->Append(file, wxT("&File"));
	SetMenuBar(menubar);
	Connect(textEvt, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::openTextSettings));
	Connect(quitEvt, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::saveGameAndExit));
	Connect(saveEvt, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(cMain::saveGameFile));
	grid = new wxGridSizer(height, width, 0, 0);
	wxBoxSizer* sizerMain = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizerInnerBottom = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizerColorParam = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizerInnerUpper = new wxBoxSizer(wxHORIZONTAL);

	restartBtn = new wxButton(this, 1000, "Restart");
	applyBtn = new wxButton(this, 600, "Apply");

	bAndW = new wxRadioButton(this, wxID_ANY, wxT("black&white"));
	bAndW->Bind(wxEVT_COMMAND_RADIOBUTTON_SELECTED, &cMain::changeToBandW, this);
	coloredB = new wxRadioButton(this, wxID_ANY, wxT("colored"));
	coloredB->Bind(wxEVT_COMMAND_RADIOBUTTON_SELECTED, &cMain::changeToCol, this);
	widthText = new wxTextCtrl(this, widthEv, wxT(""));
	heightText = new wxTextCtrl(this, heightEv, wxT(""));;
	widthLabel = new wxStaticText(this, widthLabelEv, " width: ", wxPoint(0, 0), wxDefaultSize);
	heightLabel = new wxStaticText(this, heightLabelEv, " height: ", wxPoint(0, 0), wxDefaultSize);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			ceils[x + width * y] = new wxButton(this, 20000 + (x + width * y));
			ceils[x + width * y]->SetFont(btnFont);
			ceils[x + width * y]->Enable(false);
			ceils[x + width * y]->DisableFocusFromKeyboard();
			if (ceilValues[x + width * y]) {
				ceils[x + width * y]->SetLabelText(std::to_string(ceilValues[x + width * y]));
			}
			grid->Add(ceils[x + width * y], 1, wxEXPAND);
		}
	}
	generateNew();
	colorField();
	sizerColorParam->Add(bAndW, 0, 1);
	sizerColorParam->Add(coloredB, 0, 1);
	sizerColorParam->Add(widthLabel, 0, 1);
	sizerColorParam->Add(widthText, 0, 1);
	sizerColorParam->Add(heightLabel, 0, 1);
	sizerColorParam->Add(heightText, 0, 1);
	sizerColorParam->Add(applyBtn, 0, wxLEFT | wxTOP | wxRIGHT | wxBOTTOM, 5);
	sizerInnerBottom->Add(restartBtn, 0, wxRIGHT | wxTOP | wxDOWN, 5);//Add(grid,1,wxEXPAND||wxALL, 5);
	sizerInnerUpper->Add(counter, 0, wxCENTER, 1);
	sizerMain->Add(sizerInnerUpper, 0, wxCENTER, 5);
	sizerMain->Add(grid, 1, wxEXPAND | wxLEFT | wxTOP | wxRIGHT | wxBOTTOM, 5);
	sizerMain->Add(sizerInnerBottom, 0, wxCENTER, 5);
	sizerMain->Add(sizerColorParam, 0, wxCENTER, 5);
	this->SetSizer(sizerMain);
	this->SetFocus();
	restartBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::onRestartBtnClicked, this);
	applyBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::onApplyBtnClicked, this);
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(cMain::onKeyDown));
}

void cMain::onApplyBtnClicked(wxCommandEvent& evt) {
	std::string w = widthText->GetValue().ToStdString();
	std::string h = heightText->GetValue().ToStdString();
	if (w.empty() || h.empty() || w.find_first_not_of("0123456789") != std::string::npos ||
		h.find_first_not_of("0123456789") != std::string::npos) {
		wxMessageDialog* dial = new wxMessageDialog(NULL, wxT("Incorrect data in width or height field"),
			wxT("Error"), wxOK | wxICON_ERROR);
		dial->ShowModal();
	}
	else {
		int w_tmp = std::stoi(w);
		int h_tmp = std::stoi(h);
		if (1 < w_tmp && w_tmp <= 50 && 1 < h_tmp && h_tmp <= 50) {
			cMain* m_frame1 = new cMain(w_tmp, h_tmp, true);
			m_frame1->Show();
			Destroy();
		}
		else {
			wxMessageDialog* dial = new wxMessageDialog(NULL, wxT("Incorrect data in width or height too large (>50) or too small (<2)"),
				wxT("Error"), wxOK | wxICON_ERROR);
			dial->ShowModal();
		}
	}

}

void cMain::onRestartBtnClicked(wxCommandEvent& evt) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			ceils[j + i * width]->SetBackgroundColour(wxColour(255, 255, 255));
			ceils[j + i * width]->SetLabelText("");
			ceilValues[j + i * width] = 0;
		}
	}
	theGameJustStarted = true;
	resumePrevGame = false;
	generateNew();

	evt.Skip();
}

cMain::~cMain()
{
	delete[] ceils;
}

void cMain::changeToBandW(wxCommandEvent& evt) {
	if (colored != false) {
		colored = false;
		colorField();
	}
	evt.Skip();
}

void cMain::changeToCol(wxCommandEvent& evt) {
	if (colored != true) {
		colored = true;
		colorField();
	}
	evt.Skip();
}

void cMain::colorField() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (colored) {
				switch (ceilValues[j + i * width]) {
				case 0:
					ceils[j + i * width]->SetBackgroundColour(wxColour(255, 255, 255));
					break;
				case 2:
					ceils[j + i * width]->SetBackgroundColour(wxColour(46, 22, 99));
					break;
				case 4:
					ceils[j + i * width]->SetBackgroundColour(wxColour(45, 54, 100));
					break;
				case 8:
					ceils[j + i * width]->SetBackgroundColour(wxColour(44, 74, 98));
					break;
				case 16:
					ceils[j + i * width]->SetBackgroundColour(wxColour(45, 94, 100));
					break;
				case 32:
					ceils[j + i * width]->SetBackgroundColour(wxColour(43, 100, 86));
					break;
				case 64:
					ceils[j + i * width]->SetBackgroundColour(wxColour(150, 133, 23));
					break;
				default:
					ceils[j + i * width]->SetBackgroundColour(wxColour(194, 194, 194));
				}
			}
			else {
				if (ceilValues[j + i * width]) {
					int tmp = int(log2(ceilValues[j + i * width])) % 2;
					if (tmp == 0) {
						ceils[j + i * width]->SetBackgroundColour(wxColour(0, 0, 0));
					}
					else {
						ceils[j + i * width]->SetBackgroundColour(wxColour(194, 194, 194));
					}
				}
				else {
					ceils[j + i * width]->SetBackgroundColour(wxColour(255, 255, 255));
					ceils[j + i * width]->SetForegroundColour(wxColour(0, 0, 0));
				}
			}
		}
	}
}

void cMain::generateNew() {
	if (!resumePrevGame) {
		int mines = 1;
		if (theGameJustStarted == true) {
			counterValue = 0;
			counter->SetLabelText(std::to_string(counterValue));
			resumePrevGame = true;
			mines = 2;
			theGameJustStarted = false;
		}
		int cnt = 0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if (!ceilValues[x + y * width]) {
					cnt += 1;
					if (cnt == mines) {
						break;
					}
				}
			}
			if (cnt == mines) {
				break;
			}
		}

		if (cnt != mines) {
			mines = cnt;
		}
		while (mines != 0) {
			std::random_device dev;
			std::mt19937 rng(dev());
			std::uniform_int_distribution<std::mt19937::result_type> distX(0, width-1);
			std::random_device dev2;
			std::mt19937 rng2(dev2());
			std::uniform_int_distribution<std::mt19937::result_type> distY(0, height - 1);
			int randomX = distX(rng);
			int randomY = distY(rng2);

			if (!ceilValues[randomX + randomY * width]) {
				mines -= 1;
				ceilValues[randomX + randomY * width] = 2;
				ceils[randomX + randomY * width]->SetLabelText("2");
			}
		}
		colorField();
	}
	resumePrevGame = false;
}

bool cMain::checkBeforeUp() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = i; k > 0; k--) {
				if (ceilValues[j + k * width] && !ceilValues[j + (k - 1) * width]) {
					return true;
				}
			}
		}
	}
	return false;
}

void cMain::effectUp() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = i; k > 0; k--) {
				if (ceilValues[j + k * width] && !ceilValues[j + (k - 1) * width]) {
					ceilValues[j + (k - 1) * width] = ceilValues[j + k * width];
					ceils[j + (k - 1) * width]->SetLabelText(std::to_string(ceilValues[j + k * width]));
					ceilValues[j + k * width] = 0;
					ceils[j + k * width]->SetLabelText("");
				}
			}
		}
	}
}

bool cMain::checkBeforeDown() {
	for (int i = height - 1; i > -1; i--) {
		for (int j = 0; j < width; j++) {
			for (int k = i; k < height - 1; k++) {
				if (!ceilValues[j + (k + 1) * width] && ceilValues[j + k * width]) {
					return true;
				}
				else {
					break;
				}
			}
		}
	}
	return false;
}

void cMain::effectDown() {
	for (int i = height - 1; i > -1; i--) {
		for (int j = 0; j < width; j++) {
			for (int k = i; k < height - 1; k++) {
				if (!ceilValues[j + (k + 1) * width] && ceilValues[j + k * width]) {
					ceilValues[j + (k + 1) * width] = ceilValues[j + k * width];
					ceilValues[j + k * width] = 0;
					ceils[j + k * width]->SetLabelText("");
					if (ceilValues[j + (k + 1) * width]) {
						ceils[j + (k + 1) * width]->SetLabelText(std::to_string(ceilValues[j + (k + 1) * width]));
					}
					else {
						ceils[j + (k + 1) * width]->SetLabelText("");
					}
				}
				else {
					break;
				}
			}
		}
	}
}

bool cMain::checkBeforeRight() {
	for (int i = 0; i < height; i++) {
		for (int j = width - 1; j > -1; j--) {
			for (int k = j; k < width - 1; k++) {
				if (!ceilValues[(k + 1) + i * width] && ceilValues[k + i * width]) {
					return true;
				}
			}
		}
	}
	return false;
}

void cMain::effectRight() {
	for (int i = 0; i < height; i++) {
		for (int j = width - 1; j > -1; j--) {
			for (int k = j; k < width - 1; k++) {
				if (!ceilValues[(k + 1) + i * width] && ceilValues[k + i * width]) {
					ceilValues[(k + 1) + i * width] = ceilValues[k + i * width];
					ceilValues[k + i * width] = 0;
					ceils[(k + 1) + i * width]->SetLabelText(std::to_string(ceilValues[(k + 1) + i * width]));
					ceils[k + i * width]->SetLabelText("");
				}
			}
		}
	}
}

bool cMain::checkBeforeLeft() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = j; k > 0; k--) {
				if (ceilValues[i * width + k] && !ceilValues[i * width + k - 1]) {
					return true;
				}
			}
		}
	}
	return false;
}

void cMain::effectLeft() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = j; k > 0; k--) {
				if (ceilValues[i * width + k] && !ceilValues[i * width + k - 1]) {
					ceilValues[i * width + k - 1] = ceilValues[i * width + k];
					ceilValues[i * width + k] = 0;
					ceils[i * width + k - 1]->SetLabelText(std::to_string(ceilValues[i * width + k - 1]));
					ceils[i * width + k]->SetLabelText("");
				}
			}
		}
	}
}

bool cMain::dataChangedByUp() {
	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width; j++) {
			if (ceilValues[j + i * width] == ceilValues[j + (i + 1) * width]
				&& ceilValues[j + i * width] != 0) {
				return true;
			}
		}
	}
	return false;
}


void cMain::movedUp() {
	bool movedOrNot = checkBeforeUp();
	effectUp();

	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width; j++) {
			if (ceilValues[j + i * width] == ceilValues[j + (i + 1) * width]
				&& ceilValues[j + i * width] != 0) {
				movedOrNot = true;
				counterValue += ceilValues[j + i * width] * 2;
				counter->SetLabelText(std::to_string(counterValue));
				ceilValues[j + i * width] += ceilValues[j + (i + 1) * width];
				ceilValues[j + (i + 1) * width] = 0;
				ceils[j + (i + 1) * width]->SetLabelText("");
				if (ceilValues[j + i * width]) {
					ceils[j + i * width]->SetLabelText(std::to_string(ceilValues[j + i * width]));
				}
				else {
					ceils[j + i * width]->SetLabelText("");
				}
			}
		}
	}

	movedOrNot |= checkBeforeUp();
	effectUp();
	if (movedOrNot) {
		generateNew();
	}
}

bool cMain::dataChangedByDown() {
	for (int i = height - 1; i > 0; i--) {
		for (int j = 0; j < width; j++) {
			if (ceilValues[j + i * width] == ceilValues[j + (i - 1) * width]
				&& ceilValues[j + i * width] != 0) {
				return true;
			}
		}
	}
	return false;
}

void cMain::movedDown() {
	bool movedOrNot = checkBeforeDown();
	effectDown();

	for (int i = height - 1; i > 0; i--) {
		for (int j = 0; j < width; j++) {
			if (ceilValues[j + i * width] == ceilValues[j + (i - 1) * width]
				&& ceilValues[j + i * width] != 0) {
				movedOrNot = true;
				counterValue += ceilValues[j + i * width] * 2;
				counter->SetLabelText(std::to_string(counterValue));
				ceilValues[j + i * width] += ceilValues[j + (i - 1) * width];
				ceilValues[j + (i - 1) * width] = 0;
				if (ceilValues[j + i * width]) {
					ceils[j + i * width]->SetLabelText(std::to_string(ceilValues[j + i * width]));
				}
				else {
					ceils[j + i * width]->SetLabelText("");
				}
				ceils[j + (i - 1) * width]->SetLabelText("");
			}
		}
	}
	movedOrNot |= checkBeforeDown();
	effectDown();
	if (movedOrNot == true) {
		generateNew();
	}
}

bool cMain::dataChangedByRight() {
	for (int i = 0; i < height; i++) {
		for (int j = width - 2; j >= 0; j--) {
			if (ceilValues[(j + 1) + i * width] == ceilValues[j + i * width]
				&& ceilValues[j + i * width] != 0) {
				return true;
			}
		}
	}
	return false;
}

void cMain::movedRight() {
	bool movedOrNot = checkBeforeRight();
	effectRight();

	for (int i = 0; i < height; i++) {
		for (int j = width - 2; j >= 0; j--) {
			if (ceilValues[(j + 1) + i * width] == ceilValues[j + i * width]
				&& ceilValues[j + i * width] != 0) {
				movedOrNot = true;
				counterValue += ceilValues[(j + 1) + i * width] * 2;
				counter->SetLabelText(std::to_string(counterValue));
				ceilValues[(j + 1) + i * width] += ceilValues[j + i * width];
				ceilValues[j + i * width] = 0;
				ceils[j + i * width]->SetLabelText("");
				if (ceilValues[(j + 1) + i * width]) {
					ceils[(j + 1) + i * width]->SetLabelText(std::to_string(ceilValues[(j + 1) + i * width]));
				}
				else {
					ceils[(j + 1) + i * width]->SetLabelText("");
				}
			}
		}
	}

	movedOrNot |= checkBeforeRight();
	effectRight();
	if (movedOrNot) {
		generateNew();
	}
}

bool cMain::dataChangedByLeft() {
	for (int i = 0; i < height; i++) {
		for (int j = 1; j < width; j++) {
			if (ceilValues[i * width + j] == ceilValues[i * width + j - 1]
				&& ceilValues[i * width + j] != 0) {
				return true;
			}
		}
	}
	return false;
}

void cMain::movedLeft() {
	bool movedOrNot = checkBeforeLeft();
	effectLeft();

	for (int i = 0; i < height; i++) {
		for (int j = 1; j < width; j++) {
			if (ceilValues[i * width + j] == ceilValues[i * width + j - 1]
				&& ceilValues[i * width + j] != 0) {
				movedOrNot = true;
				counterValue += ceilValues[i * width + j] * 2;
				counter->SetLabelText(std::to_string(counterValue));
				ceilValues[i * width + j - 1] += ceilValues[i * width + j];
				ceilValues[i * width + j] = 0;
				if (!ceilValues[i * width + j - 1]) {
					ceils[i * width + j - 1]->SetLabelText("");
				}
				else {
					ceils[i * width + j - 1]->SetLabelText(std::to_string(ceilValues[i * width + j - 1]));
				}
				ceils[i * width + j]->SetLabelText("");
			}
		}
	}

	movedOrNot |= checkBeforeLeft();
	effectLeft();
	if (movedOrNot) {
		generateNew();
	}
}

void cMain::checkIfGameEnd() {
	if (!(dataChangedByDown() || checkBeforeDown()) && !(dataChangedByUp() || checkBeforeUp())
		&& !(dataChangedByLeft() || checkBeforeLeft()) && !(dataChangedByRight() || checkBeforeRight())) {
		if (wxMessageBox(_("The game is over. Restart?"), _("Please confirm"),
			wxICON_QUESTION | wxYES_NO, this) == wxYES) {
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					ceils[j + i * width]->SetBackgroundColour(wxColour(255, 255, 255));
					ceils[j + i * width]->SetLabelText("");
					ceilValues[j + i * width] = 0;
				}
			}
			theGameJustStarted = true;
			resumePrevGame = false;
			generateNew();
		}
		this->SetFocus();
	}
}

void cMain::onKeyDown(wxKeyEvent& evt) {
	int keycode = evt.GetKeyCode();
	switch (keycode) {
	case WXK_LEFT:
		movedLeft();
		checkIfGameEnd();
		break;
	case WXK_RIGHT:
		movedRight();
		checkIfGameEnd();
		break;
	case WXK_DOWN:
		movedDown();
		checkIfGameEnd();
		break;
	case WXK_UP:
		movedUp();
		checkIfGameEnd();
		break;
	default:
		checkIfGameEnd();
		evt.Skip();
	}
}