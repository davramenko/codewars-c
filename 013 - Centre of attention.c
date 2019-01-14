#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Centre of attention
// https://www.codewars.com/kata/centre-of-attention/train/c

typedef struct { unsigned *pixels; unsigned width, height; } Image;
typedef struct { unsigned *values; unsigned size; } unsigned_array;


//unsigned border_disance(int i, int j, Image image) {
//	unsigned dist[3];
//	unsigned dmin, k;
//
//	dist[0] = image.height - i;
//	dist[1] = j + 1;
//	dist[2] = image.width - j;
//	dmin = i + 1;
//	for (k = 1; k < 3; k++)
//		if (dmin > dist[k])
//			dmin = dist[k];
//	return dmin;
//}

#if !defined(min)
#define min(a, b) ((a < b) ? a : b)
#endif
#define border_disance(i, j, img) (min(i + 1, min(image.height - i, min(j + 1, image.width - j))))
#define pix(img, row, col) (img.pixels[row *  img.width + col])
#define uabs(v) (((((unsigned)v) & 0x80000000U)) ? (-(v)) : (v))
#define dwidth(col, j) (uabs(col - j))
#define dheight(row, i) (uabs(row - i))
#define dval(row, col, i, j) (dwidth(col, j) + dheight(row, i))
#define set_dmin(dmin, row, col, i, j) ((dmin == 0) ? (dmin = dval(row, col, i, j)) : ((dmin > dval(row, col, i, j)) ? (dmin = dval(row, col, i, j)) : 0))

//void set_dmin(unsigned* dmin, int row, int col, int i, int j) {
//	int h = abs(row - i);
//	int w = abs(col - j);
//	int v = h + w;
//	if (*dmin == 0)
//		*dmin = v;
//	else if (*dmin > v)
//		*dmin = v;
//}

unsigned get_depth(int row, int col, int r, unsigned colour, Image image) {
	int i, j;
	unsigned dmin = 0;
	int st_col = col - r;
	int end_col = col + r;

	if (row - r < 0) return row + 1;
	if (row + r >= image.height) return image.height - row;
	if (col - r < 0) return col + 1;
	if (col + r >= image.width) return image.width - col;
	for (i = row - r; i <= row + r; i++) {
		if (i == (row - r) || i == (row + r)) {
			for (j = st_col; j <= end_col; j++)
				if (colour != pix(image, i, j))
					set_dmin(dmin, row, col, i, j);
		} else {
			if (colour != pix(image, i, st_col))
				set_dmin(dmin, row, col, i, st_col);
			if (colour != pix(image, i, end_col))
				set_dmin(dmin, row, col, i, end_col);
		}
	}
	return dmin;
}

unsigned_array central_pixels(Image image, unsigned colour) {
	unsigned_array res = {NULL, 0};
	unsigned i, j, k, res_cap = 1024;
	unsigned dmax = 0;

	res.values = calloc(res_cap, sizeof(unsigned));
	for (i = 0; i < image.height; i++) {
		for (j = 0; j < image.width; j++) {
			if (colour == pix(image, i, j)) {
				unsigned bord = border_disance(i, j, image);
				unsigned k0 = 0, d0 = 0, depth = 0;
				if (bord == 1) {
					depth = bord;
					//printf("central_pixels: depth = bord: %d\n", depth);
				} else {
					for (k = 1; k < bord; k++) {
						unsigned d = get_depth(i, j, k, colour, image);
						//printf("central_pixels: [%d,%d](%d): %d\n", i, j, k, d);
						if (d0 == 0 && d != 0) {
							d0 = d;
							k0 = k;
							//printf("central_pixels: k0 = %d, d0 = %d\n", k0, d0);
						}
						if (d != 0) {
							if ((k0 != 0 && d < d0 && d < (k + 2)) || (k0 == 0 && d < (k + 2))) {
								depth = d;
								//printf("central_pixels: depth = d: %d (break)\n", depth);
								break;
							}
						}
					}
					if (!depth || depth > bord) {
						if (d0 < bord) {
							depth = d0;
							//printf("central_pixels: depth = d0: %d(?)\n", depth);
						} else {
							depth = bord;
							//printf("central_pixels: depth = bord: %d(?)\n", depth);
						}
					}
				}
				if (depth > dmax) {
					//printf("central_pixels: new max depth: %d (prev=%d) [%d,%d]: %d\n", depth, dmax, i, j, i * image.width + j);
					dmax = depth;
					res.size = 0;
					res.values[res.size] = i * image.width + j;
					res.size++;
				} else if (depth == dmax) {
					if (res.size >= res_cap) {
						unsigned tcap = res_cap * 2;
						unsigned* buff = calloc(tcap, sizeof(unsigned));
						memcpy(buff, res.values, sizeof(unsigned) * res_cap);
						free(res.values);
						res.values = buff;
						res_cap = tcap;
					}
					//printf("central_pixels: add to depth: %d [%d,%d](%d): %d\n", depth, i, j, res.size, i * image.width + j);
					res.values[res.size] = i * image.width + j;
					res.size++;
				}
			}
		}
	}
	/*
	if (res.size == 7 && res.values[0] == 642) {
		printf("{%d,%d}; color: %d\n", image.height, image.width, colour);
		for (i = 0; i < image.height; i++) {
			for (j = 0; j < image.width; j++) {
				if (j > 0) printf(",");
				printf("%d", pix(image, i, j));
			}
			printf("\n");
		}
	}
	*/
	return res;
}

