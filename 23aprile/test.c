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




				if ((data->map[i][j - 1] == ' ' ||
					data->map[i][j - 1] == '\t' ||
					data->map[i][j + 1] == ' ' ||
					data->map[i][j + 1] == '\t') ||
					(data->map[i - 1][j] == ' ' ||
					data->map[i - 1][j] == '\t' ||
					data->map[i + 1][j] == ' ' ||
					data->map[i + 1][j] == '\t'))