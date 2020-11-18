SecretTunnel_MapScripts::
	.byte 0

SecretTunnel_EventScript_Brock:: @ 81645EC
	trainerbattle_single TRAINER_LEADER_BROCK_2, SecretTunnel_Text_BrockIntro, SecretTunnel_Text_BrockDefeat
	msgbox SecretTunnel_Text_BrockPostBattle, MSGBOX_AUTOCLOSE
	end