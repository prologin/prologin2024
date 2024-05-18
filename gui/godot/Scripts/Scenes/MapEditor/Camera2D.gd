extends Camera2D

const MAX_ZOOM_LEVEL = 0.5
const MIN_ZOOM_LEVEL = 1.5
const ZOOM_INCREMENT = 0.05
const MIN_OFFSET = -700
const MAX_OFFSET = 700

signal moved()
signal zoomed()

var _current_zoom_level = 1
var _drag = false

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("cam_drag"): # click molette
		_drag = true
	elif event.is_action_released("cam_drag"): # lacher de molette
		_drag = false

	elif event.is_action("cam_zoom_in"): # molette vers le haut
		_update_zoom(-ZOOM_INCREMENT, get_local_mouse_position())
	elif event.is_action("cam_zoom_out"): # molette vers le bas
		_update_zoom(ZOOM_INCREMENT, get_local_mouse_position())

	elif event is InputEventMouseMotion && _drag: # si en cours de drag + la souris a bougé
		var current_offset = get_offset() - event.relative*_current_zoom_level # calcul du nouveau mouvement
		set_offset((current_offset))
		emit_signal("moved")


# limite le mouvement à une certaine plage
func _get_restricted_offset(current_offset: Vector2) -> Vector2:
	if current_offset.x < MIN_OFFSET:
		current_offset.x = MIN_OFFSET
	if current_offset.y < MIN_OFFSET:
		current_offset.y = MIN_OFFSET
	if current_offset.x > MAX_OFFSET:
		current_offset.x = MAX_OFFSET
	if current_offset.y > MAX_OFFSET:
		current_offset.y = MAX_OFFSET
	return current_offset


func _update_zoom(incr: float, zoom_anchor: Vector2) -> void:
	var old_zoom = _current_zoom_level

	_current_zoom_level += incr
	if _current_zoom_level < MAX_ZOOM_LEVEL:
		_current_zoom_level = MAX_ZOOM_LEVEL
	elif _current_zoom_level > MIN_ZOOM_LEVEL:
		_current_zoom_level = MIN_ZOOM_LEVEL

	if old_zoom == _current_zoom_level:
		return

	var zoom_center = zoom_anchor - get_offset()
	var ratio = 1-_current_zoom_level/old_zoom
	set_offset(get_offset() + zoom_center*ratio)

	set_zoom(Vector2(_current_zoom_level, _current_zoom_level))
	emit_signal("zoomed")
