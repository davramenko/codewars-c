#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>


// Ranking Poker Hands
// https://www.codewars.com/kata/5739174624fc28e188000465/train/c
// https://www.pagat.com/poker/rules/ranking.html

struct Hand {
	char ranks[5];
	char suits[5];
};

//                   A  B C D E F G H I J  K  L M N O P Q  R S T
const char *kinds = "\xE\0\0\0\0\0\0\0\0\xB\xD\0\0\0\0\0\xC\0\0\xA";

#define skind(s, i) (s[i * 3])
#define ssuit(s, i) (s[i * 3 + 1])
#define k2r(k) ((k >= '2' && k <= '9') ? k - '0' : (toupper(k) >= 'A' && toupper(k) <= 'T') ? kinds[toupper(k) - 'A'] : 0)

typedef struct Hand Hand;
typedef enum { Win, Loss, Tie } Result;
typedef enum {
	ace   = 0x0E,
	king  = 0x0D,
	queen = 0x0C,
	jack  = 0x0B,
	c10   = 0x0A,
	c9    = 0x09,
	c8    = 0x08,
	c7    = 0x07,
	c6    = 0x06,
	c5    = 0x05,
	c4    = 0x04,
	c3    = 0x03,
	c2    = 0x02
} rank;

struct rank_stats {
	rank r;
	unsigned char c;
};

// ==========================================================================================================
void hand2str(Hand* ph, char s[16]);
// ==========================================================================================================

static int rs_cmp(const void *p1, const void *p2) {
	struct rank_stats* prs1 = (struct rank_stats*) p1;
	struct rank_stats* prs2 = (struct rank_stats*) p2;
	return prs2->r - prs1->r;
}

int get_kinds_occurence(Hand* ph, struct rank_stats prs[16]) {
	int i, j, len = 0;

	for (i = 0; i < 5; i++) {
		char r = ph->ranks[i];
		int k = -1;
		for (j = 0; j < len; j++) {
			if (prs[j].r == r) {
				k = j;
				break;
			}
		}
		if (k < 0) {
			prs[len].r = r;
			prs[len].c = 1;
			len++;
		} else {
			prs[k].c++;
		}
	}
	qsort((void*)prs, len, sizeof(struct rank_stats), rs_cmp);
	return len;
}

bool all_of_same_suit(Hand* ph) {
	int i;
	char suit = ph->suits[0];
	for (i = 1; i < 5; i++) {
		if (suit != ph->suits[i])
			return false;
	}
	return true;
}

int isStraightFlush(Hand* ph) {
	// returns: the highest card rank
	// condition: (stats length) == 5 && ((highest rank) - (lowest rank) == 4 || ((ace is in top) && (2 is at the bottom) && (highest rank) - (lowest rank) == 3)) && (all of the same suit)
	struct rank_stats prs[16];
	int n = get_kinds_occurence(ph, prs);
	if (n == 5 && all_of_same_suit(ph)) {
		if ((prs[0].r - prs[4].r) == 4)
			return prs[0].r;
		else if ((prs[0].r == ace && prs[4].r == c2 && (prs[1].r - prs[4].r) == 3))
			return prs[1].r;
	}
	return 0;
}

int isFourOfKind(Hand* ph) {
	// returns: ((rank of four) << 4) | (rank of kicker)
	// condition: (stats length) == 2 && (one of rank's occurence) == 4
	struct rank_stats prs[16];
	int i, n = get_kinds_occurence(ph, prs);
	if (n == 2) {
		int res = -1;
		for (i = 0; i < n; i++) {
			if (prs[i].c == 4) {
				res = prs[i].r;
				break;
			}
		}
		if (res >= 0) {
			for (i = 0; i < n; i++) {
				if (prs[i].c != 4) {
					res <<= 4;
					res |= (0x0F & prs[i].r);
				}
			}
			return res;
		}
	}
	return 0;
}

int isFullHouse(Hand* ph) {
	// returns: ((rank of three) << 4) | (rank of pair)
	// condition: (stats length) == 2
	// comment: this is enough since FourOfKind has been excluded
	struct rank_stats prs[16];
	int i, n = get_kinds_occurence(ph, prs);
	if (n == 2) {
		int res = -1;
		for (i = 0; i < n; i++) {
			if (prs[i].c == 3) {
				res = prs[i].r;
				break;
			}
		}
		if (res >= 0) {
			for (i = 0; i < n; i++) {
				if (prs[i].c == 2) {
					res <<= 4;
					res |= (0x0F & prs[i].r);
				}
			}
			return res;
		}
	}
	return 0;
}

