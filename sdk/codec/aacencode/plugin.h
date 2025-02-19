#ifndef __PLUGIN_H__
#define __PLUGIN_H__

typedef struct AUDIO_ENCODE_PARAM_TAG
{
    int nInputBufferType;  // 0: buffer from audio encoder, 1:buffer from other RISC AP, 2 : buffer from I2S
    int nEnableMixer; // 0: not use mixer    
    unsigned int nStartPointer; // if nInputBufferType == 2, it can setup start pointer
    unsigned int nStartTime; // time offset    
    unsigned char* pInputBuffer;   // I2S has most 5 input buffer pointers
    unsigned char* pInputBuffer1; 
    unsigned char* pInputBuffer2;
    unsigned char* pInputBuffer3;
    unsigned char* pInputBuffer4;    
    unsigned char* pMixBuffer; // buffer to be mixed    
    unsigned int nBufferLength;  // the length of the input buffer
    unsigned int nMixBufferLength;  // the length of the mix buffer    
    unsigned int nChannels; // most support to 7.1
    unsigned int nSampleRate;  // input sample rate
    unsigned int nOutSampleRate;  // output sample rate;
    unsigned int nBitrate;  // encode bitrate    
    int nInputAudioType; // 0:PCM
    int nSampleSize; // 16 or 32, if audio type == pcm
}AUDIO_ENCODE_PARAM;

#if 0//defined(ENABLE_CODECS_PLUGIN)
#  include "aud/codecs.h"
extern struct _codec_api *ci;
#  undef  taskYIELD
#  define PalSleep              ci->sleep
#  define PalGetClock           ci->PalGetClock
#  define PalGetDuration        ci->PalGetDuration
#  define PalGetSysClock        ci->PalGetSysClock
#  define PalWFileOpen          ci->PalWFileOpen  
#  define PalFileWrite          ci->PalFileWrite
#  define PalFileClose          ci->PalFileClose
#  define f_enterFS             ci->f_enterFS
#  define PalHeapAlloc          ci->PalHeapAlloc
#  define PalHeapFree           ci->PalHeapFree
#  define taskYIELD             ci->yield
#  define deactiveDAC           ci->deactiveDAC
#  define deactiveADC           ci->deactiveADC
#  define pauseDAC              ci->pauseDAC
#  define pauseADC              ci->pauseADC
#  define initDAC               ci->initDAC
#  define initADC               ci->initADC
#  define initCODEC             ci->initCODEC
#  define or32_invalidate_cache ci->flush_dcache
#  define dc_invalidate         ci->flush_all_dcache
#  define I2S_AD32_GET_RP       ci->I2S_AD32_GET_RP
#  define I2S_AD32_GET_WP       ci->I2S_AD32_GET_WP
#  define I2S_AD32_SET_RP       ci->I2S_AD32_SET_RP
#  define getMixerReadPorinter  ci->getMixerReadPorinter
#  define getMixerWritePorinter  ci->getMixerWritePorinter
#  define setMixerReadPorinter   ci->setMixerReadPorinter
#  define printf                ci->printf
#  define MP2Encode_GetBufInfo      codec_info

#else
#  define PalSleep              
#  define PalGetClock         
#  define PalGetDuration    
#  define PalGetSysClock    
#  define PalWFileOpen       
#  define PalFileWrite          
#  define PalFileClose          
#  define f_enterFS             
#  define PalHeapAlloc        
#  define PalHeapFree         
#  define taskYIELD             
#  define deactiveDAC         
#  define deactiveADC         
#  define pauseDAC             
#  define pauseADC             
#  define initDAC            
#  define initADC            
#  define initCODEC        
#  define or32_invalidate_cache 

//#  define dc_invalidate         
#  define printf               
#  define AACEncode_GetBufInfo codec_info
#endif


#if  defined(AUDIO_PLUGIN_MESSAGE_QUEUE)
extern char risc1_start_addr;
#define CODEC_STREAM_START_ADDRESS &risc1_start_addr+0x14

typedef struct AUDIO_CODEC_STREAM_TAG
{
    unsigned char *codecStreamBuf;    // codec defined stream
    int codecStreamLength;                // codec defined stream length
    unsigned char *codecStreamBuf1; // codec defined stream1
    int codecStreamLength1;             // codec defined stream length1
    unsigned char *codecStreamBuf2; // codec defined stream2
    int codecStreamLength2;             // codec defined stream length2
    short* codecAudioPluginBuf;
    int codecAudioPluginLength;
} AUDIO_CODEC_STREAM;
//
// cpu id / audio plugin cmd
//    2 bits                14 bits
#define SMTK_MAIN_PROCESSOR_ID   1
#define SMTK_AUDIO_PROCESSOR_ID 2

// define file info
#define SMTK_AUDIO_PLUGIN_CMD_FILE_NAME_LENGTH 4
#define SMTK_AUDIO_PLUGIN_CMD_FILE_WRITE_LENGTH 4
#define SMTK_AUDIO_PLUGIN_CMD_FILE_OPEN_TYPE 1

// for Audio_Plugin_MSG_TYPE_CMD
enum
{
    SMTK_AUDIO_PLUGIN_CMD_ID_FILE_OPEN = 0x1,
    SMTK_AUDIO_PLUGIN_CMD_ID_FILE_WRITE,    // 2
    SMTK_AUDIO_PLUGIN_CMD_ID_FILE_READ,   // 3
    SMTK_AUDIO_PLUGIN_CMD_ID_FILE_CLOSE,  // 4
    /* IIS */
    SMTK_AUDIO_PLUGIN_CMD_ID_I2S_INIT_DAC,  // 5
    SMTK_AUDIO_PLUGIN_CMD_ID_I2S_INIT_ADC, // 6
    SMTK_AUDIO_PLUGIN_CMD_ID_I2S_INIT_CODEC, // 7
    SMTK_AUDIO_PLUGIN_CMD_ID_I2S_PAUSE_DAC, // 8
    SMTK_AUDIO_PLUGIN_CMD_ID_I2S_PAUSE_ADC, // 9
    SMTK_AUDIO_PLUGIN_CMD_ID_I2S_DEACTIVE_DAC, // 10
    SMTK_AUDIO_PLUGIN_CMD_ID_I2S_DEACTIVE_ADC, // 11
    /* kernel*/
    SMTK_AUDIO_PLUGIN_CMD_ID_SLEEP, // 12
    SMTK_AUDIO_PLUGIN_CMD_ID_GET_CLOCK,   // 13
    SMTK_AUDIO_PLUGIN_CMD_ID_GET_DURATION,   // 14
    SMTK_AUDIO_PLUGIN_CMD_ID_PRINTF,  // 15
    SMTK_AUDIO_PLUGIN_CMD_ID_WMA_INFO,   // 16
    SMTK_AUDIO_PLUGIN_CMD_ID_SPECTRUM,   // 17    
};


#endif

#endif // __PLUGIN_H__
