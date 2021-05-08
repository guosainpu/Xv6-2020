#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void parsefname(char *path, char *buf)
{
    // printf("input: %s  %d\n", path, strlen(path));
    // printf("input: %s  %d\n", buf, strlen(path));
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), 0, 1);

    // printf("output: %s  %d\n", buf, strlen(path));
}

void find(char *path, char *fname)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
    {
        char cfile[DIRSIZ + 1];
        parsefname(path, cfile);
        if (strcmp(cfile, fname) == 0)
        {
            printf("%s\n", path);
        }
        break;
    }

    case T_DIR:
    {
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: path too long\n");
            break;
        }

        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';

        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (strlen(de.name) <= 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            {
                continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(buf, fname);
        }
        break;
    }
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(2, "Usage: find path filename\n");
        exit(0);
    }

    find(argv[1], argv[2]);

    exit(0);
}
