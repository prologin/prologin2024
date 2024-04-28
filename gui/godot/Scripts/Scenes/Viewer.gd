class_name Viewer

extends Node2D

signal bg_left_click(pos)
signal bg_right_click(pos)
signal bg_drag(pos)

onready var background : InteractiveTileMap = $BackgroundTileMap
onready var grid : TileMap = $Grid
onready var points : Node2D = $Points
onready var points_sample : Label = $Points/Sample

var map : Models.Map

# Tiles in order of the tileset
const tiles = [
	Constants.TypeCase.VILLAGE,
	Constants.TypeCase.NORD_OUEST,
	Constants.TypeCase.NORD_EST,
	Constants.TypeCase.SUD_EST,
	Constants.TypeCase.SUD_OUEST,
	Constants.TypeCase.OEUF_BLANC,
	Constants.TypeCase.OEUF_GRIS,
	Constants.TypeCase.OEUF_ROUGE,
	Constants.TypeCase.OEUF_JAUNE,
	Constants.TypeCase.OEUF_BLEU,
	Constants.TypeCase.AIGLE_BLANC,
	Constants.TypeCase.AIGLE_GRIS,
	Constants.TypeCase.AIGLE_ROUGE,
	Constants.TypeCase.AIGLE_JAUNE,
	Constants.TypeCase.AIGLE_BLEU,
	Constants.TypeCase.DRAPEAU_ROUGE,
	Constants.TypeCase.DRAPEAU_BLEU,
	Constants.TypeCase.DEBUG_ROUGE,
	Constants.TypeCase.DEBUG_BLEU,
	Constants.TypeCase.DEBUG_VERT,
	Constants.TypeCase.PONT_1_NORD_OUEST,
	Constants.TypeCase.PONT_1_SUD_OUEST,
	Constants.TypeCase.PONT_1_SUD_EST,
	Constants.TypeCase.PONT_1_NORD_EST,
	Constants.TypeCase.PONT_2_NORD_OUEST,
	Constants.TypeCase.PONT_2_SUD_OUEST,
	Constants.TypeCase.PONT_2_SUD_EST,
	Constants.TypeCase.PONT_2_NORD_EST,
]

# Type case 2 tile id
var case2tile = {}
var tile2case = {}


func _ready():
	for i in range(len(tiles)):
		case2tile[tiles[i]] = i
		tile2case[i] = tiles[i]


func set_alpha(alpha_carte, alpha_points):
	background.modulate.a = alpha_carte
	points.modulate.a = alpha_points


func set_tiles_mode(is_points_editor_mode: bool):
	background.set_tiles_mode(is_points_editor_mode)


# --- Update ---
func update_all(new_map):
	map = new_map

	update_zoom()
	update_grid()
	update_points()
	update_background()


func update_zoom():
	var target_height = self.get_viewport_rect().size[1]
	var target_width = self.get_viewport_rect().size[0]
	var h_target_tile_size = target_height / map.height
	var h_target_scale = h_target_tile_size / Constants.TILE_SIZE
	var w_target_tile_size = target_width / map.width
	var w_target_scale = w_target_tile_size / Constants.TILE_SIZE
	var target_scale = min(h_target_scale, w_target_scale)

	self.scale = Vector2(target_scale, target_scale)


func update_grid():
	grid.clear()

	for i in range(map.height):
		for j in range(map.width):
			grid.set_cell(j, i, 0)


func update_points():
	# Clear
	for child in points.get_children():
		if child.name != 'Sample':
			points.remove_child(child)

	# Add points
	for i in range(map.height - 1):
		for j in range(map.width - 1):
			var point : Label = points_sample.duplicate()
			var xoff = .1
			var yoff = .2
			var x = (j + 0.5 + xoff) * Constants.TILE_SIZE
			var y = (i + 0.5 + yoff) * Constants.TILE_SIZE
			point.visible = true
			point.text = str(map.points[i][j])
			point.set_position(Vector2(x, y))
			points.add_child(point)


func update_background():
	background.clear()

	for i in range(map.height):
		for j in range(map.width):
			var tile = case2tile[map.carte[i][j]]
			background.set_cell(j, i, tile)


# --- Signals ---
func _on_BackgroundTileMap_drag(pos):
	var j = pos[0]
	var i = pos[1]
	if map != null and j >= 0 and j < map.width and i >= 0 and i < map.height:
		emit_signal("bg_drag", pos)


func _on_BackgroundTileMap_click(pos, button):
	var j = pos[0]
	var i = pos[1]
	if map != null and j >= 0 and j < map.width and i >= 0 and i < map.height:
		if button == 1:
			emit_signal("bg_left_click", pos)
		else:
			emit_signal("bg_right_click", pos)
