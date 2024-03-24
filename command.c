// command.c
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//	function that opens a file and returns the type 
char* file_type(const char *filename) {
	FILE *file_in = fopen(filename, "r");
	if (file_in == NULL) {
		printf("Failed to load %s\n", filename);
		return NULL;
	}
	//	find out the type of file
	char *type = malloc(2);
	if (type == NULL) {
		printf("Memory allocation failed\n");
		fclose(file_in);
		return NULL;
	}
	fscanf(file_in, "%s", type);
	fclose(file_in);

	return type;
}


grayscale_t load_grayscale(const char *filename, const char *type) {
	grayscale_t gray_img;
	if (strcmp(type, "P2") == 0) {
		// open a txt file
		FILE *file_in = fopen(filename, "rt");
		if (file_in == NULL) {
			printf("Failed to load %s\n", filename);
			grayscale_t error;
			strcpy(error.type, "fl");
			return error;
		}
		// Read image type, width, height, max_value
		fscanf(file_in, "%*s %d %d %d", &gray_img.width, &gray_img.height, &gray_img.max_value);

		// Allocate memory for image
		gray_img.image = malloc(gray_img.height * sizeof(int *));
		if (gray_img.image == NULL) {
			printf("Memory allocation failed\n");
			fclose(file_in);
			printf("Failed to load %s\n", filename);
			grayscale_t error;
			strcpy(error.type, "fl");
			return error;
		}
		for (int i = 0; i < gray_img.height; i++) {
			gray_img.image[i] = malloc(gray_img.width * sizeof(int));
			if (gray_img.image[i] == NULL) {
				printf("Memory allocation failed\n");
				fclose(file_in);
				printf("Failed to load %s\n", filename);
				grayscale_t error;
				strcpy(error.type, "fl");
				return error;
			}
		}

		// Read pixel data
		for (int i = 0; i < gray_img.height; i++) {
			for (int j = 0; j < gray_img.width; j++) {
				fscanf(file_in, "%d", &gray_img.image[i][j]);
			}
		}

		//	select the entire image
		gray_img.selected.x1 = 0;
		gray_img.selected.y1 = 0;
		gray_img.selected.x2 = gray_img.width;
		gray_img.selected.y2 = gray_img.height;

		//	write the img type
		strcpy(gray_img.type, "P2");
		fclose(file_in);
	} else if (strcmp(type, "P5") == 0) {
		//	open a binary file
		FILE *file_in = fopen(filename, "rb");
		if (file_in == NULL) {
			printf("Failed to load %s\n", filename);
			grayscale_t error;
			strcpy(error.type, "fl");
			return error;
		}

		// Read header: skip format, read width, height, max value
		fscanf(file_in, "%*s %d %d %d", &gray_img.width, &gray_img.height, &gray_img.max_value);
		fgetc(file_in);  // Read the newline character after the header

		//	Allocate mem for the content buffer
		unsigned char* buffer = malloc((gray_img.width) * (gray_img.height));
		if (buffer == NULL) {
			perror("Memory allocation failed");
			fclose(file_in);
			printf("Failed to load %s\n", filename);
			grayscale_t error;
			strcpy(error.type, "fl");
			return error;
		}

		// Read pixel data
		fread(buffer, 1, gray_img.height * gray_img.width, file_in);

		// Allocate memory for image
		gray_img.image = malloc(gray_img.height * sizeof(int *));
		if (gray_img.image == NULL) {
			printf("Memory allocation failed\n");
			fclose(file_in);
			printf("Failed to load %s\n", filename);
			grayscale_t error;
			strcpy(error.type, "fl");
			return error;
		}
		for (int i = 0; i < gray_img.height; i++) {
			gray_img.image[i] = malloc(gray_img.width * sizeof(int));
			if (gray_img.image[i] == NULL) {
				printf("Memory allocation failed\n");
				fclose(file_in);
				printf("Failed to load %s\n", filename);
				grayscale_t error;
				strcpy(error.type, "fl");
				return error;
			}
			//	copy the data to the img
			for (int j = 0; j < gray_img.width; j++) {
				gray_img.image[i][j] = (int)(buffer[i * (gray_img.width) + j]);
			}
		}
		//	free the buffer 
		free(buffer);

		// Set selected area and type
		gray_img.selected.x1 = 0;
		gray_img.selected.y1 = 0;
		gray_img.selected.x2 = gray_img.width;
		gray_img.selected.y2 = gray_img.height;
		strcpy(gray_img.type, "P5");

		fclose(file_in);
	}
	//  print message
	printf("Loaded %s\n", filename);
	fflush(stdout);

	return gray_img;
}

