#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "superblock.h"
#include <time.h>
#include <pwd.h>
#define KBLU  "\x1B[34m"
#define KNRM  "\x1B[0m"
//#define DEBUG

int f,count=0,block_size=0,first_block_bitmap_id=0,second_block_bitmap_id=0,third_block_bitmap_id=0;
char but[1025];
int reserve_inode,Pdir_inode,size = 0;
int inode_table[5];
struct ext2_super_block es;
struct ext2_group_desc gdis;
struct ext2_inode inode;
struct ext2_inode inodelist[50];
struct ext2_dir_entry dentry;
char dirlist[50][50];
char filelist[50][50];
int dcount,fcount,dinode[100],search_finode=0,name_count=0;
int print_imode = 0;
void print_superblock(struct ext2_super_block es)
{
#ifdef DEBUG
        printf("s_inodes_count = %d\n",es.s_inodes_count);
        printf("s_blocks_count = %d\n",es.s_blocks_count);
        printf("s_r_blocks_count = %d\n",es.s_r_blocks_count);
        printf("s_free_blocks_count = %d\n",es.s_free_blocks_count);
        printf("s_free_inodes_count = %d\n",es.s_free_inodes_count);
        printf("s_first_data_block = %d\n",es.s_first_data_block);
        printf("s_log_block_size = %d\n",es.s_log_block_size);
        printf("s_log_frag_size = %d\n",es.s_log_frag_size);
        printf("s_blocks_per_group = %d\n",es.s_blocks_per_group);
        printf("s_frags_per_group = %d\n",es.s_frags_per_group);
        printf("s_inodes_per_group = %d\n",es.s_inodes_per_group);
#endif
}

void print_gdiscriptor(struct ext2_group_desc gdis)
{
#ifdef DEBUG
        printf("bg_block_bitmap = %d\n",gdis.bg_block_bitmap);
        printf("bg_inode_bitmap = %d\n",gdis.bg_inode_bitmap);
        printf("bg_inode_table = %d\n",gdis.bg_inode_table);
        printf("bg_free_blocks_count = %d\n",gdis.bg_free_blocks_count);
        printf("bg_free_inodes_count = %d\n",gdis.bg_free_inodes_count);
        printf("bg_used_dirs_count = %d\n",gdis.bg_used_dirs_count);
        printf("bg_pad = %d\n",gdis.bg_pad);
#endif
}

void print_inode(struct ext2_inode inode)
{
#ifdef DEBUG
        printf("i_mode = %x\n",inode.i_mode);
        printf("i_uid = %x\n",inode.i_uid);
        printf("i_size = %x\n",inode.i_size);
        printf("i_atime = %x\n",inode.i_atime);
        printf("i_ctime = %x\n",inode.i_ctime);
        printf("i_mtime = %x\n",inode.i_mtime);
        printf("i_dtime = %x\n",inode.i_dtime);
        printf("i_gid = %x\n",inode.i_gid);
        printf("i_links_count = %x\n",inode.i_links_count);
        printf("i_blocks = %x\n",inode.i_blocks);
        printf("i_flags = %x\n",inode.i_flags);
        printf("i_osd1 = %x\n",inode.i_osd1);
        printf("1 i_block = %x\n",inode.i_block[0]);
        printf("2 i_block = %x\n",inode.i_block[3]);
        printf("3 i_block = %x\n",inode.i_block[7]);
        printf("4 i_block = %x\n",inode.i_block[11]);
        printf("5 i_block = %x\n",inode.i_block[12]);
        printf("6 i_block = %x\n",inode.i_block[13]);

#endif
}

