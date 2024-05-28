# Singleton

extends Node

onready var default_size = OS.get_window_size()
onready var current_size = OS.get_window_size()
onready var previous_size = current_size

var ignore_x = false
var ignore_y = false


func _process(delta):
	current_size = OS.get_window_size()
	
	var next_ignore_x = false
	var next_ignore_y = false
	
	if current_size.y != previous_size.y && not ignore_y:
		var target = Vector2(default_size.x * (current_size.y / default_size.y), current_size.y)
		# Make the resize smoother when both directions change - let the y axis handle the rest
		current_size = current_size.linear_interpolate(target, 0.6)
		# Update the window as soon as possible to not see the black strips at all
		OS.set_window_size(current_size)
		next_ignore_x = true
	elif current_size.y == previous_size.y && ignore_y:
		next_ignore_y = false
		
	if current_size.x != previous_size.x && not ignore_x:
		current_size.y = default_size.y * (current_size.x / default_size.x)
		OS.set_window_size(current_size)
		next_ignore_y = true
	elif current_size.x == previous_size.x && ignore_x:
		next_ignore_x = false
		
	ignore_x = next_ignore_x
	ignore_y = next_ignore_y
	
	# Prevent the window from shrinking to much
	if current_size < default_size / 3:
		OS.set_window_size(default_size / 3)
		
	previous_size = current_size
