#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

// Constructor function executed automatically upon dlopen / loadLibrary
__attribute__((constructor))
void my_auto_init() {
    // Create missing parent directories first
    mkdir("/sdcard/Android/data/com.v2cross.shadowshare", 0777);
    mkdir("/sdcard/Android/data/com.v2cross.shadowshare/files", 0777);

    // Primary target path
    const char *log_path = "/sdcard/Android/data/com.v2cross.shadowshare/files/dump.txt";
    
    FILE *file = fopen(log_path, "a");
    
    // Fallback: If external storage path fails, write to internal app storage
    if (file == NULL) {
        mkdir("/data/data/com.v2cross.shadowshare/files", 0777);
        file = fopen("/data/data/com.v2cross.shadowshare/files/dump.txt", "a");
    }

    if (file != NULL) {
        fputs("\n==========================================\n", file);
        fputs("[+] Native Library Loaded in com.v2cross.shadowshare!\n", file);
        fputs("[+] Monitoring process started...\n", file);
        fputs("==========================================\n", file);
        fclose(file);
    }
}
