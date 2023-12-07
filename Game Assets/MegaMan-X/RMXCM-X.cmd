;===============================================================================
;-------------------------------------------------------------------------------
;http://megamugenbrasil.ucoz.com/             Fabricado no Brasil/Made in Brazil
;-------------------------------------------------------------------------------
;===============================================================================
;-| Button Remapping |----------------------------------------------------------
[Remap]
x = x
y = y
z = z
a = a
b = b
c = c
s = s

;-| Default Values |------------------------------------------------------------
[Defaults]
command.time = 30
command.buffer.time = 1

;===============================================================================
; Alex. D. X4                                 Fabricado no Brasil/Made in Brazil
;===============================================================================
;-| HYPERs Motions |------------------------------------------------------------
[Command]
name = "HyperDrive-236"
command = D, DF, F, x+y
time = 15

[Command]
name = "HyperDrive-236"
command = D, DF, F, x+b
time = 15

[Command]
name = "HyperDrive-236"
command = D, DF, F, y+b
time = 15

[Command]
name = "HyperDrive-214"
command = D, DB, B, x+y
time = 15

[Command]
name = "HyperDrive-214"
command = D, DB, B, x+b
time = 15

[Command]
name = "HyperDrive-214"
command = D, DB, B, y+b
time = 15

[Command]
name = "OverDrive"
command = z

;-| Double Input |----------------------------------------------------------------
[Command]
name = "Adv-Guard"
command = x+y

[Command]
name = "Adv-Guard"
command = y+b

[Command]
name = "Adv-Guard"
command = x+b

[Command]
name = "ReAct"
Command = y+b

[Command]
name = "Throw"
command = a+x

;-| Special Technique Motions |-------------------------------------------------
[Command]
name = "ST"
command = ~a

[Command]
name = "236-ST"
command = ~D, DF, F, a
time = 15

[Command]
name = "214-ST"
command = ~D, DB, B, a
time = 15

[Command]
name = "22-ST"
command = ~D, D, a
time = 15

[Command]
name = "ST-Blast"
command = /a

;-| SKILLs Motions |------------------------------------------------------------
[Command]
name = "236-Light ATK"
command = ~D, DF, F, x
time = 15

[Command]
name = "214-Light ATK"
command = ~D, DB, B, x
time = 15

[Command]
name = "22-Light ATK"
command = ~D, D, x
time = 15

[Command]
name = "236-Medium ATK"
command = ~D, DF, F, y
time = 15

[Command]
name = "214-Medium ATK"
command = ~D, DB, B, y
time = 15

[Command]
name = "22-Medium ATK"
command = ~D, D, y
time = 15

;-| SKILL Boost Motions |------------------------------------------------------------
[Command]
name = "236-SB"
command = ~D, DF, F, b
time = 15

[Command]
name = "214-SB"
command = ~D, DB, B, b
time = 15

[Command]
name = "22-SB"
command = ~D, D, b
time = 15

;-| Double Tap |----------------------------------------------------------------
[Command]
name = "FF"
command = F, F
time = 10

[Command]
name = "BB"
command = B, B
time = 10

[Command]
name = "Super Jump"
command = $D, $U

[Command]
name = "Super Jump"
command = /$D,c

[Command]
name = "ReAct"
Command = y+b

;-| Recovery Buttons |----------------------------------------------------
[Command]
name = "recovery"
command = /x

[Command]
name = "recovery"
command = /y

[Command]
name = "recovery"
command = /b

;-| Dir + Button |--------------------------------------------------------------
[Command]
name = "back_x"
command = /$B,x
time = 1

[Command]
name = "back_y"
command = /$B,y
time = 1

[Command]
name = "back_z"
command = /$B,z
time = 1

[Command]
name = "down_x"
command = /$D,x
time = 1

[Command]
name = "down_y"
command = /$D,y
time = 1

[Command]
name = "down_z"
command = /$D,z
time = 1

[Command]
name = "fwd_x"
command = /$F,x
time = 1

[Command]
name = "fwd_y"
command = /$F,y
time = 1

[Command]
name = "fwd_z"
command = /$F,z
time = 1

[Command]
name = "up_x"
command = /$U,x
time = 1

[Command]
name = "up_y"
command = /$U,y
time = 1

[Command]
name = "up_z"
command = /$U,z
time = 1

[Command]
name = "back_a"
command = /$B,a
time = 1

[Command]
name = "back_b"
command = /$B,b
time = 1

[Command]
name = "back_c"
command = /$B,c
time = 1

[Command]
name = "down_a"
command = /$D,a
time = 1

[Command]
name = "down_b"
command = /$D,b
time = 1

[Command]
name = "down_c"
command = /$D,c
time = 1

[Command]
name = "fwd_a"
command = /$F,a
time = 1

[Command]
name = "fwd_b"
command = /$F,b
time = 1

[Command]
name = "fwd_c"
command = /$F,c
time = 1

[Command]
name = "up_a"
command = /$U,a
time = 1

[Command]
name = "up_b"
command = /$U,b
time = 1

[Command]
name = "up_c"
command = /$U,c
time = 1

;-| Single Button |-------------------------------------------------------------
[Command]
name = "a"
command = a
time = 1

[Command]
name = "b"
command = b
time = 1

[Command]
name = "c"
command = c
time = 1

[Command]
name = "x"
command = x
time = 1

[Command]
name = "y"
command = y
time = 1

[Command]
name = "z"
command = z
time = 1

[Command]
name = "s"
command = s
time = 1

;-| Single Dir |----------------------------------------------------------------
[Command]
name = "fwd"
command = $F
time = 1

[Command]
name = "downfwd"
command = $DF
time = 1

[Command]
name = "down"
command = $D
time = 1

[Command]
name = "downback"
command = $DB
time = 1

[Command]
name = "back"
command = $B
time = 1

[Command]
name = "upback"
command = $UB
time = 1

[Command]
name = "up"
command = $U
time = 1

[Command]
name = "upfwd"
command = $UF
time = 1

;-| Hold Button |---------------------------------------------------------------
[Command]
name = "hold_x"
command = /x
time = 1

[Command]
name = "hold_y"
command = /y
time = 1

[Command]
name = "hold_z"
command = /z
time = 1

[Command]
name = "hold_a"
command = /a
time = 1

[Command]
name = "hold_b"
command = /b
time = 1

[Command]
name = "hold_c"
command = /c
time = 1

[Command]
name = "hold_s"
command = /s
time = 1

;-| Hold Dir |------------------------------------------------------------------
[Command]
name = "holdfwd"
command = /$F
time = 1

[Command]
name = "holddownfwd"
command = /$DF
time = 1

[Command]
name = "holddown"
command = /$D
time = 1

[Command]
name = "holddownback"
command = /$DB
time = 1

[Command]
name = "holdback"
command = /$B
time = 1

[Command]
name = "holdupback"
command = /$UB
time = 1

[Command]
name = "holdup"
command = /$U
time = 1

[Command]
name = "holdupfwd"
command = /$UF
time = 1

;===============================================================================
;-------------------------------------------------------------------------------
;http://megamugenbrasil.ucoz.com/             Fabricado no Brasil/Made in Brazil
;-------------------------------------------------------------------------------
;===============================================================================

[Statedef -1]

