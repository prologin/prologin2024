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


static func json_to_map(viewer: Viewer, json: Dictionary) -> Models.Map:
	var map = Models.Map.new()

	map.width = json["largeur"]
	map.height = json["hauteur"]

	map.carte = []
	for i in range(len(json["carte"])):
		map.carte.append([])
		for j in range(len(json["carte"][i])):
			var tile = viewer.char2case[json["carte"][i][j]]
			map.carte[i].append(tile)

	map.points = json["gains"]
	for aigle in json["aigles"]:
		var a = Models.Aigle.new()
		a.effet = aigle["effet"]
		a.puissance = aigle["puissance"]
		a.tour_eclosion = aigle["tour_eclosion"]
		a.pos = Vector2(aigle["pos"]["x"], aigle["pos"]["y"])
		map.aigles.append(a)

	# Players
	map.carte[json["joueur1"]["y"]][json["joueur1"]["x"]] = Constants.TypeCase.VILLAGE_J1
	map.carte[json["joueur2"]["y"]][json["joueur2"]["x"]] = Constants.TypeCase.VILLAGE_J2

	return map


static func map_to_json(viewer: Viewer, map: Models.Map) -> Dictionary:
	var json = {}
	json["largeur"] = map.width
	json["hauteur"] = map.height
	json["gain"] = map.points

	var carte = []
	for row in carte.map:
		carte.append("")
		for tile in row:
			if tile in [Constants.TypeCase.VILLAGE_J1, Constants.TypeCase.VILLAGE_J2]:
				tile = Constants.TypeCase.VILLAGE
			carte[-1] += viewer.case2char[tile]
	json["carte"] = carte

	var aigles = []
	for aigle in map.aigles:
		aigles.append({
			"pos": {"x": aigle.pos.x, "y": aigle.pos.y},
			"effet": aigle.effet,
			"puissance": aigle.puissance,
			"tour_eclosion": aigle.tour_eclosion
		})
	json["aigles"] = aigles

	# Players
	for y in len(map.carte):
		for x in len(map.carte[y]):
			if map.carte[y][x] in [Constants.TypeCase.VILLAGE_J1, Constants.TypeCase.VILLAGE_J2]:
				var player_id = "1" if map.carte[y][x] == Constants.TypeCase.VILLAGE_J1 else "2"
				json["joueur_" + player_id] = {"x": x, "y": y}

	return json
