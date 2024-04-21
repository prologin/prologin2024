extends Node2D

onready var viewer : Viewer = $HBoxContainer/ViewportContainer/Viewport/Viewer
onready var editor_mode_toggle : Button = $HBoxContainer/Selector/VBoxContainer/EditorModeToggle
onready var tiles_selector = $HBoxContainer/Selector/VBoxContainer/ViewportContainer/Viewport/BGSelector
onready var points_selector = $HBoxContainer/Selector/VBoxContainer/ViewportContainer/Viewport/PointsSelector
onready var points_selector_sample : Button = $HBoxContainer/Selector/VBoxContainer/ViewportContainer/Viewport/PointsSelectorSample

var bg_selection = null
var points_selection = null
var is_points_editor_mode = false


func _ready():
	_on_ClearMap_pressed()
	update_editor_mode()
	update_selector()


# --- UI ---
func update_editor_mode():
	var alpha_disabled = .25
	tiles_selector.visible = not is_points_editor_mode
	points_selector.visible = is_points_editor_mode
	if is_points_editor_mode:
		editor_mode_toggle.text = 'EDITION POINTS'
		viewer.set_alpha(alpha_disabled, 1)
	else:
		editor_mode_toggle.text = 'EDITION CARTE'
		viewer.set_alpha(1, alpha_disabled)


func on_point_selected(i):
	points_selection = i
	print('points_selection: ', i)


func update_selector():
	for i in range(10):
		var node = points_selector_sample.duplicate()
		node.text = str(i)
		node.visible = true
		node.rect_min_size = Vector2(40, 50)
		points_selector.add_child(node)
		node.connect("pressed", self, "on_point_selected", [i])


# --- Keyboard ---
func _input(event):
	var matching = {
		KEY_0: 0,
		KEY_1: 1,
		KEY_2: 2,
		KEY_3: 3,
		KEY_4: 4,
		KEY_5: 5,
		KEY_6: 6,
		KEY_7: 7,
		KEY_8: 8,
		KEY_9: 9
	}
	if event is InputEventKey and event.pressed and event.scancode in matching:
		var n = matching[event.scancode]
		if is_points_editor_mode:
			# TODO : Highlight selected
			# Inverse sign
			if points_selection in [n, -n]:
				points_selection = -n
			else:
				points_selection = n
		else:
			# Edit map
			var bg_selection_options = [
				Constants.TypeCase.VILLAGE,
				Constants.TypeCase.NORD_OUEST,
				Constants.TypeCase.SUD_OUEST,
				Constants.TypeCase.SUD_EST,
				Constants.TypeCase.NORD_EST,
				# TODO : Constants.TypeCase.VILLAGE_J1,
				# TODO : Constants.TypeCase.VILLAGE_J2,
			]
			if n < len(bg_selection_options):
				bg_selection = bg_selection_options[n]


# --- Click ---
func _on_click(pos):
	var x = pos[0]
	var y = pos[1]
	# Edit map
	if not is_points_editor_mode and bg_selection != null and viewer.map.carte[y][x] != bg_selection:
		viewer.map.carte[y][x] = bg_selection
		viewer.update_all(viewer.map)
	elif is_points_editor_mode and points_selection != null and viewer.map.points[y][x] != points_selection:
		viewer.map.points[y][x] = points_selection
		viewer.update_all(viewer.map)


func _on_Viewer_bg_drag(pos):
	_on_click(pos)
		
func _on_Viewer_bg_left_click(pos):
	_on_click(pos)


# --- Signals ---
func _on_ClearMap_pressed():
	var map = Models.Map.new()
	map.init(20, 10)
	viewer.update_all(map)


func _on_EditorModeToggle_pressed():
	is_points_editor_mode = not is_points_editor_mode
	viewer.set_tiles_mode(is_points_editor_mode)
	print('points_edition_mode: ', is_points_editor_mode)
	update_editor_mode()


func _on_BGSelector_on_selection(tile):
	bg_selection = viewer.tile2case[tile]
	print('bg_selection: ', bg_selection)
