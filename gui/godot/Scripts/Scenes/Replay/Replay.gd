extends Node2D


onready var viewer : Viewer = $HBoxContainer/ViewportContainer/Viewport/Viewer
onready var import_dialog : FileDialog = $Popups/ImportDialog
onready var container_interactive = $HBoxContainer
onready var viewer_viewport = $HBoxContainer/ViewportContainer/Viewport
onready var playpause = $Menu/VBoxContainer/Controls/PlayPause
onready var stateinfo = $Menu/VBoxContainer/StateInfo
onready var scorej1 = $Menu/VBoxContainer/ScoreJ1
onready var scorej2 = $Menu/VBoxContainer/ScoreJ2
onready var ticktimer = $TickTimer

const TICK_DURATION_MS = 300

var is_dialog_opened = false

var is_playing = false
# TODO : These variables might be useless with the variables within Models
# A turn is composed of multiple actions
var turn_id = 0
var action_id = 0
var is_j1_playing = true
var n_turns = 200
var j1_score = 0
var j2_score = 0


func _ready():
	var map = Models.Map.new()
	map.init(0, 0)
	viewer.update_all(map)

	ticktimer.wait_time = TICK_DURATION_MS / 1000.0
	ticktimer.start()


# --- Logic ---
# do_transition: Whether or not to play animations
func update_all(do_transition: bool):
	# TODO : Remove
	j1_score = turn_id * 2
	j2_score = turn_id * 3
	is_j1_playing = turn_id % 2 == 0
	update_info()


func update_info():
	var info = "Tour: " + str(turn_id + 1) + "/" + str(n_turns) + " (J" + str(1 if is_j1_playing else 2) + ")"
	print('Replay.update_info, ', info)
	stateinfo.text = info
	scorej1.text = "J1: " + str(j1_score)
	scorej2.text = "J2: " + str(j2_score)


# --- UI ---
func set_dialog_open(is_open):
	is_dialog_opened = is_open
	viewer_viewport.gui_disable_input = is_open


func set_playing(playing):
	is_playing = playing
	playpause.text = "Pause" if is_playing else "Jouer"	


# --- Signals ---
func _on_Import_pressed():
	import_dialog.popup_centered()


func _on_ImportDialog_file_selected(path):
	print('Importing ', path)
	# TODO : Read dump not only map
	var json = Serialization.read_json(path)
	var map = Serialization.json_to_map(viewer, json)
	viewer.update_all(map)

	# Load
	_on_Start_pressed()


func _on_ImportDialog_about_to_show():
	set_dialog_open(true)


func _on_ImportDialog_popup_hide():
	set_dialog_open(false)


func _on_Start_pressed():
	turn_id = 0
	set_playing(false)
	# TODO
	update_all(false)


func _on_Prev_pressed():
	# TODO
	# TODO : Check not first turn
	turn_id -= 1
	update_all(true)


func _on_PlayPause_pressed():
	set_playing(not is_playing)


func _on_Next_pressed():
	# TODO
	# TODO : Check not last turn, if last disable is_playing
	turn_id += 1
	update_all(true)


func _on_End_pressed():
	turn_id = n_turns - 1
	set_playing(false)
	# TODO
	update_all(false)


# When is_playing, will play next turn
func _on_TickTimer_timeout():
	if is_playing:
		_on_Next_pressed()