;===============================================================================
;-| X's AI System DX |----------------------------------------------------------
;===============================================================================
[State -1, X's AI System DX]
Type = VarSet
Triggerall = Var(59) < 1
Triggerall = RoundState = 2
Trigger1 = AILevel > 0
trigger2 = IsHomeTeam = 1
trigger2 = TeamSide = 2
trigger3 = IsHomeTeam = 1
trigger3 = MatchNo > 1
v = 59
value = 1
Ignorehitpause = 1

;===============================================================================
[State -1, IA - Combo 1-1 S-LA]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = RoundState = 2
triggerall = StateType != A
triggerall = P2StateType != L
triggerall = ((P2BodyDist X >= 0 && P2BodyDist X <= 20) && (P2BodyDist Y = 0))
triggerall = (P2BodyDist X >= -10 && P2BodyDist X <= 25) && P2BodyDist Y >= -10
TRIGGER1 = AILevel < 5
trigger1 = Ctrl = 1
trigger1 = Random < AILevel * 50
trigger1 = P2MoveType != A
TRIGGER2 = AILevel > 4
trigger2 = Ctrl = 1
trigger2 = Random < AILevel * 80
trigger2 = P2MoveType != A
TRIGGER3 = AILevel > 5
trigger3 = StateNo = 430
trigger3 = Anim = 430+var(58)
trigger3 = AnimElem = 4, >= 0
trigger3 = P2MoveType = H
TRIGGER4 = AILevel > 6
trigger4 = StateNo = 700
trigger4 = Anim = 700+var(58)
trigger4 = AnimElem = 12, >= 0
value = 200
ignorehitpause = 1

[State -1, IA - Combo 1-1 C-LA]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType != A
triggerall = P2StateType = S
triggerall = P2StateType != L
triggerall = (P2BodyDist X > -5 && P2BodyDist X < 15) && (P2BodyDist Y = 0)
triggerall = RoundState = 2
trigger1 = Ctrl = 1
trigger1 = RoundState = 2
trigger1 = Random < AILevel * 30
trigger1 = P2MoveType != A
TRIGGER2 = AILevel < 4
trigger2 = numtarget && moveguarded
trigger2 = StateNo = 200
trigger2 = Anim = 200+var(58)
trigger2 = AnimElem = 3, >= 0
TRIGGER3 = AILevel > 3
trigger3 = numtarget && (movehit || moveguarded)
trigger3 = StateNo = 200
trigger3 = Anim = 200+var(58)
trigger3 = AnimElem = 3, >= 0
value = 210
ignorehitpause = 1

[State -1, IA - Combo 1-2 S-MA]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = statetype != A
triggerall = P2StateType != L
triggerall = (P2BodyDist X > -5 && P2BodyDist X < 30) && (P2BodyDist Y = 0)
triggerall = RoundState = 2
trigger1 = numtarget && movehit
trigger1 = StateNo = 200
trigger1 = Anim = 200+var(58)
trigger1 = AnimElem = 3, >= 0
TRIGGER2 = AILevel > 3
trigger2 = numtarget && (movehit || moveguarded)
trigger2 = StateNo = 210
trigger2 = Anim = 210+var(58)
trigger2 = AnimElem = 3, >= 0
value = 201
ignorehitpause = 1

[State -1, IA - Combo 1-2 C-MA]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = statetype != A
triggerall = RoundState = 2
triggerall = ((P2BodyDist X > -5 && P2BodyDist X < 30) && (P2BodyDist Y >= -5))
trigger1 = P2StateType = L
trigger1 = Ctrl = 1
trigger1 = Random < AILevel * 30
trigger1 = P2MoveType != A
TRIGGER2 = AILevel < 4
trigger2 = numtarget && moveguarded
trigger2 = StateNo = 200
trigger2 = Anim = 200+var(58)
trigger2 = AnimElem = 3, >= 0
TRIGGER2 = AILevel > 3
trigger2 = numtarget && moveguarded
trigger2 = StateNo = 210
trigger2 = Anim = 210+var(58)
trigger2 = AnimElem = 4, >= 0
TRIGGER3 = AILevel > 3
trigger3 = numtarget && (movehit || moveguarded)
trigger3 = StateNo = 201
trigger3 = Anim = 201+var(58)
trigger3 = AnimElem = 3, >= 0
value = 211
ignorehitpause = 1

[State -1, IA - Combo 1-3 S-HA]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = statetype != A
triggerall = RoundState = 2
triggerall = ((P2BodyDist X > -10 && P2BodyDist X < 50) && (P2BodyDist Y >= -25))
TRIGGER1 = AILevel > 3 && AILevel < 6
trigger1 = numtarget && movehit
trigger1 = StateNo = 201
trigger1 = Anim = 201+var(58)
trigger1 = AnimElem = 3, >= 0
TRIGGER2 = AILevel > 5 && AILevel < 7
trigger2 = numtarget && moveguarded
trigger2 = StateNo = 211
trigger2 = Anim = 211+var(58)
trigger2 = AnimElem = 3, >= 0
TRIGGER3 = AILevel > 6
trigger3 = numtarget && (movehit || moveguarded)
trigger3 = StateNo = 211
trigger3 = Anim = 211+var(58)
trigger3 = AnimElem = 3, >= 0
TRIGGER4 = AILevel > 4
trigger4 = Ctrl = 1
trigger4 = numhelper(530) > 0
trigger4 = Random < AILevel * 60
trigger4 = P2BodyDist X > -5 && P2BodyDist X < 40
trigger4 = P2MoveType != A
value = 202
ignorehitpause = 1

[State -1, IA - Combo 1-3 C-HA]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = numtarget && movehit
triggerall = statetype != A
TRIGGER1 = AILevel > 5 && AILevel < 7
trigger1 = StateNo = 211
trigger1 = Anim = 211+var(58)
trigger1 = AnimElem = 3, >= 0
TRIGGER2 = AILevel > 3 && AILevel < 7
trigger2 = StateNo = 202
trigger2 = Anim = 202+var(58)
trigger2 = AnimElem = 5, >= 0
TRIGGER3 = AILevel > 6
trigger3 = numhelper(430) > 0
trigger3 = numhelper(530) > 0
trigger3 = StateNo = 202
trigger3 = Anim = 202+var(58)
trigger3 = AnimElem = 5, >= 0
value = 212
ignorehitpause = 1

[State -1, IA - Combo 1-4 S-Super Jump]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = numtarget && movehit
triggerall = p2Stateno = 5051
triggerall = StateNo = 212
trigger1 = Anim = 212+var(58)
trigger1 = AnimElem = 7, >= 0
value = 40
ignorehitpause = 1

;===============================================================================
[State -1, IA - Combo 2-1 J-LA]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType = A
triggerall = RoundState = 2
triggerall = ctrl = 1
trigger1 = (P2BodyDist X > -5 && P2BodyDist X <= 25) && (P2BodyDist Y <= 40 && P2BodyDist Y >= -40)
trigger1 = P2MoveType != A
trigger2 = StateNo = [45,50]
trigger2 = (P2BodyDist X > -5 && P2BodyDist X <= 25) && (P2BodyDist Y <= 0 && P2BodyDist Y >= -35)
trigger2 = P2MoveType = H
value = 220
ignorehitpause = 1

[State -1, IA - Combo 2-2 J-MA]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = numtarget && (movehit || moveguarded)
triggerall = statetype = A
triggerall = RoundState = 2
trigger1 = StateNo = 220
trigger1 = Anim = 220+var(58)
trigger1 = AnimElem = 3, >= 0
value = 221
ignorehitpause = 1

[State -1, IA - Combo 2-3 J-HA]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = statetype = A
triggerall = RoundState = 2
TRIGGER1 = AILevel < 6
trigger1 = numtarget && (movehit || moveguarded)
trigger1 = StateNo = 221
trigger1 = Anim = 221+var(58)
trigger1 = AnimElem = 4, >= 0
TRIGGER2 = AILevel > 5
trigger2 = numtarget && (movehit || moveguarded)
trigger2 = StateNo = 450
trigger2 = Anim = 450+var(58)
trigger2 = AnimElem = 7, >= 0
value = 222
ignorehitpause = 1

;===============================================================================
[State -1, IA - Basic Jump]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = RoundState = 2
triggerall = StateType != A
triggerall = TeamMode != Simul || (var(55) = 1 && NumPartner = 1)
TRIGGERALL = AILevel > 4
trigger1 = Ctrl = 1
trigger1 = Random < AILevel * 100
trigger1 = P2BodyDist Y = 0
trigger1 = ((P2Movetype = A) || (P2Movetype = A && enemy, NumHelper > 0) || P2StateType = L)
value = 40
ignorehitpause = 1

;===============================================================================
[State -1, IA - Dash FWD]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = Stateno != 100
TRIGGERALL = AILevel > 3
trigger1 = Ctrl = 1
trigger1 = RoundState = 2
trigger1 = Random < AILevel * 10
trigger1 = P2BodyDist X >= 30 && P2BodyDist X <= 1280
trigger1 = StateType != A
trigger1 = P2MoveType != A
value = 100

[State -1, IA - Air Dash FWD]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = Var(58) = 10000
triggerall = var(10) = 0
triggerall = Stateno != 102
TRIGGERALL = AILevel > 3
trigger1 = Ctrl = 1
trigger1 = RoundState = 2
trigger1 = Random < AILevel * 20
trigger1 = P2BodyDist X >= 30 && P2BodyDist X <= 1280
trigger1 = StateType = A
trigger1 = P2MoveType != A
value = 102

[State -1, IA - Back Step]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = Stateno != 105
TRIGGERALL = AILevel > 3
trigger1 = Ctrl = 1
trigger1 = RoundState = 2
trigger1 = Random < AILevel * 30
trigger1 = (P2BodyDist X >= 0 && P2BodyDist X <= 45) && P2BodyDist Y = 0
trigger1 = StateType = S
trigger1 = (P2Movetype = A) && ((enemy, NumProj > 0) || (enemy, NumHelper > 0))
trigger2 = (P2Movetype = A) && ((enemy, NumProj > 0) || (enemy, NumHelper > 0))
trigger2 = StateNo = 300
trigger2 = Anim = 300+var(58)
trigger2 = AnimElem = 6, >= 0
value = 105

;===============================================================================
[State -1, IA - S-Guard]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = Stateno != [100,119]
trigger1 = Ctrl = 1
trigger1 = StateType != A
trigger1 = P2statetype != C
trigger1 = (P2Movetype = A) && ((enemy, NumProj > 0) || (enemy, NumHelper > 0))
value = 130

[State -1, IA - S-to-C-Guard]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = Stateno != [100,119]
trigger1 = StateType != A
trigger1 = P2statetype = C
trigger1 = (P2Movetype = A) || ((enemy, NumProj > 0) || (enemy, NumHelper > 0))
trigger1 = stateno = 150
value = 152

[State -1, IA - C-Guard]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = Stateno != [100,119]
trigger1 = Ctrl = 1
trigger1 = StateType != A
trigger1 = P2statetype = C
trigger1 = (P2Movetype = A) || ((enemy, NumProj > 0) || (enemy, NumHelper > 0))
value = 131

[State -1, IA - C-to-S-Guard]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = Stateno != [100,119]
trigger1 = StateType != A
trigger1 = P2statetype != C
trigger1 = (P2Movetype = A) || ((enemy, NumProj > 0) || (enemy, NumHelper > 0))
trigger1 = stateno = 152
value = 150

;===============================================================================
[State -1, IA - Phantom Dash]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = RoundState = 2
triggerall = Stateno != [100,119]
triggerall = TeamMode != Simul || (var(55) = 1 && NumPartner = 1)
TRIGGERALL = AILevel > 4
trigger1 = Ctrl = 1
trigger1 = Random < AILevel * 40
trigger1 = (P2BodyDist X >= 90 && P2Movetype = A) || (enemy, NumProj > 0) || (enemy, Numhelper > 0)
trigger1 = StateType != A
value = 110
ignorehitpause = 1

[State -1, IA - Hop]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = RoundState = 2
triggerall = TeamMode != Simul || (var(55) = 1 && NumPartner = 1)
triggerall = P2StateType != A
TRIGGERALL = AILevel > 3
trigger1 = Ctrl = 1
trigger1 = Random < AILevel * 30
trigger1 = (P2BodyDist X <= 90 && P2Movetype = A) || (P2StateType = C)
trigger1 = StateType != A
TRIGGER2 = AILevel > 5
trigger2 = IfElse(Var(58) = 0, Power >= 1000, FVar(0) > 0)
trigger2 = numtarget && moveguarded
trigger2 = StateNo = 202
trigger2 = Anim = 202
trigger2 = AnimElem = 5, >= 0
TRIGGER3 = AILevel > 3
trigger3 = IfElse(Var(58) = 0, Power >= 1000, FVar(0) > 0)
trigger3 = numtarget && moveguarded
trigger3 = StateNo = 210
trigger3 = Anim = 210
trigger3 = AnimElem = 3, >= 0
TRIGGER4 = AILevel > 7
trigger4 = IfElse(Var(58) = 0, Power >= 1000, FVar(0) > 0)
trigger4 = numtarget && moveguarded
trigger4 = StateNo = 211
trigger4 = Anim = 211
trigger4 = AnimElem = 3, >= 0
value = 115
ignorehitpause = 1

;===============================================================================
[State -1, IA - Action Trigger - Charge Shot]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = Var(29) < 100
triggerall = StateType != A
trigger1 = Ctrl = 1
trigger1 = RoundState = 2
trigger1 = Random < AILevel * 40
trigger1 = P2BodyDist X >= 100
trigger1 = P2Movetype != A
value = 300
ignorehitpause = 1

;===============================================================================
[State -1, IA - S-ReAct-UpperCut-X]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType != A
triggerall = RoundState = 2
trigger1 = Ctrl = 1
trigger1 = Random < AILevel * 70
trigger1 = (P2BodyDist X >= 0 && P2BodyDist X <= 30) && P2BodyDist Y <= 0
trigger1 = P2Movetype = A
TRIGGER2 = AILevel > 3
trigger2 = numhelper(430) = 1 || numhelper(530) = 1
trigger2 = P2BodyDist X >= 0 && P2BodyDist X <= 30
trigger2 = StateNo = 202
trigger2 = Anim = 202
trigger2 = AnimElem = 5, >= 0
TRIGGER3 = AILevel > 6
trigger3 = Random < AILevel * 30
trigger3 = P2BodyDist X <= 30 && P2BodyDist Y >= -25
trigger3 = StateNo = 300
trigger3 = Anim = 300
value = 400
ignorehitpause = 1

;===============================================================================
[State -1, IA - S-X-Buster Charge Shot]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType != A
triggerall = P2StateType != C
triggerall = RoundState = 2
triggerall = Var(29) < 100
triggerall = var(30) < 3
trigger1 = Ctrl = 1
trigger1 = Random < AILevel * 40
trigger1 = P2BodyDist X >= 100 && P2BodyDist Y >= -50
trigger2 = numtarget && movehit
trigger2 = P2BodyDist Y < 0
trigger2 = StateNo = 202
trigger2 = Anim = 202+var(58)
trigger2 = AnimElem = 5, >= 0
TRIGGER3 = AILevel > 6
trigger3 = P2MoveType = H
trigger3 = StateNo = 560
trigger3 = Anim = 461+var(58)
trigger3 = AnimElem = 7, >= 0
TRIGGER4 = AILevel > 5
trigger4 = numtarget && movehit
trigger4 = StateNo = 461
trigger4 = Anim = 461+var(58)
trigger4 = AnimElem = 7, >= 0
value = 410
ignorehitpause = 1

[State -1, IA - S-X-Buster LV1]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = var(30) < 3
triggerall = StateType != A
trigger1 = P2Movetype = A || P2MoveType = H
trigger1 = RoundState = 2
trigger1 = P2BodyDist X >= 30
trigger1 = StateNo = 411
trigger1 = Anim = 411+var(58)
trigger1 = AnimElem = 3, >= 0
TRIGGER2 = AILevel > 4
trigger2 = numtarget && movehit
trigger2 = StateNo = 540
trigger2 = Anim = 540+var(58)
trigger2 = AnimElem = 5, >= 0
value = 411
ignorehitpause = 1

[State -1, IA - S-X-Buster LV3]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType != A
triggerall = Var(29) >= 100
triggerall = P2BodyDist Y >= -20
trigger1 = Ctrl = 1
trigger1 = RoundState = 2
trigger1 = Random < AILevel * 30
trigger1 = P2BodyDist X >= 100
trigger2 = numtarget && movehit
trigger2 = StateNo = 202
trigger2 = Anim = 202+var(58)
trigger2 = AnimElem = 7, >= 0
TRIGGER3 = AILevel > 5
trigger3 = numtarget && movehit
trigger3 = StateNo = 461
trigger3 = Anim = 461+var(58)
trigger3 = AnimElem = 7, >= 0
value = 510
ignorehitpause = 1

[State -1, IA - S-X-Buster Follow-up - Dash]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = p2MoveType = H
triggerall = statetype != A
trigger1 = StateNo = 412
trigger1 = Anim = 412+var(58)
trigger1 = AnimElem = 5, >= 0
trigger2 = StateNo = 510
trigger2 = Anim = 413+var(58)
trigger2 = AnimElem = 7, >= 0
value = 413
ignorehitpause = 1

[State -1, IA - S-X-Buster Follow-up - Super Jump]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = p2Stateno = [6000,6003]
triggerall = statetype != A
triggerall = StateNo = 413
trigger1 = Anim = 100+var(58)
trigger1 = AnimElem = 3, >= 0
value = 40
ignorehitpause = 1

;===============================================================================
[State -1, IA - A-X-Buster Charge Shot]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType = A
triggerall = Var(29) < 100
triggerall = var(30) < 3
trigger1 = Ctrl = 1
trigger1 = RoundState = 2
trigger1 = Random < AILevel * 40
trigger1 = P2BodyDist X >= 100
TRIGGER2 = AILevel < 5
trigger2 = numtarget && movehit
trigger2 = StateNo = 222
trigger2 = Anim = 222+var(58)
trigger2 = AnimElem = 6, >= 0
TRIGGER3 = AILevel > 6
trigger3 = P2MoveType = H
trigger3 = StateNo = 570
trigger3 = Anim = 470+var(58)
trigger3 = AnimElem = 7, >= 0
TRIGGER4 = AILevel > 5
trigger4 = numtarget && movehit
trigger4 = StateNo = 462
trigger4 = Anim = 462+var(58)
trigger4 = AnimElem = 6, >= 0
value = 420
ignorehitpause = 1

[State -1, IA - A-X-Buster LV1]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = var(30) < 3
triggerall = StateType = A
trigger1 = P2Movetype = A || P2MoveType = H
trigger1 = RoundState = 2
trigger1 = StateNo = 421
trigger1 = Anim = 421+var(58)
trigger1 = AnimElem = 3, >= 0
value = 421
ignorehitpause = 1

[State -1, IA - A-X-Buster LV3]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType = A
triggerall = Var(29) >= 100
trigger1 = Ctrl = 1
trigger1 = RoundState = 2
trigger1 = Random < AILevel * 30
trigger1 = P2BodyDist X >= 100
trigger2 = numtarget && movehit
trigger2 = StateNo = 222
trigger2 = Anim = 222+var(58)
trigger2 = AnimElem = 5, >= 0
TRIGGER3 = AILevel > 5
trigger3 = numtarget && movehit
trigger3 = StateNo = 462
trigger3 = Anim = 462+var(58)
trigger3 = AnimElem = 6, >= 0
value = 520
ignorehitpause = 1

;===============================================================================
[State -1, IA - S-SB-Lighting Web]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType = S
triggerall = P2StateType != L
triggerall = Var(29) >= 100 || Power >= 1000
triggerall = numhelper(430) < 1
triggerall = numhelper(530) < 1
triggerall = RoundState = 2
triggerall = P2BodyDist Y >= -30
trigger1 = Ctrl = 1
trigger1 = Random < AILevel * 10
trigger1 = (P2BodyDist X >= 90 && P2BodyDist X <= 200) && P2BodyDist Y >= 0
TRIGGER2 = AILevel > 7
trigger2 = P2BodyDist X >= 20 && P2BodyDist X <= 70
trigger2 = StateNo = 202
trigger2 = Anim = 202+var(58)
trigger2 = AnimElem = 5, >= 0
;TRIGGER3 = AILevel > 5
;trigger3 = Random < AILevel * 50
;trigger3 = var(30) > 2
;trigger3 = StateType != A
;trigger3 = P2MoveType = H
;trigger3 = P2BodyDist X >= 0
;trigger3 = StateNo = 411
;trigger3 = Anim = 411+var(58)
;trigger3 = AnimElem = 3, >= 0
value = 530
ignorehitpause = 1

[State -1, IA - S-Lighting Web]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType != A
triggerall = P2StateType = S
triggerall = RoundState = 2
triggerall = Var(29) < 100
triggerall = numhelper(430) < 1
triggerall = numhelper(530) < 1
trigger1 = Ctrl = 1
trigger1 = P2Movetype = A
trigger1 = Random < AILevel * 20
trigger1 = P2BodyDist X >= 50 && P2BodyDist Y >= 0
TRIGGER2 = AILevel > 5
trigger2 = numtarget && movehit || moveguarded
trigger2 = P2BodyDist X >= 0 && P2BodyDist X <= 70
trigger2 = P2BodyDist Y >= -5
trigger2 = StateNo = 202
trigger2 = Anim = 202+var(58)
trigger2 = AnimElem = 6, >= 0
value = 430
ignorehitpause = 1

;===============================================================================
[State -1, IA - S-SB-Gel Shaver]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType != A
triggerall = Var(29) >= 100 || Power >= 1000
triggerall = P2BodyDist Y >= 0
triggerall = RoundState = 2
trigger1 = Ctrl = 1
trigger1 = Random < AILevel * 25
trigger1 = P2statetype = S && P2MoveType = A
trigger1 = (P2BodyDist X >= 30 && P2BodyDist X <= 250) && P2BodyDist Y >= 0
TRIGGER2 = AILevel > 7
trigger2 = P2BodyDist X >= -10 && P2BodyDist X <= 70
trigger2 = P2statetype = S && (numtarget && moveguarded)
trigger2 = StateNo = 202
trigger2 = Anim = 202+var(58)
trigger2 = AnimElem = 4, >= 0
TRIGGER3 = AILevel > 7
trigger3 = Random < AILevel * 80
trigger3 = var(30) > 2
trigger3 = StateType != A
trigger3 = P2MoveType = H
trigger3 = P2BodyDist X <= 100
trigger3 = StateNo = 411
trigger3 = Anim = 411+var(58)
trigger3 = AnimElem = 3, >= 0
value = 540
ignorehitpause = 1

[State -1, IA - S-Gel Shaver]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType != A
triggerall = Var(29) < 100
triggerall = P2BodyDist Y >= 0
triggerall = RoundState = 2
trigger1 = Ctrl = 1
trigger1 = Random < AILevel * 20
trigger1 = P2statetype = S && P2MoveType != A
trigger1 = (P2BodyDist X >= 0 && P2BodyDist X <= 60)
TRIGGER2 = AILevel > 5
trigger2 = P2BodyDist X >= 0 && P2BodyDist X <= 50
trigger2 = P2statetype = S && (numtarget && moveguarded)
trigger2 = StateNo = 202
trigger2 = Anim = 202+var(58)
trigger2 = AnimElem = 5, >= 0
value = 440
ignorehitpause = 1

;-------------------------------------------------------------------------------
[State -1, IA - S-SB-Meteor Rain]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType != A
triggerall = RoundState = 2
triggerall = Var(29) >= 100
triggerall = numhelper(480) < 1
triggerall = numhelper(580) < 1
trigger1 = Ctrl = 1
trigger1 = Random < AILevel * 30
trigger1 = P2statetype = A
trigger1 = (P2BodyDist X >= -10 && P2BodyDist X <= 320) && P2BodyDist Y < 0
TRIGGER2 = AILevel > 5
trigger2 = P2BodyDist X >= 0 && P2BodyDist X <= 50
trigger2 = P2statetype = C && (numtarget && moveguarded)
trigger2 = StateNo = 202
trigger2 = Anim = 202+var(58)
trigger2 = AnimElem = 4, >= 0
value = 580
ignorehitpause = 1

[State -1, IA - S-L-Meteor Rain]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType != A
triggerall = RoundState = 2
triggerall = Var(29) < 100
triggerall = numhelper(480) < 1
triggerall = numhelper(580) < 1
triggerall = (P2BodyDist X >= -10 && P2BodyDist X <= 80) && P2BodyDist Y < 0
trigger1 = Ctrl = 1
trigger1 = Random < AILevel * 30
value = 480
ignorehitpause = 1

;-------------------------------------------------------------------------------
[State -1, IA - S-SB-Magma Blade]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType = S
triggerall = P2StateType != L
triggerall = Var(29) >= 100 || Power >= 1000
TRIGGERALL = AILevel > 4
trigger1 = numtarget && movehit
trigger1 = StateNo = 202
trigger1 = Anim = 202+var(58)
trigger1 = AnimElem = 5, >= 0
value = 560
ignorehitpause = 1

[State -1, IA - S-Light-Magma Blade]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType != A
triggerall = Var(29) < 100
triggerall = RoundState = 2
trigger1 = Ctrl = 1
trigger1 = P2StateType = L
trigger1 = Random < AILevel * 30
trigger1 = P2BodyDist X <= 60 && P2BodyDist Y >= 0
TRIGGER2 = AILevel > 5
trigger2 = numhelper(430) < 1
trigger2 = numhelper(530) < 1
trigger2 = numtarget && moveguarded
trigger2 = P2StateType != C
trigger2 = StateNo = 202
trigger2 = Anim = 202+var(58)
trigger2 = AnimElem = 5, >= 0
TRIGGER3 = AILevel > 6
trigger3 = numtarget && moveguarded
trigger3 = StateNo = 211
trigger3 = Anim = 211+var(58)
trigger3 = AnimElem = 4, >= 0
TRIGGER4 = AILevel > 7
trigger4 = numhelper(430) > 0
trigger4 = numhelper(530) > 0
trigger4 = numtarget && movehit
trigger4 = P2StateType != L
trigger4 = StateNo = 202
trigger4 = Anim = 202+var(58)
trigger4 = AnimElem = 5, >= 0
trigger4 = P2BodyDist X <= 70 && P2BodyDist Y >= 0
value = 460
ignorehitpause = 1

[State -1, IA - S-Medium-Magma Blade]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType != A
triggerall = Var(29) < 100
triggerall = RoundState = 2
trigger1 = Ctrl = 1
trigger1 = P2StateType = C
trigger1 = Random < AILevel * 40
trigger1 = P2BodyDist X <= 70 && P2BodyDist Y >= 0
value = 465
ignorehitpause = 1

[State -1, IA - A-SB-Magma Blade]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType = A
triggerall = RoundState = 2
triggerall = P2StateType != L
triggerall = Var(29) >= 100 || Power >= 1000
trigger1 = Ctrl = 1
trigger1 = Random < AILevel * 10
trigger1 = P2BodyDist X >= 0 && P2BodyDist X <= 60
trigger1 = P2BodyDist Y <= 40 && P2BodyDist Y >= -40
TRIGGER2 = AILevel > 7
trigger2 = P2MoveType = H
trigger2 = StateNo = 222
trigger2 = Anim = 222+var(58)
trigger2 = AnimElem = 7, >= 0
value = 570
ignorehitpause = 1

[State -1, IA - A-Magma Blade]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType = A
triggerall = P2StateType != L
triggerall = RoundState = 2
triggerall = Var(29) < 100
trigger1 = Ctrl = 1
trigger1 = Random < AILevel * 20
trigger1 = P2BodyDist X >= 0 && P2BodyDist X <= 45
trigger1 = P2BodyDist Y <= 35 && P2BodyDist Y >= -35
TRIGGER2 = AILevel > 5
trigger2 = numtarget && (movehit || moveguarded)
trigger2 = StateNo = 222
trigger2 = Anim = 222+var(58)
trigger2 = AnimElem = 7, >= 0
value = 470
ignorehitpause = 1

;-------------------------------------------------------------------------------
[State -1, IA - SB-A-Soul Body]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType = A
triggerall = Var(29) < 100
triggerall = RoundState = 2
triggerall = Var(29) >= 100 || Power >= 1000
TRIGGER1 = AILevel > 5
trigger1 = numtarget && movehit
trigger1 = StateNo = 222
trigger1 = Anim = 222+var(58)
trigger1 = AnimElem = 7, >= 0
value = 550
ignorehitpause = 1

[State -1, IA - A-Soul Body]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType = A
triggerall = Var(29) < 100
triggerall = RoundState = 2
TRIGGER1 = AILevel > 5
trigger1 = numtarget && movehit || moveguarded
trigger1 = StateNo = 221
trigger1 = Anim = 221+var(58)
trigger1 = AnimElem = 4, >= 0
value = 450
ignorehitpause = 1

;===============================================================================
[State -1, IA - S - Hyper 1]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType != A
triggerall = Power >= 2000
triggerall = numhelper(1600) = 0
trigger1 = Ctrl = 1
trigger1 = RoundState = 2
trigger1 = Random < AILevel * 20
trigger1 = P2BodyDist X >= 100
TRIGGER2 = AILevel > 3 && AILevel < 6
trigger2 = numtarget && movehit
trigger2 = StateNo = 202
trigger2 = Anim = 202+var(58)
trigger2 = AnimElem = 4, >= 0
TRIGGER3 = AILevel > 5
trigger3 = var(30) > 2
trigger3 = StateNo = 411
trigger3 = Anim = 411+var(58)
trigger3 = AnimElem = 3, >= 0
TRIGGER4 = AILevel > 7
trigger4 = StateNo = 510
trigger4 = Anim = 413+var(58)
trigger4 = AnimElem = 7, >= 0
value = 600
ignorehitpause = 1


;===============================================================================
[State -1, IA - Active OverClock]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType != A
triggerall = Var(58) = 0
triggerall = FVar(0) >= 270
triggerall = Life <= 500
trigger1 = Ctrl = 1
trigger1 = RoundState = 2
trigger1 = P2Movetype = A
trigger1 = Random < AILevel * 200
trigger1 = P2BodyDist X <= 50
TRIGGER2 = AILevel > 6
trigger2 = StateNo = 202
trigger2 = Anim = 202+var(58)
trigger2 = AnimElem = 3, >= 0
value = 700
ignorehitpause = 1

;-------------------------------------------------------------------------------
[State -1, IA - OverDrive]
type = ChangeState
triggerall = var(59)
triggerall = !Win
triggerall = StateType != A
triggerall = Var(58) = 10000
triggerall = FVar(0) <= 45 || Life <= 300
trigger1 = Ctrl = 1
trigger1 = RoundState = 2
trigger1 = P2Movetype = A
trigger1 = Random < AILevel * 200
trigger1 = P2BodyDist X <= 50
TRIGGER2 = AILevel > 7
trigger2 = StateNo = 202
trigger2 = Anim = 202+var(58)
trigger2 = AnimElem = 3, >= 0
value = 620
ignorehitpause = 1

;===============================================================================
; Alex. D. X4                                 Fabricado no Brasil/Made in Brazil
;===============================================================================
;===============================================================================
;-| X's Hypers Moves |----------------------------------------------------------
;===============================================================================
[State -1, S - Hyper 1]
type = ChangeState
value = 600
triggerall = !var(59)
triggerall = Power >= 2000
triggerall = command = "HyperDrive-236"
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact
trigger4 = Stateno = 400
trigger4 = movecontact
trigger5 = stateno = 411 || stateno = 412 || stateno = 510
trigger5 = time > 8
trigger6 = stateno = 430 || stateno = 530
trigger6 = time > 12
trigger7 = stateno = 440 || Stateno = 540
trigger7 = movecontact
trigger8 = stateno = 461 || stateno = 560
trigger8 = time > 15
trigger9 = stateno = 480 || stateno = 580
trigger9 = time > 21

[State -1, J - Hyper 1]
type = ChangeState
value = 605
triggerall = !var(59)
triggerall = Var(58) = 0
triggerall = Power >= 2000
triggerall = command = "HyperDrive-236"
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = stateno = 220 || stateno = 221 || stateno = 222
trigger2 = movecontact
trigger3 = stateno = 421 || stateno = 422 || stateno = 520
trigger3 = time > 8
trigger4 = stateno = 570
trigger4 = time > 16
trigger5 = stateno = 450
trigger5 = movecontact
trigger6 = stateno = 550
trigger6 = time > 20

;-------------------------------------------------------------------------------
;[State -1, S - Hyper 2]
;type = ChangeState
;value = 610
;triggerall = !var(59)
;triggerall = Var(58)= 0
;triggerall = Power >= 2000
;triggerall = command = "HyperDrive-214"
;trigger1 = statetype != A
;trigger1 = ctrl
;trigger2 = stateno = 200 || stateno = 201 || stateno = 202
;trigger2 = movecontact
;trigger3 = stateno = 210 || stateno = 211
;trigger3 = movecontact
;trigger4 = stateno = 411 || stateno = 412
;trigger4 = time > 8
;
;===============================================================================
[State -1, S - Active OverClock]
type = ChangeState
value = 700
triggerall = !var(59)
triggerall = command = "OverDrive"
triggerall = Var(58)= 0
triggerall = FVar(0) >= 270
trigger1 = statetype = S
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact

;===============================================================================
[State -1, S - OverDrive]
type = ChangeState
value = 620
triggerall = !var(59)
triggerall = Var(58) = 10000
triggerall = FVar(0) >= 1
triggerall = command = "OverDrive"
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact
trigger4 = Stateno = 400
trigger4 = movecontact
trigger5 = stateno = 411 || stateno = 412 || stateno = 510
trigger5 = time > 8
trigger6 = stateno = 430 || stateno = 530
trigger6 = time > 15
trigger7 = stateno = 440 || Stateno = 540
trigger7 = movecontact
trigger8 = stateno = 461 || stateno = 560
trigger8 = time > 15
trigger9 = stateno = 480 || stateno = 580
trigger9 = time > 21

;===============================================================================
;-| Universal Double-Inputs  |--------------------------------------------------
;===============================================================================
[State -1, S - Advancing Guard]
type = ChangeState
value = 160
triggerall = TeamMode != Simul || (var(55) = 1 && NumPartner = 1)
triggerall = IfElse(Var(58) = 0, Power >= 1000, FVar(0) > 0)
triggerall = command = "Adv-Guard" && statetype = S
trigger1 = stateno = [150,153]

[State -1, C - Advancing Guard]
type = ChangeState
value = 161
triggerall = TeamMode != Simul || (var(55) = 1 && NumPartner = 1)
triggerall = IfElse(Var(58) = 0, Power >= 1000, FVar(0) > 0)
triggerall = command = "Adv-Guard" && statetype = C
trigger1 = stateno = [150,153]

[State -1, A - Advancing Guard]
type = ChangeState
value = 162
triggerall = TeamMode != Simul || (var(55) = 1 && NumPartner = 1)
triggerall = IfElse(Var(58) = 0, Power >= 1000, FVar(0) > 0)
triggerall = command = "Adv-Guard" && statetype = A
trigger1 = stateno = [154,155]

;-------------------------------------------------------------------------------
[State -1, S - ReAct - UpperCut-X]
type = ChangeState
value = 400
triggerall = !var(59)
triggerall = numhelper(400) < 1
triggerall = command = "ReAct"
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact
trigger4 = stateno = 212
trigger4 = movecontact && time <= 15
trigger5 = stateno = 461
trigger5 = movecontact
trigger6 = stateno = 530
trigger6 = time > 15
trigger7 = Stateno = 540
trigger7 = movecontact
trigger8 = stateno = 560
trigger8 = time > 20

;-------------------------------------------------------------------------------
[State -1, S - Throw]
type = ChangeState
value = 800
triggerall = !var(59)
triggerall = TeamMode != Simul || (var(55) = 1 && NumPartner = 1)
triggerall = Stateno != 110
triggerall = command = "Throw"
trigger1 = statetype != A
trigger1 = ctrl

;===============================================================================
;-| X's Variable Weapon System |------------------------------------------------
;===============================================================================
[State -1, S - SB - X-Buster MAX]
type = ChangeState
value = 510
triggerall = !var(59)
triggerall = Var(29) >= 100 && command = "236-Light ATK"
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact
trigger4 = stateno = 440 || stateno = 540
trigger4 = movecontact
trigger5 = stateno = 461
trigger5 = movecontact
trigger6 = stateno = 530
trigger6 = time > 17
trigger7 = stateno = 560
trigger7 = time > 20

[State -1, S - Light - X-Buster Lv1]
type = ChangeState
value = 410
triggerall = !var(59)
triggerall = Var(29) < 100 && command = "236-Light ATK"
triggerall = var(30) < 3
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact
trigger4 = stateno = 430
trigger4 = time > 17 && enemynear, MoveType = H
trigger5 = stateno = 440 || stateno = 540
trigger5 = movehit && time > 18
trigger6 = stateno = 540
trigger6 = movehit && time > 25
trigger7 = stateno = 461
trigger7 = movecontact && time > 14
trigger8 = stateno = 480
trigger8 = time > 18 && enemynear, MoveType = H
trigger9 = stateno = 530
trigger9 = time > 17
trigger10 = stateno = 560
trigger10 = time > 20

[State -1, S - X-Buster Follow-up - Dash]
type = ChangeState
value = 413
triggerall = !var(59)
triggerall = command = "x"
triggerall = statetype = S
trigger1 = stateno = 412 && time > 13

;-------------------------------------------------------------------------------
[State -1, S - SB - Lighting Web]
type = ChangeState
value = 530
triggerall = !var(59)
triggerall = numhelper(430) < 1
triggerall = numhelper(530) < 1
triggerall = (Var(58) = 0 && ((command = "236-SB" && Power >= 1000) || (Var(29) >= 100 && command = "236-Medium ATK"))) || (Var(58) = 10000 && command = "236-SB")
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact

;-------------------------------------------------------------------------------
[State -1, S - Medium - Lighting Web]
type = ChangeState
value = 430
triggerall = !var(59)
triggerall = numhelper(430) < 1
triggerall = numhelper(530) < 1
triggerall = (Var(58) = 0 && (Var(29) < 100 && command = "236-Medium ATK")) || (Var(58) = 10000 && command = "236-Medium ATK")
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact

;-------------------------------------------------------------------------------
[State -1, S - SB - Meteor Rain]
type = ChangeState
value = 580
triggerall = !var(59)
triggerall = Var(29) >= 100 && command = "22-Light ATK"
triggerall = numhelper(480) < 1
triggerall = numhelper(580) < 1
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact
trigger4 = stateno = 440 || stateno = 540
trigger4 = movecontact

[State -1, S - Light - Meteor Rain]
type = ChangeState
value = 480
triggerall = !var(59)
triggerall = Var(29) < 100 && command = "22-Light ATK"
triggerall = numhelper(480) < 1
triggerall = numhelper(580) < 1
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact

;-------------------------------------------------------------------------------
[State -1, S - SB - Gel Shaver]
type = ChangeState
value = 540
triggerall = !var(59)
triggerall = (Var(58) = 0 && ((command = "22-SB" && Power >= 1000) || (Var(29) >= 100 && command = "22-Medium ATK"))) || (Var(58) = 10000 && command = "22-SB")
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact
trigger4 = stateno = 212
trigger4 = movecontact && time <= 15
trigger5 = stateno = 461
trigger5 = movecontact && time > 14

[State -1, S - Light - Gel Shaver]
type = ChangeState
value = 440
triggerall = !var(59)
triggerall = Var(29) < 100 && command = "22-Medium ATK"
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact

;-------------------------------------------------------------------------------
[State -1, S - SB - Magma Blade]
type = ChangeState
value = 560
triggerall = !var(59)
triggerall = (Var(58) = 0 && ((command = "214-SB" && Power >= 1000) || (Var(29) >= 100 && command = "214-Medium ATK"))) || (Var(58) = 10000 && command = "214-SB")
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact
trigger4 = stateno = 440 || stateno = 540
trigger4 = movecontact

[State -1, S - Light - Magma Blade]
type = ChangeState
value = 460
triggerall = !var(59)
triggerall = (Var(58) = 0 && (Var(29) < 100 && command = "214-Light ATK")) || (Var(58) = 10000 && command = "214-Light ATK")
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact

[State -1, S - Medium - Magma Blade]
type = ChangeState
value = 465
triggerall = !var(59)
triggerall = (Var(58) = 0 && (Var(29) < 100 && command = "214-Medium ATK")) || (Var(58) = 10000 && command = "214-Medium ATK")
trigger1 = statetype != A
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact

;===============================================================================
[State -1, S - SB - X-Buster MAX]
type = ChangeState
value = 520
triggerall = !var(59)
triggerall = Var(29) >= 100 && command = "236-Light ATK"
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = stateno = 220 || stateno = 221 || stateno = 222
trigger2 = movecontact
trigger3 = stateno = 570
trigger3 = time > 16
trigger4 = stateno = 450
trigger4 = movecontact
trigger5 = stateno = 462
trigger5 = movecontact
trigger6 = stateno = 550
trigger6 = time > 20

[State -1, J - Light - X-Buster Lv1]
type = ChangeState
value = 420
triggerall = !var(59)
triggerall = Var(29) < 100 && command = "236-Light ATK"
triggerall = var(30) < 3
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = stateno = 220 || stateno = 221 || stateno = 222
trigger2 = movecontact
trigger3 = stateno = 570
trigger3 = time > 16
trigger4 = stateno = 450
trigger4 = movecontact
trigger5 = stateno = 462
trigger5 = movecontact
trigger6 = stateno = 550
trigger6 = time > 20

;-------------------------------------------------------------------------------
[State -1, J - SB - Magma Blade]
type = ChangeState
value = 570
triggerall = !var(59)
triggerall = (Var(58) = 0 && ((command = "214-SB" && Power >= 1000) || (Var(29) >= 100 && command = "214-Medium ATK"))) || (Var(58) = 10000 && command = "214-SB")
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = stateno = 220 || stateno = 221 || stateno = 222
trigger2 = movecontact
trigger3 = stateno = 450
trigger3 = movecontact
trigger4 = stateno = 550
trigger4 = time > 20

[State -1, J - Medium - Magma Blade]
type = ChangeState
value = 470
triggerall = !var(59)
triggerall = (Var(58) = 0 && (Var(29) < 100 && command = "214-Medium ATK")) || (Var(58) = 10000 && command = "214-Medium ATK")
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = stateno = 220 || stateno = 221 || stateno = 222
trigger2 = movecontact
trigger3 = stateno = 450
trigger3 = movecontact
trigger4 = stateno = 550
trigger4 = time > 20

;-------------------------------------------------------------------------------
[State -1, J - SB - Soul Body]
type = ChangeState
value = 550
triggerall = !var(59)
triggerall = numhelper(550) < 1
triggerall = (Var(58) = 0 && ((command = "236-SB" && Power >= 1000) || (Var(29) >= 100 && command = "236-Medium ATK"))) || (Var(58) = 10000 && command = "236-SB")
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = stateno = 220 || stateno = 221 || stateno = 222
trigger2 = movecontact
trigger3 = stateno = 450
trigger3 = movecontact

[State -1, J - Medium - Soul Body]
type = ChangeState
value = 450
triggerall = !var(59)
triggerall = numhelper(550) < 1
triggerall = (Var(58) = 0 && (Var(29) < 100 && command = "236-Medium ATK")) || (Var(58) = 10000 && command = "236-Medium ATK")
trigger1 = statetype = A
trigger1 = ctrl
trigger2 = stateno = 220 || stateno = 221 || stateno = 222
trigger2 = movecontact

;===============================================================================
;-| X's Fighting Techniques |---------------------------------------------------
;===============================================================================
[State -1, S - Light ATK]
type = ChangeState
value = 200
triggerall = !var(59)
triggerall = command = "x"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl = 1
trigger2 = stateno = 200
trigger2 = time >= 12

;-------------------------------------------------------------------------------
[State -1, S - Medium ATK]
type = ChangeState
value = 201
triggerall = !var(59)
triggerall = command = "y"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl = 1
trigger2 = stateno = 200 || stateno = 210
trigger2 = movecontact

;-------------------------------------------------------------------------------
[State -1, S - Heavy ATK]
type = ChangeState
value = 202
triggerall = !var(59)
triggerall = command = "b"
triggerall = command != "holddown"
trigger1 = statetype = S
trigger1 = ctrl = 1
trigger2 = stateno = 200 || stateno = 210
trigger2 = movecontact
trigger3 = stateno = 201 || stateno = 211
trigger3 = movecontact

;===============================================================================
[State -1, S - Action Trigger - Charge Shot]
type = ChangeState
value = 300
triggerall = !var(59)
triggerall = command = "a"
triggerall = var(30) < 3
trigger1 = statetype != A
trigger1 = ctrl = 1
trigger2 = stateno = 200 || stateno = 201 || stateno = 202
trigger2 = movecontact
trigger3 = stateno = 210 || stateno = 211
trigger3 = movecontact

;===============================================================================
[State -1, C - Light ATK]
type = ChangeState
value = 210
triggerall = !var(59)
triggerall = command = "holddown" && command = "x"
trigger1 = statetype = C
trigger1 = ctrl = 1
trigger2 = stateno = 200
trigger2 = movecontact

;-------------------------------------------------------------------------------
[State -1, C - Medium ATK]
type = ChangeState
value = 211
triggerall = !var(59)
triggerall = command = "holddown" && command = "y"
trigger1 = statetype = C
trigger1 = ctrl
trigger2 = stateno = 200 || stateno = 210
trigger2 = movecontact
trigger3 = stateno = 201
trigger3 = movecontact

;-------------------------------------------------------------------------------
[State -1, C - Heavy ATK]
type = ChangeState
value = 212
triggerall = !var(59)
triggerall = command = "holddown" && command = "b"
trigger1 = statetype = C
trigger1 = ctrl = 1
trigger2 = stateno = 200 || stateno = 210
trigger2 = movecontact
trigger3 = stateno = 201 || stateno = 211
trigger3 = movecontact
trigger4 = stateno = 202
trigger4 = movecontact

;===============================================================================
[State -1, J - Light ATK-1]
type = ChangeState
value = 220
triggerall = !var(59)
triggerall = command = "x"
trigger1 = statetype = A
trigger1 = ctrl = 1
trigger2 = stateno = 220
trigger2 = time >= 12
trigger3 = stateno = 115
trigger3 = Var(11) < 2

;-------------------------------------------------------------------------------
[State -1, J - Medium ATK-1]
type = ChangeState
value = 221
triggerall = !var(59)
triggerall = command = "y"
trigger1 = statetype = A
trigger1 = ctrl = 1
trigger2 = stateno = 220
trigger2 = movecontact
trigger3 = stateno = 115
trigger3 = Var(11) < 2

;-------------------------------------------------------------------------------
[State -1, J - Heavy ATK]
type = ChangeState
value = 222
triggerall = !var(59)
triggerall = command = "b"
triggerall = statetype = A
trigger1 = ctrl = 1
trigger2 = stateno = 220 || stateno = 221
trigger2 = movecontact
trigger3 = stateno = 450
trigger3 = movecontact
trigger4 = stateno = 115
trigger4 = Var(11) < 2

;===============================================================================
;-| X's Support Movements |-----------------------------------------------------
;===============================================================================
[State -1, S - Phantom Dash]
type = ChangeState
value = 110
triggerall = !var(59)
triggerall = Stateno != 110
triggerall = command = "c"
triggerall = command != "holddown"
trigger1 = statetype != A
trigger1 = ctrl = 1

[State -1, S - Hop]
type = ChangeState
value = 115
triggerall = !var(59)
triggerall = command = "holddown" && command = "c"
triggerall = Stateno != 110
triggerall = Var(11) = 0
trigger1 = statetype != A
trigger1 = ctrl = 1
trigger2 = IfElse(Var(58) = 0, Power >= 1000, FVar(0) > 0)
trigger2 = Stateno = 200 || Stateno = 201 || Stateno = 202 || Stateno = 203 || Stateno = 205
trigger2 = movecontact
trigger3 = IfElse(Var(58) = 0, Power >= 1000, FVar(0) > 0)
trigger3 = Stateno = 210 || Stateno = 211 || Stateno = 212
trigger3 = movecontact

;-------------------------------------------------------------------------------
[State -1, S - Dash Fwd]
type = ChangeState
value = 100
triggerall = !var(59)
triggerall = command = "FF"
triggerall = statetype = S
trigger1 = ctrl
trigger2 = stateno = 300 && time > 8

[State -1, S - Dash Back]
type = ChangeState
value = 105
triggerall = !var(59)
triggerall = command = "BB"
triggerall = statetype = S
trigger1 = ctrl
trigger2 = stateno = 300 && time > 8

;-------------------------------------------------------------------------------
[State -1, J - Dash Fwd]
type = ChangeState
value = 102
triggerall = !var(59)
triggerall = var(58) = 10000
triggerall = var(10) = 0
triggerall = command = "FF"
triggerall = statetype = A
trigger1 = ctrl

;===============================================================================
[State -1, J - Double Jump]
type = ChangeState
value = 46
triggerall = statetype = A
triggerall = command = "up"
trigger1 = Var(9) = 0 && Var(10) = 0
trigger1 = ctrl
trigger2 = var(58) = 10000
trigger2 = Var(10) = 0
trigger2 = stateno = 220|| stateno = 221|| stateno = 222
trigger2 = movecontact

;===============================================================================
;-------------------------------------------------------------------------------
;http://megamugenbrasil.ucoz.com/             Fabricado no Brasil/Made in Brazil
;-------------------------------------------------------------------------------
;===============================================================================
