#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>  // Include for bool, true, false

#define BUFFER_SIZE 4096

typedef struct s_cub_data {
    char *north_texture_path;
    char *south_texture_path;
    char *west_texture_path;
    char *east_texture_path;
    int floor_color;
    int ceiling_color;
    char **map;
    int map_width;
    int map_height;
} t_cub_data;

// Helper function to trim spaces and get the next token
char *get_next_token(char **str) {
    char *token_start, *token_end;
    token_start = *str;
    while (*token_start == ' ' || *token_start == '\t')
        token_start++;
    token_end = token_start;
    while (*token_end != ' ' && *token_end != '\t' && *token_end != '\n' && *token_end != '\0')
        token_end++;
    *str = token_end;
    if (*token_end != '\0') {
        *token_end = '\0';
        (*str)++;
    }
    return token_start;
}

// Parse texture paths
int parse_texture(char **line, char **destination) {
    char *token = get_next_token(line);
    if (*token == '\0') return -1;
    *destination = strdup(token);
    return 0;
}

// Parse color configuration
int parse_color(char **line, int *color) {
    char *token;
    int r, g, b;

    // Parse red
    token = get_next_token(line);
    r = atoi(token);
    if (r < 0 || r > 255) return -1;

    // Parse green
    token = get_next_token(line);
    g = atoi(token);
    if (g < 0 || g > 255) return -1;

    // Parse blue
    token = get_next_token(line);
    b = atoi(token);
    if (b < 0 || b > 255) return -1;

    *color = (r << 16) | (g << 8) | b;
    return 0;
}

// Parse map lines
// Parse map lines
int parse_map_line(char *line, t_cub_data *data) {
    static int map_line_count = 0;
    data->map = realloc(data->map, (map_line_count + 1) * sizeof(char *));
    if (!data->map) {
        printf("Error: Memory allocation failed when adding a new map line.\n");
        return -1;
    }
    data->map[map_line_count++] = strdup(line);
    data->map_height = map_line_count;
    return 0;
}

// Main parsing function for line
int parse_line(char *line, t_cub_data *data) {
    char *original_line = line;
    char *trimmed_line = line;

    // Skip leading whitespace
    while (*trimmed_line == ' ' || *trimmed_line == '\t' || *trimmed_line == '\n')
        trimmed_line++;

    // Check if the line is effectively empty after trimming whitespace
    if (*trimmed_line == '\0')
        return 0;  // Just skip empty lines

    char *identifier = get_next_token(&line);

    if (!identifier || identifier[0] == '\0') {
        printf("Empty or invalid line found.\n");
        return -1;
    }

    if (strcmp(identifier, "NO") == 0) return parse_texture(&line, &(data->north_texture_path));
    else if (strcmp(identifier, "SO") == 0) return parse_texture(&line, &(data->south_texture_path));
    else if (strcmp(identifier, "WE") == 0) return parse_texture(&line, &(data->west_texture_path));
    else if (strcmp(identifier, "EA") == 0) return parse_texture(&line, &(data->east_texture_path));
    else if (identifier[0] == 'F' && strlen(identifier) == 1) return parse_color(&line, &(data->floor_color));
    else if (identifier[0] == 'C' && strlen(identifier) == 1) return parse_color(&line, &(data->ceiling_color));
    else if (strchr("01NSEW", identifier[0])) return parse_map_line(original_line, data);

    printf("Failed to match line with any expected patterns: %s\n", original_line);
    return -1;
}

void flood_fill(char **map, int x, int y, int width, int height, int *valid) {
  // Early exit if the location is out of expected bounds
  if (x < 0 || x >= width || y < 0 || y >= height) {
    *valid = 0; // Set validity to false if it goes out of bounds
    return;
  }

  // Check for wall or already visited
  if (map[y][x] == '1' || map[y][x] == 'x') {
    return;
  }

  // Mark as visited
  map[y][x] = 'x';

  // Recursively flood adjacent cells
  flood_fill(map, x + 1, y, width, height, valid);
  flood_fill(map, x - 1, y, width, height, valid);
  flood_fill(map, x, y + 1, width, height, valid);
  flood_fill(map, x, y - 1, width, height, valid);
}

