class_name Validator extends Node


# Returns null on OK
# Otherwise returns a string corresponding to the error
static func validate(map: Models.Map):
	# Map
	var largeur = map.width
	if largeur < Constants.LARGEUR_MIN or largeur > Constants.LARGEUR_MAX:
		return "Largeur incorrecte: %s n'est pas dans l'intervalle [%s, %s]" % [largeur, Constants.LARGEUR_MIN, Constants.LARGEUR_MAX]

	var hauteur = map.height
	if hauteur < Constants.HAUTEUR_MIN or hauteur > Constants.HAUTEUR_MAX:
		return "Hauteur incorrecte: %s n'est pas dans l'intervalle [%s, %s]" % [hauteur, Constants.HAUTEUR_MIN, Constants.HAUTEUR_MAX]

	# Players
	if len(map.joueurs) != 2:
		return "Deux joueurs doivent être présents (%s joueurs trouvés)" % len(map.joueurs)

	for ijoueur in len(map.joueurs):
		var joueur = map.joueurs[ijoueur]
		var village = Constants.TypeCase.VILLAGE_J1 if joueur.identifiant == 1 else Constants.TypeCase.VILLAGE_J2
		var nvillages = 0
		for row in map.carte:
			for tile in row:
				if tile == village:
					nvillages += 1
		if nvillages != 1:
			return "Le joueur %s possède %s villages au lieu d'1 seul" % [joueur.identifiant, nvillages]

	# Points
	for i in map.height - 1:
		for j in map.width - 1:
			if map.points[i][j] < Constants.POINTS_MIN or map.points[i][j] > Constants.POINTS_MAX:
				return "Le nombre de points apportés par l'emplacement ligne %s colonne %s (%s) devrait être compris entre %s et %s" % [i, j, map.points[i][j], Constants.POINTS_MIN, Constants.POINTS_MAX]

	for aigle in map.aigles:
		if aigle.pos.x < 0 or aigle.pos.x >= largeur - 1 or aigle.pos.y < 0 or aigle.pos.y >= hauteur - 1:
			return "L'aigle %s (identifiant %s) en position %s, %s n'est pas sur un emplacement valide" % [aigle.effet, aigle.identifiant, aigle.pos.x, aigle.pos.y]

		if aigle.tour_eclosion < 0 or aigle.tour_eclosion >= Constants.NB_TOURS:
			return "L'éclosion prévue tour %s de l'aigle %s (identifiant %s) est en dehors de la durée de la partie" % [aigle.tour_eclosion, aigle.effet, aigle.identifiant]


# --- Tests ---
static func tester(testname, err):
	if err:
		print('[OK] ', testname, ' (', err, ')')
		return false

	print('[ERROR] ', testname, ' does not fail')

	return true


static func test_validator():
	var sample_map = Models.Map.new()
	sample_map.rand_init(10, 10)
	var aigle = Models.Aigle.new()
	aigle.effet = 'FEU'
	aigle.pos = Vector2(1, 2)
	sample_map.aigles.append(aigle)
	
	var err_dim = sample_map.copy()
	err_dim.init(2, 300)
	if tester('err_dim', validate(err_dim)):
		return

	var err_players = sample_map.copy()
	err_players.joueurs.append(null)
	if tester('err_players', validate(err_players)):
		return

	var err_players2 = sample_map.copy()
	err_players2.carte[2][2] = Constants.TypeCase.VILLAGE_J1
	if tester('err_players2', validate(err_players2)):
		return

	var err_score = sample_map.copy()
	err_score.points[2][2] = Constants.POINTS_MAX + 42
	if tester('err_score', validate(err_score)):
		return

	var err_aigles = sample_map.copy()
	err_aigles.aigles[0].pos.x = 42
	if tester('err_aigles', validate(err_aigles)):
		return
