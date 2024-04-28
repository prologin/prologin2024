extends TileMap


signal on_selection(tile)


func _ready():
	clear()
	# Put item list
	var items = [
		[1, 2, 3],
		[4],
		[5, 6, 7],
		[8, 9],
	]
	for y in len(items):
		var row = items[y]
		for x in len(row):
			set_cell(x, y, row[x])


func _input(event):
	if event is InputEventMouseButton and event.pressed:
		var pos = self.transform.affine_inverse().xform(event.position)
		var tilepos = self.world_to_map(pos)
		var tile = self.get_cell(tilepos[0], tilepos[1])
		if tile != -1:
			emit_signal("on_selection", tile)
