.. SPDX-License-Identifier: GPL-2.0-or-later
   Copyright 2024 Association Prologin <info@prologin.org>

Résumé des commandes pour les débogueurs
========================================

C/C++
-----

Le debogueur utilisé pour les langages C et C++ s’appelle gdb. Il permet
d’exécuter votre code pas à pas et de vérifier à chaque étape l’état de vos
variables et de votre environnement.

Compiler du code pour utiliser ``gdb``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour pouvoir lancer ``gdb`` avec stechec2 il faut rajouter en option ``-d 1``.
Par exemple :

::

   stechec2-run -d 1 config.yml

Le chiffre après le ``-d`` permet de spécifier le joueur que l’on veut deboguer
(dans l’ordre de votre *config.yml*).

Liste des commandes utiles de ``gdb``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Voici un résumé des commandes de base à utiliser dans gdb :

+-----------------------+-----------------------+-----------------------+
| Action                | Commande entière      | Raccourci             |
+=======================+=======================+=======================+
| Afficher l’aide       | help                  | h                     |
+-----------------------+-----------------------+-----------------------+
| Placer un point       | breakpoint            | b                     |
| d’arrêt               |                       |                       |
+-----------------------+-----------------------+-----------------------+
| Avancer d’un pas (en  | step                  | s                     |
| entrant dans la       |                       |                       |
| fonction si possible) |                       |                       |
+-----------------------+-----------------------+-----------------------+
| Avancer sans entrer   | next                  | n                     |
| dans l’instruction    |                       |                       |
+-----------------------+-----------------------+-----------------------+
| Avancer jusqu’au      | continue              | c                     |
| prochain point        |                       |                       |
| d’arrêt               |                       |                       |
+-----------------------+-----------------------+-----------------------+
| Afficher le contenu   | print                 | p                     |
| d’une variable        | *nom_de_variable*     |                       |
+-----------------------+-----------------------+-----------------------+
| Lancer l’exécution du | run                   | r                     |
| programme             |                       |                       |
+-----------------------+-----------------------+-----------------------+
| Lancer le programme   | start                 |                       |
| en s’arrêtant à la    |                       |                       |
| première instruction  |                       |                       |
+-----------------------+-----------------------+-----------------------+
| Lister les points     | info breakpoints      | info b                |
| d’arrêt               |                       |                       |
+-----------------------+-----------------------+-----------------------+
| Supprimer un point    | delete *numéro de     | d *numéro*            |
| d’arrêt               | point d’arrêt*        |                       |
+-----------------------+-----------------------+-----------------------+
| Arrêter l’exécution   | kill                  | k                     |
| du programme          |                       |                       |
+-----------------------+-----------------------+-----------------------+
| Quitter gdb           | quit                  | Ctrl+d                |
+-----------------------+-----------------------+-----------------------+

Pour aller plus loin
~~~~~~~~~~~~~~~~~~~~

Pour pouvoir utiliser ``gdb`` en dehors du cadre du concours prologin, vous
devez compiler votre code avec l’option ``-g``, par exemple :

::

   g++ monCode.cc -o monProgramme -g

Cela permettra de rajouter des informations dans le binaire pour pouvoir faire
l’exécution pas à pas.

**N’utilisez pas cette option en dehors du cadre du débogage, car cela alourdit
votre programme inutilement !**

Cette option est déjà activée par stechec2.

Python
------

Le debogueur utilisé pour le langage Python s’appelle ``pdb``. Il permet de
s’arrêter à un point donné dans votre programme et d’analyser son exécution.

Pour placer un point d’arrêt (un breakpoint) dans votre code, ajoutez un appel à
la fonction ``breakpoint()``.

Liste des commandes utiles de ``pdb``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Voici un résumé des commandes de base à utiliser dans ``pdb`` :

+-----------------------+-----------------------+-----------------------+
| Action                | Commande              | Raccourci             |
+=======================+=======================+=======================+
| Afficher l’aide       | h                     |                       |
+-----------------------+-----------------------+-----------------------+
| Afficher la fonction  | longlist              | ll                    |
| en entier             |                       |                       |
+-----------------------+-----------------------+-----------------------+
| Afficher 10 lignes de | list .                | l .                   |
| code                  |                       |                       |
+-----------------------+-----------------------+-----------------------+
| Placer un point       | break *numéro de la   | b *numéro de la       |
| d’arrêt               | ligne*                | ligne*                |
+-----------------------+-----------------------+-----------------------+
| Avancer d’un pas (en  | step                  | s                     |
| entrant dans la       |                       |                       |
| fonction si possible) |                       |                       |
+-----------------------+-----------------------+-----------------------+
| Avancer sans entrer   | next                  | n                     |
| dans l’instruction    |                       |                       |
+-----------------------+-----------------------+-----------------------+
| Avancer jusqu’au      | continue              | c                     |
| prochain point        |                       |                       |
| d’arrêt               |                       |                       |
+-----------------------+-----------------------+-----------------------+
| Afficher le contenu   | print                 | p                     |
| d’une variable        | *nom_de_variable*     |                       |
+-----------------------+-----------------------+-----------------------+
| Ouvrir une ligne de   | interact              |                       |
| commande interactive  |                       |                       |
+-----------------------+-----------------------+-----------------------+
| Supprimer un point    | cl *numéro du point   |                       |
| d’arrêt               | d’arrêt*              |                       |
+-----------------------+-----------------------+-----------------------+
| Quitter l’exécution   | Ctrl+c                |                       |
+-----------------------+-----------------------+-----------------------+
