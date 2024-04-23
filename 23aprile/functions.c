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

int parse_details(char *line, t_cub_data *data)
{
	static const char *directions[] = {"NO", "SO", "WE", "EA"};
	char *token = next_token(&line);

	for (int i = 0; i < 4; i++)
	{
		if (strcmp(token, directions[i]) == 0)
		{
			char *path = next_token(&line);
			if (*path == '\0') return -1;
			free(data->texture_paths[i]);
			data->texture_paths[i] = strdup(path);
			return (0);
		}
	}

	if (strcmp(token, "F") == 0 || strcmp(token, "C") == 0)
	{
		int *color = (token[0] == 'F') ? &data->floor_color : &data->ceiling_color;
		int r, g, b;
		sscanf(line, "%d,%d,%d", &r, &g, &b);
		if (r < 0 || g < 0 || b < 0 || r > 255 || g > 255 || b > 255) return -1;
		*color = (r << 16) | (g << 8) | b;
		return (0);
	}
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
	{
		result = parse_details(line, data);
	}
	else
	{
		result = parse_map_line(line, data);
	}
	return result;
}

int parse_line(char *line, t_cub_data *data)
{
	char *temp = line;

	while (*temp)
	{
		if (!strchr(" \t\n", *temp))
		{ 
			break; // Break loop on finding non-whitespace
		}
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
	{
		return 0;
	}
	// If line is not empty, call the next part of processing
	return parse_line2(line, data);
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

int parse_cub_file(const char *file_path, t_cub_data *data)
{
	int fd = open(file_path, O_RDONLY);
	if (fd == -1) return perror("Error opening file"), 1;

	char buffer[BUFFER_SIZE + 1];
	ssize_t bytes_read;
	char *line = buffer, *end;
	int status = 0;

	while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		buffer[bytes_read] = '\0'; // Ensure null-terminated
		while ((end = strchr(line, '\n')) != NULL) {
			*end = '\0';
			if (parse_line(line, data) != 0)
			{
				status = 1; // Indicate error
				break;
			}
			line = end + 1;
		}
		if (status) break; // Exit if error occurred

		// Process any remaining text after the last newline
		if (*line != '\0' && parse_line(line, data) != 0) {
			status = 1;
		}
	}
	close(fd);
	return (status);
}

void	print_map_shape(t_cub_data *data)
{
	int i;

	i = 0;
	printf("Map Shape:\n");
	while (i < data->map_height)
	{
		printf("%s\n", data->map[i]);
		i++;
	}
}


bool	validate_map_encapsulation(t_cub_data *data)
{
	int i;
	int line_length;
	int j;

	i = 1;
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
					printf("Error:0 near space/tab at (%d, %d).\n", i, j);
					return (false);
				}
			}
			j++;
		}
		i++;
	}
	return (true);
}

bool validate_mapborder_encapsulation(t_cub_data *data)
{
	int i;
	int j;
	int line_length;

	i = 0;
	while (i < data->map_height)
	{
		line_length = strlen(data->map[i]);
		j = 0;
		while (j < line_length)
		{
			if (data->map[i][j] == '0')
			{
				if (i == 0 || i == data->map_height - 1 || j == 0 || j == line_length - 1)
				{
					printf("Error: '0' on map boundary at (%d, %d).\n", i, j);
					return (false);
				}
				if ((j > 0 && (data->map[i][j -1] == ' '
						|| data->map[i][j -1] == '\t'))
						||(j < line_length - 1 && (data->map[i][j+1] == ' '
						|| data->map[i][j +1] == '\t'))
						||(i > 0 && (data->map[i-1][j] == ' '
						|| data->map[i -1][j] == '\t'))
						||(i < data->map_height - 1 && (data->map[i+1][j] == ' '
						|| data->map[i +1][j] == '\t')))
				{
					printf("Error: '0' adjacent to space or tab at (%d, %d).\n", i, j);
					return (false);
				}
			}
			j++;
		}
		i++;
	}
	return (true);
}

bool validate_starting_point_enclosure(t_cub_data *data)
{
	int	 i;
	int	 line_length;
	int	 j;
	char	c;

	 i = 0;
	while (i < data->map_height)
	{
		line_length = strlen(data->map[i]);
		j = 0;
		while (j < line_length)
		{
			c = data->map[i][j];
			if (strchr("NSEW", c))
			{
				if (i > 0 && i < data->map_height - 1 && j > 0 && j < line_length - 1)
				{
					if (data->map[i - 1][j] == '1' && data->map[i + 1][j] == '1' &&
						data->map[i][j - 1] == '1' && data->map[i][j + 1] == '1')
					{
						printf("Error: Starting point '%c' at (%d, %d) is fully enclosed by walls.\n", c, i, j);
						return (false);
					}
				}
			}
			j++;
		}
		i++;
	}
	return (true);
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
