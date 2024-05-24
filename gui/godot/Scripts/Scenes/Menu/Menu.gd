extends Node2D


func _on_ButtonVisualiseurCarte_pressed() -> void:
	Scenes.open_scene(self, Scenes.visualiseur_scene)


func _on_ButtonEditeurCarte_pressed() -> void:
	$EditorPopup.popup_centered()


func _on_ButtonReplay_pressed() -> void:
	$ReplayPopup.popup_centered()


func _ready() -> void:
	if OS.has_feature('JavaScript'):
		var url : String = JavaScript.get_interface("window").location.href
		var path : String = url.rsplit("/", false)[2];
		if path == "matches":
			Scenes.open_scene(self, Scenes.replay_scene)
		elif path == "maps":
			Scenes.open_scene(self, Scenes.visualiseur_scene)
		return
	
	for arg in OS.get_cmdline_args():
		if arg.begins_with("-socket="):
			Scenes.open_scene(self, Scenes.replay_scene)
			return

func _on_Creer_pressed():
	var w = $EditorPopup/VBoxContainer/HBoxContainer/AspectRatioContainer/Largeur.text
	var h = $EditorPopup/VBoxContainer/HBoxContainer/AspectRatioContainer2/Hauteur.text
	var error = (not w.is_valid_integer()) or (not h.is_valid_integer())
	if not error:
		var width = int(w)
		var height = int(h)
		error = width < Constants.LARGEUR_MIN or width > Constants.LARGEUR_MAX or height < Constants.HAUTEUR_MIN or height > Constants.HAUTEUR_MAX
		if not error:
			Context.editor_init_width = width
			Context.editor_init_height = height
			Scenes.open_scene(self, Scenes.editeur_scene)

	if error:
		$EditorPopup/VBoxContainer/Error.show()
		yield(get_tree().create_timer(5), "timeout")
		$EditorPopup/VBoxContainer/Error.hide()


func _on_ReplayPopup_file_selected(path):
	Context.replay_path = path
	Scenes.open_scene(self, Scenes.replay_scene)
