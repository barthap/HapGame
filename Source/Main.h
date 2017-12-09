/******************************************************************************/
struct InventoryGui;
struct LOOTGUI;
struct AI;
struct _Quest;
struct _QuestGui;
struct _animFight;
struct Item;
struct scriptVariable;
struct scriptVariableManager;
struct ModClass;
struct _Time;
struct Fireball;
struct SaveGame;
struct Teleport1;
struct Horse;
struct _Storage;
struct _MerchantGui;
struct DestructibleEC;
struct Trigger;
struct Player;

class DialogueClass;
class  LuaClass;

extern AI *scriptPointer;
extern Player* plrPointer;
extern DialogueClass *dialoguePointer;

extern DialogueClass Dialogue;

//extern Bool newGame;
//extern Str  fileToLoad;
/******************************************************************************/
extern Font *defaultFont;
extern TextDS mainTds;
/******************************************************************************/
#include "../Data/Enum/_enums.h"


#include "Steering.h"

//Freecode Objects
#include "Freecode Objects/Tree.h"

#include "universalMenu.h"
#include "Options.h"

#include "scriptVariables.h"
#include "scriptFunctions.h"
#include "3DSoundObject.h"

#include "Camera.h"
#include "Inventory.h"
#include "Inventory Gui.h"
#include "Torchiere.h"
#include "Character.h"
#include "Door.h"
#include "Player.h"
#include "LootGui.h"
#include "Item.h"
#include "Messages.h"
#include "AI.h"
#include "Stats Gui.h"
#include "smallcodeObjects.h"
#include "Quest.h"
#include "QuestGui.h"
#include "Dialogue.h"
#include "Mods.h"
#include "Time.h"
#include "Weather.h"
#include "Fireball.h"
#include "SaveGame.h"
#include "Teleport.h"
#include "Horse.h"
#include "Storage.h"
#include "MerchantGui.h"
#include "Forces.h"
#include "Shortcuts.h"
#include "Loader.h"
#include "Destructible.h"
#include "Trigger.h"

#include "Menu.h"
#include "Game.h"
/******************************************************************************/
extern Game::Obj *Lit; //Highlighted world object
/******************************************************************************/
