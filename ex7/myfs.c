#include <string.h>
# include "myfs.h"
#include <stdlib.h>
#include <stdio.h>


void mymkfs(int fs_size) {

    int size = fs_size - (int)sizeof (ginfo);

    ginfo.num_inodes = (int)((size*0.1)/ sizeof(struct inode));
    ginfo.num_blocks = (int)((size*0.9) / sizeof(struct disk_info));
    ginfo.size_blocks = (int)((size*0.9));

    int i;
    inodes = malloc (sizeof (struct inode) * ginfo.num_inodes);
    for(i = 0; i<ginfo.num_inodes; i++) {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy (inodes[i].name, "");
    }

    dinfo = malloc (sizeof (struct disk_info) * ginfo.size_blocks);
    for (i=0; i<ginfo.num_blocks; i++) {
        dinfo[i].next_block_num = -1;
    }
}

int mymount(const char *source, const char *target,
            const char *filesystemtype, unsigned long
            mountflags, const void *data){
    if (source == NULL){
        printf("No device attached.");
        return -1;
    }
    FILE *file = fopen(source, "r");
    // super block
    fread(&ginfo, sizeof(struct general_info), 1, file);

    // inodes
    inodes = malloc(sizeof(struct inode) * ginfo.num_inodes);
    fread(inodes, sizeof(struct inode), ginfo.num_inodes, file);

    // dbs
    dinfo = malloc(sizeof(struct general_info) * ginfo.num_blocks);
    fread(dinfo, sizeof(struct disk_info), ginfo.num_blocks, file);
    fclose(file);
    return 0;
}


void set_filesize(int filenum, int size)
{
    int temp = size + BLOCKSIZE -1;
    int num = temp / BLOCKSIZE;
    int bn = inodes[filenum].first_block;
    //grow the file if necessary
    for (num-- ; num > 0; num--)
    {
        //check next block number
        int next_num = dbs[bn].next_block_num;
        if (next_num == -2)
        {
            int empty = find_empty_block();
            dbs[bn].next_block_num = empty;
            dbs[empty].next_block_num = -2;
        }
        bn = dbs[bn].next_block_num;
    }
    //short the file if necessary
    shorten_file(bn);
    dbs[bn].next_block_num = -2;
}

int get_block_num(int file, int offeset)
{
    int bn = inodes[file].first_block;
    for(int togo = offeset; togo > 0; togo--)
    {
        bn = dbs[bn].next_block_num;
    }
    return bn;
}