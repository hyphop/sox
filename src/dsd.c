/*
   native DSD filter - just for test

   ## hyphop ##
*/

#include "sox_i.h"

typedef struct dsd {
  sox_sample_t *buf;
} dsd_t;

static int dsd_start(sox_effect_t *eff)
{
  dsd_t *p = eff->priv;

  if (eff->in_signal.precision != 1) {
    lsx_fail("1-bit input required");
    return SOX_EOF;
  }

  if (eff->in_signal.rate != 32 * eff->out_signal.rate) {
    lsx_fail("incorrect output rate, should be %.1fk",
             eff->in_signal.rate / 32 / 1000);
    return SOX_EOF;
  }

  eff->out_signal.precision = 32;

  p->buf = lsx_calloc(eff->out_signal.channels, sizeof(*p->buf));
  return SOX_SUCCESS;

}

/*

 ./sox -n -r 44100 -c2 test2.flac synth 2 sin 440 sin 440
 ./sox test2.flac test2.dsf rate -v 2822400 sdm
 AUDIODEV=hw:4 ./sox test.dsf -t alsa -e dsd -r 88200 dsd

*/


static int dsd_flow(sox_effect_t *eff, const sox_sample_t *ibuf,
                    sox_sample_t *obuf, size_t *isamp, size_t *osamp)
{
//dsd_t *p = eff->priv;
  unsigned channels = eff->in_signal.channels;
  const sox_sample_t *in = ibuf;
  sox_sample_t *out = obuf;
  size_t ilen = *isamp;
//  size_t olen = *osamp;
  int i;

  //
  // temporary solution just for testing native dsd output
  //
  // 32 samples 1 bit => 32 bit
  // 1 or 2 channels only

//  if ( ! ilen )
//    printf("BEGIN %lu\n", olen);

  while (ilen >= 64) {
  // uint32_t s;
  // s = 0x69696969;
    uint32_t l=0;
    uint32_t r=0;
    for (i = 31; i >=0 ; i--) {
        if ( *in > 0 ) l |= 1 << i;
        in+=channels;
    }

    in-=32*channels-1;

    for (i = 31; i >=0 ; i--) {
        if ( *in > 0 ) r |= 1 << i;
        in+=channels;
    }
    in--;

//    printf("-%08x\n", l);
//    printf("+%08x\n", r);
//    printf("-%08x=%08x\n", l , __builtin_bswap32(l));

    *out++ = __builtin_bswap32(l); // L
    *out++ = __builtin_bswap32(r); // R
//  *out++ = r; // R
    ilen-=32;
    ilen-=32;
  }

   if (ilen > 0 ) {
     printf("OOPS %lu\n", ilen);
   }

   // printf("LOOP %u %u %u %u\n", in , ibuf, out, obuf);

  *isamp = in  - ibuf;
  *osamp = out - obuf;

  return SOX_SUCCESS;

}

static int dsd_drain(sox_effect_t *eff, sox_sample_t *obuf, size_t *osamp)
{
  dsd_t *p = eff->priv;
  printf("DRAIN\n");
  p=p;
  obuf=obuf;
//*obuf++ = 0x69696969;
  *osamp = 0;
//
  return SOX_SUCCESS;
}

static int dsd_stop(sox_effect_t *eff)
{
  dsd_t *p = eff->priv;
  free(p->buf);
  return SOX_SUCCESS;
}

const sox_effect_handler_t *lsx_dsd_effect_fn(void)
{
  static sox_effect_handler_t handler = {
    "dsd", NULL,
    SOX_EFF_MCHAN | SOX_EFF_PREC | SOX_EFF_RATE,
    NULL, dsd_start, dsd_flow, dsd_drain, dsd_stop, NULL,
    sizeof(dsd_t),
  };
  return &handler;
}
