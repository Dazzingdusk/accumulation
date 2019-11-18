
#include "ring.h"
#include <string.h>
#include <stddef.h>

#define min(a,b) ( (a) < (b) )? (a):(b)     

/**
  * @brief	初始化一个空环形缓冲区 
  *         
  * @note 	这里的 size 是指2^size字节大小的缓冲区。
  *      
  * @param	r环形缓冲区控制块
  * @param 	mem 指向一个内存块。
  * @param 	size内存块的大小。  
  *         
  * @retval	-1 错误        
  *			-0 成功        
  */
int ring_buf_create(ring_buf_t *r,unsigned char *mem,unsigned int size)
{
    if(r && mem)
    {
        r->buf = buf;
        r->size = (0x00000001 << size);
        r->front = r->rear = 0;
    }
    else
    {
        return 1;   
    }
    return 0;
}
/**
  * @brief	清空环形缓冲区 
  *             
  * @param	r环形缓冲区控制块  
  *         
  * @retval	None               
  */
void ring_buf_clr(ring_buf_t *r)
{
    r->front = r->rear = 0;
}
/**
  * @brief	获取环形缓冲区容量 (byte)
  *             
  * @param	r环形缓冲区控制块  
  *         
  * @retval	缓冲区中有效字节数               
  */
unsigned int ring_buf_len(ring_buf_t *r)
{
    return (unsigned int)(r->rear - r->front);
}
/**
  * @brief	向缓冲区中写入数据 
  *         
  * @note 	这里如果缓冲区无法写入全部待写入的数据，函数将返回可以写入的数据长度。
  *      
  * @param	r 环形缓冲区控制块
  * @param 	buf 待写入的数据块。
  * @param 	len 待写入数据块的大小。  
  *         
  * @retval	实际写入到缓冲区的字节数        
  *			        
  */
unsigned int ring_buf_put(ring_buf_t *r,unsigned char *buf,unsigned int len)
{
    unsigned int l;
    /*取空闲内存和待写入数据长度中小的*/
    len = min(len , r->size + r->front - r->rear);                  
    /*取空闲内存后半部分长度和待写入数据长度中最小的*/
    l = min(len, r->size - (r->rear & (r->size-1)));   
    /* 默认空闲内存是不连续 分两部分拷入 ---> |00001111111100000*/     
    memcpy(r->buf + (r->rear & (r->size-1)), buf, l); 
    memcpy(r->buf, buf + l, len - l);
    /* 直接移动 写入指针 溢出时依然可以正常计算 */    
    r->rear += len;     
    return len;

}
/**
  * @brief	从缓冲区中读取数据 
  *         
  * @note 	这里如果缓冲区无充足的数据，函数将读取缓冲区全部的数据。
  *      
  * @param	r 环形缓冲区控制块
  * @param 	buf 读出的数据存放地址。
  * @param 	len 需读取数据块的大小。  
  *         
  * @retval	实际读出到缓冲区的字节数        
  *			        
  */
unsigned int ring_buf_get(ring_buf_t *r,unsigned char *buf,unsigned int len)
{
    unsigned int l; 
    /*取有效数据和待读取数据长度中小的*/
    len = min(len , r->rear - r->front); 
    /*取有效数据后半部分长度和待写入数据长度中最小的*/    
    l = min(len, r->size - (r->front & (r->size-1))); 
    /* 默认数据是分开的 ---> |1111110000000001111111*/    
    memcpy(buf, r->buf + (r->front & (r->size-1)), l);    
    memcpy(buf + l, r->buf, len - l);
    /* 直接移动 读取指针 溢出时依然可以正常计算 */
    r->front += len;        
    return len;
}