int isFlush(Hand* ph) {
	// returns: ((rank of highest) << 16) | ((rank of second) << 12) | ((rank of third) << 8)
	//          | ((rank of fourth) << 4) | (rank of lowest)
	// condition: (all of the same suit)
	struct rank_stats prs[16];
	int i, n = get_kinds_occurence(ph, prs);
	if (n == 5 && all_of_same_suit(ph)) {
		int res = prs[0].r;
		for (i = 1; i < 5; i++) {
			res <<= 4;
			res |= (0x0F & prs[i].r);
		}
		return res;
	}
	return 0;
}

int isStraight(Hand* ph) {
	// returns: the highest card rank
	// condition: (stats length) == 5 && (highest rank) - (lowest rank) == 4
	// condition: (stats length) == 5 && ((highest rank) - (lowest rank) == 4 || ((ace is in top) && (2 is at the bottom) && (highest rank) - (lowest rank) == 3))
	struct rank_stats prs[16];
	int res, i, n = get_kinds_occurence(ph, prs);
	//char buff[16];

	//hand2str(ph, buff);
	//printf("isStraight: %s\n", buff);
	//for (i = 0; i < n; i++) {
	//	printf("isStraight: rank: 0x0%x, cnt=%d\n", prs[i].r, prs[i].c);
	//}
	res = 0;
	if (n == 5) {
		if ((prs[0].r - prs[4].r) == 4) {
			res = prs[0].r;
		} else if ((prs[0].r == ace && prs[4].r == c2 && (prs[1].r - prs[4].r) == 3)) {
			res = prs[1].r;
		}
	}
	//printf("isStraight: res=0x%08X\n\n", res);
	return res;
}

int isThreeOfKind(Hand* ph) {
	// returns: ((rank of three) << 8) | ((rank of highest odd card) << 4) | (rank of other odd card)
	// condition: (stats length) == 3 && (one of rank's occurence) == 3
	struct rank_stats prs[16];
	int i, n = get_kinds_occurence(ph, prs);
	if (n == 3) {
		int res = -1;
		for (i = 0; i < n; i++) {
			if (prs[i].c == 3) {
				res = prs[i].r;
				break;
			}
		}
		if (res >= 0) {
			for (i = 0; i < n; i++) {
				if (prs[i].c != 3) {
					res <<= 4;
					res |= (0x0F & prs[i].r);
				}
			}
			return res;
		}
	}
	return 0;
}

int isTwoPairs(Hand* ph) {
	// returns: ((rank of highest pair) << 8) | ((rank of lower pair) << 4) | (rank of odd card)
	// condition: (stats length) == 3 && (two of rank's occurence) == 2
	struct rank_stats prs[16];
	int i, n = get_kinds_occurence(ph, prs);
	if (n == 3) {
		int cnt = 0;
		for (i = 0; i < n; i++) {
			if (prs[i].c == 2)
				cnt++;
		}
		if (cnt == 2) {
			int res = 0;
			for (i = 0; i < n; i++) {
				if (prs[i].c == 2) {
					res <<= 4;
					res |= (0x0F & prs[i].r);
				}
			}
			for (i = 0; i < n; i++) {
				if (prs[i].c != 2) {
					res <<= 4;
					res |= (0x0F & prs[i].r);
				}
			}
			return res;
		}
	}
	return 0;
}

int isPair(Hand* ph) {
	// returns: ((rank of pair) << 12) | ((rank of highest odd card) << 8) | ((rank of second odd card) << 4)
	//          | (rank of lowest odd card)
	// condition: (stats length) == 4
	struct rank_stats prs[16];
	int i, n = get_kinds_occurence(ph, prs);
	if (n == 4) {
		int res = -1;
		for (i = 0; i < n; i++) {
			if (prs[i].c == 2) {
				res = prs[i].r;
				break;
			}
		}
		for (i = 0; i < n; i++) {
			if (prs[i].c != 2) {
				res <<= 4;
				res |= (0x0F & prs[i].r);
			}
		}
		return res;
	}
	return 0;
}

int rankNothing(Hand* ph) {
	// always returns geater than zero.
	// returns: ((rank of highest card) << 16) | ((rank of second card) << 12) | ((rank of third card) << 8)
	//          | ((rank of fourth card) << 4) | (rank of lowest card)
	struct rank_stats prs[16];
	int i, n = get_kinds_occurence(ph, prs);
	int res = prs[0].r;
	for (i = 1; i < n; i++) {
		res <<= 4;
		res |= (0x0F & prs[i].r);
	}
	return res;
}

int (*ranking[9])(Hand* ph) = { isStraightFlush, isFourOfKind, isFullHouse, isFlush, isStraight, isThreeOfKind, isTwoPairs, isPair, rankNothing };

