hour = 0;

function main()
	hour = getHour();

	if activeFight(5) == true then
		return textFight();
	else
		return textNormal();
	end
end

function textNormal()
	if hour >= 9 and hour <= 20 then
		return textNormalDay();
	else
		return textNormalNight();
	end
end

function textNormalDay()
	rnd = 0;
	rnd = math.random(0, 3);

	if rnd == 0 then
		return "Sound/texts/piekny_dzien.ogg";
	elseif rnd == 1 then
		return "Sound/texts/slonce_swieci.ogg";
	elseif rnd == 2 then
		return "Sound/texts/pelno_roboty.ogg";
	elseif rnd == 3 then
		return "Sound/texts/piekny_dzien.ogg";
	end
end

function textNormalNight()
	rnd = 0;
	rnd = math.random(0, 3);

	if rnd == 0 then
		return "Sound/texts/czas_spac.ogg";
	elseif rnd == 1 then
		return "Sound/texts/zmeczony.ogg";
	elseif rnd == 2 then
		return "To by³ owocny dzieñ", "owoce";
	elseif rnd == 3 then
		return "Sound/texts/pusto_tu.ogg";
	end
end

function textFight()
	rnd = 0;
	rnd = math.random(0, 3);

	if rnd == 0 then
		return "Stra¿", "";
	elseif rnd == 1 then
		return "Przywal mu", "";
	elseif rnd == 2 then
		return "Sound/texts/walka.ogg";
	elseif rnd == 3 then
		return "Oby tak dalej", "";
	end
end