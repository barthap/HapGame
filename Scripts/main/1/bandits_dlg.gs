function main()
	trigger = getObj('Trigger', 0);
	triggerState(trigger, false);

	actionBreak(getObj('Player'));

	startDialogue('Dialogues/main/1/bandits.dlg');

	bandit = getObj('Chr', 0);
	talkingWith(bandit);
end