color_t load_color(const char *filename, const char *type) {
	color_t color_img;
	FILE *file_in;

	if (strcmp(type, "P3") == 0) {
		file_in = fopen(filename, "rt");
		if (file_in == NULL) {
			printf("Failed to load %s\n", filename);
			color_t error;
			strcpy(error.type, "fl");
			return error;
		}

		fscanf(file_in, "%*s %d %d %d", &color_img.width, &color_img.height, &color_img.max_value);
		color_img.image = malloc(color_img.height * sizeof(rgb *));
		if (color_img.image == NULL) {
			printf("Memory allocation failed\n");
			fclose(file_in);
			printf("Failed to load %s\n", filename);
			color_t error;
			strcpy(error.type, "fl");
			return error;
		}
		for (int i = 0; i < color_img.height; i++) {
			color_img.image[i] = malloc(color_img.width * sizeof(rgb));
			if (color_img.image[i] == NULL) {
				printf("Memory allocation failed\n");
				fclose(file_in);
				printf("Failed to load %s\n", filename);
				color_t error;
				strcpy(error.type, "fl");
				return error;
			}
			for (int j = 0; j < color_img.width; j++) {
				fscanf(file_in, "%d %d %d", &color_img.image[i][j].red,
					&color_img.image[i][j].green, &color_img.image[i][j].blue);
			}
		}

		strcpy(color_img.type, "P3");
		fclose(file_in);
	} else if (strcmp(type, "P6") == 0) {
		file_in = fopen(filename, "rb");
		if (file_in == NULL) {
			printf("Failed to load %s\n", filename);
			color_t error;
			strcpy(error.type, "fl");
			return error;
		}

		fscanf(file_in, "%*s %d %d %d", &color_img.width, &color_img.height, &color_img.max_value);
		fgetc(file_in);  // Read the newline character after the header

		//	allocate mem for the content buffer
		unsigned char* buffer = malloc((color_img.width) * (color_img.height) * 3);
		if (buffer == NULL) {
			perror("Memory allocation failed");
			fclose(file_in);
		}
		// Read pixel data
		fread(buffer, 1, color_img.height * color_img.width * 3, file_in);

		color_img.image = malloc(color_img.height * sizeof(rgb *));
		if (color_img.image == NULL) {
			printf("Memory allocation failed\n");
			fclose(file_in);
			printf("Failed to load %s\n", filename);
			color_t error;
			strcpy(error.type, "fl");
			return error;
		}
		for (int i = 0; i < color_img.height; i++) {
			color_img.image[i] = malloc(color_img.width * sizeof(rgb));
			if (color_img.image[i] == NULL) {
				printf("Memory allocation failed\n");
				fclose(file_in);
				printf("Failed to load %s\n", filename);
				color_t error;
				strcpy(error.type, "fl");
				return error;
			}
			for (int j = 0; j < color_img.width; j++) {
				int buffer_index = (i * color_img.width + j) * 3;
				color_img.image[i][j].red = buffer[buffer_index];
				color_img.image[i][j].green = buffer[buffer_index + 1];
				color_img.image[i][j].blue = buffer[buffer_index + 2];
			}
		}
		free(buffer);

		strcpy(color_img.type, "P6");
		fclose(file_in);
	}
	// Set selected area and type
	color_img.selected.x1 = 0;
	color_img.selected.y1 = 0;
	color_img.selected.x2 = color_img.width;
	color_img.selected.y2 = color_img.height;

	//  print message
	printf("Loaded %s\n", filename);
	fflush(stdout);
	return color_img;
}

//	functions to free up a image memory
void free_gray_img(int **array, int rows) {
	for (int i = 0; i < rows; i++) {
		free(array[i]);
	}
	free(array);
}
void free_color_img(rgb **array, int rows) {
	for (int i = 0; i < rows; i++) {
		free(array[i]);
	}
	free(array);
}

void swap_if_greater(int *a, int *b) {
	if (*a > *b) {
		int temp = *a;
		*a = *b;
		*b = temp;
	}
}

void normalize_selected(selected_t *selected) {
	swap_if_greater(&selected->x1, &selected->x2);
	swap_if_greater(&selected->y1, &selected->y2);
}

