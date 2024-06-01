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
onready var toolbar : Node = $Viewer/Selector/HBoxContainer

var bg_selection = null
var points_selection = null

var points_editor_mode = false
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
	viewer.update_eagle_tooltip()
	toolbar.show_editor_controls()

	if not toolbar.connect("tool_changed", self, "_on_Toolbar_tool_changed"):
		push_warning("Could not connect to toolbar signal")

func is_selection_enabled():
	return (points_selection if points_editor_mode else bg_selection) != null

func is_background(tile: int):
	return tile in [
		Constants.TypeCase.VILLAGE,
		Constants.TypeCase.VILLAGE_J1,
		Constants.TypeCase.VILLAGE_J2,
		Constants.TypeCase.NORD_OUEST,
		Constants.TypeCase.NORD_EST,
		Constants.TypeCase.SUD_OUEST,
		Constants.TypeCase.SUD_EST,
	]


func get_tile_pos(pos: Vector2, offset: bool):
	if offset:
		var emplacement_pos = Vector2(int(pos.x - 1) / 2, int(pos.y - 1) / 2)
		return emplacement_pos
	else:
		var case_pos = Vector2(int(pos.x) / 2, int(pos.y) / 2)
		return case_pos

# --- UI ---
func update_editor_mode():
	var alpha_disabled = .25
	tiles_selector.visible = not points_editor_mode
	points_selector.visible = points_editor_mode
	points_amount.visible = points_editor_mode
	if points_editor_mode:
		editor_mode_toggle.text = 'EDITION CARTE'
		selection_rect.visible = false
		viewer.set_alpha(alpha_disabled, 1)
		toolbar.trash_button.disabled = true
	else:
		editor_mode_toggle.text = 'EDITION POINTS'
		if bg_selection != null:
			selection_rect.visible = true
		viewer.set_alpha(1, alpha_disabled)
		toolbar.trash_button.disabled = false


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
	if toolbar.edition_mode == Constants.EditorTool.SELECT:
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
	if event is InputEventKey:
		if event.pressed and event.scancode in matching:
			var n = matching[event.scancode]
			if points_editor_mode:
				on_point_selected(n)
			else:
				# Edit map
				if n < len(Viewer.tiles):
					set_bg_selection(Viewer.tiles[n])
		if not event.pressed and event.scancode == KEY_TAB:
			_on_EditorModeToggle_pressed()

	# Scroll points
	if points_editor_mode:
		if event.is_action("scroll_up") and event.shift:
			_on_PointsAmount_update_value(1)
		elif event.is_action("scroll_down") and event.shift:
			_on_PointsAmount_update_value(-1)


func set_dialog_open(is_open):
	is_dialog_opened = is_open
	viewer_viewport.gui_disable_input = is_open
	selector_viewport.gui_disable_input = is_open

func _on_click(pos: Vector2, drag: bool = false):
	var editor_tool = toolbar.edition_mode

	var tile_pos = get_tile_pos(pos, false)
	var x = tile_pos.x
	var y = tile_pos.y
	var offset_pos = get_tile_pos(pos, true)
	var ox = offset_pos.x
	var oy = offset_pos.y

	if viewer.map == null:
		return
	if x < 0 or x >= viewer.map.width or y < 0 or y >= viewer.map.height:
		return

	if points_editor_mode:
		if (editor_tool != Constants.EditorTool.PLACE or ox < 0 or ox >= viewer.map.width - 1
			or oy < 0 or oy >= viewer.map.height):
			return
		# if currently editing gains
		if (points_selection != null and viewer.map.points[oy][ox] != points_selection
			and oy < viewer.map.height - 1 and ox < viewer.map.width - 1):
			# checking if selection is not null
			viewer.map.points[oy][ox] = points_selection
			viewer.update_all(viewer.map)
		return
	
	if editor_tool == Constants.EditorTool.DELETE:
		# deleting the selected tile

		# first, find if there is an eagle in this location
		var eagle_found = false
		for i in range(len(viewer.map.aigles) - 1, -1, -1):
			var aigle = viewer.map.aigles[i]
			if aigle.pos == offset_pos:
				viewer.map.aigles.remove(i)
				eagle_found = true
				break
		if eagle_found:
			viewer.update_all(viewer.map)
			return
		
		# otherwise, delete the background tile (set it to default NW tile)
		viewer.map.carte[y][x] = Constants.TypeCase.NORD_OUEST

	elif editor_tool == Constants.EditorTool.PLACE:
		# currently editing tiles or eagles

		# if nothing is selected, ignore (and let the rectangle selection handle this)
		if bg_selection == null:
			return
		
		if is_background(bg_selection):
			# background tile (isles rotation or village)
			if viewer.map.carte[y][x] == bg_selection:
				return
			if bg_selection in [Constants.TypeCase.VILLAGE_J1, Constants.TypeCase.VILLAGE_J2]:
				remove_owned_villages(bg_selection)
			viewer.map.carte[y][x] = bg_selection

		elif oy >= 0 and oy < viewer.map.height - 1 and ox >= 0 and ox < viewer.map.width - 1 and not drag:
			# foreground click (eagle selected), disallow drag and check dimensions
			selected_aigle_data = {
				'pos': offset_pos,
				'effet': viewer.aigle2effet[bg_selection]
			}
			if Input.is_key_pressed(KEY_SHIFT):
				_on_AigleDialog_confirmed()
			else:
				aigle_dialog.popup_centered()
	
	else:
		# another unhandled tool was selected, return to avoid useless map refresh
		return

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
	_on_click(pos, true)


func _on_Viewer_bg_click(pos):
	_on_click(pos)


# --- Signals ---
func _on_ClearMap_pressed():
	var map = Models.Map.new()
	map.init(Context.editor_init_width, Context.editor_init_height)
	viewer.update_all(map)


func _on_EditorModeToggle_pressed():
	points_editor_mode = not points_editor_mode
	if points_editor_mode:
		toolbar.set_tool(Constants.EditorTool.PLACE)
	else:
		toolbar.set_tool(Constants.EditorTool.SELECT)
	update_editor_mode()


func _on_BGSelector_on_selection(tile):
	set_bg_selection(viewer.tile2case[tile])


func set_bg_selection(sel):
	if points_editor_mode:
		return

	if sel == bg_selection:
		bg_selection = null
		toolbar.set_tool(Constants.EditorTool.SELECT)
		rect_selection.disable()
	else:
		bg_selection = sel
		if bg_selection in [
			Constants.TypeCase.VILLAGE,
			Constants.TypeCase.VILLAGE_J1,
			Constants.TypeCase.VILLAGE_J2,
			Constants.TypeCase.NORD_OUEST,
			Constants.TypeCase.NORD_EST,
			Constants.TypeCase.SUD_OUEST,
			Constants.TypeCase.SUD_EST,
		] and rect_selection.enabled:
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
		else:
			toolbar.set_tool(Constants.EditorTool.PLACE)


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


func _on_PointsAmount_update_value(delta):
	if points_selection == null:
		points_selection = 0

	points_amount.text = str(points_selection + delta)
	_on_PointsAmount_text_changed()


func _on_Toolbar_tool_changed(new_tool):
	if new_tool != Constants.EditorTool.PLACE:
		bg_selection = null
		selection_rect.visible = false
