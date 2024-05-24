extends Camera2D

const MAX_ZOOM_LEVEL = 0.3
const MIN_ZOOM_LEVEL = 1
const ZOOM_INCREMENT = 0.05

signal moved()
signal zoomed()

var _current_zoom_level = 1
var _drag = false


func _ready():
	update_zoom()


func update_zoom():
	offset.x = self.get_viewport_rect().size[0] / 2
	offset.y = self.get_viewport_rect().size[1] / 2


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
		set_offset(_get_restricted_offset(current_offset))
		emit_signal("moved")


# limite le mouvement à une certaine plage
func _get_restricted_offset(current_offset: Vector2) -> Vector2:
	var w = self.get_viewport_rect().size.x
	var h = self.get_viewport_rect().size.y

	current_offset.x = clamp(current_offset.x, w / 2 * _current_zoom_level, w - w / 2 * _current_zoom_level)
	current_offset.y = clamp(current_offset.y, h / 2 * _current_zoom_level, h - h / 2 * _current_zoom_level)

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
	set_offset(_get_restricted_offset(get_offset() + zoom_center*ratio))

	set_zoom(Vector2(_current_zoom_level, _current_zoom_level))
	emit_signal("zoomed")
