#!/bin/bash
dd if=/dev/zero of=bean bs=1k count=512000
losetup /dev/loop0 bean
mke2fs /dev/loop0
mount -t ext2 /dev/loop0 /mnt/bean
mount
dd if=/dev/loop0 bs=1k count=265 |od -tx1 -Ax > /tmp/dump_sp_hex0
mkdir /mnt/bean/lost+found/test
cp /home/gen/apue_note/code/ext2/2 /mnt/bean/lost+found/test
cp /home/gen/apue_note/code/ext2/1.txt /mnt/bean/lost+found
cp /home/gen/apue_note/code/ext2/2 /mnt/bean/lost+found
cp /home/gen/apue_note/code/ext2/3 /mnt/bean/lost+found
mkdir /mnt/bean/lost+found/test/test2
cp /home/gen/apue_note/code/ext2/1.txt /mnt/bean/lost+found/test/test2
chmod o+r /dev/loop0
