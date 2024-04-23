int parse_texture_path(char **line, t_cub_data *data, const char *direction)
{
    char *path = next_token(line);
    if (*path == '\0') {
        printf("Error: Texture path is empty.\n");
        return -1;
    }

    int index = get_texture_index(direction);
    if (index == -1) {
        printf("Error: Invalid direction.\n");
        return -1;
    }

    free(data->texture_paths[index]);
    data->texture_paths[index] = strdup(path);
    return 0;
}

int assign_texture_path(char **line, t_cub_data *data, const char *direction) {
    char *path = next_token(line);
    if (*path == '\0') {
        printf("Error: Texture path is empty.\n");
        return -1;
    }

    int index = get_texture_index(direction);
    if (index == -1) {
        printf("Error: Invalid direction.\n");
        return -1;
    }

    free(data->texture_paths[index]);
    data->texture_paths[index] = strdup(path);
    return 0;
}