typedef struct s_cub_data
{
	char *texture_paths[4]; // North, South, West, East
	int floor_color;
	int ceiling_color;
	char **map;
	int map_height;
} t_cub_data;