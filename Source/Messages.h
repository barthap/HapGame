/******************************************************************************/
void AddMessage(Str msg, Flt life = 3);
void UpdateMessages();
void DrawMessages();
/******************************************************************************/
struct Message
{
   Str text;
   Flt life;

   Message()
   {
      life = 3;
   }
};
extern Memb<Message> Messages;
/******************************************************************************/
