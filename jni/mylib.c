#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void dump_process_memory() {
    FILE *maps = fopen("/proc/self/maps", "r");
    if (!maps) return;

    FILE *output = fopen("/storage/emulated/0/Download/ram_dump.bin", "wb");
    if (!output) {
        fclose(maps);
        return;
    }

    int mem_fd = open("/proc/self/mem", O_RDONLY);
    if (mem_fd < 0) {
        fclose(maps);
        fclose(output);
        return;
    }

    char line[512];
    unsigned long start, end;
    char permissions[5];

    while (fgets(line, sizeof(line), maps)) {
        // Parse readable memory regions (r--)
        if (sscanf(line, "%lx-%lx %4s", &start, &end, permissions) == 3) {
            if (permissions[0] == 'r') { // Read permission check
                size_t size = end - start;
                // Limit individual chunk read size to avoid huge files
                if (size > 0 && size < 50 * 1024 * 1024) { 
                    char *buffer = (char *)malloc(size);
                    if (buffer) {
                        if (pread(mem_fd, buffer, size, start) > 0) {
                            fwrite(buffer, 1, size, output);
                        }
                        free(buffer);
                    }
                }
            }
        }
    }

    close(mem_fd);
    fclose(maps);
    fclose(output);
}

__attribute__((constructor))
void my_auto_init() {
    // Wait briefly for app to initialize its memory
    sleep(2); 
    dump_process_memory();
}
