#if defined(HAVE_CONFIG_H)
#include "mediastreamer-config.h"
#endif

#include "mediastreamer2/msfilter.h"

typedef struct MSstate{
    MSFilter *msF;
    MSBufferizer *dualBuf;
    int framesize;
    int samplerate; 
    bool_t start;   
    ms_thread_t split_thread;	   
    ms_mutex_t mutex;    
}MSstate;


static void channel_split_init(MSFilter *f){
    MSstate *s=(MSstate *)ms_new(MSstate,1);
    f->data=s;
    s->dualBuf =ms_bufferizer_new();
    s->framesize = 256*(CFG_AUDIO_SAMPLING_RATE/8000);
    s->samplerate = CFG_AUDIO_SAMPLING_RATE;
    ms_mutex_init(&s->mutex,NULL);
    s->start = FALSE;
    printf("channel_split_init\n");   
}
/*
output[0]:mic data : speech & echo
output[1]:Line IN  : far-end data
output[2]:to video record;
*/
static void *channel_split_thread(void *arg) {
    MSstate *s=(MSstate*)arg;
    int nbytes    = s->framesize*2;
    int shift     = s->framesize*1000/s->samplerate;

    while(s->start){
        int err;
        mblk_t *im;
        im=allocb(nbytes*2,0);
        
        ms_mutex_lock(&s->mutex);
        err = ms_bufferizer_read(s->dualBuf,im->b_wptr,nbytes*2);
        ms_mutex_unlock(&s->mutex);
        if(err){      
            mblk_t *oL,*oR;
            im->b_wptr+=nbytes*2;
            oL=allocb(nbytes,0);
            oR=allocb(nbytes,0);
            for(;im->b_rptr<im->b_wptr;oL->b_wptr+=2,oR->b_wptr+=2,im->b_rptr+=4){
                *((int16_t*)(oL->b_wptr  ))=(int)*(int16_t*)(im->b_rptr  );
                *((int16_t*)(oR->b_wptr  ))=(int)*(int16_t*)(im->b_rptr+2);             
            }
            ms_queue_put(s->msF->outputs[0],oL);
            ms_queue_put(s->msF->outputs[1],oR);      
        }else{
            usleep(20000);
        }
        
        if(im) freemsg(im);
        if(s->start==FALSE) break;
        
    }

    return NULL;
}

static void channel_split_init_thread(MSstate *e){
    //ms_mutex_init(&e->hw_mutex,NULL);
    if (e->start == FALSE){
        pthread_attr_t attr;
        struct sched_param param;
        e->start=TRUE;
        pthread_attr_init(&attr);
        pthread_attr_setstacksize(&attr, 25*1024);       
        param.sched_priority = sched_get_priority_min(0);
        pthread_attr_setschedparam(&attr, &param);
        ms_thread_create(&e->split_thread, &attr, channel_split_thread, e);
    }
}

static void channel_split_stop_hwthread(MSstate *e){
    e->start=FALSE;
    //ms_mutex_destroy(e->hw_mutex);
    ms_thread_join(e->split_thread,NULL);      
}

static void channel_split_preprocess(MSFilter *f){
    MSstate *s=(MSstate*)f->data;
    s->msF = f;
    channel_split_init_thread(s);
}


static void channel_split_process(MSFilter *f){
    MSstate *s=(MSstate*)f->data;
    int nbytes=s->framesize*2;
    ms_mutex_lock(&s->mutex);
    ms_bufferizer_put_from_queue(s->dualBuf,f->inputs[0]);
    ms_mutex_unlock(&s->mutex);
}

static void channel_split_postprocess(MSFilter *f){
    MSstate *s=(MSstate*)f->data;
    channel_split_stop_hwthread(s);
    ms_bufferizer_flush (s->dualBuf);
}

static void channel_split_uninit(MSFilter *f){
    MSstate *s=(MSstate*)f->data;  
    ms_mutex_destroy(&s->mutex);
    ms_bufferizer_destroy(s->dualBuf);

    ms_free(s);
}
#ifdef _MSC_VER

MSFilterDesc ms_channel_split_desc={
	MS_CHANNEL_SPLIT_ID,
	"MSChannelSplit",
	N_("A filter that split channel duplicate or separate."),
	MS_FILTER_OTHER,
	NULL,
	1,
	2,
    channel_split_init,
	channel_split_preprocess,
	channel_split_process,
    channel_split_postprocess,
    channel_split_uninit,
    NULL
};

#else

MSFilterDesc ms_channel_split_desc={
	.id=MS_CHANNEL_SPLIT_ID,
	.name="MSChannelSplit",
	.text=N_("A filter that split channel duplicate or separate."),
	.category=MS_FILTER_OTHER,
	.ninputs=1,
	.noutputs=2,
    .init=channel_split_init,
    .preprocess=channel_split_preprocess,
    .process=channel_split_process,
    .postprocess=channel_split_postprocess,
    .uninit=channel_split_uninit
    //.methods=channel_split_methods
};

#endif

MS_FILTER_DESC_EXPORT(ms_channel_split_desc)