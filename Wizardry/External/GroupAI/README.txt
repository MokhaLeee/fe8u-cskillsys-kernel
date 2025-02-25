This patch enables an attack AI in which if any one person in the group enters the attack range, the whole group will charge in and attack.

The group is specified by AI4(Retreat AI).

In vanilla, AI4 has only 0x00 and 0x20, but groups are created by writing the group number there.

For example, AI4=0x1 means group 1, AI4=0x2 means group 2 and AI4=0x3 means group 3.

It can be used at the same time as 0x20, which is used for the boss: 0x21 for group 1, 0x22 for group 2 and 0x23 for group 3.
Patch Name:Group AI  @FE8U
Author / Source:Vesly https://github.com/Veslyquix/ASM/tree/main/GroupAI , Originally by PhantomeSentine
