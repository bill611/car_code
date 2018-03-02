#ifndef MemoryCopyH
#define MemoryCopyH 
//----------------------------------------------------------------------------
//  �Ż��ڴ濽�����̣����ٷ�ָ֧���������ˮ���ж�
//  ÿִ��8�ο����ж�һ��
//  ���width����8��������,�ڵ�һ�ο���ʱ���е���
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
//  �ڴ���ٿ���,sizeΪ�ֽ���
//----------------------------------------------------------------------------
static __inline void FastCopy(void *dst,void *src,int size)
{
	//ʹ��32��������8λ�����Լӿ쿽���ٶ�
	unsigned int *pSrc = (unsigned int *)src;
	unsigned int *pDst = (unsigned int *)dst;
//	char *pcSrc;
	char *pcDst;

	DUFFS_LOOP8( {*pDst++=*pSrc++;} ,size/4);

	//����ʣ���ֽ�
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
