using System;
using Internal;
using System.Collections.Generic;

namespace Champion
{
    class Champion
    {

        private class Tuple
        {
            public Tuple()
            {
                value_ = 0;
            }

            public Tuple(EtatCase ec)
            {
                value_ = ec.Points;
                path_.Add(ec);
            }

            public Tuple add(EtatCase ec)
            {
                value_ += ec.Points;
                path_.Add(ec);
                return this;
            }


            public int value_ = 0;
            public List<EtatCase> path_ = new List<EtatCase>();
        }

        List<EtatCase> targets = new List<EtatCase>();
        List<EtatCase> in_way = new List<EtatCase>();
        List<EtatCase> past = new List<EtatCase>();

        private Position newPosition(int Ligne, int Colonne)
        {
            Position tmp = new Position();
            tmp.Ligne = Ligne;
            tmp.Colonne = Colonne;
            return tmp;
        }

        private Position Pos_add(Position p1, Position p2)
        {
            return newPosition(p1.Ligne + p2.Ligne, p1.Colonne + p2.Colonne);
        }

        private Tuple short_path(EtatCase src, EtatCase dst)
        {
            if (src.PosCase.Equals(dst.PosCase))
                return new Tuple(src);
            Tuple result = new Tuple();
            if (src.PosCase.Ligne > dst.PosCase.Ligne)
            {
                Tuple tmp = short_path(Api.InfoCase(newPosition(src.PosCase.Ligne - 1, src.PosCase.Colonne)), dst);
                if (result.value_ < tmp.value_)
                    result = tmp;
            }
            if (src.PosCase.Ligne < dst.PosCase.Ligne)
            {
                Tuple tmp = short_path(Api.InfoCase(newPosition(src.PosCase.Ligne + 1, src.PosCase.Colonne)), dst);
                if (result.value_ < tmp.value_)
                    result = tmp;
            }
            if (src.PosCase.Colonne > dst.PosCase.Colonne)
            {
                Tuple tmp = short_path(Api.InfoCase(newPosition(src.PosCase.Ligne, src.PosCase.Colonne - 1)), dst);
                if (result.value_ < tmp.value_)
                    result = tmp;
            }
            if (src.PosCase.Colonne < dst.PosCase.Colonne)
            {
                Tuple tmp = short_path(Api.InfoCase(newPosition(src.PosCase.Ligne, src.PosCase.Colonne + 1)), dst);
                if (result.value_ < tmp.value_)
                    result = tmp;
            }
            return result.add(src);
        }

        private Position is_island(EtatCase ec)
        {
            if (ec.PosCase.Colonne == Api.DimensionsCarte().Largeur || ec.PosCase.Ligne == Api.DimensionsCarte().Hauteur)
                return newPosition(-1, -1);
            if (ec.Contenu == TypeCase.SUD_EST)
                return newPosition(0, 0);
            if (Api.InfoCase(newPosition(ec.PosCase.Ligne + 1, ec.PosCase.Colonne)).Contenu == TypeCase.NORD_EST)
                return newPosition(1, 0);
            if (Api.InfoCase(newPosition(ec.PosCase.Ligne, ec.PosCase.Colonne + 1)).Contenu == TypeCase.SUD_OUEST)
                return newPosition(0, 1);
            if (Api.InfoCase(newPosition(ec.PosCase.Ligne + 1, ec.PosCase.Colonne + 1)).Contenu == TypeCase.NORD_OUEST)
                return newPosition(1, 1);
            return null;
        }

        // Fonction appelée au début de la partie.
        void PartieInit()
        {
            // TODO
            Position mine = Api.ListeVillages(Api.Moi())[0];
            Position enemy = Api.ListeVillages(Api.Adversaire())[0];

            for (int i = 0; i < Api.DimensionsCarte().Hauteur; i++)
            {
                for (int j = 0; j < Api.DimensionsCarte().Largeur; j++)
                {
                    Position pts = newPosition(i, j);
                    if (Math.Pow(mine.Colonne - pts.Colonne, 2) + Math.Pow(mine.Ligne - pts.Ligne, 2) <
                        Math.Pow(enemy.Colonne - pts.Colonne, 2) + Math.Pow(enemy.Ligne - pts.Ligne, 2))
                    {
                        if (Api.InfoCase(pts).Points > 0)
                            targets.Add(Api.InfoCase(pts));
                    }
                }
            }
            /*Collections.sort(targets, new Comparator<EtatCase>()
            {
            public int compare(EtatCase a, EtatCase b) { return b.Points - a.Points; }
            });*/

            int max = targets[0].Points;
            Tuple result = short_path(Api.InfoCase(mine), targets[0]);
            for (int i = 1; i < targets.Count; i++)
            {
                if (targets[i].Points != max)
                    break;

                Tuple tmp = short_path(Api.InfoCase(mine), targets[i]);
                if (result.value_ < tmp.value_)
                    result = tmp;
            }
            targets = result.path_;
        }

        // Fonction appelée à chaque tour.
        void JouerTour()
        {
            // TODO
            for (int i = 0; i < targets.Count; i++)
            {
                if (Api.PointsAction(Api.Moi()) <= 0)
                    break;
                Position coor = is_island(targets[i]);
                if (coor == null)
                    continue;
                if (coor.Ligne == -1)
                {
                    Console.WriteLine("Fuck, Houston we have a problem");
                    continue;
                }
                Api.TournerCase(Pos_add(targets[i].PosCase, coor));
                targets[i] = Api.InfoCase(targets[i].PosCase);
                i--;
            }
        }

        // Fonction appelée à la fin de la partie.
        void PartieFin()
        {
            // TODO
        }
    }
}
