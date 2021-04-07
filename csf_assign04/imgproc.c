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

//helper function that dlclose() all dynamically loaded shared libraries 
//and free plugins array
void clean_up(struct Plugin *plugins, int count) {
    for (int i = 0; i < count; i++) {
        dlclose(plugins[i].handle); 
    }
    free(plugins);
}

//helper function that prints usage of imgproc program
void print_usage() {
    printf("Usage: imgproc <command> [<command args...>]\nCommands are:\n  list\n");
    printf("  exec <plugin> <input img> <output img> [<plugin args...>]\n");
}

//helper function that prints info about loaded plugins
void print_plugins(struct Plugin * plugins, int plugin_count) {
    printf("Loaded %d plugin(s)\n", plugin_count);
    //loop through all the plugins and print out name and description
    for (int i = 0; i < plugin_count; i++) {
        const char *plugin_name = (*(plugins[i].get_plugin_name))();
        const char *plugin_desc = (*(plugins[i].get_plugin_desc))();
        printf("%s: %s\n", plugin_name, plugin_desc);
    }
}

//helper function that executes plugin
void exec(struct Plugin * plugins, int plugin_count, int argc, char **argv) {
    if (argc < 3) {
        printf("Error: No Plugin Name entered.\n");
        clean_up(plugins, plugin_count);
        exit(1);
    }
    // find a plugin whose name matches the specified plugin name
    for (int i = 0; i < plugin_count; i++) {
        //if plugin name equals specified plugin command argument, 
        if (strcmp((*(plugins[i].get_plugin_name))(), argv[2]) == 0) {
            //load the specified input image (using img_read_png)
            struct Image *inputImg = img_read_png(argv[3]); 

            //pass any command line arguments (past the input and output filenames) to 
            //the plugin’s parse_arguments function to produce an argument object
            struct Arguments *parsedArgs = plugins[i].parse_arguments(argc - 5, argv+5); 
            if (parsedArgs == NULL) {
                printf("Error: Invalid Plugin Arguments.\n");
                clean_up(plugins, plugin_count);
                img_destroy(inputImg);
                exit(1);
            }

            //call the plugin’s transform_image function to perform the image transformation 
            struct Image *resultImg = plugins[i].transform_image(inputImg, parsedArgs); 

            //save the resulting image to the named output file 
            if (!img_write_png(resultImg, argv[4])) {  //if img_write_png returns 0, failed. 
                printf("Error: Failed to save transformed image to named output file.\n");
                clean_up(plugins, plugin_count);
                img_destroy(inputImg);
                img_destroy(resultImg);
                exit(1);
            }
            img_destroy(inputImg);
            img_destroy(resultImg);
            break; 
        }

        //if this point is reached on last iteration of loop, invalid specified plugin name. 
        if (i == plugin_count - 1) {
            printf("Error: Specified Plugin Name Not Found.\n");
            clean_up(plugins, plugin_count);
            exit(1); 
        }
    }
}

//helper function that builds full address 
void buildAddress(char* full_address, const char * plugin_dir, char* filename, int name_len) {
    memset(full_address, 0, strlen(plugin_dir) + name_len + 2); //initialize to 0
    strcpy(full_address, plugin_dir);
    full_address[strlen(plugin_dir)] = '/';
    strcat(full_address, filename);
}

//helper function that builds plugins and populates plugins[] 
int makePlugin(void *handle, struct Plugin *plugins, int *plugin_count) {
    struct Plugin p;
    p.handle = handle;
    //use dlsym to find addresses of loaded plugin
    *(void **) (&p.get_plugin_name) = dlsym(handle, "get_plugin_name");
    *(void **) (&p.get_plugin_desc) = dlsym(handle, "get_plugin_desc");
    *(void **) (&p.parse_arguments) = dlsym(handle, "parse_arguments");
    *(void **) (&p.transform_image) = dlsym(handle, "transform_image");
    if (&p.get_plugin_name == NULL || &p.get_plugin_desc == NULL || 
        &p.parse_arguments == NULL || &p.transform_image == NULL) {
        printf("Required API function can’t be found within a loaded plugin.\n");
        return 1;
    }
    plugins[*plugin_count] = p; 
    (*plugin_count)++;
    return 0;
}

//helper function to set the plugin directory
const char* set_dir() {
    //check if PLUGIN_DIR is set
    const char *plugin_dir = getenv("PLUGIN_DIR");
    if (plugin_dir == NULL) {
        // use default plugin directory
        plugin_dir = "./plugins";
    }
    return plugin_dir;

}

//helper function to grow dynamically allocated plugins array if necessary
void grow_plugins(struct Plugin *plugins, int count, int capacity) {
    if (count == capacity) {
        capacity *= 2;
        plugins = realloc(plugins, capacity * sizeof(struct Plugin));
    }
}

int main(int argc, char **argv) {
    if (argc == 1) { //if no command line arguments entered
        print_usage();
        return 0;
    }
    const char *plugin_dir = set_dir(); //sets plugin directory
    DIR *dir = opendir(plugin_dir); //open the plugin directory
    if (dir == NULL) {
        printf("Error: cannot open plugin directory.\n");
        return 1;
    }
    int plugins_capacity = 15;
    struct Plugin *plugins = malloc(sizeof(struct Plugin) * plugins_capacity);
    int plugin_count = 0;
    struct dirent *plugin_dirent; //read directory into dirent struct object
    
    while ((plugin_dirent = readdir(dir)) != NULL) {
        grow_plugins(plugins, plugin_count, plugins_capacity);
        char *filename = plugin_dirent->d_name;
        int name_len = strlen(filename);
        //check if filename ends with .so
        if (name_len > 3 && strcmp(filename + name_len - 3, ".so") == 0) {
            char full_address[strlen(plugin_dir) + name_len + 2];
            buildAddress(full_address, plugin_dir, filename, name_len); 

            void *handle = dlopen(full_address, RTLD_LAZY); //loads plugin dynamically
            if (handle == NULL) {
                printf("Error: cannot load plugin from %s.\n", full_address);
                continue;
            }
            if (makePlugin(handle, plugins, &plugin_count) != 0) continue; 
        }
    }
    closedir(dir);

    //print loaded plugins if "list" in command args. 
    if (strcmp(argv[1], "list") == 0) {
        print_plugins(plugins, plugin_count);
    } else if (strcmp(argv[1], "exec") == 0) {
        //carry out specified plugin if "exec" in command args.
        exec(plugins, plugin_count, argc, argv); 
    } else {
        printf("Error: unknown command name.\n");
        clean_up(plugins, plugin_count);
        return 1;
    }

    clean_up(plugins, plugin_count);
    return 0;
}
