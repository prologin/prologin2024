class_name ToolTipLabel
extends Label


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var max_width : int
var max_height : int
var aigle_pos : Vector2

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass

func set_properties(max_w, max_h, aigle_p):
	max_width = max_w
	max_height = max_h
	aigle_pos = aigle_p


func update_pos():
	if Constants.TILE_SIZE * aigle_pos.x + Constants.HALF_TILE_SIZE + rect_size.x + Constants.TILE_SIZE > max_width:
		rect_global_position.x = Constants.TILE_SIZE * aigle_pos.x + Constants.HALF_TILE_SIZE - rect_size.x
			
	if rect_global_position.y + rect_size.y > max_height:
		rect_global_position.y = max_height - rect_size.y
		

func _on_ToolTip_mouse_entered():
	update_pos()
	self_modulate.a = 1


func _on_ToolTip_mouse_exited():
	self_modulate.a = 0
