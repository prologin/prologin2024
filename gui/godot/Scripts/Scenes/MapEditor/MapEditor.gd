extends Node2D

onready var viewer : Viewer = $HBoxContainer/ViewportContainer/Viewport/Viewer
onready var editor_mode_toggle : Button = $HBoxContainer/Selector/VBoxContainer/EditorModeToggle

var bg_selection = null
var is_points_editor_mode = false


func _ready():
	_on_ClearMap_pressed()
	update_editor_mode()


# --- UI ---
func update_editor_mode():
	var alpha_disabled = .25
	if is_points_editor_mode:
		editor_mode_toggle.text = 'EDITION POINTS'
		viewer.set_alpha(alpha_disabled, 1)
	else:
		editor_mode_toggle.text = 'EDITION CARTE'
		viewer.set_alpha(1, alpha_disabled)


# --- Click ---
func _on_Viewer_bg_drag(pos):
	if bg_selection != null and viewer.map.carte[pos[1]][pos[0]] != bg_selection:
		viewer.map.carte[pos[1]][pos[0]] = bg_selection
		viewer.update_all(viewer.map)


# --- Selector ---
func _on_ClearMap_pressed():
	var map = Models.Map.new()
	map.init(20, 10)
	viewer.update_all(map)


func _on_EditorModeToggle_pressed():
	is_points_editor_mode = not is_points_editor_mode
	print(is_points_editor_mode)
	update_editor_mode()


func _on_BGSelector_on_selection(tile):
	bg_selection = viewer.tile2case[tile]
	print('bg_selection: ', bg_selection)
