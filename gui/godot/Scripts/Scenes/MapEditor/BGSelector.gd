extends TileMap


signal on_selection(tile)


func _ready():
	clear()
	for y in range(0, 2):
		for x in range(0, 2):
			var idx = x + y * 2
			set_cell(x, y, idx + 1)
	set_cell(2, 0, 0)


func _input(event):
	if event is InputEventMouseButton and event.pressed:
		var pos = self.transform.affine_inverse().xform(event.position)
		var tilepos = self.world_to_map(pos)
		var tile = self.get_cell(tilepos[0], tilepos[1])
		if tile != -1:
			emit_signal("on_selection", tile)