int check_coords_gray(grayscale_t gray_img, selected_t coord) {
	if (coord.x1 == -31415 || coord.x2 == -31415 || coord.y1 == -31415 || coord.y2 == -31415) {
		return -1;
	}
	if (coord.x2 > gray_img.width || coord.x1 < 0) {
		return 0;
	}
	if (coord.y2 > gray_img.height || coord.y1 < 0) {
		return 0;
	}
	if (coord.x1 == coord.x2 || coord.y1 == coord.y2) {
		return 0;
	}
	
	return 1;
}

void update_selection_gray(grayscale_t *gray_img, selected_t coord) {
	gray_img->selected.x1 = coord.x1;
	gray_img->selected.y1 = coord.y1;
	gray_img->selected.x2 = coord.x2;
	gray_img->selected.y2 = coord.y2;
	//  print the success message
	printf("Selected %d %d %d %d\n", coord.x1, coord.y1, coord.x2, coord.y2);
	fflush(stdout);
}

int check_coords_color(color_t color_img, selected_t coord) {
	if (coord.x1 == -31415 || coord.x2 == -31415 || coord.y1 == -31415 || coord.y2 == -31415) {
		return -1;
	}
	if (coord.x2 > color_img.width || coord.x1 < 0) {
		return 0;
	}
	if (coord.y2 > color_img.height || coord.y1 < 0) {
		return 0;
	}
	if (coord.x1 == coord.x2 || coord.y1 == coord.y2) {
		return 0;
	}
	return 1;
}

void update_selection_color(color_t *color_img, selected_t coord) {
	if(check_coords_color(*color_img, coord)) {
		color_img->selected.x1 = coord.x1;
		color_img->selected.y1 = coord.y1;
		color_img->selected.x2 = coord.x2;
		color_img->selected.y2 = coord.y2;
		// Print success message
		printf("Selected %d %d %d %d\n", coord.x1, coord.y1, coord.x2, coord.y2);
		fflush(stdout);
	} else {
		printf("Selection coordinates are out of bounds.\n");
	}
}

void save_image_gray(grayscale_t img, const char *filename, int ascii) {
	FILE *file_out = fopen(filename, ascii ? "w" : "wb");
	if (file_out == NULL) {
		printf("Failed to save %s\n", filename);
		return;
	}

	// Write header
	fprintf(file_out, ascii ? "P2\n" : "P5\n");
	fprintf(file_out, "%d %d\n", img.width, img.height);
	fprintf(file_out, "%d\n", img.max_value);

	// Write pixel data
	for (int i = 0; i < img.height; i++) {
		for (int j = 0; j < img.width; j++) {
			if (ascii) {
				fprintf(file_out, "%d ", img.image[i][j]);
			} else {
				unsigned char pixel = (unsigned char) img.image[i][j];
				fwrite(&pixel, sizeof(unsigned char), 1, file_out);
			}
		}
		if (ascii) {
			fprintf(file_out, "\n");
		}
	}

	fclose(file_out);
	printf("Saved %s\n", filename);
	fflush(stdout);
}

void save_image_color(color_t img, const char *filename, int ascii) {
	FILE *file_out = fopen(filename, ascii ? "w" : "wb");
	if (file_out == NULL) {
		printf("Failed to save %s\n", filename);
		return;
	}

	// Write header
	fprintf(file_out, ascii ? "P3\n" : "P6\n");
	fprintf(file_out, "%d %d\n", img.width, img.height);
	fprintf(file_out, "%d\n", img.max_value);

	if (ascii) {
		for (int i = 0; i < img.height; i++) {
			for (int j = 0; j < img.width; j++) {
				fprintf(file_out, "%d %d %d ", img.image[i][j].red, img.image[i][j].green, img.image[i][j].blue);
			}
			fprintf(file_out, "\n");
		}
	} else {
		// Write pixel data in binary format
		for (int i = 0; i < img.height; i++) {
			for (int j = 0; j < img.width; j++) {
				unsigned char colors[3];
				colors[0] = (unsigned char)img.image[i][j].red;
				colors[1] = (unsigned char)img.image[i][j].green;
				colors[2] = (unsigned char)img.image[i][j].blue;
				fwrite(colors, sizeof(unsigned char), 3, file_out);
			}
		}
	}

	fclose(file_out);
	printf("Saved %s\n", filename);
	fflush(stdout);
}

int** crop_array_gray(int** array, int x1, int y1, int x2, int y2) {
	int rows = y2 - y1;
	int cols = x2 - x1;

	int** cropped = malloc(rows * sizeof(int*));
	if (cropped == NULL) {
		printf("Memory allocation failed\n");
		return array;
	}
	for (int i = 0; i < rows; i++) {
		cropped[i] = malloc(cols * sizeof(int));
		if (cropped[i] == NULL) {
			printf("Memory allocation failed\n");
			return array;
		}
		for (int j = 0; j < cols; j++) {
			cropped[i][j] = array[y1 + i][x1 + j];
		}
	}

	return cropped;
}

