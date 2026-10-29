#include "modbus_rtu.h"
#include "crc16.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static size_t frame(uint8_t *dst,const uint8_t *body,size_t len){uint16_t crc;memcpy(dst,body,len);crc=modbus_crc16(dst,len);dst[len]=(uint8_t)crc;dst[len+1]=(uint8_t)(crc>>8U);return len+2U;}
int main(void){
 modbus_device_t d;uint8_t req[32],out[MODBUS_MAX_ADU];size_t n;uint8_t read[]={1,3,0,0,0,2};
 modbus_device_init(&d,1);d.holding[0]=1;d.holding[1]=123;
 n=modbus_process_request(&d,req,frame(req,read,sizeof(read)),out,sizeof(out));
 assert(n==9&&out[1]==3&&out[2]==4&&out[4]==1&&out[6]==123);
 {uint8_t badaddr[]={1,3,0,15,0,2};n=modbus_process_request(&d,req,frame(req,badaddr,sizeof(badaddr)),out,sizeof(out));assert(n==5&&out[1]==0x83&&out[2]==2);}
 {uint8_t unsupported[]={1,0x55,0,0,0,0};n=modbus_process_request(&d,req,frame(req,unsupported,sizeof(unsupported)),out,sizeof(out));assert(n==5&&out[1]==0xD5&&out[2]==1);}
 n=frame(req,read,sizeof(read));req[n-1]^=1;assert(modbus_process_request(&d,req,n,out,sizeof(out))==0);assert(d.rejected_frames==1);
 {uint8_t other[]={2,3,0,0,0,1};n=frame(req,other,sizeof(other));assert(modbus_process_request(&d,req,n,out,sizeof(out))==0);}
 puts("All Modbus RTU tests passed.");return 0;
}
