// Function to parse texture paths
int parse_texture_path(char **line, t_cub_data *data, const char *direction) {
    char *path = next_token(line);
    if (*path == '\0') {
        printf("Error: Texture path is empty.\n");
        return -1;
    }
    int index = (direction[0] == 'N' ? NORTH : direction[0] == 'S' ? SOUTH : direction[0] == 'W' ? WEST : EAST);
    free(data->texture_paths[index]);
    data->texture_paths[index] = strdup(path);
    return 0;
}

// Function to parse color values
int parse_color(char **line, int *color) {
    int r, g, b;
    char *number;

    // Parse red component
    number = next_token(line);
    r = atoi(number);
    if (r < 0 || r > 255) {
        printf("Error: Red color component out of range.\n");
        return -1;
    }

    // Parse green component
    number = next_token(line);
    g = atoi(number);
    if (g < 0 || g > 255) {
        printf("Error: Green color component out of range.\n");
        return -1;
    }

    // Parse blue component
    number = next_token(line);
    b = atoi(number);
    if (b < 0 || b > 255) {
        printf("Error: Blue color component out of range.\n");
        return -1;
    }

    *color = (r << 16) | (g << 8) | b;
    return 0;
}

// Dispatcher function to parse line details
int parse_details(char *line, t_cub_data *data) {
    static const char *directions[] = {"NO", "SO", "WE", "EA"};
    char *token = next_token(&line);
    for (int i = 0; i < 4; i++) {
        if (strcmp(token, directions[i]) == 0) {
            return parse_texture_path(&line, data, directions[i]);
        }
    }

    if (strcmp(token, "F") == 0) {
        return parse_color(&line, &data->floor_color);
    } else if (strcmp(token, "C") == 0) {
        return parse_color(&line, &data->ceiling_color);
    }

    printf("Error: Unrecognized line format.\n");
    return -1;
}
