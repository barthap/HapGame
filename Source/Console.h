/******************************************************************************/
#pragma once
namespace EEDevTools{
/******************************************************************************/
   class Console
   {
   private:
      struct Key
      {
         Str8 command;

         Console::Key(            );
         Console::Key(Str8 command);
      };
      
      struct Data
      {
         Byte *member ;

         void (*func)    (    ) ;
         void (*funcByte)(Byte) ;

         Byte minValue ;
         Byte maxValue ;

public:
   Data();
      };
   
      Flt             blinkAnim;
      Map<Key, Data>  commands;
      GuiPC           gui;
      Memc<Byte>      history;
      Str8            input;
      Str8            inputPrefix;
      Bool            invalidCommand;
      Char            key;
      Byte            maxCommandLength;
      Memc<Byte>      searchResults;
      I8              selectedIndex;
      Text            text;
      Str8            topResult;
      CChar          *validKeys;
      
      static Bool Create (Data &data, Key &key, Ptr user);
      static Int  Compare(Key  &a,    Key &b);            
      
      void clear(Bool input = true);
      
      void drawAutocomplete(Memc<Byte> indexes, Vec2 autocompleteTextOffset, Int width);
      
      Bool executeCommand(Str8 command, Str8 value);
      
      void getSearchResults(Str8 command, Bool spaces, Bool startsWith, Bool &commandFound);
      
      Str8 getValue();
      
      Bool validCommand(Str8 command);
      
   public:
      Color      autocompleteRectColor  ;    // Background color of any non-selected autocomplete command (default = Color(64, 64, 64, 128))
      Color      autocompleteTextColor  ;    // Text color of any autocomplete command (default = Color(192, 192, 192, 255))
      CChar     *blinkChar              ;    // Blinking character that indicates where the next key will appear (default = "|")
      Bool       clearOnClose           ;    // If true, clears any user input when the console is closed (default = true)
      Bool       closeOnExecute         ;    // If true, closes the console when a command is successfully executed (default = true)
      Bool       eatKeys                ;    // If true, calls Kb.eat() to prevent, for example, any character movement from occuring while the console is opened (default = true). IMPORTANT: If true, console.update() must be called BEFORE Game::World.update()!
      Bool       enabled                ;    // If true, the console is open (default = false). NOTE: Can be set to true/false if you need to open/close the console without user input.
      Color      invalidTextColor       ;    // Color of invalid commands (default = RED)
      Byte       maxHistory             ;    // Max number of previously executed commands that should appear when the console has no user input (default = 3)
      Byte       maxSearchResults       ;    // Max number of search results (autocompletion) that should appear when the console receives user input (default = 5)
      Color      rectColor              ;    // Color of the console (default = Color(0, 0, 0, 128))
      Color      selectedIndexRectColor ;    // Background color of any selected autocomplete command (default = Color(128, 128, 128, 128))
      Color      textColor              ;    // Color of user input (default = WHITE)
      KB_BUTTON  toggleKey              ;    // KB_BUTTON used to open/close the console (default = KB_TILDE)
      
      Console();
      
	  // Call once in Init()
      Console& init(Str8 inputPrefix = L"/", Byte maxCommandLength = 32);
      
	  // Call once in Update() BEFORE Game::World.update()
      void update();
      
	  // Call once in Draw() AFTER Renderer(Render)
      void draw();
      
	  // Set a command that, when executed, will set the user inputted value to the specified member. IMPORTANT: Invalid commands will NOT be set! Valid commands must be less than or equal to the maximum length defined by console.init() (default = 32) and ONLY contain characters defined by console.setValidKeys (default = "abcdefghijklmnopqrstuvwyz") and a single underscore.
      Console& setCommand(Str8 command, Byte &member, Byte minValue = 0, Byte maxValue = 1);
      
      Console& setCommand(Str8 command, Bool &member, Byte minValue = 0, Byte maxValue = 1);
      Console& setCommand(Str8 command, I8   &member, Byte minValue = 0, Byte maxValue = 1);
      Console& setCommand(Str8 command, I16  &member, Byte minValue = 0, Byte maxValue = 1);
      Console& setCommand(Str8 command, I32  &member, Byte minValue = 0, Byte maxValue = 1);
      Console& setCommand(Str8 command, I64  &member, Byte minValue = 0, Byte maxValue = 1);
      Console& setCommand(Str8 command, U16  &member, Byte minValue = 0, Byte maxValue = 1);
      Console& setCommand(Str8 command, U32  &member, Byte minValue = 0, Byte maxValue = 1);
      Console& setCommand(Str8 command, U64  &member, Byte minValue = 0, Byte maxValue = 1);
      Console& setCommand(Str8 command, Flt  &member, Byte minValue = 0, Byte maxValue = 1);
      Console& setCommand(Str8 command, Real &member, Byte minValue = 0, Byte maxValue = 1);
      
	  // Set a command that, when executed, will call the specified function. IMPORTANT: Invalid commands will NOT be set! Valid commands must be less than or equal to the maximum length defined in console.init() (default = 32) and ONLY contain characters defined in console.validKeys (default = "abcdefghijklmnopqrstuvwyz") and a single underscore.
      Console& setCommand(Str8 command, void (*func)());
      
	  // Set a command that, when executed, will call the specified function along with the user inputted value as an argument. IMPORTANT: Invalid commands will NOT be set! Valid commands must be less than or equal to the maximum length defined in console.init() (default = 32) and ONLY contain characters defined in console.validKeys (default = "abcdefghijklmnopqrstuvwyz") and a single underscore.
      Console& setCommand(Str8 command, void (*funcByte)(Byte), Byte minValue = 0, Byte maxValue = 1);
      
	  // Font for all text (default = &FontDefault). RECOMMENDED: Use the included font, "LiberationMono-Regular".
      Console& setFont(Font *font);
      
	  // Characters that appear before the user input (default = "/")
      Console& setInputPrefix(Str8 inputPrefix);
      
	  // Valid characters for commands, with the exception of a single underscore (case-insensitive, default = "abcdefghijklmnopqrstuvwxyz"). IMPORTANT: Underscores and spaces are always invalid and, if found, the specified valid keys will NOT be set!
      Console& setValidKeys(CChar *validKeys);
   };
/******************************************************************************/
} // namespace EEDevTools
/******************************************************************************/
