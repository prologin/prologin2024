class_name Viewer

extends Node2D

signal bg_left_click(pos)
signal bg_right_click(pos)
signal bg_drag(pos)

onready var interactive_tile_map : InteractiveTileMap = $ViewportContainer/Viewport/InteractiveTileMap
onready var debug : TileMap = $ViewportContainer/Viewport/DebugTileMap
onready var territory : TileMap = $ViewportContainer/Viewport/Territory
onready var eau : TileMap = $ViewportContainer/Viewport/Eau
onready var iles : TileMap = $ViewportContainer/Viewport/Iles
onready var villages : TileMap = $ViewportContainer/Viewport/Villages
onready var ponts : Node2D = $ViewportContainer/Viewport/Ponts
onready var ponts_sample_h : Sprite = $ViewportContainer/Viewport/PontsSample/h
onready var ponts_sample_v : Sprite = $ViewportContainer/Viewport/PontsSample/v
onready var foreground : TileMap = $ViewportContainer/Viewport/ForegroundTileMap
onready var grid : TileMap = $ViewportContainer/Viewport/Grid
onready var container : ViewportContainer = $ViewportContainer
onready var points : Node2D = $ViewportContainer/Viewport/Points
onready var points_sample : Label = $ViewportContainer/Viewport/Points/Sample
onready var camera : Camera2D = $ViewportContainer/Viewport/Camera2D

var map : Models.Map
var tour : int = 0
var original_res: Vector2 = Vector2.ZERO

# Tiles in order of the tileset
const tiles = [
	Constants.TypeCase.VILLAGE,
	Constants.TypeCase.NORD_OUEST,
	Constants.TypeCase.SUD_OUEST,
	Constants.TypeCase.SUD_EST,
	Constants.TypeCase.NORD_EST,
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
	Constants.TypeCase.DEBUG_JAUNE,
	Constants.TypeCase.PONT_1_NORD_OUEST,
	Constants.TypeCase.PONT_1_SUD_OUEST,
	Constants.TypeCase.PONT_1_SUD_EST,
	Constants.TypeCase.PONT_1_NORD_EST,
	Constants.TypeCase.PONT_2_NORD_OUEST,
	Constants.TypeCase.PONT_2_SUD_OUEST,
	Constants.TypeCase.PONT_2_SUD_EST,
	Constants.TypeCase.PONT_2_NORD_EST,
	Constants.TypeCase.VILLAGE_J1,
	Constants.TypeCase.VILLAGE_J2,
	Constants.TypeCase.ILOTS_NORD_OUEST,
	Constants.TypeCase.ILOTS_SUD_OUEST,
	Constants.TypeCase.ILOTS_SUD_EST,
	Constants.TypeCase.ILOTS_NORD_EST,
	Constants.TypeCase.ILE_SUD_OUEST,
	Constants.TypeCase.ILE_SUD_EST,
	Constants.TypeCase.ILE_NORD_EST,
	Constants.TypeCase.ILE_NORD_OUEST,
	Constants.TypeCase.PONT_OUEST,
	Constants.TypeCase.PONT_SUD,
	Constants.TypeCase.PONT_EST,
	Constants.TypeCase.PONT_NORD,
	Constants.TypeCase.AIGLE_VERT,
	Constants.TypeCase.AIGLE_J1,
	Constants.TypeCase.AIGLE_J2,
	Constants.TypeCase.EAU,
	Constants.TypeCase.ILOT_NORD_OUEST,
	Constants.TypeCase.ILOT_SUD_OUEST,
	Constants.TypeCase.ILOT_NORD_EST,
	Constants.TypeCase.ILOT_SUD_EST,
	Constants.TypeCase.ILE_SUD_OUEST,
	Constants.TypeCase.ILE_SUD_EST,
	Constants.TypeCase.ILE_NORD_OUEST,
	Constants.TypeCase.ILE_NORD_EST,
	Constants.TypeCase.OEUF_VERT,
	Constants.TypeCase.BTN_REPLAY,
	Constants.TypeCase.BTN_PLAY,
	Constants.TypeCase.BTN_PAUSE,
	Constants.TypeCase.BTN_NEXT,
	Constants.TypeCase.BTN_PREV,
	Constants.TypeCase.BTN_END,
	Constants.TypeCase.BTN_START,
	Constants.TypeCase.TERRITOIRE_1,
	Constants.TypeCase.TERRITOIRE_2,
	Constants.TypeCase.TERRITOIRE_1_2,
	Constants.TypeCase.CERCLE_GRIS,
	Constants.TypeCase.CERCLE_ROUGE,
	Constants.TypeCase.CERCLE_BLEU,
	Constants.TypeCase.CERCLE_JAUNE,
	Constants.TypeCase.CERCLE_VERT,
	Constants.TypeCase.FLECHE_TOURNE,
]