int main() {
	unsigned image_data[] = {1,1,4,4,4,4,2,2,2,2,
							 1,1,1,1,2,2,2,2,2,2,
							 1,1,1,1,2,2,2,2,2,2,
							 1,1,1,1,1,3,2,2,2,2,
							 1,1,1,1,1,3,3,3,2,2,
							 1,1,1,1,1,1,3,3,3,3};
	unsigned image_data2[] = {6,6,6,6,6,6,6,6,
							  5,6,6,6,6,6,6,6,
							  5,5,6,6,6,6,6,6,
							  5,5,5,6,6,6,6,6,
							  5,5,5,5,6,6,6,6,
							  5,5,5,5,5,6,6,6,
							  5,5,5,5,5,5,6,6,
							  5,5,5,5,5,5,5,6};



						// 419, 448
						//                              13                22
						//                              V                 V
	unsigned image_data3[] = {6,6,6,7,7,7,7,5,6,6,6,6,6,6,7,7,7,5,5,6,5,5,5,5,5,7,7,7,7,
							  6,6,6,7,7,7,7,5,6,6,6,6,6,6,7,7,7,5,5,6,5,5,5,5,5,7,7,7,7,
							  5,5,5,5,5,5,5,7,6,6,6,7,7,7,7,7,7,6,6,6,5,5,5,5,5,7,7,7,7,
							  5,5,5,5,5,5,5,7,6,6,6,7,7,7,7,7,7,6,6,6,5,5,5,5,5,7,7,7,7,
							  5,5,5,5,5,5,5,7,6,6,6,7,7,7,7,7,7,6,6,6,5,5,5,5,5,7,7,7,7,
							  5,5,5,5,5,5,5,7,6,6,6,7,7,7,7,7,7,6,6,6,5,5,5,5,5,7,7,7,7,  // 5
							  5,5,5,5,5,5,5,7,6,6,6,7,7,7,7,7,7,6,6,6,5,5,5,5,5,7,7,7,7,
							  5,7,7,5,5,5,5,7,5,5,5,5,5,5,6,6,6,6,6,6,7,7,7,7,7,6,6,6,6,
							  5,7,7,5,5,5,5,7,5,5,5,5,5,5,6,6,6,6,6,6,7,7,7,7,7,6,6,6,6,
							  7,7,7,7,7,7,7,5,5,5,5,7,7,7,6,6,6,5,5,5,6,6,6,6,6,7,7,7,7,
							  7,7,7,7,7,7,7,5,5,5,5,7,7,7,6,6,6,5,5,5,6,6,6,6,6,7,7,7,7,
							  7,7,7,7,7,7,7,5,5,5,5,7,7,7,6,6,6,5,5,5,6,6,6,6,6,7,7,7,7,
							  5,5,5,7,7,7,7,7,5,5,5,7,7,7,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,
							  6,7,7,6,6,6,6,5,7,7,7,5,5,5,7,7,7,6,6,7,6,6,6,6,6,7,7,7,7,
							  6,7,7,6,6,6,6,5,7,7,7,5,5,5,7,7,7,6,6,7,6,6,6,6,6,7,7,7,7,  // 14
							  6,7,7,6,6,6,6,5,7,7,7,5,5,5,7,7,7,6,6,7,6,6,6,6,6,7,7,7,7,  // 15
							  6,5,5,5,5,5,5,7,5,5,5,5,5,5,5,5,5,7,7,7,6,6,6,6,6,7,7,7,7,
							  6,5,5,5,5,5,5,7,5,5,5,5,5,5,5,5,5,7,7,7,6,6,6,6,6,7,7,7,7,  // 17
							  6,5,5,5,5,5,5,7,5,5,5,5,5,5,5,5,5,7,7,7,6,6,6,6,6,7,7,7,7,  // 18
							  6,5,5,5,5,5,5,7,5,5,5,5,5,5,5,5,5,7,7,7,6,6,6,6,6,7,7,7,7,
							  5,6,6,5,5,5,5,5,6,6,6,6,6,6,5,5,5,6,6,5,6,6,6,6,6,6,6,6,6,
							  6,5,5,7,7,7,7,7,7,7,7,6,6,6,6,6,6,7,7,5,6,6,6,6,6,6,6,6,6,
							  6,5,5,7,7,7,7,7,7,7,7,6,6,6,6,6,6,7,7,5,6,6,6,6,6,6,6,6,6,
							  6,5,5,7,7,7,7,7,7,7,7,6,6,6,6,6,6,7,7,5,6,6,6,6,6,6,6,6,6,
							  6,5,5,7,7,7,7,7,7,7,7,6,6,6,6,6,6,7,7,5,6,6,6,6,6,6,6,6,6,
							  5,7,7,7,7,7,7,7,6,6,6,5,5,5,6,6,6,7,7,5,5,5,5,5,5,5,5,5,5,
							  5,7,7,7,7,7,7,7,6,6,6,5,5,5,6,6,6,7,7,5,5,5,5,5,5,5,5,5,5,
							  7,6,6,7,7,7,7,7,5,5,5,5,5,5,5,5,5,7,7,7,5,5,5,5,5,6,6,6,6,
							  7,6,6,7,7,7,7,7,5,5,5,5,5,5,5,5,5,7,7,7,5,5,5,5,5,6,6,6,6};


	unsigned image_data4[] = {7,7,7,7,7,5,5,5,7,7,7,7,5,6,6,7,7,7,7,6,5,5,6,6,7,7,5,5,5,
							  7,7,7,7,7,5,5,5,7,7,7,7,5,6,6,7,7,7,7,6,5,5,6,6,7,7,5,5,5,
							  7,7,7,7,7,5,5,5,7,7,7,7,5,6,6,7,7,7,7,6,5,5,6,6,7,7,5,5,5,
							  5,5,5,5,5,6,6,6,5,5,5,5,7,5,5,7,7,7,7,7,6,6,5,5,5,5,7,7,7,
							  5,5,5,5,5,6,6,6,5,5,5,5,7,5,5,7,7,7,7,7,6,6,5,5,5,5,7,7,7,
							  5,5,5,5,5,6,6,6,5,5,5,5,7,5,5,7,7,7,7,7,6,6,5,5,5,5,7,7,7,
							  7,7,7,7,7,7,7,7,7,7,7,7,7,5,5,7,7,7,7,7,5,5,5,5,5,5,6,6,6,
							  7,7,7,7,7,6,6,6,7,7,7,7,5,7,7,7,7,7,7,5,7,7,5,6,7,7,7,7,7,
							  7,7,7,7,7,6,6,6,7,7,7,7,5,7,7,7,7,7,7,5,7,7,5,6,7,7,7,7,7,
							  7,7,7,7,7,6,6,6,7,7,7,7,5,7,7,7,7,7,7,5,7,7,5,6,7,7,7,7,7,
							  6,6,6,6,6,7,7,7,7,7,7,7,5,7,7,6,6,6,6,6,5,5,6,6,6,6,5,5,5,
							  6,6,6,6,6,7,7,7,7,7,7,7,5,7,7,6,6,6,6,6,5,5,6,6,6,6,5,5,5,
							  6,6,6,6,6,7,7,7,7,7,7,7,5,7,7,6,6,6,6,6,5,5,6,6,6,6,5,5,5,
							  6,6,6,6,6,7,7,7,7,7,7,7,5,7,7,6,6,6,6,6,5,5,6,6,6,6,5,5,5,
							  6,6,6,6,6,7,7,7,7,7,7,7,5,7,7,6,6,6,6,6,5,5,6,6,6,6,5,5,5,
							  6,6,6,6,6,6,6,6,5,5,5,5,7,5,5,6,6,6,6,6,5,5,7,7,5,5,5,5,5,
							  5,5,5,5,5,5,5,5,6,6,6,6,6,5,5,5,5,5,5,7,6,6,7,6,7,7,7,7,7,
							  5,5,5,5,5,5,5,5,6,6,6,6,6,5,5,5,5,5,5,7,6,6,7,6,7,7,7,7,7,
							  5,5,5,5,5,7,7,7,6,6,6,6,5,7,7,6,6,6,6,7,5,5,5,5,6,6,7,7,7,
							  5,5,5,5,5,5,5,5,7,7,7,7,5,5,5,7,7,7,7,7,7,7,5,5,6,6,6,6,6,
							  5,5,5,5,5,5,5,5,7,7,7,7,5,5,5,7,7,7,7,7,7,7,5,5,6,6,6,6,6,
							  5,5,5,5,5,5,5,5,7,7,7,7,5,5,5,7,7,7,7,7,7,7,5,5,6,6,6,6,6,
							  5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,7,7,7,7,6,6,6,7,5,7,7,6,6,6,
							  5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,7,7,7,7,6,6,6,7,5,7,7,6,6,6,  // 23
							  5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,7,7,7,7,6,6,6,7,5,7,7,6,6,6,
							  5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,7,7,7,7,6,6,6,7,5,7,7,6,6,6,
							  5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,7,7,7,7,6,6,6,7,5,7,7,6,6,6,
							  5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,7,7,7,7,6,6,6,7,5,7,7,6,6,6,
							  5,5,5,5,5,5,5,5,5,5,5,5,6,5,5,7,7,7,7,6,6,6,7,5,7,7,6,6,6};
						//                ^
						//                6

	Image image = { image_data, 10, 6 };
	Image image2 = { image_data2, 8, 8 };
	Image image3 = { image_data3, 29, 29 };
	Image image4 = { image_data4, 29, 29 };
	unsigned_array res;
	int i;

	//printf("uabs: %d\n", uabs(-1));

	//i = get_depth(3, 2, 1, 1, image); // -> 0
	//i = get_depth(3, 2, 2, 1, image); // -> 3
	//i = get_depth(2, 2, 1, 1, image); // -> 0
	//i = get_depth(2, 2, 2, 1, image); // -> 2
	//i = get_depth(2, 1, 1, 1, image); // -> 0

	//i = get_depth(2, 4, 1, 6, image2); // -> 0
	//i = get_depth(2, 4, 2, 6, image2); // -> 3
	//i = get_depth(2, 5, 2, 6, image2); // -> 3

	//printf("depth: %d\n", i);

//	printf("depth: %d\n", get_depth(2,22,3,5,image3));
//	printf("depth: %d\n", get_depth(3,22,3,5,image3));
//	printf("depth: %d\n", get_depth(4,2,1,5,image3));
//	printf("depth: %d\n", get_depth(4,3,1,5,image3));
//	printf("depth: %d\n", get_depth(4,4,1,5,image3));
//	printf("depth: %d\n", get_depth(4,22,3,5,image3));
//	printf("depth: %d\n", get_depth(5,3,2,5,image3));
//	printf("depth: %d\n", get_depth(5,4,3,5,image3)); // ?
//	printf("depth: %d\n", get_depth(6,4,1,5,image3));
//	printf("depth: %d\n", get_depth(14,13,1,5,image3));
//	printf("depth: %d\n", get_depth(16,12,1,5,image3));
//	printf("depth: %d\n", get_depth(17,11,1,5,image3));
//	printf("depth: %d\n", get_depth(17,12,1,5,image3));
//	printf("depth: %d\n", get_depth(17,13,1,5,image3));
//	printf("depth: %d\n", get_depth(18,3,1,5,image3));
//	printf("depth: %d\n", get_depth(18,4,2,5,image3));
//	printf("depth: %d\n", get_depth(18,14,1,5,image3));
/*

	exit(1);
*/
	printf("{ ");
	res = central_pixels(image, 1);
	for (i = 0; i < res.size; i++) {
		if (i > 0) printf(", ");
		printf("%u", res.values[i]);
	}
	printf(" }\n");

	printf("{ ");
	res = central_pixels(image, 2);
	for (i = 0; i < res.size; i++) {
		if (i > 0) printf(", ");
		printf("%u", res.values[i]);
	}
	printf(" }\n");

	printf("{ ");
	res = central_pixels(image, 3);
	for (i = 0; i < res.size; i++) {
		if (i > 0) printf(", ");
		printf("%u", res.values[i]);
	}
	printf(" }\n");

	printf("{ ");
	res = central_pixels(image, 4);
	for (i = 0; i < res.size; i++) {
		if (i > 0) printf(", ");
		printf("%u", res.values[i]);
	}
	printf(" }\n");

	printf("{ ");
	res = central_pixels(image, 5);
	for (i = 0; i < res.size; i++) {
		if (i > 0) printf(", ");
		printf("%u", res.values[i]);
	}
	printf(" }\n");

	printf("{ ");
	res = central_pixels(image2, 6);
	for (i = 0; i < res.size; i++) {
		if (i > 0) printf(", ");
		printf("%u", res.values[i]);
	}
	printf(" }\n");

	printf("{ ");
	res = central_pixels(image3, 5);
	for (i = 0; i < res.size; i++) {
		if (i > 0) printf(", ");
		printf("%u", res.values[i]);
	}
	printf(" }\n");
	// [ 80,     109,    118,   119,   120,   138,    148,   (149), 178,   476,     504,     (505),   506,     525,    (526),  536 ]
	// { {2,22}, {3,22}, {4,2}, {4,3}, {4,4}, {4,22}, {5,3}, {5,4}, {6,4}, {16,12}, {17,11}, {17,12}, {17,13}, {18,3}, {18,4}, {18,14}

	printf("{ ");
	res = central_pixels(image4, 5);
	for (i = 0; i < res.size; i++) {
		if (i > 0) printf(", ");
		printf("%u", res.values[i]);
	}
	printf(" }\n");
}