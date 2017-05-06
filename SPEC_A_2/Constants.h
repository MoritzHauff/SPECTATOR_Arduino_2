// Constants.h - Moritz Hauff - 18.02.2017
/*
** Diese Datei beinhaltet 
** - alle Kommunikationscodes
**
**
** Moritz Hauff, 18.02.2017
**/

#ifndef _CONSTANTS_h
#define _CCONSTANTS_h

#define C_TELEOPSTART 0x62  // Bekundet den Start einer manuellen Motorensteuerung. ('b')
#define C_TELEOPSTOP 0x65   // Bekundet das Ende einer manuellen Motorensteuerung. ('e')

#define C_MSGSTART 0x03
#define C_MSGEND 0x04
#define C_MSGSEPARATOR 0x05

#define C_LDR 0x20

#define C_MLXLinks 0x21
#define C_MLXVorne 0x22
#define C_MLXRechts 0x23

#define C_MPUYaw 0x24
#define C_MPUPitch 0x25
#define C_MPURoll 0x26

#define C_SwitchLinks 0x27
#define C_SwitchRechts 0x28

#define C_SharpLV 0x31
#define C_SharpLH 0x32
#define C_SharpRV 0x33
#define C_SharpRH 0x34

#define C_UltraschallL 0x35
#define C_UltraschallV 0x36
#define C_UltraschallR 0x37
#define C_UltraschallH 0x38

#define C_LaserV 0x39

#define C_EncoderLC 0x40
#define C_EncoderLT 0x41
#define C_EncoderRC 0x40
#define C_EncoderRT 0x41

#define C_WandLinks 0x50
#define C_WandVorne 0x51
#define C_WandRechts 0x52
#define C_WandHinten 0x53
#define C_OpferLinks 0x54
#define C_OpferVorne 0x55
#define C_OpferRechts 0x56
#define C_AktuelleRichtung 0x57
#define C_GeradeSchwarzesFeldBefahren 0x58
#define C_GeradeRampeBefahren 0x59
#define C_GeradeSonstWieNichtVorangekommen 0x60 // todo add this in the deserializer!

#define C_WeitereEreignisse 0x61

#endif