rgb** crop_array_color(rgb** array, int x1, int y1, int x2, int y2) {
	int rows = y2 - y1;
	int cols = x2 - x1;

	rgb** cropped = malloc(rows * sizeof(rgb*));
	if (cropped == NULL) {
		printf("Memory allocation failed\n");
		return array;
	}
	for (int i = 0; i < rows; i++) {
		cropped[i] = malloc(cols * sizeof(rgb));
		if (cropped[i] == NULL) {
			printf("Memory allocation failed\n");
			return array;
		}
		for (int j = 0; j < cols; j++) {
			cropped[i][j] = array[y1 + i][x1 + j];
		}
	}

	return cropped;
}

void crop_gray(grayscale_t *gray_img) {
	int new_width = gray_img->selected.x2 - gray_img->selected.x1;
	int new_height = gray_img->selected.y2 - gray_img->selected.y1;

	int **new_image = crop_array_gray(gray_img->image, gray_img->selected.x1,
		gray_img->selected.y1, gray_img->selected.x2, gray_img->selected.y2);

	// Free the old image memory
	free_gray_img(gray_img->image, gray_img->height);

	// Update the original image structure
	gray_img->image = new_image;
	gray_img->width = new_width;
	gray_img->height = new_height;
	gray_img->selected.x1 = 0;
	gray_img->selected.y1 = 0;
	gray_img->selected.x2 = new_width;
	gray_img->selected.y2 = new_height;

	printf("Image cropped\n");
	fflush(stdout);
}

void crop_color(color_t *color_img) {
	int new_width = color_img->selected.x2 - color_img->selected.x1;
	int new_height = color_img->selected.y2 - color_img->selected.y1;

	rgb **new_image = crop_array_color(color_img->image, color_img->selected.x1,
		color_img->selected.y1, color_img->selected.x2, color_img->selected.y2);

	// Free the old image memory
	free_color_img(color_img->image, color_img->height); // Implement free_color_img similarly to free_gray_img

	// Update the original image structure
	color_img->image = new_image;
	color_img->width = new_width;
	color_img->height = new_height;
	color_img->selected.x1 = 0;
	color_img->selected.y1 = 0;
	color_img->selected.x2 = new_width;
	color_img->selected.y2 = new_height;

	printf("Image cropped\n");
	fflush(stdout);
}

void select_all_gray(grayscale_t *gray_img) {
	gray_img->selected.x1 = 0;
	gray_img->selected.y1 = 0;
	gray_img->selected.x2 = gray_img->width;
	gray_img->selected.y2 = gray_img->height;
}

void select_all_color(color_t *color_img) {
	color_img->selected.x1 = 0;
	color_img->selected.y1 = 0;
	color_img->selected.x2 = color_img->width;
	color_img->selected.y2 = color_img->height;
}

//  FOR APPLY

// Function to create an empty image with given dimensions
rgb **create_empty_image(int width, int height) {
	rgb **image = malloc(height * sizeof(rgb *));
	if (image == NULL) {
		printf("Memory allocation failed\n");
		return NULL;
	}
	if (image) {
		for (int i = 0; i < height; i++) {
			image[i] = calloc(width, sizeof(rgb));
			if (image[i] == NULL) {
				printf("Memory allocation failed\n");
				return NULL;
			}
			if (!image[i]) {
				while (i > 0) {
					free(image[--i]);
				}
				free(image);
				return NULL;
			}
		}
	}
	return image;
}

// Function to copy image data from one image to another
void copy_image_data(rgb **source, rgb **destination, int width, int height) {
	for (int i = 0; i < height; i++) {
		memcpy(destination[i], source[i], width * sizeof(rgb));
	}
}

int clamp(int value) {
	if (value < 0) return 0;
	if (value > 255) return 255;
	return value;
}

void apply_kernel_to_pixel(rgb **image, int x, int y, int width, int height, const float kernel[3][3], rgb *result) {
	float red = 0.0, green = 0.0, blue = 0.0;
	for (int dy = -1; dy <= 1; dy++) {
		for (int dx = -1; dx <= 1; dx++) {
			int new_x = x + dx;
			int new_y = y + dy;
			if (new_x >= 0 && new_x < width && new_y >= 0 && new_y < height) {
				red += image[new_y][new_x].red * kernel[dy + 1][dx + 1];
				green += image[new_y][new_x].green * kernel[dy + 1][dx + 1];
				blue += image[new_y][new_x].blue * kernel[dy + 1][dx + 1];
			}
		}
	}
	result->red = clamp((int)red);
	result->green = clamp((int)green);
	result->blue = clamp((int)blue);
}

