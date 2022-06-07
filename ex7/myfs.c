#include <string.h>
# include "myfs.h"
#include <stdlib.h>


void mymkfs(char fs_size) {
    ginfo.num_inodes = (unsigned char)((fs_size*0.1)/ sizeof(struct inode));
    ginfo.num_blocks = (unsigned char)((fs_size*0.9) / sizeof(struct disk_info));
    ginfo.size_blocks = (unsigned char)((fs_size*0.9));

    int i;
    inodes = malloc (sizeof (struct inode) * ginfo.num_inodes);
    for(i = 0; i<ginfo.num_inodes; i++) {
        inodes[i].size = -1;
        strcpy (inodes[i].name, "");
    }

    dinfo = malloc (sizeof (struct disk_info) * ginfo.size_blocks);
    for (i=0; i<ginfo.num_blocks; i++) {
        dinfo[i].next_block_num = -1;
    }
}