#ifndef TBPROBE_HPP
#define TBPROBE_HPP

#include <string>

class Syzygy
{
public:
    static void initialize(const std::string& path);

    // Probe the WDL table for a particular position.
    // If success != 0, the probe was successful.
    // The return value is from the point of view of the side to move:
    // -2 : loss
    // -1 : loss, but draw under 50-move rule
    //  0 : draw
    //  1 : win, but draw under 50-move rule
    //  2 : win
    static int probeWdl(const Board& pos, int& success);

    // Probe the DTZ table for a particular position.
    // If success != 0, the probe was successful.
    // The return value is from the point of view of the side to move:
    //         n < -100 : loss, but draw under 50-move rule
    // -100 <= n < -1   : loss in n ply (assuming 50-move counter == 0)
    //         0        : draw
    //     1 < n <= 100 : win in n ply (assuming 50-move counter == 0)
    //   100 < n        : win, but draw under 50-move rule
    //
    // The return value n can be off by 1: a return value -n can mean a loss
    // in n+1 ply and a return value +n can mean a win in n+1 ply. This
    // cannot happen for tables with positions exactly on the "edge" of
    // the 50-move rule.
    //
    // This implies that if dtz > 0 is returned, the position is certainly
    // a win if dtz + 50-move-counter <= 99. Care must be taken that the engine
    // picks moves that preserve dtz + 50-move-counter <= 99.
    //
    // If n = 100 immediately after a capture or pawn move, then the position
    // is also certainly a win, and during the whole phase until the next
    // capture or pawn move, the inequality to be preserved is
    // dtz + 50-movecounter <= 100.
    //
    // In short, if a move is available resulting in dtz + 50-move-counter <= 99,
    // then do not accept moves leading to dtz + 50-move-counter == 100.
    static int probeDtz(const Board& pos, int& success);

    // Use the DTZ tables to filter out moves that don't preserve the win or draw.
    // If the position is lost, but DTZ is fairly high, only keep moves that
    // maximise DTZ.
    //
    // A return value false indicates that not all probes were successful and that
    // no moves were filtered out.
    static bool rootProbe(const Board& pos, MoveList& rootMoves, int& score);

    // Use the WDL tables to filter out moves that don't preserve the win or draw.
    // This is a fallback for the case that some or all DTZ tables are missing.
    //
    // A return value false indicates that not all probes were successful and that
    // no moves were filtered out.
    static bool rootProbeWdl(const Board& pos, MoveList& rootMoves, int& score);

    static int maxCardinality;
};

#endif