void apply_filter(color_t *img, const char *filter) {
	const float edge_kernel[3][3] = {{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};
	const float sharpen_kernel[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
	const float blur_kernel[3][3] = {{1.0/9, 1.0/9, 1.0/9}, {1.0/9, 1.0/9, 1.0/9}, {1.0/9, 1.0/9, 1.0/9}};
	const float gaussian_blur_kernel[3][3] = {{1.0/16, 2.0/16, 1.0/16}, {2.0/16, 4.0/16, 2.0/16}, {1.0/16, 2.0/16, 1.0/16}};

	// Choose the appropriate kernel based on the filter parameter
	const float (*kernel)[3][3] = NULL;
	if (strcmp(filter, "EDGE") == 0) {
		kernel = &edge_kernel;
	} else if (strcmp(filter, "SHARPEN") == 0) {
		kernel = &sharpen_kernel;
	} else if (strcmp(filter, "BLUR") == 0) {
		kernel = &blur_kernel;
	} else if (strcmp(filter, "GAUSSIAN_BLUR") == 0) {
		kernel = &gaussian_blur_kernel;
	} else {
		printf("APPLY parameter invalid\n");
		return;
	}

	// Apply the kernel to each pixel within the selected region
	rgb **new_image = create_empty_image(img->width, img->height);
	copy_image_data(img->image, new_image, img->width, img->height);
	for (int y = img->selected.y1; y < img->selected.y2; y++) {
		for (int x = img->selected.x1; x < img->selected.x2; x++) {
			// Check if pixel is not on the edge to have enough neighbors
			if (y > 0 && y < img->height - 1 && x > 0 && x < img->width - 1) {
				apply_kernel_to_pixel(img->image, x, y, img->width, img->height, *kernel, &new_image[y][x]);
			}
		}
	}

	// Replace old image data with the new image data
	free_color_img(img->image, img->height);
	img->image = new_image;

	printf("APPLY %s done\n", filter);
}

void print_histogram_grayscale(grayscale_t *img, int max_stars, int num_bins) {
	int histogram[256] = {0};
	for (int y = 0; y < img->height; ++y) {
		for (int x = 0; x < img->width; ++x) {
			histogram[img->image[y][x]]++;
		}
	}

	int consolidated_histogram[num_bins];
	int bin_size = 256 / num_bins;
	for (int i = 0; i < num_bins; i++) {
		consolidated_histogram[i] = 0;
		for (int j = 0; j < bin_size; j++) {
			consolidated_histogram[i] += histogram[i * bin_size + j];
		}
	}

	int max_frequency = 0;
	for (int i = 0; i < num_bins; ++i) {
		if (consolidated_histogram[i] > max_frequency) {
			max_frequency = consolidated_histogram[i];
		}
	}

	for (int i = 0; i < num_bins; ++i) {
		int stars_to_print = (consolidated_histogram[i] * max_stars) / max_frequency;
		printf("%d\t|\t", stars_to_print);
		for (int j = 0; j < stars_to_print; j++) {
			printf("*");
		}
		printf("\n");
	}
}

int is_power_of_two(int n) {
	if (n >= 2 && n <= 256 && (n & (n - 1)) == 0) {
		return 1;
	}
	return 0;
}

void equalize_grayscale(grayscale_t *img) {
	int histogram[256] = {0};
	int cdf[256] = {0};
	int area = img->width * img->height;
	float scale_factor = 255.0 / area;

	// Calculate histogram
	for (int y = 0; y < img->height; y++) {
		for (int x = 0; x < img->width; x++) {
			int pixel_value = img->image[y][x];
			histogram[pixel_value]++;
		}
	}

	int sum = 0;
	for (int i = 0; i < 256; i++) {
		sum += histogram[i];
		cdf[i] = (int)(scale_factor * sum);
	}

	// Apply the equalization
	for (int y = 0; y < img->height; y++) {
		for (int x = 0; x < img->width; x++) {
			int old_value = img->image[y][x];
			int new_value = cdf[old_value];
			img->image[y][x] = clamp(new_value);
		}
	}

	printf("Equalize done\n");
}

