#ifndef BITBOARD_CPP
#define BITBOARD_CPP

#include "bitboard.h"

array<uint64_t, Squares> bit;
array<uint64_t, Squares> kingAttacks;
array<uint64_t, Squares> knightAttacks;
array<uint64_t, Squares> pawnAttacks[2];
array<uint64_t, Squares> pawnSingleMoves[2];
array<uint64_t, Squares> pawnDoubleMoves[2];
array<uint64_t, Squares> rays[8];

// Not really a bitboard but it is easiest to initialize here.
array<int, Squares> heading[Squares];

void initializeBitMasks();
void initializeKingAttacks();
void initializeKnightAttacks();
void initializePawnAttacks();
void initializePawnMoves();
void initializeRays();

void initializeBitboards() 
{	
   initializeBitMasks();
   initializeKingAttacks();
   initializeKnightAttacks();
   initializePawnAttacks();
   initializePawnMoves();
   initializeRays();
}

void initializeBitMasks() 
{
	for (int sq = A1; sq <= H8; sq++) 
	{
		bit[sq] = (uint64_t)1 << sq;
	}
}

void initializeKingAttacks()
{
	uint64_t notAFile = 0xFEFEFEFEFEFEFEFE;
	uint64_t notHFile = 0x7F7F7F7F7F7F7F7F;

	for (int sq = A1; sq <= H8; sq++)
	{
		uint64_t kingSet = 0;

		kingSet |= bit[sq];
		kingSet |= (((uint64_t)1 << sq) << 1) & notAFile;
		kingSet |= (((uint64_t)1 << sq) >> 1) & notHFile;

		kingAttacks[sq] |= kingSet << 8;
		kingAttacks[sq] |= kingSet >> 8;
		kingAttacks[sq] |= kingSet;
		kingAttacks[sq] ^= bit[sq];
	}
}

void initializeKnightAttacks()
{
	uint64_t notAFile = 0xFEFEFEFEFEFEFEFE;
	uint64_t notHFile = 0x7F7F7F7F7F7F7F7F;
	uint64_t notABFile = 0xFCFCFCFCFCFCFCFC;
	uint64_t notGHFile = 0x3F3F3F3F3F3F3F3F;

	for (int sq = A1; sq <= H8; sq++)
	{
		uint64_t knightSet = 0;

		knightSet |= (((uint64_t)1 << sq) << 17) & notAFile;
		knightSet |= (((uint64_t)1 << sq) << 10) & notABFile;
		knightSet |= (((uint64_t)1 << sq) << 15) & notHFile;
		knightSet |= (((uint64_t)1 << sq) << 6) & notGHFile;
		knightSet |= (((uint64_t)1 << sq) >> 17) & notHFile;
		knightSet |= (((uint64_t)1 << sq) >> 10) & notGHFile;
		knightSet |= (((uint64_t)1 << sq) >> 15) & notAFile;
		knightSet |= (((uint64_t)1 << sq) >> 6) & notABFile;

		knightAttacks[sq] = knightSet;
	}
}
	
void initializePawnAttacks()
{
	uint64_t notAFile = 0xFEFEFEFEFEFEFEFE;
	uint64_t notHFile = 0x7F7F7F7F7F7F7F7F;
	for (int sq = A1; sq <= H8; sq++)
	{
		pawnAttacks[White][sq] |= (((uint64_t)1 << sq) << 9) & notAFile;
		pawnAttacks[White][sq] |= (((uint64_t)1 << sq) << 7) & notHFile;

		pawnAttacks[Black][sq] |= (((uint64_t)1 << sq) >> 9) & notHFile;
		pawnAttacks[Black][sq] |= (((uint64_t)1 << sq) >> 7) & notAFile;
	}
}
   
void initializePawnMoves()
{
	for (int sq = A1; sq <= H8; sq++) 
	{
		if (sq <= H7)
		{
			pawnSingleMoves[White][sq] = bit[sq + 8];
		}
		if (sq >= A2)
		{
			pawnSingleMoves[Black][sq] = bit[sq - 8];
		}
		if (sq <= H2)
		{
			pawnDoubleMoves[White][sq] = bit[sq + 16];
		}
	    if (sq >= A7)
	    {
			pawnDoubleMoves[Black][sq] = bit[sq - 16];
	    }
    }	
}	

void initializeRays()
{
	array<int, 8> rankDirection = { -1, -1, -1, 0, 0, 1, 1, 1 };
	array<int, 8> fileDirection = { -1, 0, 1, -1, 1, -1, 0, 1 };

	for (int i = SW; i <= NE; i++)
	{
		heading[i].fill(-1);
	}

	for (int sq = A1; sq <= H8; sq++)
	{
		int rank = sq / 8;
		int file = sq % 8;

		for (int i = SW; i <= NE; i++)
		{
			for (int j = 1; j < 8; j++)
			{
				int toRank = rankDirection[i] * j + rank;
				int toFile = fileDirection[i] * j + file;
				// Check if we went over the side of the board.
				if (toRank < 0 || toRank > 7 || toFile < 0 || toFile > 7)
				{
					break;
				}
				heading[sq][toRank * 8 + toFile] = i;
				rays[i][sq] |= bit[toRank * 8 + toFile];
			}
		}
	}
}

#endif

