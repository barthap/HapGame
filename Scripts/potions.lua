function healthPotion(addHeal)
   heal = Get('Player::Health')
   newHeal = heal + addHeal  -- Add health
   Set('Player::Health', newHeal)
end

function manaPotion(addMana)
   local mana = Player.mana;--Get('Player::Mana')
   local newMana = mana + addMana;  -- Add mana
   Player.mana = newMana;
  --Set('Player::Mana', newMana)
end