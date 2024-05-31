// Include standard library in documentation: `cargo doc --open`
#[doc(inline)]
pub use std;

pub mod api;
mod ffi;

/// Represents a player (duh)
#[derive(Debug)]
struct Player {
    id: i32,
    position: api::Position,
    /// Mirrors the State's map, true means that the player has the position at i, j
    territory: Vec<Vec<bool>>,
}

impl Player {
    pub fn new(id: i32, dims: api::Position) -> Self {
        Self {
            id,
            // The first village at the start is the player pos
            position: api::liste_villages(id)[0],
            // Intersections, not tiles
            territory: vec![vec![false; dims.0 as usize]; dims.1 as usize],
        }
    }
}

/// Represents the current state of the game
/// Assuming this is a 2-player game
struct State {
    champion: Player,
    adversary: Player,
    map: Vec<Vec<api::EtatCase>>,
}

impl std::fmt::Display for State {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        self.map.iter().for_each(|row| {
            row.iter().for_each(|tile| {
                write!(f, "{} ", {
                    use api::TypeCase::*;
                    match tile.contenu {
                        NordEst => "▜",
                        NordOuest => "▛",
                        SudEst => "▟",
                        SudOuest => "▙",
                        Village => "V",
                        CaseInvalide => "╳",
                    }
                })
                .expect("Could not write ?!");
            });
            write!(f, "\n").expect("Could not write ?!");
        });
        std::fmt::Result::Ok(())
    }
}

// Avoiding this would have been great
// But since I can't change the definition of jouer_tour.....
// Do not fret ! Every unsafe block in here is safe, because there is no multithreading
static mut STATE: State = State {
    champion: Player {
        id: -1,
        position: (-1, -1),
        territory: vec![],
    },
    adversary: Player {
        id: -1,
        position: (-1, -1),
        territory: vec![],
    },
    map: vec![],
};

/// Fonction appelée au début de la partie.
pub fn partie_init() {
    let dims = api::dimensions_carte();
    // Create the map, like a double for but :sparkles: rusty :sparkles:
    let map: Vec<Vec<api::EtatCase>> = (0..dims.1)
        .map(|y| (0..dims.0).map(|x| api::info_case((x, y))).collect())
        .collect();

    // No race conditions possible, this block is safe
    unsafe {
        STATE = State {
            champion: Player::new(api::moi(), dims),
            adversary: Player::new(api::adversaire(), dims),
            map,
        };
        // Populate players territory
        api::recuperer_territoire(STATE.champion.id)
            .into_iter()
            .for_each(|(x, y)| STATE.champion.territory[y as usize][x as usize] = true);
        api::recuperer_territoire(STATE.adversary.id)
            .into_iter()
            .for_each(|(x, y)| STATE.adversary.territory[y as usize][x as usize] = true);

        // println!("{}", STATE);
    }; // end unsafe
}

/// Self explanatory, it updates STATE
fn update_map() {
    let dims = api::dimensions_carte();
    unsafe {
        (0..dims.1).for_each(|y| {
            (0..dims.0).for_each(|x| {
                let (x_usize, y_usize) = (x as usize, y as usize);
                STATE.map[y_usize][x_usize] = api::info_case((x, y));
                // Territory might have been destroyed !
                STATE.champion.territory[y_usize][x_usize] = false;
                STATE.adversary.territory[y_usize][x_usize] = false;
            })
        });
        api::recuperer_territoire(STATE.champion.id)
            .into_iter()
            .for_each(|(x, y)| STATE.champion.territory[y as usize][x as usize] = true);
        api::recuperer_territoire(STATE.adversary.id)
            .into_iter()
            .for_each(|(x, y)| STATE.adversary.territory[y as usize][x as usize] = true);
    };
}

/// This returns the closest free eagle of each type.
fn get_closest_eagles() -> Vec<Option<api::Aigle>> {
    use api::EffetAigle::*;
    let eagles = api::info_aigles();
    // That's kind of meaty BUT
    // For each type, get the closest one, that's about it
    [EffetVie, EffetFeu, EffetGel, EffetMeteore, EffetMort]
        .iter()
        .map(|&effet| {
            eagles
                .clone()
                .into_iter()
                .filter(|e| e.effet == effet && e.joueur == -1)
                .min_by(|a, b| {
                    // All of these are safe :)
                    let x_delta_a = unsafe { (a.pos.0 - STATE.champion.position.0).abs() } as f64;
                    let y_delta_a = unsafe { (a.pos.1 - STATE.champion.position.1).abs() } as f64;
                    let x_delta_b = unsafe { (b.pos.0 - STATE.champion.position.0).abs() } as f64;
                    let y_delta_b = unsafe { (b.pos.1 - STATE.champion.position.1).abs() } as f64;
                    // Won't be NaN, probably...
                    (x_delta_a * x_delta_a + y_delta_a * y_delta_a)
                        .sqrt()
                        .total_cmp(&(x_delta_b * x_delta_b + y_delta_b + y_delta_b).sqrt())
                })
        })
        .collect()
}

fn create_path(dst: api::Position) -> bool {
    let mut resources = api::points_action(unsafe { STATE.champion.id });
    let src = unsafe { STATE.champion.position };
    // let path = recurse_path(src, dst, &mut resources);
    fuck_it_we_ball(src, dst, &mut resources);
    true
}