# Type case 2 tile id
var case2tile = {}
var tile2case = {}

var effet2aigle = {
	'VIE': Constants.TypeCase.AIGLE_VERT,
	'MORT': Constants.TypeCase.AIGLE_GRIS,
	'FEU': Constants.TypeCase.AIGLE_ROUGE,
	'GEL': Constants.TypeCase.AIGLE_BLEU,
	'METEORE': Constants.TypeCase.AIGLE_JAUNE,
}
var aigle2effet = {}
var effet2oeuf = {
	'VIE': Constants.TypeCase.OEUF_VERT,
	'MORT': Constants.TypeCase.OEUF_GRIS,
	'FEU': Constants.TypeCase.OEUF_ROUGE,
	'GEL': Constants.TypeCase.OEUF_BLEU,
	'METEORE': Constants.TypeCase.OEUF_JAUNE,
}

var case2char = {
	Constants.TypeCase.VILLAGE: "X",
	Constants.TypeCase.NORD_EST: "1",
	Constants.TypeCase.NORD_OUEST: "2",
	Constants.TypeCase.SUD_OUEST: "3",
	Constants.TypeCase.SUD_EST: "4",
}
var char2case = {}

var drakkar2case = {
	Constants.DrakkarDebug.BLEU: Constants.TypeCase.DEBUG_BLEU,
	Constants.DrakkarDebug.ROUGE: Constants.TypeCase.DEBUG_ROUGE,
	Constants.DrakkarDebug.JAUNE: Constants.TypeCase.DEBUG_JAUNE,
}

var rotate = {
	Constants.TypeCase.NORD_OUEST: Constants.TypeCase.SUD_OUEST,
	Constants.TypeCase.SUD_OUEST: Constants.TypeCase.SUD_EST,
	Constants.TypeCase.SUD_EST: Constants.TypeCase.NORD_EST,
	Constants.TypeCase.NORD_EST: Constants.TypeCase.NORD_OUEST,
}
var unrotate = {}
var rotatable = rotate.keys()

# Cache
var cache_computed = false

# Sprite matrix
var hponts = null
var vponts = null
var points_labels = null

# ilots_map[i][j] = 1 => there is an "ilot"
# 2x2 times the size of the map
var ilots_map = null
# connections[i][j] = true if there is an island in the lower right of the (i, j) tile
var connections = null

var territory2tile = {
	1: Constants.TypeCase.TERRITOIRE_1,
	2: Constants.TypeCase.TERRITOIRE_2,
	3: Constants.TypeCase.TERRITOIRE_1_2,
}

func _ready():
	for i in range(len(tiles)):
		case2tile[tiles[i]] = i
		tile2case[i] = tiles[i]
	for k in effet2aigle:
		aigle2effet[effet2aigle[k]] = k
	for k in effet2oeuf:
		aigle2effet[effet2oeuf[k]] = k

	for k in case2char:
		char2case[case2char[k]] = k

	for k in rotate:
		unrotate[rotate[k]] = k

	original_res = container.rect_size

func set_alpha(alpha_carte, alpha_points):
	iles.modulate.a = alpha_carte
	ponts.modulate.a = alpha_carte
	villages.modulate.a = alpha_carte
	foreground.modulate.a = alpha_carte
	points.modulate.a = alpha_points


