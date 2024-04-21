extends TileMap

signal leftclick(pos)

func _input(event):
	if event is InputEventMouseButton and event.pressed and self.get_viewport_rect().has_point(event.position):
		# Inverse viewer transform
		var mouse_pos = self.get_parent().transform.affine_inverse().xform(event.position)
		var pos = self.world_to_map(mouse_pos)
		emit_signal("leftclick", pos)