bool validate_map(t_cub_data *data) {
    int valid = 1;
    int startX = -1, startY = -1;  // Initialize start position

    char **tempMap = malloc(data->map_height * sizeof(char *));
    if (!tempMap) {
        printf("Error: Memory allocation failed.\n");
        return false;
    }

    for (int i = 0; i < data->map_height; i++) {
        tempMap[i] = strdup(data->map[i]);
        if (!tempMap[i]) {
            for (int j = 0; j < i; j++) free(tempMap[j]);
            free(tempMap);
            printf("Error: Memory allocation failed for map line.\n");
            return false;
        }
    }

    // Check the first and last rows for complete wall enclosure
    for (size_t x = 0, flen = strlen(tempMap[0]), llen = strlen(tempMap[data->map_height - 1]); x < flen && x < llen; x++) {
        if (tempMap[0][x] != '1' || tempMap[data->map_height - 1][x] != '1') {
            printf("Error: Map is not properly enclosed by walls.\n");
            valid = 0;
            break;
        }
    }

    // Check vertical edges of each row
    for (int y = 0; y < data->map_height; y++) {
        size_t len = strlen(tempMap[y]);
        if (tempMap[y][0] != '1' || tempMap[y][len - 1] != '1') {
            printf("Error: Vertical edges of row %d are not properly enclosed by walls.\n", y);
            valid = 0;
            break;
        }
        // Locate the starting position
        for (size_t x = 0; x < len; x++) {
            if (strchr("NSEW", tempMap[y][x])) {
                if (startX != -1) {  // If a starting position has already been found
                    printf("Error: Multiple starting positions found.\n");
                    valid = 0;
                    break;
                }
                startX = x;
                startY = y;
            }
        }
        if (!valid) break;
    }

    if (valid && startX != -1 && startY != -1) {
        // Assume flood_fill is defined elsewhere correctly
        // Uncomment the line below if you need to perform flood fill
        // flood_fill(tempMap, startX, startY, data->map_height, &valid);
    } else if (startX == -1 || startY == -1) {
        printf("Error: No valid starting position found.\n");
        valid = 0;
    }

    for (int i = 0; i < data->map_height; i++) free(tempMap[i]);
    free(tempMap);

    return valid == 1;
}



// Initialize the cub3d data structure
void init_cub3(t_cub_data *data) {
    data->north_texture_path = NULL;
    data->south_texture_path = NULL;
    data->west_texture_path = NULL;
    data->east_texture_path = NULL;
    data->floor_color = -1;
    data->ceiling_color = -1;
    data->map = NULL;
    data->map_width = 0;
    data->map_height = 0;
}

// Parse the .cub file
int parse_cub_file(char *file_path, t_cub_data *data) {
    int fd;
    ssize_t bytes_read;
    char buffer[BUFFER_SIZE];
    char *line = NULL, *end;

    fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';
        line = buffer;
        while ((end = strchr(line, '\n')) != NULL) {
            *end = '\0';
            if (parse_line(line, data) != 0) {
                printf("Failed to parse line: %s\n", line);
                close(fd);
                return 1;
            }
            line = end + 1;
        }
    }
    close(fd);
    if (!validate_map(data)) {
        printf("Map validation failed\n");
        return 1;
    }
    return 0;
}

// Parse command-line arguments and initiate parsing
int parse_cub3(int argc, char **argv, t_cub_data *data)
{
    if (argc != 2) {
        printf("Usage: %s <path to .cub file>\n", argv[0]);
        return 1;
    }

    if (parse_cub_file(argv[1], data) != 0) {
        printf("Failed to parse the .cub file.\n");
        return 1;
    }

    if (!validate_map(data)) {
        printf("The map is not valid.\n");
        return 1;
    }

    return 0;
}

// Main function, entry point of the program
int main(int argc, char **argv) {
    t_cub_data data;
    int parse_result;

    init_cub3(&data);

    parse_result = parse_cub3(argc, argv, &data);
    if (parse_result != 0) {
        // Free allocated memory and other cleanup tasks
        return parse_result;
    }

    printf("The .cub file has been successfully parsed and validated.\n");
    // Proceed with game initialization and launching

    // Cleanup code before exiting
    return 0;
}
