#include "file.h"

char	*next_token(char **str)
{
	char	*token;

	while (**str && strchr(" \t\n", **str))
		(*str)++;
	token = *str;
	while (**str && !strchr(" \t\n", **str))
		(*str)++;
	if (**str)
	{
		*(*str)++ = '\0';
	}
	return (token);
}


//whatthefuck
// Function to parse texture paths
/*
int parse_texture_path(char **line, t_cub_data *data, const char *direction) {
    char *path = next_token(line);
    if (*path == '\0') {
        printf("Error: Texture path is empty.\n");
        return -1;
    }
    int index;
    if (direction[0] == 'N')
        index = NORTH;
    else if (direction[0] == 'S')
        index = SOUTH;
    else if (direction[0] == 'W')
        index = WEST;
    else if (direction[0] == 'E')
        index = EAST;
    else {
        printf("Error: Invalid direction.\n");
        return -1;
    }
    free(data->texture_paths[index]);
    data->texture_paths[index] = strdup(path);
    return 0;
}*/

int get_texture_index(const char *direction)
{
    if (direction[0] == 'N')
		return NORTH;
    if (direction[0] == 'S')
		return SOUTH;
    if (direction[0] == 'W')
		return WEST;
    if (direction[0] == 'E')
		return EAST;
    return -1; // Return -1 if direction is invalid
}
int parse_texture_path(char **line, t_cub_data *data, const char *direction)
{
    char *path = next_token(line);


    if (*path == '\0')
	{
        printf("Error: Texture path is empty.\n");
        return -1;
    }
    int index = get_texture_index(direction);
    if (index == -1)
	{
        printf("Error: Invalid direction.\n");
        return -1;
    }
    free(data->texture_paths[index]);
    data->texture_paths[index] = strdup(path);
    return 0;
}

// Function to parse color values
int parse_color_component(char **line)
{
    char *number = next_token(line);


    int value = atoi(number);


    if (value < 0 || value > 255)
	{
        printf("Error: Color component %d out of range.\n", value);
        return -1;
    }
    return value;
}

int convert_rgb_to_int(int r, int g, int b)
{
    return (r << 16) | (g << 8) | b;
}

int parse_color(char **line, int *color)
{
    int r;
	int g;
	int b;

    // Parse each component
    r = parse_color_component(line);
    if (r == -1) return -1;  // Stop if there's an error
    g = parse_color_component(line);
    if (g == -1) return -1;  // Stop if there's an error
    b = parse_color_component(line);
    if (b == -1) return -1;  // Stop if there's an error
    // Combine components into one integer and assign it to color
    *color = convert_rgb_to_int(r, g, b);
    return 0;
}
// Dispatcher function to parse line details
int parse_details(char *line, t_cub_data *data)
{
    static const char	*directions[] = {"NO", "SO", "WE", "EA"};
    char *token;
    int result;

    token = next_token(&line);
    int i = 0;
    while (i < 4)
    {
        if (strcmp(token, directions[i]) == 0)
        {
            result = parse_texture_path(&line, data, directions[i]);
            return (result);
        }
        i++;
    }
    if (strcmp(token, "F") == 0)
    {
        result = parse_color(&line, &data->floor_color);
        return (result);
    }
    else if (strcmp(token, "C") == 0)
    {
        result = parse_color(&line, &data->ceiling_color);
        return (result);
    }
    printf("Error: Unrecognized line format.\n");
    return (-1);
}



int	parse_map_line(char *line, t_cub_data *data)
{
	char	*new_line;

	new_line = strdup(line);
	if (!new_line)
	{
		printf("Error: Memory allocation failed.\n");
		return (-1);
	}
	data->map = realloc(data->map, (data->map_height + 1) * sizeof(char *));
	if (!data->map)
	{
		free(new_line);
		return (-1);
	}
	data->map[data->map_height++] = new_line;
	return (0);
}