fn fuck_it_we_ball(
    src: api::Position,
    dst: api::Position,
    resources: &mut i32,
) -> Option<(Vec<api::Position>, i32)> {
    let mut src = src;
    let direction = (dst.0 - src.0, dst.1 - src.1);
    let mut path = vec![];
    while src.0 != dst.0 {
        println!("Making an island..");
        if !try_make_island((src.0 + 1, src.1), resources) {
            println!("Failed :(");
            return None;
        }
        println!("Made an island!");
        src.0 += direction.0;
        path.push(src);
    }
    println!("Got under the eagle");
    while src.1 != dst.1 {
        println!("Making an island..");
        if !try_make_island((src.0, src.1 + 1), resources) {
            println!("Failed :(");
            return None;
        }
        println!("Made an island!");
        src.1 += direction.1;
        path.push(src);
    }

    None
}

// Unused
fn _recurse_path(
    src: api::Position,
    dst: api::Position,
    resources: &mut i32,
) -> Option<(Vec<api::Position>, i32)> {
    let direction = ((dst.0 - src.0).signum(), (dst.1 - src.1).signum());
    let (mut rec1, mut rec2) = (None, None);
    dbg!(src, dst, direction);
    if (dst.0 - src.0) * direction.0 > 0 {
        let new_src = (src.0 + direction.0, src.1);
        if try_make_island(new_src, resources) {
            rec1 = _recurse_path(new_src, dst, resources);
        }
    }
    if (dst.1 - src.1) * direction.1 > 0 {
        let new_src = (src.0, direction.1 + src.1);
        if try_make_island(new_src, resources) {
            rec2 = _recurse_path(new_src, dst, resources);
        }
    }

    if rec1.is_none() && rec2.is_some() {
        let mut r2u = rec2.unwrap();
        r2u.0.push((src.0, src.1 + direction.1));
        Some(r2u)
    } else if let Some(mut r2u) = rec2 {
        let mut r1u = rec1.unwrap();
        r1u.0.push((src.0 + direction.0, src.1));
        r2u.0.push((src.0, src.1 + direction.1));
        if r1u.1 < r2u.1 {
            Some((r1u.0, *resources))
        } else {
            Some((r2u.0, *resources))
        }
    } else {
        None
    }
}

fn try_make_island(pos: api::Position, resources: &mut i32) -> bool {
    let opposite = |t: (i32, i32)| -> api::TypeCase {
        if t == pos {
            api::TypeCase::SudEst
        } else if t == (pos.0 + 1, pos.1) {
            api::TypeCase::SudOuest
        } else if t == (pos.0, pos.1 + 1) {
            api::TypeCase::NordEst
        } else {
            api::TypeCase::NordOuest
        }
    };
    for pos in vec![
        pos,
        (pos.0 + 1, pos.1),
        (pos.0, pos.1 + 1),
        (pos.0 + 1, pos.1 + 1),
    ] {
        // println!(
        //     "Accessing {:?}, dims are {:?}",
        //     pos,
        //     api::dimensions_carte()
        // );
        let info = api::info_case(pos);
        if info.contenu == opposite(pos) {
            if api::tourner_case(pos) != api::Erreur::Ok {
                return false;
            }
            // Osef il reste 2 heures
            *resources -= api::COUT_ROTATION_STANDARD;
        }
    }
    true
}

/// Fonction appelée à chaque tour.
pub fn jouer_tour() {
    update_map();
    let mut eagles = get_closest_eagles();
    // TODO: Choose the right eagle ?
    // For now, get the closest one
    eagles.sort_by(|a, b| {
        if a.is_none() {
            return std::cmp::Ordering::Greater;
        }
        if b.is_none() {
            return std::cmp::Ordering::Less;
        }

        let (a, b) = (a.unwrap(), b.unwrap());
        // All of these are safe :)
        let x_delta_a = unsafe { (a.pos.0 - STATE.champion.position.0).abs() } as f64;
        let y_delta_a = unsafe { (a.pos.1 - STATE.champion.position.1).abs() } as f64;
        let x_delta_b = unsafe { (b.pos.0 - STATE.champion.position.0).abs() } as f64;
        let y_delta_b = unsafe { (b.pos.1 - STATE.champion.position.1).abs() } as f64;
        // Won't be NaN, probably...
        (x_delta_a * x_delta_a + y_delta_a * y_delta_a)
            .sqrt()
            .total_cmp(&(x_delta_b * x_delta_b + y_delta_b + y_delta_b).sqrt())
    });
    println!("Player at {:?}", unsafe { STATE.champion.position });
    for eagle in eagles {
        if let Some(eagle) = eagle {
            println!("Choosing eagle {} at {:?}", eagle.identifiant, eagle.pos);
            if create_path(eagle.pos) {
                println!("WTF IS A KILOMETERRRRRRRRRRRRRRRRRRRRR");
                break;
            }
            println!("Didn't find path :(");
        }
    }
}

/// Fonction appelée à la fin de la partie.
pub fn partie_fin() {
    println!("Bienvenue au meilleur partie_fin() de l'année");
}
