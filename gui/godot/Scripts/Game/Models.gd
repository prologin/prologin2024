class_name Models

extends Node

# --- See doc/format_carte_dump.md ---

class Player:
	var identifiant : int = 0
	var scores
	var actions


class Aigle:
	 var identifiant: int = 0
	 var joueur: Player
	 var pos: Vector2
	 var effet: String
	 var puissance: int
	 var tour_eclosion: int
	 var existant: bool


class Map:
	var width
	var height
	var carte = []
	var points = []
	var debug = []
	var joueurs = []
	var aigles = []
	
	func clear():
		carte.clear()
		points.clear()
		debug.clear()
		joueurs.clear()
		aigles.clear()
		
	func init(width, height):
		self.width = width
		self.height = height

		self.clear()

		# Cases + score
		for _i in range(height):
			var row = []
			var points_row = []
			for _j in range(width):
				row.append(Constants.TypeCase.NORD_OUEST)
				if _j < width - 1:
					points_row.append((_j + _i) % 2)
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

	
	# TODO
	# func copy() -> models.Map:
	#	var map := Map.new()
	#	...
	#	return map


class Action:
	var action_type
	var debut
	var fin
	var identifiant_aigle


class Tour:
	var joueur_actuel
	var id_tour
	var fin


class GameState:
	var map : Map = null
	var tour : Tour = null
