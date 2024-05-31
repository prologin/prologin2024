import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;


public class Champion extends Api
{
    private class Tuple
    {
        public Tuple() {
            value_ = 0;
        }

        public Tuple(EtatCase ec) {
            value_ = ec.points;
            path_.add(ec);
        }

        public Tuple add(EtatCase ec) {
            value_ += ec.points;
            path_.add(ec);
            return this;
        }

        public void print() {
            System.out.println("Player: " + Api.moi());
            for (int i = 0; i < path_.size(); i++) {
                System.out.println(i + ": pos { colonne: " + path_.get(i).pos_case.colonne + ", ligne: " + path_.get(i).pos_case.ligne + " }");
            }
            System.out.println("Total points: " + value_);
            System.out.println();
        }

        public int value_ = 0;
        public List<EtatCase> path_ = new ArrayList<>();
    }
    List<EtatCase> targets = new ArrayList<>();
    List<EtatCase> in_way = new ArrayList<>();
    List<EtatCase> past = new ArrayList<>();
     
    private Position newPosition(int ligne, int colonne) {
        Position tmp = new Position();
        tmp.ligne = ligne;
        tmp.colonne = colonne;
        return tmp;
    }

    private Position Pos_add(Position p1, Position p2) {
        return newPosition(p1.ligne + p2.ligne, p1.colonne + p2.colonne);
    }
     
    // private boolean within(Position bound1, Position bound2, Position in) {
    //     // System.out.println("moi: " + Api.moi() + " ligne: " + in.ligne + ", colonne: " + in.colonne);
    //     if (bound1.ligne >= bound2.ligne && bound1.colonne >= bound2.colonne) {
    //         return bound1.ligne >=  in.ligne && in.ligne >= bound2.ligne &&
    //             bound1.colonne >=  in.colonne && in.colonne >= bound2.colonne;
    //     }
    //     else if (bound1.ligne <= bound2.ligne && bound1.colonne >= bound2.colonne) {
    //         return bound1.ligne <=  in.ligne && in.ligne <= bound2.ligne &&
    //             bound1.colonne >=  in.colonne && in.colonne >= bound2.colonne;
    //     }
    //     else if (bound1.ligne >= bound2.ligne && bound1.colonne <= bound2.colonne) {
    //         return bound1.ligne >=  in.ligne && in.ligne >= bound2.ligne &&
    //             bound1.colonne <=  in.colonne && in.colonne <= bound2.colonne;
    //     }
    //     else {
    //         return bound1.ligne <=  in.ligne && in.ligne <= bound2.ligne &&
    //             bound1.colonne <=  in.colonne && in.colonne <= bound2.colonne;
    //     }
    // }

    private Tuple short_path(EtatCase src, EtatCase dst) {
        if (src.pos_case.equals(dst.pos_case))
            return new Tuple(src);
        Tuple result = new Tuple();
        if (src.pos_case.ligne > dst.pos_case.ligne) {
            Tuple tmp = short_path(Api.info_case(newPosition(src.pos_case.ligne - 1, src.pos_case.colonne)), dst);
            if (result.value_ < tmp.value_)
                result = tmp;
        }
        if (src.pos_case.ligne < dst.pos_case.ligne) {
            Tuple tmp = short_path(Api.info_case(newPosition(src.pos_case.ligne + 1, src.pos_case.colonne)), dst);
            if (result.value_ < tmp.value_)
                result = tmp;
        }
        if (src.pos_case.colonne > dst.pos_case.colonne) {
            Tuple tmp = short_path(Api.info_case(newPosition(src.pos_case.ligne, src.pos_case.colonne - 1)), dst);
            if (result.value_ < tmp.value_)
                result = tmp;
        }
        if (src.pos_case.colonne < dst.pos_case.colonne) {
            Tuple tmp = short_path(Api.info_case(newPosition(src.pos_case.ligne, src.pos_case.colonne + 1)), dst);
            if (result.value_ < tmp.value_)
                result = tmp;
        }
        return result.add(src);
    }

    private Position is_island(EtatCase ec)  {
        if (Api.info_case(ec.pos_case).contenu == TypeCase.CASE_INVALIDE
            || Api.info_case(Pos_add(ec.pos_case, newPosition(1, 1))).contenu == TypeCase.CASE_INVALIDE)
            return newPosition(-1, -1);
        if (ec.contenu == TypeCase.SUD_EST)
            return newPosition(0, 0);
        if (Api.info_case(newPosition(ec.pos_case.ligne + 1, ec.pos_case.colonne)).contenu == TypeCase.NORD_EST)
            return newPosition(1, 0);
        if (Api.info_case(newPosition(ec.pos_case.ligne, ec.pos_case.colonne + 1)).contenu == TypeCase.SUD_OUEST)
            return newPosition(0, 1);
        if (Api.info_case(newPosition(ec.pos_case.ligne + 1, ec.pos_case.colonne + 1)).contenu == TypeCase.NORD_OUEST)
            return newPosition(1, 1);
        return null;
    }