void print_dir_data_block()
{
int f_end,f_start,strlnth = 0,loop=0;
__u32 d_create=0,d_delete=0,d_mode=0;
char tmp_inode[1025];
char fname[100];
//char *fname;
int soffset=0;
        count = 0;
	dcount = 0;
	fcount = 0;
	while(1)
	{
	memcpy(&dentry, &but[count], sizeof(dentry));
//	count+=dentry.rec_len;
//	count+=sizeof(dentry);
	if(!dentry.inode)
	break;

	lseek(f, inode_table[(dentry.inode-1)/es.s_inodes_per_group]*block_size, SEEK_SET);
//	soffset = lseek(f, 0, SEEK_CUR);
//	printf("1 offset = %d\n",soffset);
//	printf("dentry.inode = %d\n",dentry.inode);
//	printf("es.s_inodes_per_group = %d\n",es.s_inodes_per_group);

	lseek(f, ((dentry.inode % es.s_inodes_per_group)-1) * 128, SEEK_CUR);
//	soffset = lseek(f, 0, SEEK_CUR);
//	printf("2 offset = %d\n",soffset);
        read(f, tmp_inode, 128);

        memcpy(&inode, &tmp_inode[0], sizeof(inode));

	if(((inode.i_mode & 0x4000) && (dentry.file_type == 2)) || ((inode.i_mode & 0x8000) && (dentry.file_type == 1)))
	{
#ifdef DEBUG
        if(inode.i_mode || inode.i_links_count)
        {
        printf("Reading inode c = %x d = %x\n",inode.i_ctime,inode.i_dtime);
        print_inode(inode);
        }
#endif

	if(inode.i_dtime < inode.i_ctime)
	{
#ifdef DEBUG
        printf("inode no. = %d\n",dentry.inode);
        printf("rec_len = %d\n",dentry.rec_len);
        printf("name_len = %d\n",dentry.name_len);
        printf("file_type = %d\n",dentry.file_type);
#endif
	memcpy(&fname, &but[count+sizeof(dentry)], dentry.name_len);
	fname[dentry.name_len] = '\0';

      dinode[dcount]=dentry.inode;
      strcpy(dirlist[dcount],fname);
      memcpy(&inodelist[dcount], &inode, sizeof(inode));
	dcount++;

//	if(dentry.file_type == 2)
//	{
//	dinode[dcount]=dentry.inode;
//	strcpy(dirlist[dcount],fname);
//	memcpy(&inodelist[], &inode, sizeof(inode));
//#ifdef DEBUG
//	printf("Directory in dlist = %s\n",dirlist[dcount]);
//#endif
//	dcount++;
//	}
//	else
//	{
//	strcpy(filelist[fcount],fname);
//#ifdef DEBUG
//        printf("name = %s\n",fname);
//#endif
//	fcount++;
//	}
	}
	}
//	count+=dentry.name_len;
//	count = count + ( 4 - (count % 4));
	count+=dentry.rec_len;
	}
}


int print_block_group(int bitmap_id)
{
int loop,inode_offset=0,offset=0;
#ifdef DEBUG
	printf("Reading block bitmap block\n");
#endif
	offset = lseek(f, block_size * (bitmap_id), SEEK_SET);

        read(f, but, 1024);

        count = 0;

#ifdef DEBUG
	do{
        memcpy(&size, &but[count], 4);
        printf("1 block_bitmap = %x\n",size);
	count+=4;
	}
	while(size);
#endif

#ifdef DEBUG
	printf("Reading inode bitmap block\n");
#endif

        read(f, but, 1024);

        count = 0;

#ifdef DEBUG
        do{
	memcpy(&size, &but[count], 4);
        printf("2 inode_bitmap = %x\n",size);
	count+=4;
	}
	while(count<40);
#endif

	if(reserve_inode)
	offset = lseek(f, reserve_inode * 128, SEEK_CUR); // reserved

//	printf("offset %d\n",offset);
//	printf("inode size %d\n",sizeof(inode));

	for(loop=0;loop<es.s_inodes_per_group;loop++)
	{

	if(inode_offset)
	lseek(f, inode_offset, SEEK_SET);
        read(f, but, 128);

	inode_offset = lseek(f, 0, SEEK_CUR);

	memcpy(&inode, &but[0], sizeof(inode));

	if(inode.i_mode || inode.i_links_count)
	{
#ifdef DEBUG
	printf("Reading %dth unreserved inode\n",loop);
#endif
	print_inode(inode);
	}

	if(inode.i_block[0] && inode.i_dtime < inode.i_ctime)
	{
#ifdef DEBUG
        printf("Reading data block block %d\n",inode.i_block[0]);

	printf("Reading data from %d\n",block_size * (inode.i_block[0] -1));
#endif
        lseek(f, block_size * (inode.i_block[0]), SEEK_SET);

        read(f, but, 1024);

//      memcpy(&size, &but[count], 4);
//        printf("data = %s\n",but[0]);
//        count+=4;


        count = 0;
	if(inode.i_mode & 0x4000)
	{
	count = 0;
        do{
        memcpy(&size, &but[count], 4);
        printf("%d databocks = %x\n",count,size);
        count+=4;
        }
        while(count<50);


	print_dir_data_block();
	}
#ifdef DEBUG
	else
	do{
        memcpy(&size, &but[count], 4);
        printf("i_blocks = %x\n",size);
        count+=4;
	}
	while(size);
#endif

	}
	}

return 0;
}


