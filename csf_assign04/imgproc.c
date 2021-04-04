//Lucy Zhang, Ajay Ananthakrishnan
//wzhang99@jhu.edu, ajayananth@jhu.edu
//
// Driver program for loading image pluginsn and carrying
// out command specified by command-line parameters
//

#include <stdio.h>
#include <string.h>
#include <dirent.h>

struct Plugin {
    void *handle;
    const char *(*get_plugin_name)(void);
    const char *(*get_plugin_desc)(void);
    void *(*parse_arguments)(int num_args, char *args[]);
    struct Image *(*transform_image)(struct Image *source, void *arg_data);
};

void print_usage() {
    printf("Usage: imgproc <command> [<command args...>]\nCommands are:\n  list\n  exec <plugin> <input img> <output img> [<plugin args...>]\n");
}

void print_plugins() {
    printf("Loaded 5 plugin(s)\n mirrorh: mirror image horizontally\n mirrorv: mirroro image vertically\n swapbg: swap blue and green color component values\n tile: tile source image in an NxN arrangement\n expose: adjust the intensity of all pixels\n");
}

int main(int argc, char **argv) {
    if (argc == 1) { //if no command line arguments entered
        print_usage();
        return 0;
    }
    //check if PLUGIN_DIR is set
    const char *plugin_dir = getenv("PLUGIN_DIR");
    if (plugin_dir == NULL) {
        // use default plugin directory
        plugin_dir = "./plugins";
    }
    dir = opendir(plugin_dir); //open the plugin directory
    if (dir == NULL) {
        printf("Error: cannot open plugin directory.\n");
        return 1;
    }
    while (plugin_dirent = readdir(dir) != NULL) {
        printf("%s\n", plugin_dirent->d_name);
        //if (plugin_dirent->d_name)

    }
    closedir(dir);


    if (strcmp(argv[1], "list") == 0) {
        print_plugins();
        return 0;
    }

    return 0;

}