    /*
     * Pos
     *  0 | 1
     * -------
     *  2 | 3
     */
    private int turn_value(int pos, TypeCase rota) {
        if ((pos == 0 && rota == TypeCase.SUD_OUEST)
            || (pos == 1 && rota == TypeCase.NORD_OUEST)
            || (pos == 2 && rota == TypeCase.SUD_EST)
            || (pos == 3 && rota == TypeCase.NORD_EST))
            return 1;
        if ((pos == 0 && rota == TypeCase.NORD_OUEST)
            || (pos == 1 && rota == TypeCase.NORD_EST)
            || (pos == 2 && rota == TypeCase.SUD_OUEST)
            || (pos == 3 && rota == TypeCase.SUD_EST))
            return 2;
        if ((pos == 0 && rota == TypeCase.NORD_EST)
            || (pos == 1 && rota == TypeCase.SUD_EST)
            || (pos == 2 && rota == TypeCase.NORD_OUEST)
            || (pos == 3 && rota == TypeCase.SUD_OUEST))
            return 3;
        return 4;
    }

    private Position annoy_enemy()
    {
        Position enemy = Api.liste_villages(Api.adversaire())[0];
        List<EtatCase> tocheck = new ArrayList<>();
        List<Integer> values = new ArrayList<>();
        
        for (int i = -1; i < 1; i++) {
            for (int j = -1; j < 1; j++) {
                var tmp = Api.info_case(Pos_add(enemy, newPosition(i, j)));
                if (is_island(tmp) == null) {
                    for (int x = 0; x < 2; x++) {
                        for (int y = 0; y < 2; y++) {
                            var tmp1 = Api.info_case(Pos_add(tmp.pos_case, newPosition(x, y)));
                            if (!tocheck.contains(tmp1)) {
                                tocheck.add(Api.info_case(Pos_add(tmp.pos_case, newPosition(x, y))));
                                values.add(turn_value(x * 2 + y, tmp1.contenu));
                            }
                        }
                    }
                }
            }
        }

        for (int i = 0; i < tocheck.size(); i++) {
            if (tocheck.get(i).contenu == TypeCase.VILLAGE) {
                tocheck.remove(i);
                values.remove(i);
            }
        }
        
        if (tocheck.size() == 0)
            return null;

        int argmin = 1;
        for (int i = 1; i < values.size(); i++) {
            if (values.get(i) < values.get(argmin))
                argmin = i;
        }

        return tocheck.get(argmin).pos_case;
    }

    /**
     * Fonction appelée au début de la partie.
     */
    public void partie_init()
    {
        Position mine = Api.liste_villages(Api.moi())[0];
        Position enemy = Api.liste_villages(Api.adversaire())[0];
        
        for (int i = 0; i < Api.dimensions_carte().hauteur; i++) {
            for (int j = 0; j < Api.dimensions_carte().largeur; j++) {
                Position pts = newPosition(i, j);
                if (Math.pow(mine.colonne - pts.colonne, 2) + Math.pow(mine.ligne - pts.ligne, 2) <
                    Math.pow(enemy.colonne - pts.colonne, 2) + Math.pow(enemy.ligne - pts.ligne, 2)) {
                    if (Api.info_case(pts).points > 0)
                        targets.add(Api.info_case(pts));
                }
            }
        }
        Collections.sort(targets, new Comparator<EtatCase>(){
            public int compare(EtatCase a, EtatCase b){ return b.points - a.points; }
        });

        int max = targets.get(0).points;
        Tuple result = short_path(Api.info_case(mine), targets.get(0));
        for (int i = 1; i < targets.size(); i++) {
            if (targets.get(i).points != max)
                break;

            Tuple tmp = short_path(Api.info_case(mine), targets.get(i));
            if (result.value_ < tmp.value_)
                result = tmp;
        }
        // result.print();
        targets = result.path_;
    }

    /**
     * Fonction appelée à chaque tour.
     */
    public void jouer_tour()
    {
        for (int i = 0; i < targets.size(); i++) {
            if (Api.points_action(Api.moi()) <= 0)
                break;
            Position coor = is_island(targets.get(i));
            if (coor == null)
                continue;
            if (coor.ligne == -1) {
                System.out.println("Fuck, Houston we have a problem");
                continue;
            }
            Api.tourner_case(Pos_add(targets.get(i).pos_case, coor));
            targets.set(i, Api.info_case(targets.get(i).pos_case));
            i--;
        }
        if (Api.points_action(Api.moi()) != 2)
            return;
        Position turnme = annoy_enemy();
        if (turnme != null)
            Api.tourner_case(turnme);
    }

    /**
     * Fonction appelée à la fin de la partie.
     */
    public void partie_fin()
    {
        // if (Api.moi() != 0)
        //     return;
        // for (int i = 0; i < Api.dimensions_carte().hauteur; i++) {
        //     for (int j = 0; j < Api.dimensions_carte().largeur; j++) {
        //         if (Api.info_case(newPosition(i, j)).contenu == TypeCase.NORD_EST)
        //             System.out.print("▙|");
        //         else if (Api.info_case(newPosition(i, j)).contenu == TypeCase.NORD_OUEST)
        //             System.out.print("▟|");
        //         else if (Api.info_case(newPosition(i, j)).contenu == TypeCase.SUD_OUEST)
        //             System.out.print("▜|");
        //         else if (Api.info_case(newPosition(i, j)).contenu == TypeCase.SUD_EST)
        //             System.out.print("▛|");
        //         else
        //             System.out.print("X|");
        //     }
        //     System.out.println("\n--------------------");
        // }
    }
}