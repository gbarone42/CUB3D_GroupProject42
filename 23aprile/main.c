#include "file.h"

bool check_texture_set(const t_cub_data *data, int index) {
    if (!data->is_set[index]) {
        printf("Error: Texture for %s not set.\n",
               index == 0 ? "NO" : index == 1 ? "SO" : index == 2 ? "WE" : "EA");
        return false;
    }
    return true;
}

bool check_color_set(const t_cub_data *data, int index) {
    if (!data->is_set[index]) {
        if (index == 4) {
            printf("Error: Floor color not set.\n");
        } else {
            printf("Error: Ceiling color not set.\n");
        }
        return false;
    }
    return true;
}

bool check_configurations_set(const t_cub_data *data)
{
    int index;

	index = 0;
    while (index < 4)
	{ // Check texture paths
        if (!check_texture_set(data, index))
		{
            return false;
        }
        index++;
    }
    // Check color settings
    if (!check_color_set(data, 4)) return false;
    if (!check_color_set(data, 5)) return false;

    return true;
}

int main(int argc, char **argv)
{
	t_cub_data data;

	if (!initialize_and_check_args(argc, argv, &data))
		return 1;
	if (!parse_cub_wrapper(argv[1], &data))
		return 1;
	if (!validate_starting_points_wrapper(&data))
		return 1;
	if (!validate_starting_point_enclosure_wrapper(&data))
		return 1;
	if (!validate_mapborder_and_characters(&data))
		return 1;
	if (!check_configurations_set(&data))
   		return 1;
	print_map_shape(&data);
	printf("The .cub file has been successfully parsed and validated.\n");
	return 0;
}
