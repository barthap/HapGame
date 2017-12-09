-- Name: Mod testowy
-- Copyrights(c) by Kacper

--------------------------------------------------------------
-- Start code - running when the aplication is initializing
--------------------------------------------------------------

playerPtr = nil;
characterPtr = nil;

moveToX = 2.5;
moveToY = 0;
moveToZ = 2.5;

--------------------------------------------------------------
-- Main Functions
--------------------------------------------------------------

function init()
	-- Taking pointer to player from game
	-- Player is only one so we don't need to set index
	playerPtr = getObj("Player");

	-- Creating character
	characterPtr = chrCreate("Obj/Chr/Goblin/0.obj", -20, 2, -1);
	-- Setting dialogue
	setDialoguePath(characterPtr, "Dialogues/goblin.dlg");
end

function update()
	-- Move
	if variableExist("goblin_dialogueEnd") == true then
		if getVariable("goblin_dialogueEnd") == true then
			moveWalking(characterPtr, true);
			moveTo(characterPtr, moveToX, moveToY, moveToZ);
			deleteVariable("goblin_dialogueEnd");
		end
	end

	if Equal(3, posX(characterPtr), posY(characterPtr), posZ(characterPtr), moveToX, moveToY, moveToZ) == true then
		moveWalking(characterPtr, false);
	end
end

function draw()

end

function shut()
	playerPtr = nil;
	characterPtr = nil;
end

--------------------------------------------------------------
-- Saving & Loading
--------------------------------------------------------------

function save()

end

function load()
	SLEEP(1500);
end