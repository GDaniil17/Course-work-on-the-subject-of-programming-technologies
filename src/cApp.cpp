#include "cApp.h"
#include "cMain.h"

wxIMPLEMENT_APP(cApp);

bool cApp::OnInit()
{
	m_frame1 = new cMain(4, 4, false);
	m_frame1 -> Show();
	return true;
}
