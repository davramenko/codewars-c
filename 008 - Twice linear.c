#include <stdio.h>
#include <stdlib.h>

// Twice linear

int dblLinear(int n) {
	int* u = NULL;
	int res, i, j = 0, x = 0, y = 0;

	u = (int*)calloc(n * 2, sizeof(int));
	u[j++] = 1;
	for (i = 0; i < n; i++) {
		int nextX = 2 * u[x] + 1;
		int nextY = 3 * u[y] + 1;
		if (nextX <= nextY) {
			u[j++] = nextX;
			x++;
			if (nextX == nextY)
				y++;
		} else {
			u[j++] = nextY;
			y++;
		}
	}
	res = u[n];
	free(u);
	return res;
}

/*
function dblLinear($n) {
	$u = [1];
	$x = 0;
	$y = 0;
	for ($i = 0; $i < $n; $i++) {
		$nextX = 2 * $u[$x] + 1;
		$nextY = 3 * $u[$y] + 1;
		if ($nextX <= $nextY) {
			array_push($u, $nextX);
			$x++;
			if ($nextX == $nextY)
				$y++;
		} else {
			array_push($u, $nextY);
			$y++;
		}
	}
	return $u[$n];
}
*/

int main() {
	printf("%d\n", dblLinear(10));
	printf("%d\n", dblLinear(20));
	printf("%d\n", dblLinear(30));
	printf("%d\n", dblLinear(50));
}
