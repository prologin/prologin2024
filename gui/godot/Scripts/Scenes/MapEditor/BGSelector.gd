extends TileMap


signal on_selection(tile)

onready var select_rect : Node2D = $"../../SelectionRect"

func _ready():
	var viewer : Viewer = get_tree().current_scene.find_node("Viewer")
	
	clear()

	# Put item list
	var items = [
		[Constants.TypeCase.VILLAGE, Constants.TypeCase.VILLAGE_J1, Constants.TypeCase.VILLAGE_J2],
		[Constants.TypeCase.NORD_OUEST, Constants.TypeCase.NORD_EST],
		[Constants.TypeCase.SUD_OUEST, Constants.TypeCase.SUD_EST],
		[Constants.TypeCase.OEUF_VERT, Constants.TypeCase.OEUF_BLEU, Constants.TypeCase.OEUF_ROUGE],
		[Constants.TypeCase.OEUF_JAUNE, Constants.TypeCase.OEUF_GRIS],
	]
	var labels = {
		Constants.TypeCase.OEUF_VERT: "Aigle de vie",
		Constants.TypeCase.OEUF_BLEU: "Aigle de gel",
		Constants.TypeCase.OEUF_ROUGE: "Aigle de feu",
		Constants.TypeCase.OEUF_JAUNE: "Aigle de météore",
		Constants.TypeCase.OEUF_GRIS: "Aigle de mort",
	}
	
	var size = Vector2(110, 110)
	for y in len(items): 
		var row = items[y]
		for x in len(row):
			var cell = row[x]
			set_cell(x, y, viewer.case2tile[cell])
			if labels.has(cell):
				var label = labels.get(cell)
				add_collision_body(viewer, Vector2(x, y), size, label)

func add_collision_body(node2D: Node2D, pos: Vector2, size: Vector2, label: String):
	var rect = ColorRect.new()
	var world_pos = map_to_world(pos)
	rect.rect_position = world_pos
	rect.color = Color(0.0, 0.0, 0.0, 0.0)
	rect.rect_size = size
	
	rect.set_theme(get_parent().theme)
	rect.set_tooltip(label)
	
	add_child(rect)

func set_selection_rect(tilepos):
	var pos : Vector2 = to_global(map_to_world(Vector2(tilepos[0], tilepos[1])))
	select_rect.position.x = pos.x - 3
	select_rect.position.y = pos.y - 3
	select_rect.visible = true

func _input(event):
	if event is InputEventMouseButton and event.pressed:
		var pos = self.transform.affine_inverse().xform(event.position)
		var tilepos = self.world_to_map(pos)
		var tile = self.get_cell(tilepos[0], tilepos[1])
		if tile != -1:
			set_selection_rect(tilepos)
			emit_signal("on_selection", tile)
