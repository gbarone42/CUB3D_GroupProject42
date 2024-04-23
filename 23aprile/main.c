#include "file.h"


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
	print_map_shape(&data);
	printf("The .cub file has been successfully parsed and validated.\n");
	return 0;
}
