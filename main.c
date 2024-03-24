#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"

grayscale_t load_grayscale(const char *filename, const char *type);
char *file_type(const char *filename);
void free_gray_img(int **array, int rows);
color_t load_color(const char *filename, const char *type);
void free_color_img(rgb **array, int rows);
void normalize_selected(selected_t *selected);
void swap_if_greater(int *a, int *b);
int check_coords_gray(grayscale_t gray_img, selected_t coord);
void update_selection_gray(grayscale_t *gray_img, selected_t coord);
int check_coords_color(color_t color_img, selected_t coord);
void update_selection_color(color_t *color_img, selected_t coord);
void save_image_gray(grayscale_t img, const char *filename, int ascii);
void save_image_color(color_t img, const char *filename, int ascii);
void crop_color(color_t *color_img);
void crop_gray(grayscale_t *gray_img);
void select_all_gray(grayscale_t *gray_img);
void select_all_color(color_t *color_img);
void apply_filter(color_t *img, const char *filter);
void print_histogram_grayscale(grayscale_t *img, int max_stars, int num_bins);
int is_power_of_two(int n);
void equalize_grayscale(grayscale_t *img);

int main(void)
{
	char input[100]; // Buffer for the entire input line
	char command[50]; // Buffer for the command
	int loaded_gray = 0, loaded_col = 0;
	selected_t coord;
	grayscale_t gray_img;
	char* type;
	color_t color_img;

	while (1) {
		fgets(input, sizeof(input), stdin); // Read the entire line
		sscanf(input, "%s", command); // Extract the first word as command

		if (strcmp(command, "EXIT") == 0) {
			if (loaded_gray) {
				free_gray_img(gray_img.image, gray_img.height);
				break;
			} else if (loaded_col) {
				free_color_img(color_img.image, color_img.height);
				break;
			} else {
				printf("No image loaded\n");
				break;
			}
		} else if (strcmp(command, "LOAD") == 0) {
			if (loaded_gray) {
				free_gray_img(gray_img.image, gray_img.height);
			} else if (loaded_col) {
				free_color_img(color_img.image, color_img.height);
			}
			char filename[100];
			sscanf(input + strlen(command), "%s", filename); // Extract the filename
			type = file_type(filename);		//	get the file type
			if (type != NULL) {
				if (strcmp(type, "P2") == 0 || strcmp(type, "P5") == 0) {
					//	deal with a grayscale img
					gray_img = load_grayscale(filename, type);
					loaded_gray = 1;
					loaded_col = 0;
					free(type);
				} else {
					//	deal with a color img
					color_img = load_color(filename, type);
					loaded_col = 1;
					loaded_gray = 0;
					free(type);
				}
			} else {
				//	in case the loading fails reset the loaded source
				loaded_col = 0;
				loaded_gray = 0;
			}
		} else if (strcmp(command, "SELECT") == 0) {
			if (loaded_col == 0 && loaded_gray == 0) {
				printf("No image loaded\n");
			} else {
				char all[12] = "demo";
				sscanf(input + strlen(command), "%s", all);
				if (strcmp(all, "ALL") == 0) {
					//	select all case
					if (loaded_gray) {
						select_all_gray(&gray_img);
					} else {
						select_all_color(&color_img);
					}
					printf("Selected ALL\n");
				} else {
					//	give coord demo values to test validity
					//	if they are modified, it means the user did not respect the correct format
					coord.x1 = -31415;
					coord.x2 = -31415;
					coord.y1 = -31415;
					coord.y2 = -31415;
					//	extract the coordinates and do eventual swapping
					sscanf(input + strlen(command), "%d %d %d %d", &coord.x1, &coord.y1, &coord.x2, &coord.y2);
					normalize_selected(&coord);
					//	check values
					if (loaded_gray) {
						if (check_coords_gray(gray_img, coord) == 0) {
							printf("Invalid set of coordinates\n");
						} else if (check_coords_gray(gray_img, coord) == -1) {
							printf("Invalid command\n");
						} else {
							//	update the img data
							update_selection_gray(&gray_img, coord);
						}
					} else if (loaded_col) {
						if (check_coords_color(color_img, coord) == 0) {
							printf("Invalid set of coordinates\n");
						} else if (check_coords_color(color_img, coord) == -1) {
							printf("Invalid command\n");
						}else {
							//	update the rgb img
							update_selection_color(&color_img, coord);
						}
					}
				}
			}
		} else if (strcmp(command, "SAVE") == 0) {
			if (loaded_col == 0 && loaded_gray == 0) {
				printf("No image loaded\n");
			} else {
				char filename_out[100];
				char format[10] = "";
				int ascii = 0;	//	standard for binary file
				int params_no = sscanf(input + strlen(command), "%s %s", filename_out, format);
				if (params_no == 2 && strcmp(format, "ascii") == 0) {
					ascii = 1;
				}
				if (loaded_gray) {
					//	save a grayscale img
					save_image_gray(gray_img, filename_out, ascii);
				} else if (loaded_col) {
					//	save a color img
					save_image_color(color_img, filename_out, ascii);
				}
			}
		} else if (strcmp(command, "CROP") == 0) {
			if (loaded_col == 0 && loaded_gray == 0) {
				printf("No image loaded\n");
			} else {
				if (loaded_gray) {
					crop_gray(&gray_img);
				} else if (loaded_col) {
					crop_color(&color_img);
				}
			}
		} else if (strcmp(command, "APPLY") == 0) {
			//	extract parameter
			char param[52] = "demo";
			sscanf(input + strlen(command), "%s", param);
			if (loaded_col == 0 && loaded_gray == 0) {
				printf("No image loaded\n");
			} else if (strcmp(param, "demo") == 0) {
				printf("Invalid command\n");
			} else if (loaded_gray) {
				printf("Easy, Charlie Chaplin\n");
			} else {
				apply_filter(&color_img, param);
			}
		} else if (strcmp(command, "HISTOGRAM") == 0) {
			int x = -1, y = -1;
			//	next 2 variables are used to test the user's input
			//	if they are modified, it means the user did not respect the correct format
			char garbage_str[32] = "garbage";
			int garbage_int = -31415;
			sscanf(input + strlen(command), "%d %d %d %s", &x, &y, &garbage_int, garbage_str);
			if (loaded_col == 0 && loaded_gray == 0) {
				printf("No image loaded\n");
			} else if (loaded_col == 1) {
				if (x == -1 || y == -1 || strcmp(garbage_str, "garbage") != 0 || garbage_int != -31415) {
					printf("Invalid command\n");
				} else {
					printf("Black and white image needed\n");
				}
			} else {
				if (x == -1 || y == -1 || strcmp(garbage_str, "garbage") != 0 || garbage_int != -31415) {
					printf("Invalid command\n");
				} else if (is_power_of_two(y) == 0) {
					printf("Invalid set of parameters\n");
				} else {
					print_histogram_grayscale(&gray_img, x, y);
				}
			}
		} else if (strcmp(command, "EQUALIZE") == 0) {
			if (loaded_col == 0 && loaded_gray == 0) {
				printf("No image loaded\n");
			} else if (loaded_col == 1) {
				printf("Black and white image needed\n");
			} else {
				equalize_grayscale(&gray_img);
			}
		} else {
			printf("Invalid command\n");
		}
	}

	return 0;
}