int print_f_permissions(struct ext2_inode inode)
{
struct tm *info;
struct passwd *pw;
int bitmap = 0x4000;
if(inode.i_mode & bitmap) printf("d");
else printf("-");
bitmap = bitmap >> 6;
while(bitmap)
{
if(inode.i_mode & bitmap) printf("r");
else printf("-");
bitmap = bitmap >> 1;
if(inode.i_mode & bitmap) printf("w");
else printf("-");
bitmap = bitmap >> 1;
if(inode.i_mode & bitmap) printf("x");
else printf("-");
bitmap = bitmap >> 1;
}
printf("\t");
pw = getpwuid(inode.i_uid);
printf("%s\t",pw->pw_name);
printf("%d\t",inode.i_size);
info = localtime( &inode.i_mtime );
printf("%s", asctime(info));
}

int list_files(char *file_name,int f_inode)
{
dcount = 0;
fcount = 0;

//      lseek(f, block_size * (first_block_bitmap_id + 2), SEEK_SET);
	lseek(f, inode_table[(f_inode-1)/es.s_inodes_per_group]*block_size, SEEK_SET);
	lseek(f, ((f_inode % es.s_inodes_per_group)-1) * 128, SEEK_CUR);
//      read(f, but, 128);
        read(f, but, 128);
        memcpy(&inode, &but[0], sizeof(inode));
        lseek(f, block_size * inode.i_block[0], SEEK_SET);

        read(f, but, 1024);
        count = 0;
	name_count--;
        if(inode.i_mode & 0x4000)
        print_dir_data_block();

	while(dcount--)
	{
	if(print_imode)
	{
//	printf("imode = %x\t",inodelist[dcount].i_mode);

	if(inodelist[dcount].i_mode & 0x4000)
	printf("%s",KBLU);
        printf("%s\t",dirlist[dcount]);
	printf("%s", KNRM);
	print_f_permissions(inodelist[dcount]);
	}
	if((file_name)&&(!strcmp(dirlist[dcount], file_name)))
	{
	if(inodelist[dcount].i_mode & 0x4000)
	{
		if(name_count==0)
		print_imode = 1;
		return dinode[dcount];
	}
	else
	{
		printf("%s\t",file_name);
		print_f_permissions(inodelist[dcount]);
		return 0;
	}
	}
	}
//        while(dcount--)
//        {
//        printf("%sdirectories = %s - %d\n",KBLU,dirlist[dcount],dinode[dcount]);
//	printf("%s", KNRM);
//	if(file_name)
//	if(!strcmp(dirlist[dcount], file_name))
//	return dinode[dcount];
//	}
//        while(fcount--)
//	{
//        printf("files = %s\n",filelist[fcount]);
//	if(file_name)
//	if(!strcmp(filelist[fcount], file_name))
//	return 0;
//	}
	if(!file_name)
	return 0;
return -1;
}


