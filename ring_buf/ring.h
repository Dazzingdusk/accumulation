#ifndef _RING_H_
#define _RING_H_

/*环形缓冲区管理器*/
typedef struct
{
     unsigned char *buf;    /*缓冲区            */
     unsigned int size;     /*缓冲区大小        */
     unsigned int front;    /*读指针            */
     unsigned int rear;     /*写指针            */
}ring_buf_t;

/*-------------------------接口声明----------------------------*/

int ring_buf_create(ring_buf_t *r,unsigned char *buf,unsigned int size);

void ring_buf_clr(ring_buf_t *r);

unsigned int ring_buf_len(ring_buf_t *r);

unsigned int ring_buf_put(ring_buf_t *r,unsigned char *buf,unsigned int len);

unsigned int ring_buf_get(ring_buf_t *r,unsigned char *buf,unsigned int len);

#endif

