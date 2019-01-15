#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Centre of attention
// https://www.codewars.com/kata/centre-of-attention/train/c

typedef struct { unsigned *pixels; unsigned width, height; } Image;
typedef struct { unsigned *values; unsigned size; } unsigned_array;

#if !defined(min)
#define min(a, b) ((a < b) ? a : b)
#endif
#define pix(img, row, col) (img.pixels[(row) * img.width + (col)])
#define ppix(pimg, row, col) (pimg->pixels[(row) * pimg->width + (col)])

void set_depth(Image* pimage, Image* pdist, int row, int col) {
	int color = ppix(pimage, row, col);

	if (ppix(pimage, row - 1, col) == color &&
		ppix(pimage, row + 1, col) == color &&
		ppix(pimage, row, col - 1) == color &&
		ppix(pimage, row, col + 1) == color
		) {
		ppix(pdist, row, col) = 1 + min(
			min(ppix(pdist, row - 1, col), ppix(pdist, row + 1, col)),
			min(ppix(pdist, row, col - 1), ppix(pdist, row, col + 1))
		);
	} else {
		ppix(pdist, row, col) = 1;
	}
}


unsigned_array central_pixels(Image image, unsigned colour) {
	unsigned_array res = {NULL, 0};
	unsigned res_cap = 1024;
	unsigned i, row, col, l;
	unsigned dmax = 0;
	unsigned dmaxval = image.width * image.height;
	unsigned* dstbuff = calloc(image.width * image.height, sizeof(unsigned));
	Image dist = { dstbuff, image.width, image.height };

	//printf("dist.width=%d, dist.height=%d\n", dist.width, dist.height);
	//printf("---=== 1 ===---\n");
	for (i = 0; i < (dist.width * dist.height); i++)
		dist.pixels[i] = dmaxval;
	//printf("---=== 1.a ===---\n");
	for (col = 0; col < dist.width; col++) {
		pix(dist, 0, col) = 1;
		//printf("---=== 1.a.1 (col=%d) ===---\n", col);
		pix(dist, dist.height - 1, col) = 1;
		//printf("---=== 1.a.2 (col=%d) ===---\n", col);
	}
	//printf("---=== 1.b ===---\n");
	for (row = 1; row < (dist.height - 1); row++)
		pix(dist, row, 0) = pix(dist, row, dist.width - 1) = 1;

	//printf("---=== 2 ===---\n");
	for (l = 1; l < min(dist.width, dist.height) / 2 + 1; l++) {
		for (col = l; col < dist.width - l; col++) {
			set_depth(&image, &dist, l, col);
			set_depth(&image, &dist, dist.height - l - 1, col);
		}
		for (row = l; row < dist.height - l; row++) {
			set_depth(&image, &dist, row, l);
			set_depth(&image, &dist, row, dist.width - l - 1);
		}
	}
	for (l = min(dist.width, dist.height) / 2; l >= 1; l--) {
		for (col = dist.width - l - 1; col >= l; col--) {
			set_depth(&image, &dist, l, col);
			set_depth(&image, &dist, dist.height - l - 1, col);
		}
		for (row = dist.height - l - 1; row >= l; row--) {
			set_depth(&image, &dist, row, l);
			set_depth(&image, &dist, row, dist.width - l - 1);
		}
	}

	//printf("---=== 3 ===---\n");
	res.values = calloc(res_cap, sizeof(unsigned));
	for (i = 0; i < (dist.width * dist.height); i++) {
		if (image.pixels[i] == colour) {
			if (dist.pixels[i] > dmax) {
				dmax = dist.pixels[i];
				res.size = 0;
				res.values[res.size] = i;
				res.size++;
			} else if (dist.pixels[i] == dmax) {
				if (res.size >= res_cap) {
					unsigned tcap = res_cap * 2;
					unsigned* buff = calloc(tcap, sizeof(unsigned));
					memcpy(buff, res.values, sizeof(unsigned) * res_cap);
					free(res.values);
					res.values = buff;
					res_cap = tcap;
				}
				res.values[res.size] = i;
				res.size++;
			}
		}
	}
	//printf("---=== 4 ===---\n");

	if (res.size == 2 && res.values[0] == 16 && res.values[1] == 17) {
		printf("{%d,%d}; dmax=%d; color: %d\n", image.height, image.width, dmax, colour);
		for (row = 0; row < image.height; row++) {
			for (col = 0; col < image.width; col++) {
				if (col > 0) printf(",");
				printf("%d", pix(image, row, col));
			}
			printf("\n");
		}
		printf("============================\n");
		for (row = 0; row < image.height; row++) {
			for (col = 0; col < image.width; col++) {
				if (col > 0) printf(",");
				printf("%02d", pix(dist, row, col));
			}
			printf("\n");
		}
	}

	free(dstbuff);
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

			 //                      2
             // {7,7}                V
	unsigned image_data5[] = {40,40,40,40,40,40,40,
							  40,40,40,40,40,40,41,
							  40,40,40,40,40,41,41, // 2
							  40,40,40,40,41,41,41,
							  40,40,40,41,41,41,41,
							  40,40,41,41,41,41,41,
							  40,41,41,41,41,41,41};


// {8,8}; dmax=3; color: 5
	unsigned image_data5[] = {6,6,6,6,6,6,6,6,
							  5,6,6,6,6,6,6,6,
							  5,5,6,6,6,6,6,6,
							  5,5,5,6,6,6,6,6,
							  5,5,5,5,6,6,6,6,
							  5,5,5,5,5,6,6,6, // 5
							  5,5,5,5,5,5,6,6,
							  5,5,5,5,5,5,5,6};
						//        ^
						//        2
//============================
//01,01,01,01,01,01,01,01
//01,01,02,02,02,02,02,01
//01,01,01,02,03,03,02,01
//01,02,01,01,02,03,02,01
//01,02,02,01,01,02,02,01
//01,02,03,03,01,01,02,01
//01,02,02,02,02,01,01,01
//01,01,01,01,01,01,01,01

	Image image = { image_data, 10, 6 };
	Image image2 = { image_data2, 8, 8 };
	Image image3 = { image_data3, 29, 29 };
	Image image4 = { image_data4, 29, 29 };
	Image image5 = { image_data5, 7, 7 };
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
*/
	printf("{ ");
	res = central_pixels(image5, 40);
	for (i = 0; i < res.size; i++) {
		if (i > 0) printf(", ");
		printf("%u", res.values[i]);
	}
	printf(" }\n");
}