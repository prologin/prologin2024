class_name Models

extends Node

# --- See doc/format_carte_dump.md ---


class Player:
	var identifiant : int = 0
	var score : int = 0

	func copy():
		var p = Player.new()
		p.identifiant = self.identifiant
		p.score = self.score

		return p


class Aigle:
	var identifiant: int = 0
	var joueur: Player
	var pos: Vector2
	var effet: String
	var puissance: int
	var tour_eclosion: int
	var existant: bool

	func copy():
		var aigle = Aigle.new()
		aigle.identifiant = self.identifiant
		aigle.joueur = self.joueur
		aigle.pos = self.pos
		aigle.effet = self.effet
		aigle.puissance = self.puissance
		aigle.tour_eclosion = self.tour_eclosion
		aigle.existant = self.existant
		
		return aigle

	func case_dans_rayon(x, y):
		var x_min = pos.x - self.puissance
		var x_max = pos.x + self.puissance + 1
		var y_min = pos.y - self.puissance
		var y_max = pos.y + self.puissance + 1
		return x >= x_min && x <= x_max && y >= y_min && y <= y_max


class Map:
	var width
	var height
	var carte = []
	var points = []
	var debug = []
	var territoire = []
	var joueurs = []
	var aigles = []
	
	func clear():
		carte.clear()
		points.clear()
		debug.clear()
		joueurs.clear()
		aigles.clear()

	func copy():
		var map = Map.new()
		map.width = width
		map.height = height
		map.carte = []
		for row in self.carte:
			var newrow = []
			for e in row:
				newrow.append(e)
			map.carte.append(newrow)
		map.points = []
		for row in self.points:
			var newrow = []
			for e in row:
				newrow.append(e)
			map.points.append(newrow)
		map.territoire = []
		for row in self.territoire:
			var newrow = []
			for e in row:
				newrow.append(e)
			map.territoire.append(newrow)
		map.debug = []
		for row in self.debug:
			var newrow = []
			for e in row:
				newrow.append(e)
			map.debug.append(newrow)
		map.joueurs = []
		for j in self.joueurs:
			map.joueurs.append(j.copy())
		map.aigles = []
		for a in self.aigles:
			map.aigles.append(a.copy())
		
		return map

	func init(width, height):
		self.width = width
		self.height = height

		self.clear()

		# Cases + score
		for _i in range(height):
			var row = []
			var points_row = []
			var territory_row = []
			for _j in range(width):
				row.append(Constants.TypeCase.NORD_OUEST)
				if _j < width - 1:
					points_row.append((_j + _i) % 2)
					territory_row.append(0)
			self.carte.append(row)
			if _i < height - 1:
				self.points.append(points_row)
				self.territoire.append(territory_row)

		# Villages
		if len(carte) > 0:
			carte[0][0] = Constants.TypeCase.VILLAGE_J1
			carte[-1][-1] = Constants.TypeCase.VILLAGE_J2

		# Players
		var j1 = Player.new()
		j1.identifiant = 1
		var j2 = Player.new()
		j2.identifiant = 2
		self.joueurs = [j1, j2]
	
	func rand_init(width, height):
		self.width = width
		self.height = height

		self.clear()

		#random int for selection
		var x = randi() % 4
		# Cases + score
		for _i in range(height):
			var row = []
			var points_row = []
			for _j in range(width):
				x = randi() % 4
				if (x == 0):
					row.append(Constants.TypeCase.NORD_OUEST)
				if (x == 1):
					row.append(Constants.TypeCase.NORD_EST)
				if (x == 2):
					row.append(Constants.TypeCase.SUD_OUEST)
				if (x == 3):
					row.append(Constants.TypeCase.SUD_EST)
				if _j < width - 1:
					points_row.append(randi() % 10)
			self.carte.append(row)
			if _i < height - 1:
				self.points.append(points_row)

		# Villages
		if len(carte) > 0:
			carte[0][0] = Constants.TypeCase.VILLAGE_J1
			carte[-1][-1] = Constants.TypeCase.VILLAGE_J2

		# Players
		var j1 = Player.new()
		j1.identifiant = 1
		var j2 = Player.new()
		j2.identifiant = 2
		self.joueurs = [j1, j2]

	static func from_json(viewer, json: Dictionary, from_dump = false) -> Map:
		var map = Map.new()

		if json.has("largeur") and json.has("hauteur"):
			map.width = json["largeur"]
			map.height = json["hauteur"]
		else:
			map.width = len(json["carte"][0])
			map.height = len(json["carte"])

		map.carte = []
		for i in range(len(json["carte"])):
			map.carte.append([])
			for j in range(len(json["carte"][i])):
				var tile = viewer.char2case[json["carte"][i][j]]
				map.carte[i].append(tile)

		map.points = json["gains"]
		map.territoire = json.get("territoire", null)
		for aigle in json["aigles"]:
			var a = Aigle.new()
			a.effet = aigle["effet"]
			a.puissance = aigle["puissance"]
			a.tour_eclosion = aigle["tour_eclosion"]
			a.pos = Vector2(aigle["pos"]["x"], aigle["pos"]["y"])
			map.aigles.append(a)

		# Players
		if from_dump:
			for village in json["joueur1"]["villages"]:
				map.carte[village["y"]][village["x"]] = Constants.TypeCase.VILLAGE_J1
			for village in json["joueur2"]["villages"]:
				map.carte[village["y"]][village["x"]] = Constants.TypeCase.VILLAGE_J2
		else:
			map.carte[json["joueur1"]["y"]][json["joueur1"]["x"]] = Constants.TypeCase.VILLAGE_J1
			map.carte[json["joueur2"]["y"]][json["joueur2"]["x"]] = Constants.TypeCase.VILLAGE_J2

		var j1 = Player.new()
		j1.score = json["joueur1"].get("score", 0)
		var j2 = Player.new()
		j2.score = json["joueur2"].get("score", 0)
		map.joueurs = [j1, j2]

		return map

	func to_json(viewer) -> Dictionary:
		var json = {}
		json["largeur"] = self.width
		json["hauteur"] = self.height
		json["gains"] = self.points

		if self.territoire != null:
			json["territoire"] = self.territoire

		var carte = []
		for row in self.carte:
			carte.append("")
			for tile in row:
				if tile in [Constants.TypeCase.VILLAGE_J1, Constants.TypeCase.VILLAGE_J2]:
					tile = Constants.TypeCase.VILLAGE
				carte[-1] += viewer.case2char[tile]
		json["carte"] = carte

		var aigles = []
		for aigle in self.aigles:
			aigles.append({
				"pos": {"x": aigle.pos.x, "y": aigle.pos.y},
				"effet": aigle.effet,
				"puissance": aigle.puissance,
				"tour_eclosion": aigle.tour_eclosion
			})
		json["aigles"] = aigles

		# Players
		for y in len(self.carte):
			for x in len(self.carte[y]):
				if self.carte[y][x] in [Constants.TypeCase.VILLAGE_J1, Constants.TypeCase.VILLAGE_J2]:
					var player_id = "1" if self.carte[y][x] == Constants.TypeCase.VILLAGE_J1 else "2"
					json["joueur" + player_id] = {"x": x, "y": y}

		return json

	func remove_aigle(id):
		for i in len(self.aigles):
			if self.aigles[i].identifiant == id:
				self.aigles.remove(i)
				return true

		return false


class Action:
	var action_type
	var position : Vector2
	var id

	static func from_json(json):
		var act = Action.new()
		act.action_type = json['type']
		act.position = Vector2(json['position']['x'], json['position']['y'])
		if 'id' in json:
			act.id = json['id']
		return act

	func debug():
		return '%s{pos=(%s, %s), id=%s}' % [self.action_type, self.position[0], self.position[1], self.id if self.id != null else 'null']


class Tour:
	var joueur_actuel
	var id_tour
	var fin

	static func from_json(json):
		var tour = Tour.new()
		tour.joueur_actuel = json['joueur_actuel']
		tour.id_tour = json['id_tour']
		tour.fin = json['fin']
		return tour


class GameState:
	var map : Map = null
	var tour : Tour = null
	var actions : Array = []

	static func from_json(viewer, json):
		var state = GameState.new()
		state.map = Map.from_json(viewer, json['jeu'], true)
		for action in json['actions']:
			state.actions.append(Action.from_json(action))
		state.tour = Tour.from_json(json['tour'])
		return state
