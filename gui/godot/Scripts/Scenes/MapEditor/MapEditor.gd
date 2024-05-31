class_name MapEditor extends Node2D

onready var viewer : Viewer = $Viewer
onready var rect_selection: RectSelection = $RectSelection
onready var editor_mode_toggle : Button = $Selector/VBoxContainer/EditorModeToggle
onready var tiles_selector = $Selector/VBoxContainer/ViewportContainer/Viewport/CenterContainer/BGSelector
onready var points_selector = $Selector/VBoxContainer/ViewportContainer/Viewport/CenterContainer/PointsSelector
onready var points_selector_sample : Button = $Selector/VBoxContainer/ViewportContainer/Viewport/CenterContainer/PointsSelectorSample
onready var export_dialog : FileDialog = $Popups/ExportDialog
onready var import_dialog : FileDialog = $Popups/ImportDialog
onready var viewer_viewport = $Viewer/ViewportContainer/Viewport
onready var selector_viewport = $Selector/VBoxContainer/ViewportContainer/Viewport
onready var aigle_dialog = $Popups/AigleDialog
onready var aigle_dialog_puissance : LineEdit = $Popups/AigleDialog/VBoxContainer/Puissance
onready var aigle_dialog_eclosion : LineEdit = $Popups/AigleDialog/VBoxContainer/Eclosion
onready var points_amount : TextEdit = $Selector/VBoxContainer/PointsAmount
onready var selection_rect : Sprite = $Selector/VBoxContainer/ViewportContainer/Viewport/SelectionRect
onready var validator_fail_dialog : AcceptDialog = $Popups/ValidatorFailDialog
onready var validator_fail_text : Label = $Popups/ValidatorFailDialog/Container/Error
onready var quit_dialog : ConfirmationDialog = $Popups/QuitDialog

var bg_selection = null
var points_selection = null

var points_editor_mode = Constants.EditorMode.BACKGROUND
var selected_aigle_data = null
var is_dialog_opened = false
var points_amount_focused = false


func _ready():
	if Context.map_path.empty():
		_on_ClearMap_pressed()
	else:
		_on_ImportDialog_file_selected(Context.map_path)
	update_editor_mode()
	update_selector()

func is_selection_enabled():
	match points_editor_mode:
		Constants.EditorMode.POINTS:
			return points_selection != null
		_:
			return bg_selection != null

# --- UI ---
func update_editor_mode():
	var alpha_disabled = .25
	tiles_selector.visible = points_editor_mode != Constants.EditorMode.POINTS
	points_selector.visible = points_editor_mode == Constants.EditorMode.POINTS
	points_amount.visible = points_editor_mode == Constants.EditorMode.POINTS
	if points_editor_mode == Constants.EditorMode.POINTS:
		editor_mode_toggle.text = 'EDITION CARTE'
		selection_rect.self_modulate.a = 0
		viewer.set_alpha(alpha_disabled, 1)
	else:
		editor_mode_toggle.text = 'EDITION POINTS'
		if bg_selection != null:
			selection_rect.self_modulate.a = 1
		viewer.set_alpha(1, alpha_disabled)


func on_point_selected(i):
	# TODO : Highlight selected if exists
	var new_selection = null
	if points_selection in [i, -i]:
		new_selection = -points_selection
	else:
		new_selection = i
	points_selection = new_selection if points_selection != new_selection else null
	if rect_selection.enabled:
		for tile in rect_selection.get_selected_tiles(true):
			if viewer.map.points[tile.y][tile.x] == points_selection:
				continue
			viewer.map.points[tile.y][tile.x] = points_selection
		viewer.update_all(viewer.map)
		points_selection = null
		rect_selection.disable()
	else:
		points_amount.text = str(points_selection) if points_selection != null else ""
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
	if !is_selection_enabled():
		if event is InputEventMouseButton:
			rect_selection.handle_mouse_event(event)
		elif event is InputEventMouseMotion:
			rect_selection.handle_move_event(event)
	
	if event.is_action_pressed("ui_cancel"):
		rect_selection.disable()
	
	if is_dialog_opened or points_amount_focused:
		return

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
			on_point_selected(n)
		else:
			# Edit map
			if n < len(Viewer.tiles):
				set_bg_selection(Viewer.tiles[n])


