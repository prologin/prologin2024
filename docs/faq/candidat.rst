.. SPDX-License-Identifier: GPL-2.0-or-later
   Copyright 2019 Thibault Allançon
   Copyright 2023 Association Prologin <info@prologin.org>

=================
FAQ Prologin 2024
=================

**Quelques conseils avant de poser une question**

1. Lire le sujet
2. Relire le sujet
3. Lire votre code
4. Relire votre code
5. Contacter un orga

.. TODO: FAQ Sujet

Bug / Question / Technique
==========================

Les questions, rapports de bug et problèmes techniques peuvent être posés aux
organisateurs principalement via l'ouverture d'un ticket.

`Lien pour les tickets <https://bugs.prologin.eu>`_

Erreurs fréquentes
==================

- Ne pas appeler les fonctions de l'API avant l'appel à ``partie_init``.
- **Ne pas modifier** les fichiers générés (``api.*``, ``interface.*``, ...)
    - Attention, ça ne fera pas d'erreurs en local mais cela ne marchera pas sur
      le serveur (donc pour les tournois).
- Si vous faites des fichiers annexes, n'oubliez pas de les mettre en plus dans
  le Makefile après ``champion.*`` dans ``CHAMPIONS_FILES``

Plantage avec stechec
---------------------

1. Lire le message d’erreur de stechec
2. Vérifier que les chemins de vos sources sont les bons dans le ``Makefile`` et
   surtout ``config.yml``.
3. **Tout appel à une fonction de l’API avant partie_init() fera une segfault.**
   Par exemple en C/C++, faire ``int moi = moi();`` en variable globale entraînera
   un segfault.
4. En **Python**, votre ``Champion.py`` doit être dans le dossier dans lequel
   vous lancez stechec.
5. Appelez un.e organisateurice. Ne soyez pas timide, on est aussi là pour vous
   aider. :-)

Plantage avec le serveur de finale (et/ou votre champion fait n’importe quoi sur le serveur)
--------------------------------------------------------------------------------------------

1. Vérifiez, quand vous faîtes ``make tar``, que vos fichiers sont bien inclus
   dans le bidule.tar.
2. Ne modifiez pas les fichiers de l’api (``api.*``, ``prologin.hh``,
   ``prologin.h``, ``interface.*``)
3. Vérifiez qu’il n’y a pas d’ouverture de fichiers dans votre code.
4. Vérifiez que dans votre Makefile, vos fichiers à vous sont bien inclus au bon
   endroit.
5. Appelez un.e organisateurice. Ne soyez pas timide, on est aussi là pour vous
   aider. :-)

Créer ses propres maps
======================

Il est possible de créer vos propre maps en utilisant l'éditeur de map qui vous
est fourni. Celui-ci est accessible en lançant ``prologin2024`` et en cliquant
sur 'Éditeur de cartes'.
