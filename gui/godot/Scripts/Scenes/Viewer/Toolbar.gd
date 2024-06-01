extends Node

signal tool_changed(newtool)

onready var viewer : Viewer = $"../../../Viewer"
onready var camera : Camera2D = $"../../ViewportContainer/Viewport/Camera2D"
onready var select_button : ToolButton = $SelectionButton
onready var place_button : ToolButton = $PlaceButton
onready var move_button : ToolButton = $MoveButton
onready var trash_button : ToolButton = $DeleteButton
onready var zoom_button : ToolButton = $ResetZoom

export var edition_mode = Constants.EditorTool.SELECT

func show_editor_controls():
	trash_button.visible = true
	place_button.visible = true

func _ready():
	select_button.hint_tooltip += " (%s)" % OS.get_scancode_string(KEY_V)
	place_button.hint_tooltip += " (%s)" % OS.get_scancode_string(KEY_T)
	move_button.hint_tooltip += " (%s)" % OS.get_scancode_string(KEY_M)
	trash_button.hint_tooltip += " (%s)" % OS.get_scancode_string(KEY_F)
	zoom_button.hint_tooltip += " (%s)" % OS.get_scancode_string(KEY_R)

func set_tool(new_tool):
	match new_tool:
		Constants.EditorTool.SELECT:
			select_button.pressed = true
		Constants.EditorTool.PLACE:
			place_button.pressed = true
		Constants.EditorTool.DELETE:
			trash_button.pressed = true
		Constants.EditorTool.MOVE:
			move_button.pressed = true


func _on_ResetZoom_pressed():
	viewer.update_zoom()
	camera.set_zoom(Vector2.ONE)


func _on_SelectionButton_toggled(button_pressed):
	if button_pressed:
		edition_mode = Constants.EditorTool.SELECT
		emit_signal("tool_changed", edition_mode)

func _on_PlaceButton_toggled(button_pressed:bool):
	if button_pressed:
		edition_mode = Constants.EditorTool.PLACE
		emit_signal("tool_changed", edition_mode)

func _on_MoveButton_toggled(button_pressed):
	if button_pressed:
		edition_mode = Constants.EditorTool.MOVE
		emit_signal("tool_changed", edition_mode)

func _on_DeleteButton_toggled(button_pressed):
	if button_pressed:
		edition_mode = Constants.EditorTool.DELETE
		emit_signal("tool_changed", edition_mode)

func _input(event: InputEvent):
	if event is InputEventKey:
		match event.physical_scancode:
			KEY_V:
				edition_mode = Constants.EditorTool.SELECT
				select_button.pressed = true
			KEY_T:
				edition_mode = Constants.EditorTool.PLACE
				place_button.pressed = true
			KEY_F:
				edition_mode = Constants.EditorTool.DELETE
				trash_button.pressed = true
			KEY_M:
				edition_mode = Constants.EditorTool.MOVE
				move_button.pressed = true
			KEY_R:
				_on_ResetZoom_pressed()
			_:
				return
