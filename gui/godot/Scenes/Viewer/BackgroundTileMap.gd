extends TileMap

signal leftclick(pos)

func _input(event):
	if event is InputEventMouseButton and event.pressed:
		var pos = self.world_to_map(event.position)
		emit_signal("leftclick", pos)
