#ifndef CMAIN_H
#define CMAIN_H

#include <wx/wx.h>
#include <vector>
#include <wx/menu.h>

/// cMain Class
///
/// Class that provides us with game field configurations and game logic

class cMain : public wxFrame
{
public:
	/// The constructor that initializes our field with certain height, width and ability to resume previous game
	///
	/// @param wid is responsible for width of the new field
	/// @param hei is responsible for height of the new field
	/// @param newSizes is responsible for generating new field (if true) and resuming previous game (if false)
	cMain(int wid, int hei, bool newSizes);

	/// The destructor
	~cMain();

	/// Field height variable by default 3
	int height = 3;

	/// Field with variable by default 3
	int width = 3;

	/// Colored variable by default false. It is responsible for colored field (if true) and black&white (if false)
	bool colored = false;

	/// theGameJustStarted variable is responsible for detecting if the game just started or it continues
	bool theGameJustStarted = true;

	/// ceils variable is responsible for UI representation of the cMain::ceilValues
	wxButton** ceils;

	/// restartBtn allows to restart the game without resuming the previously saved one
	wxButton* restartBtn;

	/// applyBtn allows to check correctness of cMain::widthText and cMain::heightText and if it is good then it generates the new field with custom parametrs
	wxButton* applyBtn;

	/// counter is responsible for UI representation of the score cMain::counter
	wxStaticText* counter;

	/// grid is responsible for UI elements alignment
	wxGridSizer* grid;

	/// widthText is responsible for custom width input
	wxTextCtrl* widthText;

	/// heightText is responsible for custom height input
	wxTextCtrl* heightText;

	/// widthLabel is an inscription for cMain::widthText
	wxStaticText* widthLabel;

	/// heightLabel is an inscription for cMain::heightText
	wxStaticText* heightLabel;

	/// bAndW is a radio button for black&white mode
	wxRadioButton* bAndW;

	/// coloredB is a radio button for colored mode
	wxRadioButton* coloredB;

	/// counterValue is responsible for storing the value that is shown in cMain::counter (UI representation)
	int counterValue = 0;

	/// ceilValues vector is responsible for storing the value that is shown in cMain::ceils (UI representation)
	std::vector<int> ceilValues;

	/// The function that is called after File -> Quit, allows to save game before closing the game or just close without saving
	///
	/// @param evt is wxCommandEvent variable that has to be in every event handler function
	void saveGameAndExit(wxCommandEvent& evt);

	/// The function that is called after File -> Save Field, allows to save game (current field)
	///
	/// @param evt is wxCommandEvent variable that has to be in every event handler function
	void saveGameFile(wxCommandEvent& evt);

	/// The function that is called after File -> Text Settings, allows to choose text parametrs for the field
	///
	/// @param evt is wxCommandEvent variable that has to be in every event handler function
	void openTextSettings(wxCommandEvent& evt);

	/// A menu bar in the top of a frame (game window), contains cMain::file with different oprions (more in cMain::file)
	wxMenuBar* menubar;

	/// A menu in the top of a frame (game window), it shows after clicking on "File" contains "Text Settings", "Save Field", "Quit"
	wxMenu* file;

	/// resumePrevGame variable is oposite to cMain::newSizes. It is responsible for detecting if the game is resumed
	bool resumePrevGame = false;

	/// The function check if the next move exists. If not than the window with option of restarting game appears.
	void checkIfGameEnd();

	/// The function that implements move up (after the arrow up was pressed) 
	void movedUp();

	/// The function that return true if the move up (that adds points to the score) exists else false
	bool dataChangedByUp();

	/// The function that return true if the move up (that shifts plates without adding points to the score) exists else false
	bool checkBeforeUp();

	/// The function that shifts plates up without adding points to the score
	void effectUp();

	/// The function that implements move down (after the arrow down was pressed) 
	void movedDown();

	/// The function that return true if the move down (that adds points to the score) exists else false
	bool dataChangedByDown();

	/// The function that return true if the move down (that shifts plates without adding points to the score) exists else false
	bool checkBeforeDown();

	/// The function that shifts plates down without adding points to the score
	void effectDown();

	/// The function that implements move right (after the arrow right was pressed) 
	void movedRight();

	/// The function that return true if the move right (that adds points to the score) exists else false
	bool dataChangedByRight();

	/// The function that return true if the move right (that shifts plates without adding points to the score) exists else false
	bool checkBeforeRight();

	/// The function that shifts plates to the right without adding points to the score
	void effectRight();

	/// The function that implements move left (after the arrow left was pressed) 
	void movedLeft();

	/// The function that return true if the move left (that adds points to the score) exists else false
	bool dataChangedByLeft();

	/// The function that return true if the move left (that shifts plates without adding points to the score) exists else false
	bool checkBeforeLeft();

	/// The function that shifts plates to the left without adding points to the score
	void effectLeft();

	/// The function generates new field and generates "2" according to current step (at the beginning 2 plates, further 1 plate if it is possible)
	void generateNew();

	/// The function colores field (more precisely colores plates) according to current color mode (cMain::bAndW, cMain::coloredB)
	void colorField();

	/// The function calls cMain::colorField if current mode is different from cMain::bAndW
	///
	/// @param evt is wxCommandEvent variable that has to be in every event handler function
	void changeToBandW(wxCommandEvent& evt);

	/// The function calls cMain::colorField if current mode is different from cMain::coloredB
	///
	/// @param evt is wxCommandEvent variable that has to be in every event handler function
	void changeToCol(wxCommandEvent& evt);

	/// The function restarts the game by generating new field and returning variable to initial values
	///
	/// @param evt is wxCommandEvent variable that has to be in every event handler function
	void onRestartBtnClicked(wxCommandEvent& evt);

	/// The function restarts the game by generating new field and returning variable to initial values
	///
	/// @param evt is wxCommandEvent variable that has to be in every event handler function
	void onApplyBtnClicked(wxCommandEvent& evt);

protected:
	/// The function that handles the keyboard events (if up, down, left, right key is pressed than we process the action)
	///
	/// @param evt is wxCommandEvent variable that has to be in every event handler function
	void onKeyDown(wxKeyEvent& evt);
};
#endif	