#ifndef MemoryCopyH
#define MemoryCopyH 
//----------------------------------------------------------------------------
//  优化内存拷贝过程，减少分支指令带来的流水线中断
//  每执行8次拷贝判断一次
//  如果width不是8的整数倍,在第一次拷贝时进行调整
//----------------------------------------------------------------------------
#define DUFFS_LOOP8(pixel_copy_increment, width)	\
{ int n = ((width)+7)/8;							\
	switch ((width) & 7) {							\
	default: do {	pixel_copy_increment;			\
	case 7:		pixel_copy_increment;				\
	case 6:		pixel_copy_increment;				\
	case 5:		pixel_copy_increment;				\
	case 4:		pixel_copy_increment;				\
	case 3:		pixel_copy_increment;				\
	case 2:		pixel_copy_increment;				\
	case 1:		pixel_copy_increment;				\
		} while ( --n > 0 );						\
	}												\
}
//----------------------------------------------------------------------------
//  内存快速拷贝,size为字节数
//----------------------------------------------------------------------------
static __inline void FastCopy(void *dst,void *src,int size)
{
	//使用32拷贝代替8位拷贝以加快拷贝速度
	unsigned int *pSrc = (unsigned int *)src;
	unsigned int *pDst = (unsigned int *)dst;
//	char *pcSrc;
	char *pcDst;

	DUFFS_LOOP8( {*pDst++=*pSrc++;} ,size/4);

	//拷贝剩余字节
//	pcSrc = (char*)pSrc;
	pcDst = (char*)pDst;
	switch(size % 4)
	{
	default:
	case 3:		*pcDst++=*pSrc++;
	case 2:		*pcDst++=*pSrc++;
	case 1:		*pcDst++=*pSrc++;
	}
}
//----------------------------------------------------------------------------
#endif