int main(int argc, char *argv[])
{
int loop,inode_offset=0,result=0;
char *search_fname;
char regular_file[50],tmp_file[50],*tmp2_file;


	if(argc < 2)
	{
	printf("please mention the filesystem file\n");
        return 0;
	}
//	f = open("/home/nilesh.tekale/exercise/sampleext2", O_RDONLY);
	f = open(argv[1], O_RDONLY);
        read(f, but, 1024);

#ifdef DEBUG
        printf("Reading super block\n");
#endif
        read(f, but, 1024);

        memcpy(&es, &but[0], sizeof(es));
        block_size = 1024 << es.s_log_block_size;

        print_superblock(es);   // Superblock

#ifdef DEBUG
        printf("Reading group discriptor block\n");
#endif

        read(f, but, 1024);
        memcpy(&gdis, &but[0], sizeof(gdis));

#ifdef DEBUG
        printf("First block group\n");
#endif

        first_block_bitmap_id = gdis.bg_block_bitmap;

	inode_table[0] = gdis.bg_inode_table;

        print_gdiscriptor(gdis);        // First block group

        memcpy(&gdis, &but[sizeof(gdis)], sizeof(gdis));


#ifdef DEBUG
        printf("Second block group\n");
#endif

        second_block_bitmap_id = gdis.bg_block_bitmap;

	inode_table[1] = gdis.bg_inode_table;

        print_gdiscriptor(gdis);        // Second block group

        memcpy(&gdis, &but[2 * sizeof(gdis)], sizeof(gdis));


#ifdef DEBUG
        printf("Third block group\n");
#endif

        third_block_bitmap_id = gdis.bg_block_bitmap;

	inode_table[2] = gdis.bg_inode_table;

        print_gdiscriptor(gdis);        // third block group


//	reserve_inode = 11;
//	print_block_group(first_block_bitmap_id);
//	reserve_inode = 0;
//	print_block_group(second_block_bitmap_id);
//	print_block_group(third_block_bitmap_id);
//	printf("pdir = %d\n",Pdir_inode);





//	lseek(f, block_size * (first_block_bitmap_id + 2), SEEK_SET);
//        for(loop=0;loop<4 /*es.s_inodes_per_group*/;loop++)
//        {
//
//      if(inode_offset)
//      lseek(f, inode_offset, SEEK_SET);
//        read(f, but, 128);
//
//      inode_offset = lseek(f, 0, SEEK_CUR);
//        memcpy(&inode, &but[0], sizeof(inode));
//
////        if(inode.i_mode || inode.i_links_count)
//        {
//        printf("Reading %dth unreserved inode\n",loop);
//        print_inode(inode);
//        }
//
//        if(inode.i_block[0])
//        {
//        printf("Reading data block block %d\n",inode.i_block[0]);
//
//        printf("Reading data from %d\n",block_size * (inode.i_block[0] /*-1*/));
//
//        lseek(f, block_size * (inode.i_block[0]), SEEK_SET);
//
//        read(f, but, 1024);
//
////      memcpy(&size, &but[count], 4);
////        printf("data = %s\n",but[0]);
////        count+=4;
//        count = 0;
//        if(inode.i_mode & 0x4000)
//        print_dir_data_block();
//        else
//        do{
//        memcpy(&size, &but[count], 4);
//        printf("i_blocks = %x\n",size);
//        count+=4;
//        }
//        while(count<80);
//
//        }
//        }

	if(argc > 2)
	{
	strcpy(tmp_file,argv[2]);
	tmp2_file = strtok(tmp_file,"/");
	while(tmp2_file)
	{
	name_count++;
	tmp2_file = strtok(NULL,"/");
//	if(!tmp2_file)
//	break;
	}
	}

	if(argc < 3)
	search_fname = NULL;
	else
	{
	strcpy(regular_file,argv[2]);
        search_fname = strtok(argv[2],"/");
	}
	search_finode = 2;
	result = list_files(search_fname,search_finode);
	while(1)
	{
	if(result<0)
	{
		printf("%s: No such file exists\n",regular_file);
		return 0;
	}
	else if(result)
	{
		search_finode = result;
		search_fname = strtok(NULL,"/");
		result = list_files(search_fname,search_finode);
        }
	else
	{
//		if(argc >= 2)
//		printf("The file %s exist\n",regular_file);
		return 0;
        }
        }

return 0;
}

