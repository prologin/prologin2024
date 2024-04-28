extends Node2D

onready var viewer : Viewer = $HBoxContainer/ViewportContainer/Viewport/Viewer
onready var editor_mode_toggle : Button = $Selector/VBoxContainer/EditorModeToggle
onready var tiles_selector = $Selector/VBoxContainer/ViewportContainer/Viewport/CenterContainer/BGSelector
onready var points_selector = $Selector/VBoxContainer/ViewportContainer/Viewport/CenterContainer/PointsSelector
onready var points_selector_sample : Button = $Selector/VBoxContainer/ViewportContainer/Viewport/CenterContainer/PointsSelectorSample
onready var export_dialog : FileDialog = $Popups/ExportDialog
onready var import_dialog : FileDialog = $Popups/ImportDialog
onready var container_interactive = $HBoxContainer
onready var viewer_viewport = $HBoxContainer/ViewportContainer/Viewport
onready var selector_viewport = $Selector/VBoxContainer/ViewportContainer/Viewport

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
	if points_selection in [i, -i]:
		points_selection = -points_selection
	else:
		points_selection = i
	print('points_selection: ', points_selection)


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
				points_selection = -points_selection
			else:
				points_selection = n
			print('points_selection: ', points_selection)
		else:
			# Edit map
			if n < len(Viewer.tiles):
				bg_selection = Viewer.tiles[n]


# --- Click ---
func _on_click(pos):
	var x = pos[0]
	var y = pos[1]
	# Edit map
	if not is_points_editor_mode and bg_selection != null and viewer.map.carte[y][x] != bg_selection:
		viewer.map.carte[y][x] = bg_selection
		viewer.update_all(viewer.map)
	elif is_points_editor_mode and points_selection != null and y < viewer.map.height - 1 and x < viewer.map.width - 1 and viewer.map.points[y][x] != points_selection:
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


func _on_Export_pressed():
	export_dialog.popup_centered()


func _on_Import_pressed():
	import_dialog.popup_centered()


func _on_ExportDialog_file_selected(path):
	# TODO : Export dump to path
	var json = Serialization.map_to_json(viewer.map)
	Serialization.write_json(json, path)
	print('Saved to ', path)


func _on_ImportDialog_file_selected(path):
	print('Importing ', path)
	var json = Serialization.read_json(path)
	var map = Serialization.json_to_map(json)
	viewer.update_all(map)


func _on_ExportDialog_about_to_show():
	viewer_viewport.gui_disable_input = true
	selector_viewport.gui_disable_input = true


func _on_ExportDialog_popup_hide():
	viewer_viewport.gui_disable_input = false
	selector_viewport.gui_disable_input = false


func _on_ImportDialog_about_to_show():
	viewer_viewport.gui_disable_input = true
	selector_viewport.gui_disable_input = true


func _on_ImportDialog_popup_hide():
	viewer_viewport.gui_disable_input = false
	selector_viewport.gui_disable_input = false
