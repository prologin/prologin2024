class_name Serialization extends Node


static func read_json(path):
	var file = File.new()
	file.open(path, file.READ)
	var content : String = file.get_as_text(true)
	file.close()

	return JSON.parse(content).result


static func write_json(json, path):
	var content = JSON.print(json, "\t")
	var file = File.new()
	file.open(path, file.WRITE)
	file.store_line(content)
	file.close()


static func json_to_map(json) -> Models.Map:
	# TODO
	var map = Models.Map.new()
	return map


static func map_to_json(map: Models.Map):
	var json = {}
	# TODO
	return json
