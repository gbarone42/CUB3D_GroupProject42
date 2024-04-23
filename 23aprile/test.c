int parse_details(char *line, t_cub_data *data) {
    static const char *directions[] = {"NO", "SO", "WE", "EA"};
    static const int dir_index[] = {NORTH, SOUTH, WEST, EAST}; // Indexes for NO, SO, WE, EA
    char *token;
    int index = 0;
    int result;

    token = next_token(&line);

    while (index < 4) {
        if (strcmp(token, directions[index]) == 0) {
            if (data->is_set[dir_index[index]]) {
                printf("Error: %s texture path already set.\n", directions[index]);
                return -1;
            }
            data->is_set[dir_index[index]] = true;
            return parse_texture_path(&line, data, directions[index]);
        }
        index++;
    }

    if (strcmp(token, "F") == 0) {
        if (data->is_set[4]) {
            printf("Error: Floor color already set.\n");
            return -1;
        }
        data->is_set[4] = true;
        return parse_color(&line, &data->floor_color);
    } else if (strcmp(token, "C") == 0) {
        if (data->is_set[5]) {
            printf("Error: Ceiling color already set.\n");
            return -1;
        }
        data->is_set[5] = true;
        return parse_color(&line, &data->ceiling_color);
    }

    printf("Error: Unrecognized line format.\n");
    return -1;
}