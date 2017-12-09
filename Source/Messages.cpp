/******************************************************************************/
#include "stdafx.h"
#include "Main.h"
/******************************************************************************/
Memb<Message> Messages;
/******************************************************************************/
void AddMessage(Str msg, Flt life)
{
   Message *msgPtr = &Messages.New();
   msgPtr->text = msg;
   msgPtr->life = life;
}
/******************************************************************************/
void UpdateMessages()
{
   REPA(Messages) if((Messages[i].life -= Time.d()) <= 0) Messages.remove(i, true);
}
/******************************************************************************/
void DrawMessages()
{
   FREPA(Messages)
   {
      Message &msg = Messages[i];

      TextDS tds;
	  tds.color = ColorAlpha(tds.color, Sat(msg.life * 2));
	  tds.font = defaultFont;

      D.text(tds, 0, 0.70 - 0.07 * i, msg.text);
   }
}
/******************************************************************************/
