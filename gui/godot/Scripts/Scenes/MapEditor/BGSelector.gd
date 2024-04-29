extends TileMap


signal on_selection(tile)


func _ready():
	var viewer : Viewer = get_tree().current_scene.find_node("Viewer")

	clear()

	# Put item list
	var items = [
		[Constants.TypeCase.VILLAGE, Constants.TypeCase.VILLAGE_J1, Constants.TypeCase.VILLAGE_J2],
		[Constants.TypeCase.NORD_OUEST, Constants.TypeCase.NORD_EST],
		[Constants.TypeCase.SUD_OUEST, Constants.TypeCase.SUD_EST],
		[Constants.TypeCase.OEUF_BLANC, Constants.TypeCase.OEUF_BLEU, Constants.TypeCase.OEUF_ROUGE],
		[Constants.TypeCase.OEUF_JAUNE, Constants.TypeCase.OEUF_GRIS],
	]
	for y in len(items):
		var row = items[y]
		for x in len(row):
			set_cell(x, y, viewer.case2tile[row[x]])


func _input(event):
	if event is InputEventMouseButton and event.pressed:
		var pos = self.transform.affine_inverse().xform(event.position)
		var tilepos = self.world_to_map(pos)
		var tile = self.get_cell(tilepos[0], tilepos[1])
		if tile != -1:
			emit_signal("on_selection", tile)
