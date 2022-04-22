#ifndef CAPP_H
#define CAPP_H

#include <wx/wx.h>
#include "cMain.h"

/// cApp Class
///
/// Class that calls cMain class to start our game

class cApp: public wxApp
{
public:
	/// The constructor of cApp by default
	cApp() = default;
	/// The destructor of cApp by default
	~cApp() = default;

private:
	/// The field variable of cMain type
	cMain* m_frame1 = nullptr;
	
public:
	/// Here we start the game
	virtual bool OnInit();

};
#endif

