//Lucy Zhang, Ajay Ananthakrishnan
//wzhang99@jhu.edu, ajayananth@jhu.edu
//
// Driver program for loading image pluginsn and carrying
// out command specified by command-line parameters
//

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "image_plugin.h"
#include "image.h"

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

void print_plugins(struct Plugin * plugins, int plugin_count) {
    printf("Loaded %d plugin(s)\n", plugin_count);
    for (int i = 0; i < plugin_count; i++) {
        const char *plugin_name = (*(plugins[i].get_plugin_name))();
        const char *plugin_desc = (*(plugins[i].get_plugin_desc))();
        printf("%s: %s\n", plugin_name, plugin_desc);
    }
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


    DIR *dir = opendir(plugin_dir); //open the plugin directory
    if (dir == NULL) {
        printf("Error: cannot open plugin directory.\n");
        return 1;
    }
    struct Plugin plugins[15];
    int plugin_count = 0;

    struct dirent *plugin_dirent; //read directory into dirent struct object
    while ((plugin_dirent = readdir(dir)) != NULL) {
        char *filename = plugin_dirent->d_name;
        int name_len = strlen(filename);
        //check if filename ends with .so
        if (name_len > 3 && strcmp(filename + name_len - 3, ".so") == 0) {
            char full_address[strlen(plugin_dir) + name_len + 2];
            memset(full_address, 0, strlen(plugin_dir) + name_len + 2); //initialize to 0
            strcpy(full_address, plugin_dir);
            full_address[strlen(plugin_dir)] = '/';
            strcat(full_address, filename);
            void *handle = dlopen(full_address, RTLD_LAZY); //loads plugin dynamically
            if (handle == NULL) {
                printf("Error: cannot load plugin from %s.\n", full_address);
                continue;
            }
            struct Plugin p;
            p.handle = handle;
            //use dlsym to find addresses of loaded plugin
            *(void **) (&p.get_plugin_name) = dlsym(handle, "get_plugin_name");
            *(void **) (&p.get_plugin_desc) = dlsym(handle, "get_plugin_desc");
            *(void **) (&p.parse_arguments) = dlsym(handle, "parse_arguments");
            *(void **) (&p.transform_image) = dlsym(handle, "transform_image");
            plugins[plugin_count] = p; 
            plugin_count++;
        }
    }
    closedir(dir);

    //dynamic loading complete at this point

    //print loaded plugins if "list" in command args. 
    if (strcmp(argv[1], "list") == 0) {
        print_plugins(plugins, plugin_count);
    }

    // ./imgproc exec swapbg data/kitten.png kitten_swapbg.png

    //carry out specified plugin if "exec" in command args. 
    if (strcmp(argv[1], "exec") == 0) {
        
        // find a plugin whose name matches the specified plugin name
        for (int i = 0; i < plugin_count; i++) {
            //if plugin name equals specified plugin command argument, 
            if (strcmp((*(plugins[i].get_plugin_name))(), argv[2]) == 0) {
                //load the specified input image (using img_read_png)
                struct Image *inputImg = img_read_png(argv[3]); 

                //pass any command line arguments (past the input and output filenames) to the plugin’s parse_arguments function to produce an argument object
                struct Arguments *parsedArgs = plugins[i].parse_arguments(argc - 5, argv+5); 

                if (parsedArgs == NULL) {
                    printf("Error: Invalid Plugin Arguments.\n");
                    return 1;
                }

                //call the plugin’s transform_image function to perform the image transformation (passing the argument object returned by parse_arguments)
                struct Image *resultImg = plugins[i].transform_image(inputImg, parsedArgs); 

                //save the resulting image to the named output file (using img_write_png)
                if (!img_write_png(resultImg, argv[4])) {  //if img_write_png returns 0 (false), failed. 
                    printf("Error: Failed to save transformed image to named output file.\n");
                    return 1;
                }
                break; 
            }

            //if this point is reached on last iteration of loop, invalid specified plugin name. 
            if (i == plugin_count - 1) {
                printf("Error: Specified Plugin Name Not Found.\n");
                return 1; 
            }
        }
    }

    //CLEAN UP: dlclose() all dynamically loaded shared libraries
    for (int i = 0; i < plugin_count; i++) {
        dlclose(plugins[i].handle); 
    }

    //end reached
    return 0;

}