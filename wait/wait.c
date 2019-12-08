#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

int main(int argc, char **argv)
{
    int length, i = 0;
    int fd;
    int wd;
    char buffer[BUF_LEN];

    if (argc != 2) {
        perror("Usage: ./file_wait filename\n");
        return 1;
    }

    fd = inotify_init();

    if (fd < 0)
    {
        perror("inotify_init");
    }

    wd = inotify_add_watch(fd, ".",
                           IN_MODIFY | IN_CREATE | IN_DELETE);
    while (1)
    {
        length = read(fd, buffer, BUF_LEN);

        if (length < 0)
        {
            perror("read");
        }

        while (i < length)
        {
            struct inotify_event *event =
                (struct inotify_event *)&buffer[i];
            if (event->len && event->mask & IN_CREATE)
            {
                printf("The file %s was created.\n", event->name);
                int ret = strcmp(event->name, argv[1]);
                if (!ret) {
                    printf("Unlocked\n");
                    return 0;
                }
            }
            i += EVENT_SIZE + event->len;
        }
        i = 0;
    }

    (void)inotify_rm_watch(fd, wd);
    (void)close(fd);

    return 0;
}