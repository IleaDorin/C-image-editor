// structs.h
#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
	int x1, x2, y1, y2;
} selected_t;

typedef struct {
	int red;
	int green;
	int blue;
} rgb;

//	next 2 structurest represent the 2 types of images

typedef struct {
	char type[2];
	int width, height;
	int max_value;
	int **image;	//	the actual pixels
	selected_t selected;
} grayscale_t;

typedef struct {
	char type[2];
	int width, height;
	int max_value;
	rgb **image;	//	the actual pixels
	selected_t selected;
} color_t;

#endif // STRUCTS_H
