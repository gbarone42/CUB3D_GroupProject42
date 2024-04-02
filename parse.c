#include "cub3d.h"

int parse_colors(char *line, t_cub_data *data)
{
    char id = line[0];
    line += 2; // Skip over the identifier and the following space

    int r = ft_atoi(line);
    while (*line != ',') line++;
    line++;
    int g = ft_atoi(line);
    while (*line != ',') line++;
    line++;
    int b = ft_atoi(line);

    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) return -1; // Invalid color value

    int color = (r << 16) | (g << 8) | b;
    if (id == 'F') data->floor_color = color;
    else if (id == 'C') data->ceiling_color = color;
    else return -1; // Unrecognized identifier

    return 0;
}

int parse_textures(char *line, t_cub_data *data)
{
    if (ft_strncmp(line, "NO ", 3) == 0)
    {
        data->north_texture_path = ft_strdup(line + 3);
    }
    else if (ft_strncmp(line, "SO ", 3) == 0)
    {
        data->south_texture_path = ft_strdup(line + 3);
    }
    else if (ft_strncmp(line, "WE ", 3) == 0)
    {
        data->west_texture_path = ft_strdup(line + 3);
    }
    else if (ft_strncmp(line, "EA ", 3) == 0)
    {
        data->east_texture_path = ft_strdup(line + 3);
    }
    else 
    {
        return -1;
    }
    return 0;
}

int parse_map(char *line, t_cub_data *data)
{
    // Calculate new size of the map (new row)
    int new_size = data->map_height + 1;

    // Allocate memory for the new row
    char **new_map = ft_realloc(data->map, new_size * sizeof(char *));
    if (new_map == NULL)
    {
        return -1; // Memory allocation failed
    }
    data->map = new_map;
    // Duplicate the line and store it in the map
    data->map[data->map_height] = ft_strdup(line);
    if (data->map[data->map_height] == NULL)
    {
        return -1; // Memory allocation failed
    }
    // Update map height
    data->map_height = new_size;
    // Optionally update map_width if this row is wider than the previous widest
    int line_length = ft_strlen(line);
    if (line_length > data->map_width)
    {
        data->map_width = line_length;
    }
    return 0;
}

bool validate_map(t_cub_data *data)
{
    bool has_valid_start = false;

    // Check top and bottom borders
    for (int i = 0; i < data->map_width; i++)
    {
        if (data->map[0][i] != '1' || data->map[data->map_height - 1][i] != '1')
        {
            return false; // Top or bottom row is not enclosed by walls
        }
    }
    // Check left and right borders and look for a valid starting position
    for (int i = 0; i < data->map_height; i++)
    {
        if (data->map[i][0] != '1' || data->map[i][data->map_width - 1] != '1')
        {
            return false; // Left or right column is not enclosed by walls
        }
        for (int j = 0; j < data->map_width; j++)
        {
            char c = data->map[i][j];
            if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
            {
                if (has_valid_start)
                {
                    return false; // Multiple starting positions
                }
                has_valid_start = true;
            }
        }
    }
    // Ensure there is exactly one valid starting position
    return has_valid_start;
}

int parse_cub_file(char *file_path, t_cub_data *data)
{
    int fd = open(file_path, O_RDONLY);
    if (fd == -1)
    {
        // perror("Error opening file"); // Replaced with a manual error message
        printf("Error opening file: %s\n", file_path);
        return -1;
    }
    char buffer[BUFFER_SIZE + 1];
    int bytes_read;
    char *line = malloc(BUFFER_SIZE + 1);
    if (!line)
    {
        printf("Memory allocation failed\n");
        close(fd);
        return -1;
    }
    int line_length = 0;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        buffer[bytes_read] = '\0'; // Null-terminate the buffer
        for (int i = 0; i < bytes_read; ++i) {
            if (buffer[i] == '\n' || buffer[i] == '\0')
            {
                // End of line or end of file
                line[line_length] = '\0'; // Null-terminate the line

                // Process the line
                if (line[0] == 'N' || line[0] == 'S' || line[0] == 'W' || line[0] == 'E')
                {
                    if (parse_textures(line, data) != 0)
                    {
                        close(fd);
                        free(line);
                        return -1;
                    }
                }
                else if (line[0] == 'F' || line[0] == 'C')
                {
                    if (parse_colors(line, data) != 0)
                    {
                        close(fd);
                        free(line);
                        return -1;
                    }
                }
                else if (line[0] == '1' || line[0] == '0')
                {
                    if (parse_map(line, data) != 0)
                    {
                        close(fd);
                        free(line);
                        return -1;
                    }
                }
                // Reset line for next read
                line_length = 0;
            }
            else
            {
                // Add character to line
                line[line_length++] = buffer[i];
                if (line_length >= BUFFER_SIZE)
                {
                    // Line is too long
                    printf("Line too long\n");
                    close(fd);
                    free(line);
                    return -1;
                }
            }
        }
    if (bytes_read == -1)
    {
        // Error reading file
        printf("Error reading file: %s\n", file_path);
        close(fd);
        free(line);
        return -1;
    }
    close(fd);
    free(line);
    return 0; // Success
}