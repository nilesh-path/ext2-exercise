#include <linux/types.h>

struct ext2_super_block {
__u32 s_inodes_count;	/* Inodes count */
__u32 s_blocks_count;	/* Blocks count */
__u32 s_r_blocks_count;	/* Reserved blocks count */
__u32 s_free_blocks_count;	/* Free blocks count */
__u32 s_free_inodes_count;	/* Free inodes count */
__u32 s_first_data_block;	/* First Data Block [always 1 --pls]*/
__u32 s_log_block_size;	/* Block size */
/* s_log_block_size expresses block size as a power of 2, using 1024
* bytes as the unit. So 0 would be a 1024-byte block, 1 a 2048-byte
* block, etc. Note that the fragmentation stuff (below) is not yet
* implemented --pls */
__s32 s_log_frag_size;	/* Fragment size */
__u32 s_blocks_per_group;	/* # Blocks per group */
__u32 s_frags_per_group;	/* # Fragments per group */
__u32 s_inodes_per_group;	/* # Inodes per group */
__u32 s_mtime;	/* [last] Mount time */
__u32 s_wtime;	/* Write time */
/* fields below cause the filesystem checker (ext2fsck) to
* run after a predefined number of mounts or a certain amount of
* time has passes since the last check. --pls */
__u16 s_mnt_count;	/* Mount count */
__s16 s_max_mnt_count;	/* Maximal mount count [before check]*/
__u16 s_magic;	/* Magic signature */
__u16 s_state;	/* File system state */
__u16 s_errors;	/* Behaviour when detecting errors */
__u16 s_minor_rev_level;	/* minor revision level */
__u32 s_lastcheck;	/* time of last check */
__u32 s_checkinterval;	/* max. time between checks */
__u32 s_creator_os;	/* OS */
__u32 s_rev_level;	/* Revision level */
__u16 s_def_resuid;	/* Default uid for reserved blocks*/
__u16 s_def_resgid;	/* Default gid for reserved blocks */
__u8 s_prealloc_blocks;	/* Nr of blocks to try to preallocate*/
__u8 s_prealloc_dir_blocks; /* Nr to preallocate for dirs */
__u16 s_padding1;	/* Padding to the end of the block */
__u32 s_reserved[204];
};




struct ext2_group_desc
{
__u32 bg_block_bitmap;	/* [number of] Blocks bitmap block */
__u32 bg_inode_bitmap;	/* [number of]Inodes bitmap block */
/* the next field actually holds the block number of the first block
* of the inode table --pls*/
__u32 bg_inode_table;	/* Inodes table block */
__u16 bg_free_blocks_count;	/* Free blocks count */
__u16 bg_free_inodes_count;	/* Free inodes count */
__u16 bg_used_dirs_count;	/* Directories count */
__u16 bg_pad;	/* alignment to word boundary --pls*/
__u32 bg_reserved[3];	/* nulls to pad out 24 bytes --pls*/
};


struct ext2_inode {
__u16 i_mode;	/* File mode */
__u16 i_uid;	/* Low 16 bits of Owner Uid */
/* The highest-order bit of i_size is not used, effectively limiting
* the file size to 2GB, though there are ways of getting around this
* on certain 64-bit architectures. --pls */
__u32 i_size;	/* Size in bytes */
__u32 i_atime;	/* [last] Access time */
__u32 i_ctime;	/* Creation time */
__u32 i_mtime;	/* Modification time */
__u32 i_dtime;	/* Deletion Time */
__u16 i_gid;	/* Low 16 bits of Group Id */
__u16 i_links_count; /* Links count */
/* Note that each file is currently stored in a whole number of
* blocks, so an empty file will start out with one block.
* This may change when/if fragmentation is implemented --pls*/
__u32 i_blocks;	/* Blocks count */
__u32 i_flags;	/* File flags */
__u32 i_osd1;	/* big union to hold os-specific stuff --pls*/
__u32 i_block[15]; /* Pointers to [data] blocks */
__u32 i_generation;
__u32 i_file_acl;
__u32 i_dir_acl;
__u32 i_faddr;
__u32 i_osd2[3];
};

struct ext2_dir_entry {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u8	name_len;		/* Name length */
	__u8	file_type;
//	char	name[];	/* File name */
};
