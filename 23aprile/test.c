int	process_buffer(char *buffer, t_cub_data *data)
{
	char	*line;
	char	*end;

	end = strchr(line, '\n');
	line = buffer;
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
