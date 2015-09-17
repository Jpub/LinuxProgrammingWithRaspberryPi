#ifndef __WAVHEADER_H
#define __WAVHEADER_H

typedef struct wavFile_t {
  unsigned char  riffID[4];                  /* "RIFF" */           /* riffID[] = {'R', 'I', 'F', 'F'}; */
  unsigned long  riffLen;                    /* Size of data to follow */
  unsigned char  waveID[4];              /* "WAVE" */         /* waveID[] = {'W', 'A', 'V', 'E'}; */
  unsigned char  fmtID[4];                 /* "fmt " */             /* fmtID[] = {'f', 'm', 't', ' '}; */
  unsigned long  fmtLen;                   /* 16 */
  unsigned short fmtTag;                   /* 1 */
  unsigned short nChannels;             /* 채널의 수 */
  unsigned long  sampleRate;           /* 샘플링 레이트 */
  unsigned long  avgBytesPerSec;    /* Bytes / sec */
  unsigned short nblockAlign;            /* 1 */
  unsigned short bitsPerSample;       /* Bits / sample */
  unsigned char  dataID[4];                /* "data" */           /* dataID[] = {'d', 'a', 't', 'a'}; */
  unsigned long  dataLen;                  /* 샘플의 수 */
} WAVHEADER;

#endif

