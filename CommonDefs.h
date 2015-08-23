#ifndef COMMONDEFINITIONS_H
#define COMMONDEFINITIONS_H

#define SRB_DIR_IN                  97        // Transfer from SCSI target to host
#define SRB_DIR_OUT                 98        // Transfer from host to SCSI target
#define SRB_EVENT_NOTIFY            0x40        // Enable ASPI event notification

#define SLSFD_SUCCESS													0x00000000
#define SLSFD_FAILURE													0xffffffff

typedef unsigned int       DWORD;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

typedef struct
{
    BYTE        SRB_Flags;                      // ASPI request flags
    DWORD       SRB_BufLen;                     // Data Allocation Length
    BYTE        SRB_CDBLen;                     // CDB Length
    BYTE        CDBByte[16];                    // SCSI CDB
    BYTE        *SRB_BufPointer;            // Data Buffer Pointer
    BYTE        SRB_Status;                     // ASPI command status byte
}SLSCSICmd;

#endif // COMMONDEFINITIONS_H
