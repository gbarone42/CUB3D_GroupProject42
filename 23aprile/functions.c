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

int	get_texture_index(const char *direction)
{
	if (direction[0] == 'N')
		return (NORTH);
	if (direction[0] == 'S')
		return (SOUTH);
	if (direction[0] == 'W')
		return (WEST);
	if (direction[0] == 'E')
		return (EAST);
	return (-1);
}

int	parse_texture_path(char **line, t_cub_data *data, const char *direction)
{
	char		*path;
	int			index;

    path = next_token(line);
	if (*path == '\0')
	{
		printf("Error: Texture path is empty.\n");
		return (-1);
	}
	index = get_texture_index(direction);
	if (index == -1)
	{
		printf("Error: Invalid direction.\n");
		return (-1);
	}
	free(data->texture_paths[index]);
	data->texture_paths[index] = strdup(path);
	return (0);
}

// Function to parse color values
int	parse_color_component(char **line)
{
	char	*number;
	int		value;

	number = next_token(line);
	value = atoi(number);
	if (value < 0 || value > 255)
	{
		printf("Error: Color component %d out of range.\n", value);
		return (-1);
	}
	return (value);
}

int	convert_rgb_to_int(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}

int	parse_color(char **line, int *color)
{
	int		r;
	int		g;
	int		b;

	r = parse_color_component(line);
	if (r == -1)
		return (-1);
	g = parse_color_component(line);
	if (g == -1)
		return (-1);
	b = parse_color_component(line);
	if (b == -1)
		return (-1);
	*color = convert_rgb_to_int(r, g, b);
	return (0);
}

/**/
// Dispatcher function to parse line details
int	parse_details(char *line, t_cub_data *data) {
	static const char *directions[] = {"NO", "SO", "WE", "EA"};
	static const int dir_index[] = {NORTH, SOUTH, WEST, EAST}; // Indexes for NO, SO, WE, EA
	char *token;
	int index;

    index = 0;
	token = next_token(&line);
	while (index < 4) {
		if (strcmp(token, directions[index]) == 0) {
			if (data->is_set[dir_index[index]]) {
				printf("Error: %s texture path already set.\n", directions[index]);
				return (-1);
			}
			data->is_set[dir_index[index]] = true;
			return (parse_texture_path(&line, data, directions[index]));
		}
		index++;
	}
	if (strcmp(token, "F") == 0)
    {
		if (data->is_set[4])
        {
			printf("Error: Floor color already set.\n");
			return (-1);
		}
		data->is_set[4] = true;
		return (parse_color(&line, &data->floor_color));
	}
    else if (strcmp(token, "C") == 0)
    {
		if (data->is_set[5])
        {
			printf("Error: Ceiling color already set.\n");
			return (-1);
		}
		data->is_set[5] = true;
		return (parse_color(&line, &data->ceiling_color));
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

int		parse_line2(char *line, t_cub_data *data)
{
	int		result;

	if (strchr("NSWEFC", line[0]) != NULL)
		result = parse_details(line, data);
	else
		result = parse_map_line(line, data);
	return (result);
}

int		parse_line(char *line, t_cub_data *data)
{
	char	*temp = line;

	while (*temp)
	{
		if (!strchr(" \t\n", *temp))
			break ;
		temp++;
	}
	if (*temp == '\0' && data->map_height > 0)
	{
		printf("Error: Map contains an empty line.\n");
		return (-1);
	}
	if (*line == '\0')
		return (0);
	return (parse_line2(line, data));
}

int	process_buffer(char *buffer, t_cub_data *data)
{
	char	*line = buffer;
	char	*end;

	while ((end = strchr(line, '\n')) != NULL)
	{
		*end = '\0';
		if (parse_line(line, data) != 0)
			return (1);
		line = end + 1;
	}
	if (*line != '\0' && parse_line(line, data) != 0)
		return (1);
	return (0);
}

int parse_cub_file(const char *file_path, t_cub_data *data)
{
	char		buffer[BUFFER_SIZE + 1];
	ssize_t		bytes_read;
	int			status;
	int			fd;

	status = 0;
	fd = open(file_path, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		return (1);
	}
	while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		buffer[bytes_read] = '\0';
		status = process_buffer(buffer, data);
		if (status)
			break ;
	}
	close(fd);
	return (status);
}

int	check_starting_points(t_cub_data *data, const char *valid_starts, int *start_count)
{
	int		i;
	int		j;

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
	int		start_count;

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

bool	is_properly_encapsulated(t_cub_data *data, int i, int j)
{
	if ((data->map[i][j - 1] == ' '
		|| data->map[i][j - 1] == '\t' ||
		data->map[i][j + 1] == ' ' ||
		data->map[i][j + 1] == '\t') ||
		(data->map[i - 1][j] == ' ' ||
		data->map[i - 1][j] == '\t' ||
		data->map[i + 1][j] == ' ' ||
		data->map[i + 1][j] == '\t'))
	{
		printf("Error: '0' near space/tab at (%d, %d).\n", i, j);
		return (false);
	}
	return (true);
}

bool	validate_map_encapsulation(t_cub_data *data)
{
	int		i;
	int		line_length;
	int		j;

	i = 1;
	while (i < data->map_height - 1)
	{
		line_length = strlen(data->map[i]);
		j = 1;
		while (j < line_length - 1)
		{
			if (data->map[i][j] == '0')
			{
				if (!is_properly_encapsulated(data, i, j))
				{
					return (false);
				}
			}
			j++;
		}
		i++;
	}
	return (true);
}

bool	is_starting_point_enclosed(t_cub_data *data, int i, int j)
{
	int		line_length;

	line_length = strlen(data->map[i]);
	if (i > 0 && i < data->map_height - 1 && j > 0 && j < line_length - 1)
	{
		if (data->map[i - 1][j] == '1' && data->map[i + 1][j] == '1' &&
			data->map[i][j - 1] == '1' && data->map[i][j + 1] == '1')
		{
			printf("'%c' (%d, %d) around walls.\n", data->map[i][j], i, j);
			return (true);
		}
	}
	return (false);
}

bool	validate_starting_point_enclosure(t_cub_data *data)
{
	int		i;
	int		j;
	int		line_length;
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
				if (is_starting_point_enclosed(data, i, j))
					return (false);
			}
			j++;
		}
		i++;
	}
	return (true);
}

bool	is_zero_adjacent_to_space_or_tab(t_cub_data *data, int i, int j, int line_length)
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

bool	check_zero_adjacency(t_cub_data *data)
{
	int		i;
	int		j;
	int		line_length;

    i = 0;
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
					printf("Error: 0 near space or tab at (%d, %d).\n", i, j);
					return (false);
				}
			}
			j++;
		}
		i++;
	}
	return (true);
}

bool	is_zero_on_boundary(int map_height, int line_length, int i, int j)
{
	return (i == 0 || i == map_height - 1 || j == 0 || j == line_length - 1);
}

bool	check_map_boundaries(t_cub_data *data)
{
	int		i;
	int		j;
	int		line_length;

    i = 0;
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
					return (false);
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
