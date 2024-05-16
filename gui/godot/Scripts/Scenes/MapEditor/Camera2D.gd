extends Camera2D

signal area_selected
signal atart_move_selection

export var SPEED: float = 30.0
export var ZOOM_SPEED: float = 10.0
export var ZOOM_MARGIN: float = 0.1
export var ZOOM_MIN: float = 0.5
export var ZOOM_MAX: float = 3.0

var zoomFactor: float = 1.0
var zoomPos: Vector2 = Vector2()
var zooming: bool = false

var mousePos: Vector2 = Vector2()
var mousePosGlobal: Vector2 = Vector2()
var start: Vector2 = Vector2()
var startV: Vector2 = Vector2()
var end: Vector2 = Vector2()
var endV: Vector2 = Vector2()
var isDragging: bool = false

func _ready():
	update_zoom()


func update_zoom():
	offset.x = self.get_viewport_rect().size[0] / 2
	offset.y = self.get_viewport_rect().size[1] / 2


func _process(delta):
	zoom_camera(delta)
	move_camera(delta)


func zoom_camera(delta: float) -> void:
	zoom.x = lerp(zoom.x, zoom.x * zoomFactor, ZOOM_SPEED * delta)
	zoom.y = lerp(zoom.y, zoom.y * zoomFactor, ZOOM_SPEED * delta)

	zoom.x = clamp(zoom.x, ZOOM_MIN, ZOOM_MAX)
	zoom.y = clamp(zoom.y, ZOOM_MIN, ZOOM_MAX)
	if not zooming:
		zoomFactor = 1.0


func move_camera(delta: float) -> void:
	var inputX: int = get_input_x()
	var inputY: int = get_input_y()
	
	offset.x = lerp(offset.x, offset.x + inputX * SPEED * zoom.x, SPEED * delta)
	offset.y = lerp(offset.y, offset.y + inputY * SPEED * zoom.y, SPEED * delta)


func get_input_x() -> int:
	return int(Input.is_action_pressed("ui_right")) - int(Input.is_action_pressed("ui_left"))
	
	
func get_input_y() -> int:
	return int(Input.is_action_pressed("ui_down")) - int(Input.is_action_pressed("ui_up"))
	

func _input(event: InputEvent) -> void:
	input_for_zoom(event)


func input_for_zoom(event: InputEvent) -> void:
	if abs(zoomPos.x - get_global_mouse_position().x) > ZOOM_MARGIN:
		zoomFactor = 1.0
	if abs(zoomPos.y - get_global_mouse_position().y) > ZOOM_MARGIN:
		zoomFactor = 1.0
		
	if event is InputEventMouseButton:
		if event.is_pressed():
			zooming = true
			if event.is_action("WheelDown"):
				zoomFactor -= 0.01 * ZOOM_SPEED
				zoomPos = get_global_mouse_position()
			if event.is_action("WheelUp"):
				zoomFactor += 0.01 * ZOOM_SPEED
				zoomPos = get_global_mouse_position()
		else:
			zooming = true

