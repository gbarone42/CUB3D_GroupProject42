#include "cub3d.h"

int main(int argc, char **argv)
{
    t_cub_data data;
    bool valid;

    // Initialize data structure
    data.north_texture_path = NULL;
    data.south_texture_path = NULL;
    data.west_texture_path = NULL;
    data.east_texture_path = NULL;
    data.floor_color = -1;
    data.ceiling_color = -1;
    data.map = NULL;
    data.map_width = 0;
    data.map_height = 0;

    if (argc != 2) {
        printf("Usage: %s <path to .cub file>\n", argv[0]);
        return 1;
    }

    // Parse the .cub file
    if (parse_cub_file(argv[1], &data) != 0) {
        printf("Failed to parse the .cub file.\n");
        // Remember to free any allocated resources here
        return 1;
    }

    // Validate the parsed map data
    valid = validate_map(&data);
    if (!valid) {
        printf("The map is not valid.\n");
        // Remember to free any allocated resources here
        return 1;
    }

    // At this point, the .cub file has been successfully parsed and validated.
    printf("The .cub file has been successfully parsed and validated.\n");

    // Your code to initialize and launch the game should go here

    // Remember to free any allocated resources before exiting
    // Free data fields as needed

    return 0;
}