func set_dialog_open(is_open):
	is_dialog_opened = is_open
	viewer_viewport.gui_disable_input = is_open
	selector_viewport.gui_disable_input = is_open


# --- Click ---
enum ClickType {
	DRAG,
	LEFT,
	RIGHT,
}

func _on_click(pos, click_type):
	var x = pos[0]
	var y = pos[1]

	# Edit map
	if points_editor_mode == Constants.EditorMode.BACKGROUND and bg_selection != null and viewer.map.carte[y][x] != bg_selection:
		if bg_selection in [Constants.TypeCase.VILLAGE_J1, Constants.TypeCase.VILLAGE_J2]:
			remove_owned_villages(bg_selection)
		# Background click
		viewer.map.carte[y][x] = bg_selection
		viewer.update_all(viewer.map)
	elif points_editor_mode != Constants.EditorMode.BACKGROUND and y < viewer.map.height - 1 and x < viewer.map.width - 1:
		if points_editor_mode == Constants.EditorMode.POINTS and points_selection != null and viewer.map.points[y][x] != points_selection:
			# Point click
			viewer.map.points[y][x] = points_selection
			viewer.update_all(viewer.map)
		elif points_editor_mode == Constants.EditorMode.FOREGROUND and bg_selection != null:
			# Foreground click
			if click_type == ClickType.LEFT:
				selected_aigle_data = {
					'pos': pos,
					'effet': viewer.aigle2effet[bg_selection]
				}
				if Input.is_key_pressed(KEY_SHIFT):
					_on_AigleDialog_confirmed()
				else:
					aigle_dialog.popup_centered()
			elif click_type == ClickType.RIGHT:
				# Find eagle in this location
				for i in range(len(viewer.map.aigles) - 1, -1, -1):
					var aigle = viewer.map.aigles[i]
					if aigle.pos == pos:
						viewer.map.aigles.remove(i)
						break
				viewer.update_all(viewer.map)


func remove_owned_villages(tile):
	"""
	Removes villages of J1 / J2 (replaced by neutral village)
	"""
	for y in viewer.map.height:
		for x in viewer.map.width:
			if viewer.map.carte[y][x] == tile:
				viewer.map.carte[y][x] = Constants.TypeCase.VILLAGE


func _on_Viewer_bg_drag(pos):
	_on_click(pos, ClickType.DRAG)


func _on_Viewer_bg_left_click(pos):
	_on_click(pos, ClickType.LEFT)


func _on_Viewer_bg_right_click(pos):
	_on_click(pos, ClickType.RIGHT)


# --- Signals ---
func _on_ClearMap_pressed():
	var map = Models.Map.new()
	map.init(Context.editor_init_width, Context.editor_init_height)
	viewer.update_all(map)


func _on_EditorModeToggle_pressed():
	if points_editor_mode == Constants.EditorMode.POINTS:
		points_editor_mode = Constants.EditorMode.BACKGROUND
	else:
		points_editor_mode = Constants.EditorMode.POINTS
	viewer.set_tiles_mode(points_editor_mode)
	update_editor_mode()


func _on_BGSelector_on_selection(tile):
	set_bg_selection(viewer.tile2case[tile])


