#ifndef CUB3D_H
#define CUB3D_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h> // For open
#include <unistd.h> // For read, close
#include <string.h> // For strlen, etc. - assuming allowed for basic string operations

#define BUFFER_SIZE 1024

/* Define the structure to store .cub file data */
typedef struct s_cub_data {
    char *north_texture_path;
    char *south_texture_path;
    char *west_texture_path;
    char *east_texture_path;
    int floor_color; /* Consider storing colors as separate RGB components */
    int ceiling_color; /* Consider storing colors as separate RGB components */
    char **map;
    int map_width;
    int map_height;
} t_cub_data;

/* Function prototypes */

/**
 * Parses the given .cub file and populates the data structure.
 * @param file_path The path to the .cub file.
 * @param data Pointer to the data structure to be filled.
 * @return 0 on success, non-zero on failure.
 */
int parse_cub_file(char *file_path, t_cub_data *data);

/**
 * Parses texture paths from a line of the .cub file.
 * @param line The line containing texture information.
 * @param data Pointer to the data structure to store the path.
 * @return 0 on success, non-zero on failure.
 */
int parse_textures(char *line, t_cub_data *data);

/**
 * Parses floor and ceiling color from a line of the .cub file.
 * @param line The line containing color information.
 * @param data Pointer to the data structure to store the colors.
 * @return 0 on success, non-zero on failure.
 */
int parse_colors(char *line, t_cub_data *data);

/**
 * Parses the map from the .cub file.
 * @param line The line containing map information.
 * @param data Pointer to the data structure to store the map.
 * @return 0 on success, non-zero on failure.
 */
int parse_map(char *line, t_cub_data *data);

/**
 * Validates the parsed map data for correctness.
 * @param data Pointer to the data structure containing the map.
 * @return true if valid, false otherwise.
 */
bool validate_map(t_cub_data *data);

#endif /* CUB3D_H */
