-- Name: Place Manager
-- Copyright (c) by Barthap

--------------------------------------------------------------
-- Start code - running when the aplication is initializing
--------------------------------------------------------------
playerPtr = nil;

x = nil;
y = nil;
z = nil;
--------------------------------------------------------------
-- Main Functions
--------------------------------------------------------------

function init()
	playerPtr = getObjPtr(ObjType.player);
      --playerPtr = toPointer(Player, ObjType.player); --2gi sposob
end

function update()

end

function draw()
	x = posX(playerPtr);
	y = posY(playerPtr);
	z = posZ(playerPtr);
	
	text = "Player pos - x: " .. x .. " y: " .. y .. " z: " .. z ;

	if KeyPressed("KB_9", true) then
		
		Player.mana = 80;
		Text(0, 0.8, text);
	end
end

function shut()
	playerPtr = nil;
	
	x = nil;
	y = nil;
	z = nil;
end

--------------------------------------------------------------
-- Saving & Loading
--------------------------------------------------------------

function save()

end

function load()

end