func set_bg_selection(sel):
	if points_editor_mode != Constants.EditorMode.POINTS:
		bg_selection = sel if sel != bg_selection else null
		if bg_selection == null or bg_selection in [
			Constants.TypeCase.VILLAGE,
			Constants.TypeCase.VILLAGE_J1,
			Constants.TypeCase.VILLAGE_J2,
			Constants.TypeCase.NORD_OUEST,
			Constants.TypeCase.NORD_EST,
			Constants.TypeCase.SUD_OUEST,
			Constants.TypeCase.SUD_EST,
		]:
			if bg_selection != null and rect_selection.enabled:
				var tiles = rect_selection.get_selected_tiles()
				for tile in tiles:
					if viewer.map.carte[tile.y][tile.x] == bg_selection:
						continue
					if bg_selection in [Constants.TypeCase.VILLAGE_J1, Constants.TypeCase.VILLAGE_J2]:
						remove_owned_villages(bg_selection)
					viewer.map.carte[tile.y][tile.x] = bg_selection
				
				rect_selection.disable()
				viewer.update_all(viewer.map)
				
				bg_selection = null
				return

			points_editor_mode = Constants.EditorMode.BACKGROUND
		else:
			points_editor_mode = Constants.EditorMode.FOREGROUND
			
		rect_selection.disable()
		viewer.set_tiles_mode(points_editor_mode)


func _on_Export_pressed(force=false):
	if force:
		export_dialog.popup_centered()
		return

	var err = Validator.validate(viewer.map)
	if not err:
		export_dialog.popup_centered()
	else:
		validator_fail_text.text = err
		validator_fail_dialog.popup_centered()

func _on_Import_pressed():
	import_dialog.popup_centered()


func _on_ExportDialog_file_selected(path):
	# TODO : Export dump to path
	var json = viewer.map.to_json(viewer)
	Serialization.write_json(json, path)
	print('Saved to ', path)


func _on_ImportDialog_file_selected(path):
	print('Importing ', path)
	var json = Serialization.read_json(path)
	var map = Models.Map.from_json(viewer, json)
	viewer.update_all(map)


func _on_Randomizer_pressed():
	var new_map = Models.Map.new()
	new_map.rand_init(Context.editor_init_width, Context.editor_init_height)
	viewer.update_all(new_map)

func _on_Back_to_main_menu_pressed():
	quit_dialog.popup_centered()

func _on_ExportDialog_about_to_show():
	set_dialog_open(true)


func _on_ExportDialog_popup_hide():
	set_dialog_open(false)


func _on_ImportDialog_about_to_show():
	set_dialog_open(true)


func _on_ImportDialog_popup_hide():
	set_dialog_open(false)


func _on_AigleDialog_about_to_show():
	aigle_dialog.window_title = 'Aigle - ' + selected_aigle_data['effet']
	set_dialog_open(true)


func _on_AigleDialog_popup_hide():
	set_dialog_open(false)


func _on_AigleDialog_confirmed():
	var tour_eclosion = int(aigle_dialog_eclosion.text)
	var puissance = int(aigle_dialog_puissance.text)

	var selected_aigle = Models.Aigle.new()
	viewer.map.aigles.append(selected_aigle)
	selected_aigle.effet = selected_aigle_data['effet']
	selected_aigle.pos = selected_aigle_data['pos']
	selected_aigle.tour_eclosion = tour_eclosion
	selected_aigle.puissance = puissance

	viewer.update_all(viewer.map)


func _on_PointsAmount_text_changed():
	var n_points = points_amount.text.strip_edges().to_int()
	if n_points != null:
		points_selection = n_points
		print('points_selection: ', points_selection)


func _on_PointsAmount_focus_entered():
	points_amount_focused = true


func _on_PointsAmount_focus_exited():
	points_amount_focused = false


func _on_PointsAmount_gui_input(event):
	if event is InputEventKey and event.pressed and event.scancode == KEY_ENTER:
		points_amount.release_focus()
		points_amount.text = str(points_selection) if points_selection != null else ""


func _on_ValidatorFailDialog_confirmed():
	_on_Export_pressed(true)


func _on_QuitDialog_confirmed():
	Context.map_path = ""
	Scenes.open_scene(self, Scenes.menu_scene)


func _on_QuitDialog_about_to_show():
	set_dialog_open(true)


func _on_QuitDialog_popup_hide():
	set_dialog_open(false)
