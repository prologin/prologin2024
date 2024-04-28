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


static func json_to_map(json: Dictionary) -> Models.Map:
	var map = Models.Map.new()
	map.width = json["largeur"]
	map.height = json["hauteur"]
	map.carte = []
	for i in range(len(json["carte"])):
		map.carte.append([])
		for j in range(len(json["carte"][i])):
			map.carte[i].append(int(json["carte"][i][j]))
	map.points = json["gain"]
	for aigle in json["aigles"]:
		var a = Models.Aigle.new()
		a.effet = aigle["effet"]
		a.puissance = aigle["puissance"]
		a.tour_eclosion = aigle["tour_eclosion"]
		a.pos = Vector2(aigle["pos"]["x"], aigle["pos"]["y"])
		map.aigles.append(a)
	return map


static func map_to_json(map: Models.Map) -> Dictionary:
	var json = {}
	json["largeur"] = map.width
	json["hauteur"] = map.height
	json["carte"] = map.carte
	json["gain"] = map.points
	var aigles = []
	for aigle in map.aigles:
		aigles.append({
			"pos": {"x": aigle.pos.x, "y": aigle.pos.y},
			"effet": aigle.effet,
			"puissance": aigle.puissance,
			"tour_eclosion": aigle.tour_eclosion
		})
	json["aigles"] = aigles
	return json
