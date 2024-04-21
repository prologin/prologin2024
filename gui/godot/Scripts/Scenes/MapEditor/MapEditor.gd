extends Node2D

onready var viewer : Viewer = $HBoxContainer/ViewportContainer/Viewport/Viewer

var bg_selection = null


func _ready():
	_on_ClearMap_pressed()


# --- Click ---
func _on_Viewer_bg_drag(pos):
	if bg_selection != null and viewer.map.carte[pos[1]][pos[0]] != bg_selection:
		viewer.map.carte[pos[1]][pos[0]] = bg_selection
		viewer.update_all(viewer.map)


# --- Selector ---
func _on_SampleMap_pressed():
	var map = Models.Map.new()
	viewer.update_all(map)


func _on_ClearMap_pressed():
	var map = Models.Map.new()
	map.init(20, 10)
	viewer.update_all(map)


func _on_BGSelector_on_selection(tile):
	bg_selection = viewer.tile2case[tile]
	print('bg_selection: ', bg_selection)
