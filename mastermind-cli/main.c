#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define WIDTH 4
#define COLORS 8
#define REPETITION 1

size_t BAG_SIZE;

uint32_t pow_int(uint32_t a, uint32_t b) {
	uint32_t result = 1;
	while (b > 0) {
		if (b % 2 == 1) {
			result *= a;
		}
		b /= 2;
		a *= a;
	}
	return result;
}

uint32_t permutations(uint32_t n, uint32_t p) {
	uint32_t result = 1;
	p = n - p;
	while (n > p) {
		result *= n;
		n--;
	}
	return result;
}

typedef struct {
	uint8_t good;
	uint8_t bad;
} Result;

typedef struct {
	uint8_t colors[WIDTH];
} Combination;

typedef struct {
	size_t size;
	Combination* combinations;
} Bag;

void combination_print(Combination combination) {
	for (size_t i = 0; i < WIDTH; i++) {
		printf("%d ", combination.colors[i]);
	}
	puts("");
}

Result combination_expected_result(Combination combination, Combination solution) {
	Result result = {0, 0};
	bool used[WIDTH] = {false};
	for (size_t i = 0; i < WIDTH; i++) {
		if (combination.colors[i] == solution.colors[i]) {
			result.good++;
			used[i] = true;
		}
	}
	for (size_t i = 0; i < WIDTH; i++) {
		if (combination.colors[i] == solution.colors[i]) continue;
		for (size_t j = 0; j < WIDTH; j++) {
			if (!used[j] && combination.colors[i] == solution.colors[j]) {
				result.bad++;
				used[j] = true;
				break;
			}
		}
	}
	return result;
}

uint32_t combination_score(Combination combination, Bag bag) {
	uint32_t scores[(WIDTH + 1) * (WIDTH + 1)] = {0};
	for (size_t i = 0; i < bag.size; i++) {
		Result result = combination_expected_result(combination, bag.combinations[i]);
		scores[result.good * (WIDTH + 1) + result.bad]++;
	}
	uint32_t best_score = 0;
	for (size_t i = 0; i < (WIDTH + 1) * (WIDTH + 1); i++) {
		uint32_t score = scores[i];
		if (score > best_score) {
			best_score = score;
		}
	}
	return best_score;
}

#if REPETITION

void bag_init(Bag* bag) {
	bag->size = BAG_SIZE;
	bag->combinations = malloc(bag->size * (sizeof *(bag->combinations)));
	for (size_t i = 0; i < bag->size; i++) {
		for (size_t j = 0; j < WIDTH; j++) {
			bag->combinations[i].colors[j] = (i / pow_int(COLORS, j)) % COLORS + 1;
		}
	}
}

#else

void bag_init(Bag* bag) {
	bag->size = BAG_SIZE;
	bag->combinations = malloc(bag->size * (sizeof *(bag->combinations)));
	size_t indices[COLORS];
	for (size_t i = 0; i < COLORS; i++) {
		indices[i] = i;
	}
	size_t cycles[WIDTH];
	for (size_t i = 0; i < WIDTH; i++) {
		cycles[i] = COLORS - i;
		bag->combinations[0].colors[i] = i + 1;
	}
	size_t index = 1;
	while (index < BAG_SIZE) {
		for (size_t i = WIDTH; i > 0; i--) {
			size_t j = i - 1;
			cycles[j]--;
			if (cycles[j] == 0) {
				size_t tmp = indices[j];
				for (size_t k = j + 1; k < COLORS; k++) {
					indices[k - 1] = indices[k];
				}
				indices[COLORS - 1] = tmp;
				cycles[j] = COLORS - j;
			} else {
				{
					size_t k = COLORS - cycles[j];
					size_t tmp = indices[j];
					indices[j] = indices[k];
					indices[k] = tmp;
				}
				for (size_t k = 0; k < WIDTH; k++) {
					bag->combinations[index].colors[k] = indices[k] + 1;
				}
				index++;
				break;
			}
		}
	}
}

#endif

void bag_print(Bag bag) {
	for (size_t i = 0; i < bag.size; i++) {
		combination_print(bag.combinations[i]);
	}
}

void bag_free(Bag* bag) {
	free(bag->combinations);
}

Combination bag_best(Bag bag) {
	size_t best;
	uint32_t best_score = UINT_MAX;
	for (size_t i = 0; i < bag.size; i++) {
		uint32_t score = combination_score(bag.combinations[i], bag);
		if (score < best_score) {
			best_score = score;
			best = i;
		}
	}
	return bag.combinations[best];
}

void bag_update(Bag* bag, Combination combination, Result result) {
	Combination* new_combinations = malloc(BAG_SIZE * (sizeof *new_combinations));
	size_t new_size = 0;
	for (size_t i = 0; i < bag->size; i++) {
		Result expected_result = combination_expected_result(bag->combinations[i], combination);
		if (expected_result.good == result.good && expected_result.bad == result.bad) {
			new_combinations[new_size] = bag->combinations[i];
			new_size++;
		}
	}
	free(bag->combinations);
	bag->combinations = new_combinations;
	bag->size = new_size;
}

Result read_result() {
	Result result;
	scanf("%hhd", &(result.good));
	scanf("%hhd", &(result.bad));
	return result;
}

void init() {
#if REPETITION
	BAG_SIZE = pow_int(COLORS, WIDTH);
#else
	BAG_SIZE = permutations(COLORS, WIDTH);
#endif
}

int main() {
	init();
	Bag bag;
	bag_init(&bag);
	for(;;) {
		Combination played = bag_best(bag);
		combination_print(played);
		fflush(stdout);
		Result result = read_result();
		if (result.good == WIDTH) break;
		bag_update(&bag, played, result);
	}
	bag_free(&bag);
	return 0;
}
