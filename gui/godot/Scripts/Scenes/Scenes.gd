class_name Scenes extends Node


const menu_scene := "res://Scenes/Menu/Menu.tscn"
const visualiseur_scene := "res://Scenes/Visualiseur/Visualiseur.tscn"
const editeur_scene := "res://Scenes/MapEditor/MapEditor.tscn"
const replay_scene := "res://Scenes/Replay/Replay.tscn"


static func open_scene(node: Node2D, scene_path: String) -> void:
	if node.get_tree().change_scene(scene_path) == ERR_CANT_OPEN:
		print("error opening the scene ", scene_path)