int parse_line2(char *line, t_cub_data *data)
{
	int result;

	// Processing the line based on its content
	if (strchr("NSWEFC", line[0]) != NULL)
		result = parse_details(line, data);
	else
		result = parse_map_line(line, data);
	return result;
}

int parse_line(char *line, t_cub_data *data)
{
	char *temp = line;

	while (*temp)
	{
		if (!strchr(" \t\n", *temp))
			break; // Break loop on finding non-whitespace
		temp++;
	}
	// Check for empty lines within the map
	if (*temp == '\0' && data->map_height > 0)
	{
		printf("Error: Map contains an empty line.\n");
		return (-1);
	}
	// Return early for completely empty lines
	if (*line == '\0')
		return 0;
	// If line is not empty, call the next part of processing
	return parse_line2(line, data);
}

int process_buffer(char *buffer, t_cub_data *data)
{
    char *line = buffer;
    char *end;
    while ((end = strchr(line, '\n')) != NULL)
	{
        *end = '\0'; // Null-terminate the current line
        if (parse_line(line, data) != 0)
            return 1; // Error occurred while parsing li
        line = end + 1; // Move to the start of the next line
    }
    // Process any remaining text after the last newline
    if (*line != '\0' && parse_line(line, data) != 0)
        return 1; // Error occurred while parsing the last line
    return 0; // No errors, all lines processed successfully
}

int parse_cub_file(const char *file_path, t_cub_data *data)
{
	char buffer[BUFFER_SIZE + 1];
    ssize_t bytes_read;
    int status = 0;
    int fd = open(file_path, O_RDONLY);
    if (fd == -1)
	{
        perror("Error opening file");
        return 1;
    }
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
        buffer[bytes_read] = '\0'; // Ensure null-terminated
        status = process_buffer(buffer, data); // Process the current buffer
        if (status) break; // Break if there was an error
    }
    close(fd); // Ensure file is always closed
    return status; // Return the status to the caller
}

