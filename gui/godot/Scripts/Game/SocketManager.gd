class_name SocketManager extends Node

static func init_socket(port: int) -> bool:
	print("Trying to establish connection on port " + str(port))
	Context.socket = StreamPeerTCP.new()
	while true:
		var connected = Context.socket.connect_to_host("127.0.0.1", port)
		if connected == OK:
			print("Connected")
			return true
			
		OS.delay_msec(500)
		continue
		
	return false  # Godot is dumb and cannot understand that this is unreachable...


static func ack():
	var socket = Context.socket
	socket.put_utf8_string("NEXT")


static func step(handler: FuncRef):
	# nop if there is nothing to read
	if Context.is_done:
		return
	
	var socket = Context.socket
	
	var json_dump = null
	var raw = ""
	var available = 0
	while json_dump == null:
		while available <= 0:
			available = socket.get_available_bytes()
		
		# get_available_bytes is called again on purpose
		# - sometimes the values gets updated in between.
		raw += socket.get_string(socket.get_available_bytes())
		json_dump = JSON.parse(raw).result
	
	handler.call_func(json_dump)
	Context.is_done = json_dump["tour"]["fin"]
	
	ack()
