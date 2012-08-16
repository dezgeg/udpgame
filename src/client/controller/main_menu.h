#ifndef CLIENT_CONTROLLER_MAIN_MENU_H
#define CLIENT_CONTROLLER_MAIN_MENU_H

#include "client/controller/screen.h"
#include "client/view/button.h"

class MainMenu: public Screen
{
public:
  MainMenu();
  void Update(InputManager&, Real);
  void Draw(const Renderer&);
  void On();
private:
  Button m_quit;
  Button m_start;
};

extern MainMenu g_main_menu;

#endif
