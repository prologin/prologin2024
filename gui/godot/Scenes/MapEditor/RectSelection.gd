class_name RectSelection extends ColorRect

onready var border = $"../RectBorder"

var start_pos = null
var end_pos = null
var border_start_pos = null
var border_end_pos = null

var dragging = false
var enabled = false

func disable():
	dragging = false
	enabled = false
	border.hide()
	hide()

func handle_mouse_event(event):
	assert(event is InputEventMouseButton)
	
	if event.button_index != 1:  # button left
		disable()
		return
	
	if event.pressed:
		disable()
		start_pos = event.position
		end_pos = event.position
		border_start_pos = event.position
		border_end_pos = event.position
		dragging = true
	elif dragging:
		var pos = translate_positions()
		var size = Vector2(pos[1].x - pos[0].x, pos[1].y - pos[0].y)
		var threshold = Constants.TILE_SIZE
		if dragging and size.x > threshold and size.y > threshold:
			dragging = false
			enabled = true
			update_position(event.position)
			fit_tiles(true)
			if end_pos.x - start_pos.x <= 0 or end_pos.y - start_pos.y <= 0:
				disable()
				return
			border.show()
			update_display()
		else:
			disable()


func handle_move_event(event):
	assert(event is InputEventMouseMotion)
	
	if not dragging:
		return
	
	update_position(event.position)
	update_display()


func update_position(position):
	end_pos = position
	fit_tiles(false)

func update_display():
	show()
	border.show()
	
	var pos = normal_positions()
	self.rect_position = pos[0]
	self.rect_size = Vector2(pos[1].x - pos[0].x, pos[1].y - pos[0].y)
	
	border.rect_position = border_start_pos
	border.rect_size = Vector2(
		border_end_pos.x - border_start_pos.x,
		border_end_pos.y - border_start_pos.y
	)
	
	self.update()
	border.update()

func normal_positions():
	var start = Vector2(
		min(start_pos.x, end_pos.x),
		min(start_pos.y, end_pos.y)
	)
	var end = Vector2(
		max(start_pos.x, end_pos.x),
		max(start_pos.y, end_pos.y)
	)
	return [start, end]

func translate_positions():
	var viewer = get_tree().current_scene.find_node("Viewer")
	var pos = normal_positions()
	var scale = viewer.container.rect_scale.x
	var zoom_level = viewer.camera._current_zoom_level
	
	var factor = zoom_level / scale
	
	var viewport_size = Vector2(viewer.map.width, viewer.map.height) * Constants.TILE_SIZE
	var offset = (Vector2(viewer.original_res[0], viewer.original_res[1]) - viewport_size / factor) / 2 * factor
	offset += viewport_size / 2 - viewer.camera.offset
	offset.y += 30 * factor
	
	var start = pos[0] * factor - offset
	var end = pos[1] * factor - offset
	
	start.x = clamp(start.x, 0, Constants.TILE_SIZE * viewer.map.width)
	start.y = clamp(start.y, 0, Constants.TILE_SIZE * viewer.map.height)
	end.x = clamp(end.x, 0, Constants.TILE_SIZE * viewer.map.width)
	end.y = clamp(end.y, 0, Constants.TILE_SIZE * viewer.map.height)
	
	return [start, end]


func inverse_transform(position):
	var viewer = get_tree().current_scene.find_node("Viewer")
	
	var scale = viewer.container.rect_scale.x
	var zoom_level = viewer.camera._current_zoom_level
	var factor = zoom_level / scale
	
	var viewport_size = Vector2(viewer.map.width, viewer.map.height) * Constants.TILE_SIZE
	var offset = (Vector2(viewer.original_res[0], viewer.original_res[1]) - viewport_size / factor) / 2 * factor
	offset += viewport_size / 2 - viewer.camera.offset
	offset.y += 30 * factor
	
	return (position + offset) / factor


func fit_tiles(all: bool):
	var pos = translate_positions()
	var start = Vector2(
		floor((pos[0].x + Constants.TILE_SIZE - 1) / Constants.TILE_SIZE) * Constants.TILE_SIZE,
		floor((pos[0].y + Constants.TILE_SIZE - 1) / Constants.TILE_SIZE) * Constants.TILE_SIZE
	)
	var end = Vector2(
		floor(pos[1].x / Constants.TILE_SIZE) * Constants.TILE_SIZE - 1,
		floor(pos[1].y / Constants.TILE_SIZE) * Constants.TILE_SIZE - 1
	)
	
	var new_start = inverse_transform(start)
	var new_end = inverse_transform(end)
	
	if all:
		start_pos = new_start
		end_pos = new_end
		
	border_start_pos = new_start
	border_end_pos = new_end


func get_selected_tiles(emplacements: bool = false):
	var pos = translate_positions()
	var start = Vector2(
		floor((pos[0].x + Constants.TILE_SIZE - 1) / Constants.TILE_SIZE),
		floor((pos[0].y + Constants.TILE_SIZE - 1) / Constants.TILE_SIZE)
	)
	var end = Vector2(
		floor(pos[1].x / Constants.TILE_SIZE),
		floor(pos[1].y / Constants.TILE_SIZE)
	)
	if not emplacements:
		end.x += 1
		end.y += 1
	
	var positions = []
	for x in range(start.x, end.x):
		for y in range(start.y, end.y):
			positions.append(Vector2(x, y))
	
	return positions
