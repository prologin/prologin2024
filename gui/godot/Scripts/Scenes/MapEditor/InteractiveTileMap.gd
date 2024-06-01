class_name InteractiveTileMap

extends TileMap

# Tilemap we can interact with (click / drag)

signal click(pos, button)
signal drag(pos)

var mouse_pressed = false
var mouse_inside = false


func translate_pos(event: InputEvent) -> Vector2:
	var mouse_pos = self.make_input_local(event).position
	# Pos within the map that contains tile quarters
	var map_pos = self.world_to_map(mouse_pos)
	return map_pos


func _input(event: InputEvent):
	if not mouse_inside:
		return

	if event is InputEventMouseMotion and mouse_pressed:
		var pos = translate_pos(event)
		emit_signal("drag", pos)

	if event is InputEventMouseButton:
		if event.button_index != BUTTON_LEFT:
			return
		mouse_pressed = event.pressed

		if event.pressed:
			var pos = translate_pos(event)
			emit_signal("click", pos, event.button_index)


func _on_ViewportContainer_mouse_entered():
	mouse_inside = true


func _on_ViewportContainer_mouse_exited():
	mouse_inside = false
