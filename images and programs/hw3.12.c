#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS   		 		 128
#define COLS				 128
#define LOGICAL_X_MIN			-4.0
#define LOGICAL_X_MAX			 4.0
#define LOGICAL_Y_MIN			-4.0
#define LOGICAL_Y_MAX			 4.0

void clear(unsigned char image[][COLS]);
int plot_logical_point(float x, float y, unsigned char image[][COLS]);
int plot_physical_point(int x, int y, unsigned char image[][COLS]);
int in_range(int x, int y);
void header(int row, int col, unsigned char head[32]);/*write header for the output images, do not modify this*/


int main(int argc, char **argv)
{
	int				i;
	float			x1,x2,y1,y2,z0,a,b,c,t, x, y, z, xprime, yprime, xx, yy, zz, xxprime, yyprime;
	float           xbarprime, ybarprime, xxbarprime, yybarprime;
	FILE			*fp;
	char			 *ofile;
	unsigned char	image[ROWS][COLS];
	unsigned char head[32];

	clear(image);

	x1 = -1 ;
	x2 = 1 ;
	y1 = -1;
	y2 = -1;
	z0 = 0;
	a = 0 ;
	b = -1 ;
	c = -1 ;


	for (t = 0.01; t < 10000.01; t += .01)  //change the t range
	{
		/*Modify the coefficiences according to questions*/


		x = x1 +t*a ;
		y = y1 + t*b;
		z = z0 + t*c;
		xx = x2 + t*a;
		yy = y2 + t*b;
		zz = z0 + t*c;
	


		xprime = 1.0 * x / z; /* this is an example of pers */
		yprime = 1.0 * y / z;

		xxprime = 1.0 * xx / zz;
		yyprime = 1.0 * yy / zz;

		//xbarprime = x;
		//ybarprime = y;

		//xxbarprime = xx;
		//yybarprime = yy;

		/* Modify these according to the projection */
		/* this is an example of orth */

		plot_logical_point(xprime, yprime, image);
		plot_logical_point(xxprime, yyprime, image);
		//plot_logical_point(xbarprime, ybarprime, image);
		//plot_logical_point(xxbarprime, yybarprime, image);

	}
	//_CRT_SECURE_NO_WARNINGS
	/* Create a header */ 
	header(ROWS, COLS, head);

	ofile = "hw1ex3.12.ras";// this is the name of your output file, modify the name according to the requirements

	if (!(fp = fopen(ofile, "wb")))
		fprintf(stderr, "error: could not open %s\n", ofile), exit(1);

	/* Write the header */
	fwrite(head, 4, 8, fp);

	for (i = 0; i < ROWS; i++) fwrite(image[i], 1, COLS, fp);
	fclose(fp);

	return 0;
}

void clear(unsigned char image[][COLS])
{
	int	i, j;
	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLS; j++) image[i][j] = 0;
}

int plot_logical_point(float x, float y, unsigned char image[][COLS])
{
	int	nx, ny;
	float	xc, yc;
	xc = COLS / ((float)LOGICAL_X_MAX - LOGICAL_X_MIN);
	yc = ROWS / ((float)LOGICAL_Y_MAX - LOGICAL_Y_MIN);
	nx = (x - LOGICAL_X_MIN) * xc;
	ny = (y - LOGICAL_Y_MIN) * yc;
	return plot_physical_point(nx, ny, image);
}

int plot_physical_point(int x, int y, unsigned char image[][COLS])
{
	if (in_range(x, y)) //return 0;
	return image[y][x] = 255;
}
int in_range(int x, int y)
{
	return x >= 0 && x < COLS && y >= 0 && y < ROWS;
}

void header(int row, int col, unsigned char head[32])
{
	int *p = (int *)head;
	char *ch;
	int num = row * col;

	/* Choose little-endian or big-endian header depending on the machine. Don't modify the contents */
	
	/* Little-endian for PC */
	*p = 0x956aa659;
	*(p + 3) = 0x08000000;
	*(p + 5) = 0x01000000;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8000000;

	ch = (char*)&col;
	head[7] = *ch;
	ch++;
	head[6] = *ch;
	ch++;
	head[5] = *ch;
	ch++;
	head[4] = *ch;

	ch = (char*)&row;
	head[11] = *ch;
	ch++;
	head[10] = *ch;
	ch++;
	head[9] = *ch;
	ch++;
	head[8] = *ch;

	ch = (char*)&num;
	head[19] = *ch;
	ch++;
	head[18] = *ch;
	ch++;
	head[17] = *ch;
	ch++;
	head[16] = *ch;

	/*
	// Big-endian for unix
	*p = 0x59a66a95;
	*(p + 1) = col;
	*(p + 2) = row;
	*(p + 3) = 0x8;
	*(p + 4) = num;
	*(p + 5) = 0x1;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8;
	*/
}