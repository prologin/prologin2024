class_name Viewer

extends Node2D

signal bg_left_click(pos)

onready var background : TileMap = $BackgroundTileMap

var map : Models.Map

# Tiles in order
const tiles = [
	Constants.TypeCase.VILLAGE,
	Constants.TypeCase.NORD_OUEST,
	Constants.TypeCase.NORD_EST,
	Constants.TypeCase.SUD_EST,
	Constants.TypeCase.SUD_OUEST,
]

# Type case 2 tile id
var case2tile = {}
var tile2case = {}


func _ready():
	for i in range(len(tiles)):
		case2tile[tiles[i]] = i
		tile2case[i] = tiles[i]


func update_all(new_map):
	map = new_map

	update_background()


func update_background():
	background.clear()
	
	for i in range(map.height):
		for j in range(map.width):
			var tile = case2tile[map.carte[i][j]]
			background.set_cell(j, i, tile)


func _on_BackgroundTileMap_leftclick(pos):
	var j = pos[0]
	var i = pos[1]
	if map != null and j >= 0 and j < map.width and i >= 0 and i < map.height:
		emit_signal("bg_left_click", pos)