func set_tiles_mode(points_editor_mode):
	interactive_tile_map.set_tiles_mode(points_editor_mode)


# --- Update ---
func update_all(new_map, new_tour = 0):
	if map == null or new_map.width != map.width or new_map.height != new_map.height:
		cache_computed = false

	map = new_map
	tour = new_tour

	if cache_computed == false:
		update_cache()
		update_zoom()
	update_grid()
	update_points()
	update_iles()
	update_villages()
	update_ponts()
	update_foreground()
	update_territory()


func update_zoom():
	if map.height == 0 or map.width == 0:
		return

	var target_height = container.get_rect().size[1]
	var target_width = container.get_rect().size[0]
	var h_target_tile_size = target_height / map.height
	var h_target_scale = h_target_tile_size / Constants.TILE_SIZE
	var w_target_tile_size = target_width / map.width
	var w_target_scale = w_target_tile_size / Constants.TILE_SIZE
	var target_scale = min(h_target_scale, w_target_scale)

	container.rect_size = original_res / target_scale
	container.rect_scale = Vector2(target_scale, target_scale)

	camera.offset = map.size * Constants.TILE_SIZE / 2


func update_cache():
	cache_computed = true

	# Water
	eau.clear()
	for i in map.height:
		for j in map.width:
			eau.set_cell(j, i, case2tile[Constants.TypeCase.EAU])

	# Ponts
	for c in ponts.get_children():
		ponts.remove_child(c)

	hponts = []
	vponts = []
	for i in map.height - 1:
		var hrow = []
		var vrow = []
		for j in map.width - 1:
			if j + 2 != map.width:
				var hpont = ponts_sample_h.duplicate()
				hpont.position.x = (j + 1) * Constants.TILE_SIZE + Constants.HALF_TILE_SIZE
				hpont.position.y = (i + 1) * Constants.TILE_SIZE
				hpont.visible = false
				ponts.add_child(hpont)
				hrow.append(hpont)

			if i + 2 != map.height:
				var vpont = ponts_sample_v.duplicate()
				vpont.position.x = (j + 1) * Constants.TILE_SIZE
				vpont.position.y = (i + 1) * Constants.TILE_SIZE + Constants.HALF_TILE_SIZE
				vpont.visible = false
				ponts.add_child(vpont)
				vrow.append(vpont)
		hponts.append(hrow)
		vponts.append(vrow)

	# Points
	for child in points.get_children():
		if child.name != 'Sample':
			points.remove_child(child)

	points_labels = []
	for i in range(map.height - 1):
		var row = []
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
			row.append(point)
		points_labels.append(row)


func update_grid():
	grid.clear()

	for i in range(map.height):
		for j in range(map.width):
			grid.set_cell(j, i, 0)


func update_points():
	# Update points
	for i in range(map.height - 1):
		for j in range(map.width - 1):
			var point : Label = points_labels[i][j]
			var new_text = str(map.points[i][j])
			if point.text != new_text:
				point.text = new_text


