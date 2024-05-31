class_name Replay

extends Node2D


onready var viewer : Viewer = $Viewer
onready var import_dialog : FileDialog = $Popups/ImportDialog
onready var container_interactive = $Menu/VBoxContainer/Controls
onready var viewer_viewport = $Viewer/ViewportContainer/Viewport
onready var playpause = $Menu/VBoxContainer/Controls/PlayPause
onready var next_button = $Menu/VBoxContainer/Controls/Next
onready var end_button = $Menu/VBoxContainer/Controls/End
onready var import_button = $Menu/VBoxContainer/Import
onready var back_button = $"Menu/VBoxContainer/Back to main menu"
onready var stateinfo = $Menu/VBoxContainer/StateInfo
onready var scorej1 = $Menu/VBoxContainer/ScoreJ1
onready var scorej2 = $Menu/VBoxContainer/ScoreJ2
onready var ticktimer = $TickTimer

const TICK_DURATION_MS = 300

var is_dialog_opened = false

var is_playing = false
var is_j1_playing = true

var igui_started = false

var manager : ReplayManager

var play_button = load('res:///Assets/PlayButton.tres')
var pause_button = load('res:///Assets/PauseButton.tres')



func _ready():
	ticktimer.wait_time = TICK_DURATION_MS / 1000.0
	ticktimer.start()

	manager = ReplayManager.new()
	manager.init(viewer)

	if OS.has_feature('JavaScript'):
		var text = JavaScript.eval("dump_value", true)
		print('Reading dump text ' + str(text))
		var content = text.strip_edges().split('\n')

		var json = []
		for ln in content:
			json.append(JSON.parse(ln).result)

		for turn in json:
			add_state(turn)

		# Load
		_on_Start_pressed()


	if Context.replay_path != "":
		_on_ImportDialog_file_selected(Context.replay_path)

	if Context.socket != null:
		SocketManager.step(funcref(self, "add_state"))
		import_button.visible = false
		back_button.visible = false


# Callback used for the socket
func add_state(turn):
	var game_state = Models.GameState.from_json(viewer, turn)
	manager.add_state(game_state)

	if not igui_started:
		igui_started = true
		_on_Start_pressed()


# --- Logic ---
# do_transition: Whether or not to play animations
func update_all(do_transition: bool):
	var tour = manager.current_state.tour
	# TODO : Verify 0 or 1
	is_j1_playing = tour.joueur_actuel == 0
	update_info()
	viewer.update_all(manager.current_map(), tour.id_tour)


func update_info():
	var n_turns = str(str(manager.states[-1].tour.id_tour))
	var info = "Tour: " + str(manager.current_state.tour.id_tour)
	if Context.socket == null:
		info += "/" + n_turns

	info +=  " (J" + str(1 if is_j1_playing else 2) + "), Action: " + str(manager.icurrent_state + 1)

	print('Replay.update_info, ', info)
	stateinfo.text = info
	scorej1.text = "J1: " + str(manager.current_state.map.joueurs[0].score)
	scorej2.text = "J2: " + str(manager.current_state.map.joueurs[1].score)


# --- UI ---
func set_dialog_open(is_open):
	is_dialog_opened = is_open
	viewer_viewport.gui_disable_input = is_open


func set_playing(playing):
	is_playing = playing
	playpause.icon = pause_button if is_playing else play_button


# --- Signals ---
func _on_Import_pressed():
	import_dialog.popup_centered()


func _on_ImportDialog_file_selected(path):
	print('Importing ', path)
	var json = Serialization.read_multiline_json(path)

	for turn in json:
		add_state(turn)

	# Load
	_on_Start_pressed()


func _on_ImportDialog_about_to_show():
	set_dialog_open(true)


func _on_ImportDialog_popup_hide():
	set_dialog_open(false)


func _on_Start_pressed():
	manager.set_state(0)
	set_playing(false)
	update_all(false)


func _on_Prev_pressed():
	manager.prev()
	set_playing(false)
	update_all(true)


func _on_PlayPause_pressed():
	set_playing(not is_playing)


func _on_Next_pressed(is_autoplay = false):
	ticktimer.set_paused(true)
	next_button.disabled = true
	end_button.disabled = true

	if Context.socket != null:
		SocketManager.step(funcref(self, "add_state"))

	manager.next()
	if not is_autoplay or manager.icurrent_state + 1 == len(manager.states):
		set_playing(false)
	update_all(true)

	end_button.disabled = false
	next_button.disabled = false
	ticktimer.set_paused(false)


func _on_End_pressed():
	manager.set_state(len(manager.states) - 1)
	set_playing(false)
	update_all(false)


# When is_playing, will play next turn
func _on_TickTimer_timeout():
	if is_playing:
		_on_Next_pressed(true)


func _on_Back_to_main_menu_pressed():
	Scenes.open_scene(self, Scenes.menu_scene)
