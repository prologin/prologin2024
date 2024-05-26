extends Camera2D

const MAX_ZOOM_LEVEL = 0.3
const MIN_ZOOM_LEVEL = 2
const ZOOM_INCREMENT = 0.05

signal moved()
signal zoomed()

var _current_zoom_level = 1
var _drag = false

onready var viewer : Viewer = $"../../../../Viewer"
onready var viewport : ViewportContainer = $"../../../ViewportContainer"


func _input(event: InputEvent) -> void:
	if event.is_action_pressed("cam_drag"): # click molette
		_drag = true
	elif event.is_action_released("cam_drag"): # lacher de molette
		_drag = false

	elif event.is_action("cam_zoom_in"): # molette vers le haut
		_update_zoom(-ZOOM_INCREMENT, make_input_local(event).position)
	elif event.is_action("cam_zoom_out"): # molette vers le bas
		_update_zoom(ZOOM_INCREMENT, make_input_local(event).position)

	elif event is InputEventMouseMotion && _drag: # si en cours de drag + la souris a bougé
		var current_offset = get_offset() - event.relative*_current_zoom_level # calcul du nouveau mouvement
		set_offset(_get_restricted_offset(current_offset))
		emit_signal("moved")


# limite le mouvement à une certaine plage
func _get_restricted_offset(current_offset: Vector2) -> Vector2:
	var w = viewer.map.width * Constants.TILE_SIZE
	var h = viewer.map.height * Constants.TILE_SIZE

	current_offset.x = clamp(current_offset.x, 0, w)
	current_offset.y = clamp(current_offset.y, 0, h)

	return current_offset


func _update_zoom(incr: float, zoom_anchor: Vector2) -> void:
	var old_zoom = _current_zoom_level

	var max_zoom = MAX_ZOOM_LEVEL * viewport.rect_scale[0]
	_current_zoom_level = clamp(_current_zoom_level+incr, max_zoom, MIN_ZOOM_LEVEL)

	if old_zoom == _current_zoom_level:
		return

	var zoom_center = zoom_anchor - get_offset()
	var ratio = 1-_current_zoom_level/old_zoom
	set_offset(_get_restricted_offset(get_offset() + zoom_center*ratio))

	set_zoom(Vector2(_current_zoom_level, _current_zoom_level))
	emit_signal("zoomed")
