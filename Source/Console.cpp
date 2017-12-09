/******************************************************************************/
#include "stdafx.h"
#include "Console.h"
/******************************************************************************/
namespace EEDevTools
{
         Console::Key::Key(            ) {                      }
         Console::Key::Key(Str8 command) { T.command = command; }
      Bool Console::Create(Data &data, Key &key, Ptr user) { return true; }
      Int  Console::Compare(Key  &a,    Key &b)             { return 1;    }
      void Console::clear(Bool input )
      {
         if (input) T.input = inputPrefix;
         
         invalidCommand = false;
         selectedIndex  = -1;
         topResult      = S;
         
         searchResults.clear();
      }
      void Console::drawAutocomplete(Memc<Byte> indexes, Vec2 autocompleteTextOffset, Int width)
      {
         FREPA(indexes)
         {
            Key  &key  = commands.lockedKey (indexes(i));
            Data &data = commands.lockedData(indexes(i));
            
            Byte minY = 20 * (i + 1);
            D.rect(((i == selectedIndex) ? selectedIndexRectColor : autocompleteRectColor), D.pixelToScreen(Rect(0, minY, width, minY + 20)));
            
            text.setPos(autocompleteTextOffset - D.pixelToScreenSize(Vec2(0, minY)));
            text.set(key.command + ((data.minValue != data.maxValue) ? S + L' ' + data.minValue + L" .. " + data.maxValue : L''));
            text.draw(gui);
         }
      }
      Bool Console::executeCommand(Str8 command, Str8 value)
      {
         Data *data = NULL;
         
         commands.lock();
         
         FREPA(commands)
         {
            if (!Equal(command, commands.lockedKey(i).command)) continue;
            
            if (maxHistory)
            {
               Byte index = history.find(i);
               if (index >= 0) history.remove(index, true);
               
               history.NewAt(0) = i;
               if (history.elms() > maxHistory) history.removeLast();
            }
            
            data = &commands.lockedData(i);
            break;
         }
         
         commands.unlock();
         
         if (data == NULL)
         {
            invalidCommand = true;
            return false;
         }
         
         Byte byteValue = TextInt(value);
         if (byteValue < data->minValue) byteValue = data->minValue;
         if (byteValue > data->maxValue) byteValue = data->maxValue;
         
         if (data->member != NULL) {
            *data->member = byteValue;
         } else if (data->func != NULL) {
            data->func();
         } else {
            data->funcByte(byteValue);
         }
         
         return true;
      }
      void Console::getSearchResults(Str8 command, Bool spaces, Bool startsWith, Bool &commandFound)
      {
         if (commandFound || searchResults.elms() == maxSearchResults) return;
         
         FREPA(commands)
         {
            Key &key = commands.lockedKey(i);
            if (searchResults.has(i) || (startsWith && !Starts(key.command, command)) || (!startsWith && !Contains(key.command, command))) continue;
            
            if (Equal(command, key.command))
            {
               commandFound = true;
               
               if (spaces) {
                  searchResults.clear();
                  searchResults.add(i);
                  return;
               }
            }
            
            searchResults.add(i);
            if (searchResults.elms() == maxSearchResults) return;
         }
      }
      Str8 Console::getValue()
      {
         Memc<Str> parts = Split(input, L' ');
         if (!parts.elms()) return S;
         
         return parts(1);
      }
      Bool Console::validCommand(Str8 command)
      {
         if (!command.is() || command.length() > maxCommandLength || Split(command, L'_').elms() > 2) return false;
         
         FREP(command.length())
         {
            if (Contains(validKeys, CaseDown(command[i])) || command[i] == L'_') continue;
            return false;
         }
         
         return true;
      }
      Console::Console() : commands(Create, Compare), autocompleteRectColor(Color( 64,  64,  64, 128)), autocompleteTextColor(Color(192, 192, 192, 255)), blinkChar(L"|"), clearOnClose(true), closeOnExecute(true), eatKeys(true), enabled(false), invalidTextColor(RED), maxHistory(3), maxSearchResults(5), rectColor(Color(  0,   0,   0, 128)), selectedIndexRectColor(Color(128, 128, 128, 128)), textColor(WHITE), toggleKey(KB_TILDE) {}
      Console& Console::init(Str8 inputPrefix , Byte maxCommandLength )
      {
         setInputPrefix(inputPrefix);
         T.maxCommandLength = ((maxCommandLength > 0) ? maxCommandLength : 32);
         
         selectedIndex = -1;
         setValidKeys(L"abcdefghijklmnopqrstuvwxyz");
         
         text.create();
         text.tds->align = Vec2(1, 0);
         
         return T;
      }
      void Console::update()
      {
         if (Kb.bp(toggleKey))
         {
            enabled = !enabled;
            if (!enabled && clearOnClose) clear();
         }
         
         if (!enabled) return;
         key = Kb.c();
         
         blinkAnim += Time.d();
         if (blinkAnim >= 1) blinkAnim = 0;
         
         Str8 lastInput  = input;
         Bool hasCommand = (input.length() > inputPrefix.length());
         
         if (Kb.f(KF_BACK) && hasCommand) input.removeLast();
         
         Bool spaces = Contains(input, L' ');
         Str8 value  = getValue();
         
         if ((!spaces && ((input.length() < inputPrefix.length() + maxCommandLength && (Contains(validKeys, CaseDown(key)) || (key == L'_' && !Contains(input, L'_')))) || (key == L' ' && hasCommand))) || (spaces && value.length() < 3 && Contains(L"1234567890", key))) input += key;
         
         if (Kb.bp(KB_ENTER))
         {
            if (selectedIndex >= 0) { 
               commands.lock();
               
               input = inputPrefix + commands.lockedKey(((searchResults.elms()) ? searchResults(selectedIndex) : history(selectedIndex))).command + L' ';
                              
               commands.unlock();
            } else if (topResult.is()) {
               input += topResult + L' ';
            } else {
               Str8 command = SkipStart(SkipEnd(input, (Str8)L' ' + value), inputPrefix);
               
               if (executeCommand(command, value))
               {
                  clear();
                  
                  if (closeOnExecute)
                  {
                     enabled = false;
                     return;
                  }
               }
            }
         }
         
         if (Byte elms = ((searchResults.elms()) ? searchResults.elms() : history.elms()))
         {
            if (Kb.f(KF_DOWN) && selectedIndex <  elms - 1) selectedIndex++;
            if (Kb.f(KF_UP)   && selectedIndex >= 0)        selectedIndex--;
         }
         
         if (eatKeys) Kb.eat();
         if (Equal(input, lastInput)) return;
         
         clear(false);
         if (!maxSearchResults || Equal(input, inputPrefix)) return;
         
         spaces = Contains(input, L' ');
         value  = getValue();
         
         Str8 command      = SkipStart(SkipEnd(input, (Str8)L' ' + value), inputPrefix);
         Bool commandFound = false;
         
         commands.lock();
         
         getSearchResults(command, spaces, true,  commandFound);
         getSearchResults(command, spaces, false, commandFound);
         
         if ((!commandFound && spaces) || (commandFound && value.is())) searchResults.clear();
         
         if (searchResults.elms()) {
            if (!commandFound && !spaces)
            {
               Key &key = commands.lockedKey(searchResults(0));
               if (Starts(key.command, command)) topResult = SkipStart(key.command, command);
            }
         } else if (!commandFound) {
            invalidCommand = true;
         }
         
         commands.unlock();
      }
      void Console::draw()
      {
         if (!enabled) return;
         gui.clip = D.viewRect();
         
         text.tds->color = autocompleteTextColor;
         
         Vec2 textOffset             = D.pixelToScreen(Vec2(5, 10));
         Vec2 autocompleteTextOffset = textOffset + Vec2(text.tds->textWidth(inputPrefix), 0);
         Int  width                  = D.screenToPixel(gui.clip).w();
         
         commands.lock();
         
         drawAutocomplete(((Equal(input, inputPrefix)) ? history : searchResults), autocompleteTextOffset, width);
         
         commands.unlock();
         
         D.rect(rectColor, D.pixelToScreen(Rect(0, 0, width, 20)));
         
         if (topResult.is())
         {
            text.setPos(textOffset + Vec2(text.tds->textWidth(SkipEnd(input, topResult)), 0));
            text.set(topResult);
            text.draw(gui);
         }
         
         text.tds->color = ((invalidCommand) ? invalidTextColor : textColor);
         
         text.setPos(textOffset);
         text.set(input + ((blinkAnim > 0.5f) ? blinkChar : S));
         text.draw(gui);
         
         D.clip();
      }
      Console& Console::setCommand(Str8 command, Byte &member, Byte minValue , Byte maxValue )
      {
         if (!validCommand(command)) return T;
         
         Data *data   = commands(Key(command));
         data->member = &member;
         
         if (!minValue && !maxValue) maxValue = 1;
         data->minValue = ((minValue > maxValue) ? maxValue : minValue);
         data->maxValue = ((maxValue < minValue) ? minValue : maxValue);
         
         return T;
      }
      Console& Console::setCommand(Str8 command, Bool &member, Byte minValue , Byte maxValue ) { return setCommand(command, (Byte &)member, minValue, maxValue); }
      Console& Console::setCommand(Str8 command, I8   &member, Byte minValue , Byte maxValue ) { return setCommand(command, (Byte &)member, minValue, maxValue); }
      Console& Console::setCommand(Str8 command, I16  &member, Byte minValue , Byte maxValue ) { return setCommand(command, (Byte &)member, minValue, maxValue); }
      Console& Console::setCommand(Str8 command, I32  &member, Byte minValue , Byte maxValue ) { return setCommand(command, (Byte &)member, minValue, maxValue); }
      Console& Console::setCommand(Str8 command, I64  &member, Byte minValue , Byte maxValue ) { return setCommand(command, (Byte &)member, minValue, maxValue); }
      Console& Console::setCommand(Str8 command, U16  &member, Byte minValue , Byte maxValue ) { return setCommand(command, (Byte &)member, minValue, maxValue); }
      Console& Console::setCommand(Str8 command, U32  &member, Byte minValue , Byte maxValue ) { return setCommand(command, (Byte &)member, minValue, maxValue); }
      Console& Console::setCommand(Str8 command, U64  &member, Byte minValue , Byte maxValue ) { return setCommand(command, (Byte &)member, minValue, maxValue); }
      Console& Console::setCommand(Str8 command, Flt  &member, Byte minValue , Byte maxValue ) { return setCommand(command, (Byte &)member, minValue, maxValue); }
      Console& Console::setCommand(Str8 command, Real &member, Byte minValue , Byte maxValue ) { return setCommand(command, (Byte &)member, minValue, maxValue); }
      Console& Console::setCommand(Str8 command, void (*func)())
      {
         if (func == NULL || !validCommand(command)) return T;
         
         Data *data = commands(Key(command));
         data->func = func;
         
         return T;
      }
      Console& Console::setCommand(Str8 command, void (*funcByte)(Byte), Byte minValue , Byte maxValue )
      {
         if (funcByte == NULL || !validCommand(command)) return T;
         
         Data *data     = commands(Key(command));
         data->funcByte = funcByte;
         
         if (!minValue && !maxValue) maxValue = 1;
         data->minValue = ((minValue > maxValue) ? maxValue : minValue);
         data->maxValue = ((maxValue < minValue) ? minValue : maxValue);
         
         return T;
      }
      Console& Console::setFont(Font *font)
      {
         text.tds->font = font;
         return T;
      }
      Console& Console::setInputPrefix(Str8 inputPrefix)
      {
         T.inputPrefix =   inputPrefix;
         T.input       = T.inputPrefix;
         
         return T;
      }
      Console& Console::setValidKeys(CChar *validKeys)
      {
         if (Contains(validKeys, L'_') || Contains(validKeys, L' ')) return T;
         
         T.validKeys = validKeys;
         return T;
      }
   Console::Data::Data() : member(NULL), func(NULL), funcByte(NULL), minValue(0), maxValue(0) {}

} // namespace EEDevTools
/******************************************************************************/
