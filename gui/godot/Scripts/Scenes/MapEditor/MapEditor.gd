extends Node2D

onready var viewer : Viewer = $HBoxContainer/ViewportContainer/Viewport/Viewer
onready var editor_mode_toggle : Button = $HBoxContainer/Selector/VBoxContainer/EditorModeToggle
onready var points_selector = $HBoxContainer/Selector/VBoxContainer/ViewportContainer/Viewport/PointsSelector
onready var points_selector_sample = $HBoxContainer/Selector/VBoxContainer/ViewportContainer/Viewport/PointsSelectorSample

var bg_selection = null
var is_points_editor_mode = false


func _ready():
	_on_ClearMap_pressed()
	update_editor_mode()
	update_selector()


# --- UI ---
func update_editor_mode():
	var alpha_disabled = .25
	if is_points_editor_mode:
		editor_mode_toggle.text = 'EDITION POINTS'
		viewer.set_alpha(alpha_disabled, 1)
	else:
		editor_mode_toggle.text = 'EDITION CARTE'
		viewer.set_alpha(1, alpha_disabled)


func update_selector():
	for i in range(10):
		var node = points_selector_sample.duplicate()
		node.text = str(i)
		node.visible = true
		node.rect_min_size = Vector2(40, 50)
		points_selector.add_child(node)


# --- Click ---
func _on_Viewer_bg_drag(pos):
	# Edit map
	if not is_points_editor_mode and bg_selection != null and viewer.map.carte[pos[1]][pos[0]] != bg_selection:
		viewer.map.carte[pos[1]][pos[0]] = bg_selection
		viewer.update_all(viewer.map)


# --- Signals ---
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