int	check_starting_points(t_cub_data *data, const char *valid_starts, int *start_count)
{
	int i;
	int j;

	i = 0;
	while (i < data->map_height)
	{
		j = 0;
		while (data->map[i][j])
		{
			if (strchr(valid_starts, data->map[i][j]))
			{
				(*start_count)++;
				if (*start_count > 1)
				{
					printf("Error: More than one starting point found.\n");
					return (0);
				}
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	validate_starting_points(t_cub_data *data)
{
	char	*valid_starts;
	int	 start_count;

	valid_starts = "NSEW";
	start_count = 0;
	if (!check_starting_points(data, valid_starts, &start_count))
		return (0);
	if (start_count == 0)
	{
		printf("Error: No starting point found.\n");
		return (0);
	}
	return (1);
}



bool is_properly_encapsulated(t_cub_data *data, int i, int j)
{
    // Ensure '0' is not adjacent to space or tab
				if ((data->map[i][j - 1] == ' ' ||
					data->map[i][j - 1] == '\t' ||
					data->map[i][j + 1] == ' ' ||
					data->map[i][j + 1] == '\t') ||
					(data->map[i - 1][j] == ' ' ||
					data->map[i - 1][j] == '\t' ||
					data->map[i + 1][j] == ' ' ||
					data->map[i + 1][j] == '\t'))
    {
        printf("Error: '0' near space/tab at (%d, %d).\n", i, j);
        return false;
    }
    return true;
}

bool validate_map_encapsulation(t_cub_data *data)
{
    int i = 1;
    int line_length;
    int j;

    // Start from second line and end before last line
    while (i < data->map_height - 1)
    {
        line_length = strlen(data->map[i]);
        j = 1;
        // Check from second char to second last char
        while (j < line_length - 1)
        {
            if (data->map[i][j] == '0')
            {
                if (!is_properly_encapsulated(data, i, j))
                {
                    return false;
                }
            }
            j++;
        }
        i++;
    }
    return true;
}

bool is_starting_point_enclosed(t_cub_data *data, int i, int j)
{
    int line_length = strlen(data->map[i]);
    // Check if it's not on the boundary to prevent accessing out of bounds
    if (i > 0 && i < data->map_height - 1 && j > 0 && j < line_length - 1)
	{
        if (data->map[i - 1][j] == '1' && data->map[i + 1][j] == '1' &&
            data->map[i][j - 1] == '1' && data->map[i][j + 1] == '1')
			{
            printf("Error: Starting point '%c' at (%d, %d) is fully enclosed by walls.\n", data->map[i][j], i, j);
            return true; // Enclosed
        }
    }
    return false; // Not enclosed
}

bool validate_starting_point_enclosure(t_cub_data *data)
{
    int i = 0;
    int j;
    int line_length;
    char c;

    while (i < data->map_height)
	{
        line_length = strlen(data->map[i]);
        j = 0;
        while (j < line_length)
		{
            c = data->map[i][j];
            if (strchr("NSEW", c))
			{
                if (is_starting_point_enclosed(data, i, j))
                    return false; // Stop checking if any starting point is enclosed
            }
            j++;
        }
        i++;
    }
    return true; // All starting points checked and valid
}

bool is_zero_adjacent_to_space_or_tab(t_cub_data *data, int i, int j, int line_length)
{
	return ((j > 0 && (data->map[i][j - 1] == ' '
			|| data->map[i][j - 1] == '\t'))
			|| (j < line_length - 1 && (data->map[i][j + 1]
			== ' ' || data->map[i][j + 1] == '\t'))
			|| (i > 0 && (data->map[i - 1][j] == ' '
			|| data->map[i - 1][j] == '\t'))
			|| (i < data->map_height - 1 && (data->map[i + 1][j] == ' '
			|| data->map[i + 1][j] == '\t')));
}


bool check_zero_adjacency(t_cub_data *data)
{
    int i = 0;
    int j;
    int line_length;

    while (i < data->map_height)
	{
        line_length = strlen(data->map[i]);
        j = 0;
        while (j < line_length)
		{
            if (data->map[i][j] == '0')
			{
                if (is_zero_adjacent_to_space_or_tab(data, i, j, line_length))
				{
                    printf("Error: '0' adjacent to space or tab at (%d, %d).\n", i, j);
                    return false;
                }
            }
            j++;
        }
        i++;
    }
    return true;
}

bool	is_zero_on_boundary(int map_height, int line_length, int i, int j)
{
	return (i == 0 || i == map_height - 1 || j == 0 || j == line_length - 1);
}

bool check_map_boundaries(t_cub_data *data)
{
    int i = 0;
    int j;
    int line_length;

    while (i < data->map_height)
	{
        line_length = strlen(data->map[i]);
        j = 0;
        while (j < line_length)
		{
            if (data->map[i][j] == '0')
			{
                if (is_zero_on_boundary(data->map_height, line_length, i, j))
				{
                    printf("Error: '0' on map boundary at (%d, %d).\n", i, j);
                    return false;
                }
            }
            j++;
        }
        i++;
    }
    return true;
}


bool	validate_map_characters(t_cub_data *data)
{
	int			i;
	int			j;
	int			line_length;
	const char	*valid_chars;

	i = 0;
	valid_chars = "01NSEW \t";
	while (i < data->map_height)
	{
		line_length = strlen(data->map[i]);
		j = 0;
		while (j < line_length)
		{
			if (!strchr(valid_chars, data->map[i][j]))
			{
				printf("Error: Invalid character '%c'row %d, col %d.\n",
							data->map[i][j], i, j);
				return (false);
			}
			j++;
		}
		i++;
	}
	return (true);
}

void	print_map_shape(t_cub_data *data)
{
	int	i;

	i = 0;
	printf("Map Shape:\n");
	while (i < data->map_height)
	{
		printf("%s\n", data->map[i]);
		i++;
	}
}
