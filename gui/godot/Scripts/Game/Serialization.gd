class_name Serialization extends Node


static func read_json(path):
	var file = File.new()
	file.open(path, file.READ)
	var content : String = file.get_as_text()
	file.close()

	return JSON.parse(content).result


static func write_json(json, path):
	var content = JSON.print(json)
	var file = File.new()
	file.open(path, file.WRITE)
	file.store_line(content)
	file.close()


static func read_multiline_json(path):
	var file = File.new()
	file.open(path, file.READ)
	var content = file.get_as_text().strip_edges().split('\n')
	file.close()

	var result = []
	for ln in content:
		result.append(JSON.parse(ln).result)

	return result
