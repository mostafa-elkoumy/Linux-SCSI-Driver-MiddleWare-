#include <stdio.h>
#include <scsi/sg.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <scsi/scsi_ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "CommonDefinitions.h"
#include <fstream>
#include <iostream>


using namespace std;

/* process a complete scsi cmd. Use the generic scsi interface. */
static DWORD slsfd_linux_DriveManager_SendSCSICmd(char *devPath, SLSCSICmd *scsiCmd)
{
    struct sg_io_hdr *psgHeader = new struct sg_io_hdr;
    memset(psgHeader, 0, sizeof(struct sg_io_hdr));

    if (psgHeader)
    {
        psgHeader->interface_id = 'S'; /* this is the only choice we have! */
        psgHeader->flags = SG_FLAG_LUN_INHIBIT; /* this would put the LUN to 2nd byte of cdb*/
    }

    BYTE* buffer = new BYTE[128];
    memset(buffer, 0, 128);

    BYTE* sense = new BYTE[32];
    memset(sense, 0, 32);

    int fd;

    fd = open(devPath, O_RDWR);
    if(fd <= 0)
    {
        return SLSFD_FAILURE;
    }
    if(scsiCmd)
    {
        if(scsiCmd->SRB_Flags == SRB_DIR_OUT)
        {
            psgHeader->dxfer_direction = SG_DXFER_TO_DEV;
            memcpy(buffer, scsiCmd->SRB_BufPointer, scsiCmd->SRB_BufLen);
        }
        else
        {
            psgHeader->dxfer_direction = SG_DXFER_FROM_DEV;
        }
        psgHeader->cmdp = scsiCmd->CDBByte;
        psgHeader->cmd_len = 6;
        psgHeader->dxferp = buffer;
        psgHeader->dxfer_len = scsiCmd->SRB_BufLen;
        psgHeader->sbp = sense;
        psgHeader->mx_sb_len = 32;

        int ret = ioctl(fd, SG_IO, psgHeader);

        if ((ret<0) || (psgHeader->status != 0))
        {
            close(fd);
            delete(psgHeader);
            delete(buffer);
            delete(sense);
            return SLSFD_FAILURE;
        }
        BYTE i;
        if(scsiCmd->SRB_Flags == SRB_DIR_IN)
        {
            for(i=0 ; i<scsiCmd->SRB_BufLen ; i++)
            {
                putchar(buffer[i]);
            }
        }
    }
    close(fd);
    delete(psgHeader);
    delete(buffer);
    delete(sense);

    return SLSFD_SUCCESS;
}

int main(int argc, char *argv[])
{
    unsigned char shortCommand[8];
    char path[9];
    unsigned char dataCommand[64];
     memset(dataCommand, 0, 64);

    memcpy(shortCommand, argv[2], 8);
    strcpy(path, argv[1]);
    if(shortCommand[0] == SRB_DIR_OUT)
    {
        memcpy(dataCommand, argv[3], shortCommand[1]);
        int i;
        for(i=0 ; i<shortCommand[1] ; i++)
        {
            printf("%d ", dataCommand[i]);
        }
    }

    SLSCSICmd scsiCommand;
    memset(&scsiCommand, 0, sizeof(scsiCommand));

    scsiCommand.SRB_Flags = shortCommand[0];
    scsiCommand.SRB_BufLen = shortCommand[1];
	BYTE i;
	for(i=0 ; i<6 ; i++)
	{
       scsiCommand.CDBByte[i] = shortCommand[i+2] - 48;
	}
	if(scsiCommand.SRB_Flags == SRB_DIR_OUT)
	{
       scsiCommand.SRB_BufPointer = dataCommand;
	}
	
    slsfd_linux_DriveManager_SendSCSICmd(path, &scsiCommand);
   

    return 0;
}

