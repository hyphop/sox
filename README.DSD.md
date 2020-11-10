DSD sox usage
-------------

tags: DSD DSF PCM DOP ALSA Native-DSD

## Alsa PCM output

Convert from dsd to pcm before output to alsa pcm

```
AUDIODEV=hw:1 ./sox test.dsf -t alsa -r 44100

access: RW_INTERLEAVED
format: S16_LE
subformat: STD
channels: 2
rate: 44100 (44100/1)
period_size: 2048
buffer_size: 16384
```

## Alsas DOP output

```
AUDIODEV=hw:1 ./sox test.dsf -t alsa -b32 -r 176.4k dop

access: RW_INTERLEAVED
format: S32_LE
subformat: STD
channels: 2
rate: 176400 (176400/1)
period_size: 1024
buffer_size: 8192

```

NOTE: DOP need 2 x Rates for same dsd source
and its problem for DSD256 and DSD512

## Alsa native DSD output

NOTE: its experemental implementation just for testing

alsa DSD_U32_BE format is 32 bit need setup 88200
logic example: 2822400 = 44100 * 64 = 88200 * 32

```
AUDIODEV=hw:1 ./sox test.dsf -t alsa -e dsd -r 88200 dsd

access: RW_INTERLEAVED
format: DSD_U32_BE
subformat: STD
channels: 2
rate: 88200 (88200/1)
period_size: 1024
buffer_size: 8192
```

## Alsa native DSD output generator

Output stereo sin 440

```
AUDIODEV=hw:1 ./sox -c2 -n -t alsa -e dsd -r 88200 synth 2 sin 440 rate -v 2822400 sdm dsd repeat 100

```

Output stereo sin 440 left and sin 120 right

```
AUDIODEV=hw:1 ./sox -c2 -n -t alsa -e dsd -r 88200 synth 2 sin 440 sin 120 rate -v 2822400 sdm dsd repeat 100
```

## PCM analog example

```
AUDIODEV=hw:1 ./sox -c2 -n -t alsa -r 44100 synth 2 sin 440 sin 120 repeat 100
```

## More info

If u have some questions ask me ## hyphop ##

