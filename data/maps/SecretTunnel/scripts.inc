SecretTunnel_MapScripts::
	.byte 0

SecretTunnel_EventScript::
    cleartrainerflag TRAINER_LASS_KAY
	trainerbattle_single TRAINER_LASS_KAY, CeladonCity_Gym_Text_KayIntro, CeladonCity_Gym_Text_KayDefeat
	msgbox CeladonCity_Gym_Text_KayPostBattle, MSGBOX_AUTOCLOSE
	end
