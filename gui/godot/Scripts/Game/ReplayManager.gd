class_name ReplayManager extends Node


# Combines GameState and Action
class ReplayState:
	var map : Models.Map
	var tour : Models.Tour
	var action : Models.Action


# Total turns by the end of the game
var current_state : ReplayState = null
var icurrent_state = 0
var states = []
var total_actions = 0
var viewer = null


func init(viewer):
	self.viewer = viewer


func current_map():
	return current_state.map


# Adds a full game state (state + actions)
func add_game_state(game_state):
	for action in game_state.actions:
		add_action(action, game_state.tour)
	if len(game_state.actions) == 0:
		add_state(game_state)


# Adds a state with an empty action
func add_state(game_state: Models.GameState):
	var state = ReplayState.new()
	state.map = game_state.map.copy()
	state.tour = game_state.tour
	state.action = null
	states.append(state)


# Play an action from the last state
# 1. Set the last state action to `action`
# 2. Duplicate the last state and update apply the action on it
func add_action(action, tour):
	# Set action for the last state
	var last_state = states[-1]
	last_state.action = action

	# Copy last state
	var state = ReplayState.new()
	state.map = last_state.map.copy()
	apply_action(action, state.map, false)
	state.tour = tour
	state.action = action
	states.append(state)


func set_state(i):
	# TODO : Update
	icurrent_state = i
	update_states_actions()


# Next step, a step is an action and can switch state if last action of current state
func next(play_anim = false):
	if icurrent_state == len(states) - 1:
		return false

	icurrent_state += 1
	update_states_actions()
	
	# TODO : Play animation if current_state.action

	return true


func prev(play_anim = false):
	if icurrent_state == 0:
		return false

	# TODO : Play animation if current_state.action

	icurrent_state -= 1

	update_states_actions()

	return true


func update_states_actions():
	current_state = states[icurrent_state]

	var act_debug = 'null'
	if current_state.action:
		act_debug = '(' + current_state.action.debug() + ')'
	print('ReplayManager: current_state=', icurrent_state, ' current_action=', act_debug)


func apply_action(action, map, debug):
	if action == null:
		return

	if debug:
		print('ReplayManager: Apply ', action.debug())

	match action.action_type:
		'action_tourner_case':
			map.carte[action.position[1]][action.position[0]] = viewer.rotate[map.carte[action.position[1]][action.position[0]]]

		'action_deplacer_aigle':
			var aigle = trouver_aigle(action.id, map)
			aigle.pos = action.position

		'action_activer_aigle':
			var aigle: Models.Aigle = trouver_aigle(action.id, map)
			match aigle.effet:
				'VIE':
					map.remove_aigle(aigle.identifiant)
				'MORT':
					for i in range(len(map.aigles) - 1, -1, -1):
						var a = map.aigles[i]
						if a.pos == aigle.pos:
							map.aigles.remove(i)
				'FEU':
					pass
				'GEL':
					pass
				'METEORE':
					tourner_cases(action, map, aigle)
					map.remove_aigle(aigle.identifiant)


# --- Utils ---
# As in backend
func trouver_aigle(id, map) -> Models.Aigle:
	var aigle : Models.Aigle = null
	for a in map.aigles:
		if a.identifiant == id:
			aigle = a
			break

	if not aigle:
		print('[ERROR] Cannot find aigle with id ', id)
		return null
	
	return aigle


func case_bloquee(map, x, y):
	for aigle in map.aigles:
		if aigle.effet == 'GEL' && aigle.case_dans_rayon(x, y):
			return true
	return false


func tourner_cases(action, map, aigle):
	for x in range(aigle.pos.x - aigle.puissance, aigle.pos.x + aigle.puissance + 2):
		for y in range(aigle.pos.y - aigle.puissance, aigle.pos.y + aigle.puissance + 2):
			if map.carte[y][x] in viewer.rotatable:
				for i in 2:
					if not case_bloquee(map, x, y):
						map.carte[y][x] = viewer.rotate[map.carte[y][x]]
