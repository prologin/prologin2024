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

var points_editor_mode = Constants.EditorMode.BACKGROUND


func _ready():
	_on_ClearMap_pressed()
	update_editor_mode()
	update_selector()


# --- UI ---
func update_editor_mode():
	var alpha_disabled = .25
	tiles_selector.visible = points_editor_mode != Constants.EditorMode.POINTS
	points_selector.visible = points_editor_mode == Constants.EditorMode.POINTS
	if points_editor_mode == Constants.EditorMode.POINTS:
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
		if points_editor_mode == Constants.EditorMode.POINTS:
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
	if points_editor_mode == Constants.EditorMode.BACKGROUND and bg_selection != null and viewer.map.carte[y][x] != bg_selection:
		viewer.map.carte[y][x] = bg_selection
		viewer.update_all(viewer.map)
	elif points_editor_mode != Constants.EditorMode.BACKGROUND and y < viewer.map.height - 1 and x < viewer.map.width - 1:
		if points_editor_mode == Constants.EditorMode.POINTS and points_selection != null and viewer.map.points[y][x] != points_selection:
			viewer.map.points[y][x] = points_selection
			viewer.update_all(viewer.map)
		elif points_editor_mode == Constants.EditorMode.FOREGROUND:
			# TODO : Dialog
			var tour_eclosion = 42
			var puissance = 2

			# Find eagle in this location
			var selected_aigle = null
			for aigle in viewer.map.aigles:
				if aigle.pos == pos:
					selected_aigle = aigle
					break

			if selected_aigle == null:
				selected_aigle = Models.Aigle.new()
				viewer.map.aigles.append(selected_aigle)

			selected_aigle.effet = viewer.aigle2effet[bg_selection]
			selected_aigle.pos = pos
			selected_aigle.tour_eclosion = tour_eclosion
			selected_aigle.puissance = puissance
			print('EFFET ', selected_aigle.effet)
			print('POS ', selected_aigle.pos)
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
	if points_editor_mode == Constants.EditorMode.POINTS:
		points_editor_mode = Constants.EditorMode.BACKGROUND
	else:
		points_editor_mode = Constants.EditorMode.POINTS
	viewer.set_tiles_mode(points_editor_mode)
	print('points_edition_mode: ', points_editor_mode)
	update_editor_mode()


func _on_BGSelector_on_selection(tile):
	bg_selection = viewer.tile2case[tile]
	print('bg_selection: ', bg_selection)
	if points_editor_mode != Constants.EditorMode.POINTS:
		if bg_selection in [
			Constants.TypeCase.VILLAGE,
			Constants.TypeCase.VILLAGE_J1,
			Constants.TypeCase.VILLAGE_J2,
			Constants.TypeCase.NORD_OUEST,
			Constants.TypeCase.NORD_EST,
			Constants.TypeCase.SUD_OUEST,
			Constants.TypeCase.SUD_EST,
		]:
			points_editor_mode = Constants.EditorMode.BACKGROUND
		else:
			points_editor_mode = Constants.EditorMode.FOREGROUND


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
