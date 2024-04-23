#include "file.h"

bool validate_mapborder_and_characters(t_cub_data *data)
{
	if (!validate_mapborder_encapsulation(data))
	{
		printf("Map border encapsulation validation failed.\n");
		return false;
	}
	if (!validate_map_characters(data))
	{
		return false;
	}
	return true;
}

bool validate_starting_point_enclosure_wrapper(t_cub_data *data)
{
	if (!validate_starting_point_enclosure(data))
	{
		printf("Starting point enclosure validation failed.\n");
		return false;
	}
	return true;
}

bool validate_map_encapsulation_wrapper(t_cub_data *data)
{
	if (!validate_map_encapsulation(data))
	{
		printf("Map encapsulation validation failed.\n");
		return false;
	}
	return true;
}

bool validate_starting_points_wrapper(t_cub_data *data)
{
	if (!validate_starting_points(data))
	{
		return false;
	}
	return true;
}

bool parse_cub_wrapper(char *filename, t_cub_data *data)
{
	if (parse_cub_file(filename, data) != 0)
	{
		printf("Failed to parse the .cub file.\n");
		return false;
	}
	return true;
}

void init_cub3(t_cub_data *data)
{
	memset(data, 0, sizeof(*data));
}


bool initialize_and_check_args(int argc, char **argv, t_cub_data *data)
{
	if (argc != 2)
	{
		printf("Usage: %s <path to .cub file>\n", argv[0]);
		return false;
	}
	init_cub3(data);
	return true;
}
