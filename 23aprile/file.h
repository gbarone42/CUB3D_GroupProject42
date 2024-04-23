#ifndef CUB_DATA_H
#define CUB_DATA_H

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 4096

// Enum for easier handling of texture directions
enum { NORTH, SOUTH, WEST, EAST };

// Structure to hold all necessary game data
typedef struct s_cub_data
{
	char *texture_paths[4];
	bool is_set[6]; // Tracks whether NO, SO, WE, EA, F, C are set
	int floor_color;		// RGB color for floor
	int ceiling_color;	  // RGB color for ceiling
	char **map;			 // Array of strings for the map
	int map_height;		 // Number of lines in the map
} t_cub_data;
// Function prototypes
// Initialization and argument checking
bool    initialize_and_check_args(int argc, char **argv, t_cub_data *data);
// Parsing functions
int     parse_cub_file(const char *file_path, t_cub_data *data);
int parse_line2(char *line, t_cub_data *data);
int parse_line(char *line, t_cub_data *data);
int parse_details(char *line, t_cub_data *data);
int parse_map_line(char *line, t_cub_data *data);
bool parse_cub_wrapper(char *filename, t_cub_data *data);
char *next_token(char **str);
// Validation functions
bool check_map_boundaries(t_cub_data *data);
bool check_zero_adjacency(t_cub_data *data);
bool validate_map_encapsulation_wrapper(t_cub_data *data);
int validate_starting_points(t_cub_data *data);
bool validate_starting_points_wrapper(t_cub_data *data);
bool validate_starting_point_enclosure(t_cub_data *data);
bool validate_starting_point_enclosure_wrapper(t_cub_data *data);
bool validate_mapborder_encapsulation(t_cub_data *data);
bool validate_mapborder_and_characters(t_cub_data *data);
bool	validate_map_characters(t_cub_data *data);
bool validate_map_encapsulation(t_cub_data *data);
// Utility fuctions
void init_cub3(t_cub_data *data);
void print_map_shape(t_cub_data *data);
int get_texture_index(const char *direction);
int parse_texture_path(char **line, t_cub_data *data, const char *direction);
int parse_color_component(char **line);
int convert_rgb_to_int(int r, int g, int b);
int parse_color(char **line, int *color);
int parse_details(char *line, t_cub_data *data);
int process_buffer(char *buffer, t_cub_data *data);
int	checkstartpoint(t_cub_data *data, const char *valid_starts, int *start_count);
bool is_properly_encapsulated(t_cub_data *data, int i, int j);
bool is_starting_point_enclosed(t_cub_data *data, int i, int j);
bool is_0_near_spaceortab(t_cub_data *data, int i, int j, int line_length);
bool check_zero_adjacency(t_cub_data *data);
bool	is_zero_on_boundary(int map_height, int line_length, int i, int j);
bool check_map_boundaries(t_cub_data *data);


#endif // CUB_DATA_H
