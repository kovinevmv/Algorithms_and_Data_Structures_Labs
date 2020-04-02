const getCoords = require('../utils/getCoords');
const Taken = {
    setSee(input, team, side, id, steps) {
        let ball = null;
        const ballObj = input.find((obj) => obj.cmd && (obj.cmd.p[0] === 'b' || obj.cmd.p[0] === 'B'));
        if (ballObj) {
            ball = {
                dist: ballObj.p[0],
                angle: ballObj.p[1]
            }
        }
        const playersListAll = input.filter((obj) => obj.cmd && obj.cmd.p[0] === 'p');
        const playersListMy = input.filter((obj) => obj.cmd && obj.cmd.p[0] === 'p' && obj.cmd.p[1] === `"${team}"`);
        console.log(team, id);
        const flagsList = input.filter((obj) => obj.cmd && (obj.cmd.p[0] === 'f' || obj.cmd.p[0] === 'g'));
        let flags = {};
        input.forEach((fl) => {
            if (fl.cmd) {
                flags[fl.cmd.p.join('')] = {
                    dist: fl.p[0],
                    angle: fl.p[1]
                }
            }
        });
        let myPos = null;
        if (flagsList.length === 2) {
            myPos = getCoords.getAnswerForTwoFlags(flagsList.filter((p) => true), getCoords.FlagsCoords)
        } else if (flagsList.length > 2) {
            myPos = getCoords.getAnswerForThreeFlags(flagsList.filter((p) => true), getCoords.FlagsCoords)
        }
        const goalOwnTeam = input.find((obj) => obj.cmd && (obj.cmd.p.join('') === ((side === 'l') ? 'gl' : 'gr')));
        let goalOwn = (goalOwnTeam) ? {
            dist: goalOwnTeam.p[0],
            angle: goalOwnTeam.p[1]
        } : null;
        const goalOtherTeam = input.find((obj) => obj.cmd && (obj.cmd.p.join('') === ((side === 'r') ? 'gl' : 'gr')));
        let goalOther = (goalOtherTeam) ? {
            dist: goalOtherTeam.p[0],
            angle: goalOtherTeam.p[1]
        } : null;
        return {
            ball,
            flags,
            id,
            myPos,
            team,
            playersListMy,
            goalOther,
            goalOwn,
            side,
            steps,
            closest(myTeam) {
                if (ball) {
                    if (flagsList.length < 2) {
                        console.log('Мало флагов')
                    } else {
                        const flagsForBall = getCoords.getDistanceForOtherPlayer(ballObj, flagsList.filter((p) => true));
                        const ballCoords = getCoords.getAnswerForThreeFlags(flagsForBall, getCoords.FlagsCoords);
                        const distanceList = [];
                        let playersList = [];
                        if (myTeam) {
                            playersList = playersListMy
                        } else {
                            playersList = playersListAll
                        }
                        playersList.forEach((p) => {
                            const newFlags = getCoords.getDistanceForOtherPlayer(p, flagsList.filter((p) => true));
                            const playerCoords = getCoords.getAnswerForThreeFlags(newFlags, getCoords.FlagsCoords);
                            if (playerCoords) {
                                distanceList.push({
                                    coords: playerCoords,
                                    dist: Math.sqrt(ballObj.p[0] ** 2 + p.p[0] ** 2 - 2 * ballObj.p[0] * p.p[0] * Math.cos((p.p[1] - ballObj.p[[1]]) * Math.PI / 180))
                                })
                            }
                        });
                        distanceList.sort((dist1, dist2) => {
                            return dist1.dist - dist2.dist
                        });
                        return distanceList
                    }
                }
                return []
            }
        }
    }
};
module.exports = Taken;
