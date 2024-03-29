#include "cub3d.h"

void init_cub3(t_cub_data *data)
{
    data->north_texture_path = NULL;
    data->south_texture_path = NULL;
    data->west_texture_path = NULL;
    data->east_texture_path = NULL;
    data->floor_color = -1;
    data->ceiling_color = -1;
    data->map = NULL;
    data->map_width = 0;
    data->map_height = 0;
}

int parse_cub3(int argc, char **argv, t_cub_data *data)
{
    if (argc != 2) {
        printf("Usage: %s <path to .cub file>\n", argv[0]);
        return 1;
    }

    // Parse the .cub file
    if (parse_cub_file(argv[1], data) != 0) {
        printf("Failed to parse the .cub file.\n");
        return 1; // Indicate failure to parse
    }

    // Validate the parsed map data
    if (!validate_map(data)) {
        printf("The map is not valid.\n");
        return 1; // Indicate invalid map
    }

    return 0; // Indicate success
}

int main(int argc, char **argv) {
    t_cub_data data;
    int parse_result;

    // Initialize the cub3 data structure
    init_cub3(&data);

    // Parse the .cub file and validate it
    parse_result = parse_cub3(argc, argv, &data);
    if (parse_result != 0) {
        // Remember to free any allocated resources here if necessary
        return parse_result;
    }

    // At this point, the .cub file has been successfully parsed and validated.
    printf("The .cub file has been successfully parsed and validated.\n");

    // Your code to initialize and launch the game should go here

    // Remember to free any allocated resources before exiting
    // Free data fields as needed

    return 0;
}