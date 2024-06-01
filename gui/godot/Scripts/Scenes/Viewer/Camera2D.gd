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
onready var toolbar : Node = $"../../../Selector/HBoxContainer"
onready var tilemap : InteractiveTileMap = $"../InteractiveTileMap"


func _input(event: InputEvent) -> void:
	if not tilemap.mouse_inside:
		return
	if (event.is_action("cam_drag") or
		(event is InputEventMouseButton and event.button_index == BUTTON_LEFT
		and toolbar.edition_mode == Constants.EditorTool.MOVE)):
		if event.is_pressed():
			_drag = true
		else:
			_drag = false

	elif event.is_action("scroll_up") and not event.shift: # molette vers le haut
		_update_zoom(-ZOOM_INCREMENT, make_input_local(event).position)
	elif event.is_action("scroll_down") and not event.shift: # molette vers le bas
		_update_zoom(ZOOM_INCREMENT, make_input_local(event).position)

	elif event is InputEventMouseMotion && _drag: # si en cours de drag + la souris a bougé
		var current_offset = get_offset() - event.relative*_current_zoom_level # calcul du nouveau mouvement
		set_offset(_get_restricted_offset(current_offset))
		emit_signal("moved")
	elif event.is_action("kbd_zoom_in"):
		_update_zoom(-ZOOM_INCREMENT, offset)
	elif event.is_action("kbd_zoom_out"):
		_update_zoom(ZOOM_INCREMENT, offset)

func _physics_process(delta):
	var offset = Vector2.ZERO
	if Input.is_physical_key_pressed(KEY_A):
		offset += Vector2.LEFT
	if Input.is_physical_key_pressed(KEY_D):
		offset += Vector2.RIGHT
	if Input.is_physical_key_pressed(KEY_W):
		offset += Vector2.UP
	if Input.is_physical_key_pressed(KEY_S):
		offset += Vector2.DOWN
	if offset != Vector2.ZERO:
		set_offset(_get_restricted_offset(get_offset() + offset*_current_zoom_level*5))
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