// return value will be freed
Hand* PokerHand(const char *cards) {
	Hand* ph = malloc(sizeof(Hand));
	int i;
	for (i = 0; i < 5; i++) {
		ph->ranks[i] = k2r(skind(cards, i));
		ph->suits[i] = ssuit(cards, i);
		//printf("rank: 0x0%x, suit: '%c'\n", ph->ranks[i], ph->suits[i]);
	}
	return ph;
}

Result compare(Hand* player, Hand* opponent) {
	int i, r1, r2;
	Result res = Tie;
	for (i = 0; i < 9; i++) {
		r1 = (ranking[i])(player);
		r2 = (ranking[i])(opponent);
		if (r1 && !r2) {
			res = Win;
			break;
		} else if (!r1 && r2) {
			res = Loss;
			break;
		} else if (r1 && r2) {
			if (r1 > r2)
				res = Win;
			else if (r1 < r2)
				res = Loss;
			break;	
		}
	}
	return res;
}

// ==========================================================================================================

void hand2str(Hand* ph, char s[16]) {
	int i;

	memset(s, 32, 16);
	for (i = 0; i < 5; i++) {
		s[i * 3] = (char)((ph->ranks[i] < 10) ? ph->ranks[i] + '0' : (ph->ranks[i] - 10) + 'A');
		s[i * 3 + 1] = ph->suits[i];
	}
	s[14] = 0;
}

bool run_test_hands(Hand *player, Hand *opponent, Result outcome) {
	Result result = compare(player, opponent);
	return outcome == result;
}

bool run_test(const char *player, const char *opponent, Result outcome) {
	Hand *p = PokerHand(player), *o = PokerHand(opponent);
	bool result = run_test_hands(p, o, outcome);
	free (p); free (o);
	return result;
}

void cr_assert(bool result, const char* msg) {
	if (!result)
		printf("* %s\n", msg);
}

int main () {
/*
	//Hand *p = PokerHand("2H 3H 4H 5H 6H");
	//Hand *p = PokerHand("KS AS TS QS JS");
	Hand *p = PokerHand("2S AH 2H AS AC");
	//Hand *p = PokerHand("4S 5H 6H TS AC");
	char str[16];
	struct rank_stats prs[16];
	int i, n;

	hand2str(p, str);
	printf("[%s]\n", str);

	n = get_kinds_occurence(p, prs);
	for (i = 0; i < n; i++) {
		printf("rank: 0x0%x, cnt=%d\n", prs[i].r, prs[i].c);
	}

	free(p);
	return;
*/
	cr_assert(run_test("2H 3H 4H 5H 6H", "KS AS TS QS JS", Loss), "Highest straight flush wins");
	cr_assert(run_test("2H 3H 4H 5H 6H", "AS AD AC AH JD", Win ), "Straight flush wins of 4 of a kind");
	cr_assert(run_test("AS AH 2H AD AC", "JS JD JC JH 3D", Win ), "Highest 4 of a kind wins");
	cr_assert(run_test("2S AH 2H AS AC", "JS JD JC JH AD", Loss), "4 Of a kind wins of full house");
	cr_assert(run_test("2S AH 2H AS AC", "2H 3H 5H 6H 7H", Win ), "Full house wins of flush");
	cr_assert(run_test("AS 3S 4S 8S 2S", "2H 3H 5H 6H 7H", Win ), "Highest flush wins");
	cr_assert(run_test("2H 3H 5H 6H 7H", "2S 3H 4H 5S 6C", Win ), "Flush wins of straight");
	cr_assert(run_test("2S 3H 4H 5S 6C", "3D 4C 5H 6H 2S", Tie ), "Equal straight is tie");
	cr_assert(run_test("2S 3H 4H 5S 6C", "AH AC 5H 6H AS", Win ), "Straight wins of three of a kind");
	cr_assert(run_test("2S 3H 4H 5S AC", "AH AC 5H 6H AS", Win ), "Low-ace straight wins of three of a kind");
	cr_assert(run_test("2S 2H 4H 5S 4C", "AH AC 5H 6H AS", Loss), "3 Of a kind wins of two pair");
	cr_assert(run_test("2S 2H 4H 5S 4C", "AH AC 5H 6H 7S", Win ), "2 Pair wins of pair");
	cr_assert(run_test("6S AD 7H 4S AS", "AH AC 5H 6H 7S", Loss), "Highest pair wins");
	cr_assert(run_test("2S AH 4H 5S KC", "AH AC 5H 6H 7S", Loss), "Pair wins of nothing");
	cr_assert(run_test("2S 3H 6H 7S 9C", "7H 3C TH 6H 9S", Loss), "Highest card loses");
	cr_assert(run_test("4S 5H 6H TS AC", "3S 5H 6H TS AC", Win ), "Highest card wins");
	cr_assert(run_test("2S AH 4H 5S 6C", "AD 4C 5H 6H 2C", Tie ), "Equal cards is tie");
}