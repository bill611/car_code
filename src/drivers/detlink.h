#ifndef  _detlink_h
#define  _detlink_h

#ifdef __cplusplus
extern "C"
{
#endif

int dec_init();
int decodefunc(unsigned char *istream,int istream_size,unsigned char *ostream,int * LeaveSize);
int decodefunc2(unsigned char *istream,int istream_size,unsigned char *ostream,int * LeaveSize);

#ifdef __cplusplus
}
#endif

#endif
