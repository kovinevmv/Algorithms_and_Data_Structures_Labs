const AUTOMAT_TYPES = {
    GOALIE: require('../goalie_logic').automaton,
    PLAYER: require('../player_logic').automaton
};

module.exports = {AUTOMAT_TYPES};