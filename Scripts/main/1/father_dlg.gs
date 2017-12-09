function main()
	trigger = getObj('Trigger', 1);
	triggerState(trigger, false);

	actionBreak(getObj('Player'));

	startDialogue('Dialogues/main/1/beginning.dlg');

	father = getObj('Chr', 1);
	talkingWith(father);
end