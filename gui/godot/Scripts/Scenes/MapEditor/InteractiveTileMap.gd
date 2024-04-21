extends TileMap

# Tilemap we can interact with (click / drag)

signal click(pos, button)
signal drag(pos)

var mouse_pressed = false


func get_tile_pos(position):
	var mouse_pos = self.get_parent().transform.affine_inverse().xform(position)
	return self.world_to_map(mouse_pos)


func _input(event):
	if event is InputEventMouseMotion and self.get_viewport_rect().has_point(event.position) and mouse_pressed:
		var pos = get_tile_pos(event.position)
		emit_signal("drag", pos)

	if event is InputEventMouseButton:
		if event.button_index == 1:
			mouse_pressed = event.pressed

		if event.pressed and self.get_viewport_rect().has_point(event.position):
			# Inverse viewer transform
			var pos = get_tile_pos(event.position)
			emit_signal("click", pos, event.button_index)
