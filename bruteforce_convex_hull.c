#include <stdlib.h> // atoi, rand, malloc, realloc
#include <stdio.h>
#include <time.h> //time

#define RANGE 10000

typedef struct
{
	int x;
	int y;
} t_point;

typedef struct
{
	t_point from;
	t_point to;
} t_line;

////////////////////////////////////////////////////////////////////////////////
void print_header( char *filename)
{
	printf( "#! /usr/bin/env Rscript\n");
	printf( "png(\"%s\", width=700, height=700)\n", filename);
	
	printf( "plot(1:%d, 1:%d, type=\"n\")\n", RANGE, RANGE);
}
////////////////////////////////////////////////////////////////////////////////
void print_footer( void)
{
	printf( "dev.off()\n");
}

////////////////////////////////////////////////////////////////////////////////
/*
#points
points(2247,7459)
points(616,2904)
points(5976,6539)
points(1246,8191)
*/
void print_points( t_point *points, int num_point)
{
	printf("#points\n");
	for(int i=0 ; i<num_point ; i++){
		int x = points[i].x;
		int y = points[i].y;
		printf("points(%d,%d)\n",x,y);
	}

}

/*
#line segments
segments(7107,2909,7107,2909)
segments(43,8,5,38)
segments(43,8,329,2)
segments(5047,8014,5047,8014)
*/
void print_line_segments( t_line *lines, int num_line)
{
	printf("#line segments");
	for(int i =0;i< num_line;i++){
		int from_x = lines[i].from.x;
		int from_y = lines[i].from.y;
		int to_x = lines[i].to.x;
		int to_y = lines[i].to.y;
		printf("segments(%d,%d,%d,%d)\n",from_x,from_y,to_x,to_y);  

	}
}

// [input] points : set of points
// [input] num_point : number of points
// [output] num_line : number of line segments that forms the convex hull
// return value : set of line segments that forms the convex hull

// 리스트 점들 중 2개를 고른다. 
	// 2개의 점으로 직선을 만든다. 
	// 기울기는 (y2-y1)/(x2-x1) = m
	// 직선은 y - y1 = m(x-x1) -> y = m(x-x1)+y1
	// ky가 
	// 두 직선을 제외한 모든 점들이  x 값을 대입했을 때, y 값보다 큰 지 작은 지 조사
	// 모두 크거나 모두 작은 경우 두 점을 배열 생성해서 할당 + numline 값 올리기
	// lines return하기 
t_line *convex_hull( t_point *points, int num_point, int *num_line)
{
	*num_line = 0;
	t_line *convex_lines = (t_line *) calloc( 100, sizeof( t_line));
	// 두 점 선택해서 직선 만들기
	// 
	for(int i = 0; i < num_point-1 ; i++){
		for(int j = i+1; j < num_point ; j++){
			if(i == j){
				continue;
			}
			float inclination = (points[j].y - points[i].y)/(points[j].x - points[i].x);
			int zerocount = 0;
			int upcount = 0;
			int downcount = 0;
			// 직선과 나머지 점들 간의 위 아래 비교
			// 직선을 기준으로 
			for(int k =0; k<num_point; k++){
				float check_num = inclination*(points[k].x-points[j].x) + points[j].y - points[k].y;
				if(check_num>0){
					upcount ++;
				}
				else if(check_num<0){
					downcount ++;
				}
				else{
					zerocount++;
				}
			}
			// lines에 추가하기 
			if((upcount <1 && downcount>0) || (downcount <1 && upcount >0)){
				convex_lines[*num_line].from.x = points[i].x;
				convex_lines[*num_line].from.y = points[i].y;
				convex_lines[*num_line].to.x = points[j].x;
				convex_lines[*num_line].to.y = points[j].y;
				*num_line += 1;
			}
		}


	}

	return convex_lines;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int x, y;
	int num_point; // number of points
	int num_line; // number of lines
	
	if (argc != 2)
	{
		printf("%s number_of_points\n", argv[0]);
		return 0;
	}

	num_point = atoi( argv[1]);
	if (num_point <= 0)
	{
		printf( "The number of points should be a positive integer!\n");
		return 0;
	}

	t_point *points = (t_point *) malloc( 30 * sizeof( t_point));
		
	t_line *lines;

	// making n points
	srand( time(NULL));
	for (int i = 0; i < num_point; i++)
	{
		x = rand() % RANGE + 1; // 1 ~ RANGE random number
		y = rand() % RANGE + 1;
		
		points[i].x = x;
		points[i].y = y;
 	}

	fprintf( stderr, "%d points created!\n", num_point);

	print_header( "convex.png");
	
	print_points( points, num_point);
	
	lines = convex_hull( points, num_point, &num_line);

	fprintf( stderr, "%d lines created!\n", num_line);

	print_line_segments( lines, num_line);
	
	print_footer();
	
	free( points);
	free( lines);
	
	return 0;
}

