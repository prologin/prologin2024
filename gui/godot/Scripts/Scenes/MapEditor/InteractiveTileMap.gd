class_name InteractiveTileMap

extends TileMap

# Tilemap we can interact with (click / drag)

signal click(pos, button)
signal drag(pos)

var mouse_pressed = false
var points_editor_mode = Constants.EditorMode.BACKGROUND


func set_tiles_mode(mode):
	points_editor_mode = mode


func get_tile_pos(position):
	var mouse_pos = self.get_parent().transform.affine_inverse().xform(position)
	if points_editor_mode != Constants.EditorMode.BACKGROUND:
		mouse_pos[0] -= Constants.TILE_SIZE / 2
		mouse_pos[1] -= Constants.TILE_SIZE / 2
	return self.world_to_map(mouse_pos)


func _input(event):
	if event is InputEventMouseMotion and self.get_viewport_rect().has_point(get_global_mouse_position()) and mouse_pressed:
		var pos = get_tile_pos(get_global_mouse_position())
		emit_signal("drag", pos)

	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT:
			mouse_pressed = event.pressed

		if event.pressed and self.get_viewport_rect().has_point(get_global_mouse_position()):
			# Inverse viewer transform
			var pos = get_tile_pos(get_global_mouse_position())
			emit_signal("click", pos, event.button_index)
