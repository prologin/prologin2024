======
 Unix
======

.. tip::
   Le signe ``$`` correspond au prompt de votre shell.

**Lister les dossiers et les fichiers du dossier courant ou d’un dossier donné**

::

   $ ls
   $ ls mon_dossier

**Se déplacer dans un dossier**

::

   $ cd mon_dossier

**Créer un dossier**

::

   $ mkdir perdu

**Créer un fichier**

::

   $ touch prologin.php

**Copier un fichier vers un dossier**

::

   $ cp prologin.php perdu/

**Déplacer un fichier vers un dossier**

::

   $ mv prologin.php perdu/

**Renomer un ficher**

::

   $ mv 41.py 42.py

**Supprimer un dossier. L’option supprime -r récursivement.**

::

   $ rm test.py
   $ rm -rf perdu/

.. warning::
   Cette opération est irréversible

**Changer le mapping du clavier**

::

   $ setxkbmap fr
   $ setxkbmap us
