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
    char *texture_paths[4]; // Textures for North, South, West, East
    int floor_color;        // RGB color for floor
    int ceiling_color;      // RGB color for ceiling
    char **map;             // Array of strings for the map
    int map_height;         // Number of lines in the map
} t_cub_data;

// Function prototypes

// Initialization and argument checking
bool initialize_and_check_args(int argc, char **argv, t_cub_data *data);

// Parsing functions
int parse_cub_file(const char *file_path, t_cub_data *data);
int parse_line(char *line, t_cub_data *data);
int parse_details(char *line, t_cub_data *data);
int parse_map_line(char *line, t_cub_data *data);
bool parse_cub_wrapper(char *filename, t_cub_data *data);
char *next_token(char **str);

// Validation functions
bool validate_map_encapsulation(t_cub_data *data);
bool validate_map_encapsulation_wrapper(t_cub_data *data);
int validate_starting_points(t_cub_data *data);
bool validate_starting_points_wrapper(t_cub_data *data);
bool validate_starting_point_enclosure(t_cub_data *data);
bool validate_starting_point_enclosure_wrapper(t_cub_data *data);
bool validate_mapborder_encapsulation(t_cub_data *data);
bool validate_mapborder_and_characters(t_cub_data *data);
bool	validate_map_characters(t_cub_data *data);

// Utility functions
void init_cub3(t_cub_data *data);
void print_map_shape(t_cub_data *data);

#endif // CUB_DATA_H
