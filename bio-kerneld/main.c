#include <stdio.h>
#include <dirent.h>
#include <errno.h>


static const char BIOKERNEL_MOUNT_PATH[] = "/bio";


int mount_bio_path() {
    DIR* dir = opendir(BIOKERNEL_MOUNT_PATH);
    
    if (dir) {
        /* Directory exists. */
        closedir(dir);
        
        return EPERM; // opretation not permitted - another instance likely running or controlling this path
    } else if (ENOENT == errno) {
        /* Directory does not exist. */
        return 0;
    } else {
        /* opendir() failed for some other reason. */
        return errno;
    }
}


int main(int argc, char *argv[]) {
    int mount_bio = mount_bio_path();
    
    if(mount_bio != 0) {
        return mount_bio;
    }

    return 0;
}
