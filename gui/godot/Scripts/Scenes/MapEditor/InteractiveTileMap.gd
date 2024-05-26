class_name InteractiveTileMap

extends TileMap

# Tilemap we can interact with (click / drag)

signal click(pos, button)
signal drag(pos)

var mouse_pressed = false
var points_editor_mode = Constants.EditorMode.BACKGROUND


func set_tiles_mode(mode):
	points_editor_mode = mode


func get_tile_pos(event):
	var mouse_pos = self.make_input_local(event).position
	# Pos within the map that contains tile quarters
	var map_pos = self.world_to_map(mouse_pos)

	if points_editor_mode != Constants.EditorMode.BACKGROUND:
		var emplacement_pos = Vector2(int(map_pos.x - 1) / 2, int(map_pos.y - 1) / 2)
		return emplacement_pos
	else:
		var case_pos = Vector2(int(map_pos.x) / 2, int(map_pos.y) / 2)
		return case_pos


func _input(event):
	if event is InputEventMouseMotion and mouse_pressed:
		var pos = get_tile_pos(event)
		emit_signal("drag", pos)

	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT:
			mouse_pressed = event.pressed

		if event.pressed:
			var pos = get_tile_pos(event)
			emit_signal("click", pos, event.button_index)
