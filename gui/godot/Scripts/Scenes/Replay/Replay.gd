extends Node2D


onready var viewer : Viewer = $HBoxContainer/ViewportContainer/Viewport/Viewer
onready var import_dialog : FileDialog = $Popups/ImportDialog
onready var container_interactive = $HBoxContainer
onready var viewer_viewport = $HBoxContainer/ViewportContainer/Viewport

var is_dialog_opened = false


func _ready():
	var map = Models.Map.new()
	map.init(0, 0)
	viewer.update_all(map)


func set_dialog_open(is_open):
	is_dialog_opened = is_open
	viewer_viewport.gui_disable_input = is_open


func _on_Import_pressed():
	import_dialog.popup_centered()


func _on_ImportDialog_file_selected(path):
	print('Importing ', path)
	# TODO : Read dump not only map
	var json = Serialization.read_json(path)
	var map = Serialization.json_to_map(viewer, json)
	viewer.update_all(map)


func _on_ImportDialog_about_to_show():
	set_dialog_open(true)


func _on_ImportDialog_popup_hide():
	set_dialog_open(false)
