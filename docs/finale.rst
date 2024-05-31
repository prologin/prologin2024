===========================
 Champions - Jeu de finale
===========================

Écrire un champion
------------------

Dans ton dossier ``afs``, exécute la commande suivante :

.. code-block:: sh

    $ prologin2024-player-environment champions

Un dossier ``champions`` est alors généré. Il contient des sous-dossiers
correspondants aux langages disponibles. Tu peux alors écrire un champion dans
le langage de ton choix.

Par exemple, pour écrire un champion en Python, déplace-toi dans le dossier
``python`` et ouvre ``Champion.py``. La fonction ``partie_init`` contient est
exécutée en début de partie, ``partie_fin`` est exécutée en fin de partie et
``joueur_tour`` à chaque tour.

.. warning::

    Le support de JavaScript étant incomplet, les champions écrit dans ce
    langage ne seront pas supportés.

Compiler un champion
--------------------

Dans le dossier contenant ton champion tu peux le compiler avec la commande suivante :

.. code-block:: sh

    $ make

Le fichier ``champion.so`` ainsi généré est nécessaire pour lancer une partie,
comme détaillé dans la section suivante (:ref:`Launch game`).

.. tip::

    Si ton champion est écrit dans un autre langage que C ou C++, il peut être
    nécessaire d'exécuter ``make -B``.

Pour publier ton champion, éxecute ``make tar`` et dépose le fichier
``champion.tgz`` sur le `site de finale <https://finale.prologin.eu/champions/new/>`_.

.. _Launch game:

Créer une partie avec deux champions
------------------------------------

Pour pouvoir lancer une partie entre deux champions, il faut créer un fichier
YAML de configuration pour stechec2, le programme jouant la partie.

Ce fichier doit être rempli comme suit :

.. code-block:: yaml
    :caption: config.yml

    rules: libprologin2024.so
    verbose: 3
    clients:
        - ./champion.so
        - /path/to/other/champion.so
    names:
        - Player 1
        - Player 2

où :

* ``clients`` est la liste des champions que tu veux voir jouer
* ``names`` comporte les noms des deux joueurs

Pour lancer la partie, entre la commande suivante dans un terminal :

.. code:: bash

    $ stechec2-run config.yml
