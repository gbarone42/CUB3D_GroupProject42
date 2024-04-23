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
