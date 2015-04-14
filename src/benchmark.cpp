/*
    Hakkapeliitta - A UCI chess engine. Copyright (C) 2013-2015 Mikko Aarnos.

    Hakkapeliitta is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Hakkapeliitta is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Hakkapeliitta. If not, see <http://www.gnu.org/licenses/>.
*/

#include "benchmark.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "utils/stopwatch.hpp"

std::pair<uint64_t, uint64_t> Benchmark::runPerft(const Position root, const int depth)
{
    Stopwatch sw;

    sw.start();
    const auto perftResult = perft(root, depth);
    sw.stop();

    return std::make_pair(perftResult, sw.elapsed<std::chrono::milliseconds>());
}

uint64_t Benchmark::perft(const Position& pos, const int depth)
{
    MoveList moves;
    auto nodes = 0ull; 
    const auto inCheck = pos.inCheck();

    inCheck ? moveGen.generateLegalEvasions(pos, moves) : moveGen.generatePseudoLegalMoves(pos, moves);
    for (auto i = 0; i < moves.size(); ++i)
    {
        const auto& move = moves[i];
        if (!pos.legal(move, inCheck))
        {
            continue;
        }

        if (depth == 1)
        {
            ++nodes;
            continue;
        }

        Position newPos(pos);
        newPos.makeMove(move);
        nodes += depth == 1 ? 1 : perft(newPos, depth - 1);
    }

    return nodes;
}