func update_iles():
	iles.clear()

	var tiles = []
	var cell_choices = [
		[Constants.TypeCase.ILOT_NORD_OUEST, Constants.TypeCase.ILOT_NORD_EST],
		[Constants.TypeCase.ILOT_SUD_OUEST, Constants.TypeCase.ILOT_SUD_EST]
	]
	# Make background without mask
	for i in range(map.height * 2):
		var row = []
		var choices = cell_choices[i % 2]
		for j in range(map.width):
			for c in choices:
				row.append(c)
		tiles.append(row)

	ilots_map = []
	for i in range(map.height):
		var row1 = []
		var row2 = []
		for j in range(map.width):
			var template = [
				[1, 1],
				[1, 1]
			]
			match map.carte[i][j]:
				Constants.TypeCase.NORD_OUEST:
					template = [
						[0, 1],
						[1, 1],
					]
				Constants.TypeCase.SUD_OUEST:
					template = [
						[1, 1],
						[0, 1],
					]
				Constants.TypeCase.SUD_EST:
					template = [
						[1, 1],
						[1, 0],
					]
				Constants.TypeCase.NORD_EST:
					template = [
						[1, 0],
						[1, 1],
					]
			row1.append_array(template[0])
			row2.append_array(template[1])
		ilots_map.append(row1)
		ilots_map.append(row2)

	# Make connections
	connections = []
	for i in map.height - 1:
		var row = []
		for j in map.width - 1:
			# 4 ilots = 1 island
			var is_connected = ilots_map[2 * i + 1][2 * j + 1] && ilots_map[2 * i + 2][2 * j + 1] && ilots_map[2 * i + 1][2 * j + 2] && ilots_map[2 * i + 2][2 * j + 2]
			if is_connected:
				for di in 2:
					for dj in 2:
						# It is the part of an island
						ilots_map[2 * i + 1 + di][2 * j + 1 + dj] = 2

				tiles[2 * i + 1][2 * j + 1] = Constants.TypeCase.ILE_SUD_OUEST
				tiles[2 * i + 2][2 * j + 1] = Constants.TypeCase.ILE_NORD_OUEST
				tiles[2 * i + 2][2 * j + 2] = Constants.TypeCase.ILE_NORD_EST
				tiles[2 * i + 1][2 * j + 2] = Constants.TypeCase.ILE_SUD_EST
			row.append(is_connected)
		connections.append(row)

	# Mask
	for i in range(map.height):
		for j in range(map.width):
			var iiles = 2 * i
			var jiles = 2 * j
			for di in 2:
				for dj in 2:
					var ii = iiles + di
					var jj = jiles + dj
					if ilots_map[ii][jj]:
						iles.set_cell(jj, ii, case2tile[tiles[ii][jj]])


func update_villages():
	villages.clear()

	for i in map.height:
		for j in map.width:
			if map.carte[i][j] in [Constants.TypeCase.VILLAGE, Constants.TypeCase.VILLAGE_J1, Constants.TypeCase.VILLAGE_J2]:
				villages.set_cell(j, i, case2tile[map.carte[i][j]])


func update_ponts():
	for i in map.height - 1:
		for j in map.width - 1:
			if i + 2 != map.height:
				vponts[i][j].visible = connections[i][j] and connections[i + 1][j]

			if j + 2 != map.width:
				hponts[i][j].visible = connections[i][j] and connections[i][j + 1]


func update_foreground():
	foreground.clear()
	debug.clear()

	for aigle in map.aigles:
		var transform = effet2oeuf if tour < aigle.tour_eclosion else effet2aigle
		var tile = case2tile[transform[aigle.effet]]
		foreground.set_cell(aigle.pos[0], aigle.pos[1], tile)
	
	if len(map.debug) > 0:
		for i in range(map.height):
			for j in range(map.width):
				var drakkar = map.debug[i][j]
				if drakkar == Constants.DrakkarDebug.PAS_DE_DRAKKAR:
					continue
				var tile = case2tile[drakkar2case[drakkar]]
				debug.set_cell(j, i, tile)


func update_territory():
	if map.territoire == null:
		return
	territory.clear()

	for i in range(map.height - 1):
		for j in range(map.width - 1):
			if map.territoire[i][j] == 0:
				continue
			var tile = territory2tile[int(map.territoire[i][j])]
			territory.set_cell(j, i, tile)


# --- Signals ---
func _on_InteractiveTileMap_drag(pos):
	var j = pos[0]
	var i = pos[1]
	if map != null and j >= 0 and j < map.width and i >= 0 and i < map.height:
		emit_signal("bg_drag", pos)


func _on_InteractiveTileMap_click(pos, button):
	var j = pos[0]
	var i = pos[1]
	if map != null and j >= 0 and j < map.width and i >= 0 and i < map.height:
		if button == BUTTON_LEFT:
			emit_signal("bg_left_click", pos)
		elif button == BUTTON_RIGHT:
			emit_signal("bg_right_click", pos)


func _on_GridOpacitySlider_value_changed(value):
	self.grid.modulate.a = value / 100
	
func _on_PointOpacitySlider_value_changed(value):
	self.points.modulate.a = value / 100
