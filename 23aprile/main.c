#include "file.h"

bool check_configurations_set(const t_cub_data *data) {
    // There are 6 necessary configurations: 4 texture paths + 2 color settings
    for (int i = 0; i < 6; i++) {
        if (!data->set_flags[i]) {
            if (i < 4) {
                printf("Error: Texture for %s not set.\n", i == 0 ? "NO" : i == 1 ? "SO" : i == 2 ? "WE" : "EA");
            } else if (i == 4) {
                printf("Error: Floor color not set.\n");
            } else if (i == 5) {
                printf("Error: Ceiling color not set.\n");
            }
            return false;
        }
    }
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
