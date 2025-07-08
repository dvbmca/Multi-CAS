# Multi-CAS
A STB middleware supports several CAS(conditional access system), such as Irdeto, NDS, Conax7, TVCAS4, Verimatrix, CTI, Griffin, CryptoGuard, DVCrypt, Crypton, Sumavision, Sochuang, ABV, Topreal, TongFang, VeriGuard and so on. Also it supports Dolby(AC3) software decoding and CSA(Common Scrambling Algorithm) software  decryption & encryption.

## How to use

### 1. Gx STB

**1st step:** copy directory "Multi-CAS" to SDK directory : solution/app/

**2nd step:** remove directory "Multi-CAS/adaptation_layer/src_mstar"

**3rd step:** add these statements in "solution/app/Makefile" as follow:

```makefile
CFLAGS  += -I./Multi-CAS/inc
CFLAGS  += -I./Multi-CAS/adaptation_layer/inc
LDFLAGS += -L./Multi-CAS/lib -lmca_core_0_gx
```

Here is "solution/app/Makefile" after you edit:

```makefile
LIB = libapp.a
BIN = out.elf

CFLAGS += -I./include
#CFLAGS += -I./module/cim

CFLAGS += -I./log/include

CFLAGS += -I./networkmanager/include
CFLAGS += -I./player/include
CFLAGS += -I./netapps

CFLAGS  += -I$(GXLIB_PATH)/include/bus
CFLAGS  += -I$(GXLIB_PATH)/include/bus/gui_core
CFLAGS  += -I$(GXLIB_PATH)/include/bus/service
CFLAGS  += -I$(GXLIB_PATH)/include/bus/module/player
CFLAGS  += -I$(GXLIB_PATH)/include/bus/module/ca
CFLAGS  += -I$(GXLIB_PATH)/include/app/ca
CFLAGS  += -I$(GXSRC_PATH)/include

#######################Multi-CAS#######################
CFLAGS  += -I./Multi-CAS/inc
CFLAGS  += -I./Multi-CAS/adaptation_layer/inc
LDFLAGS += -L./Multi-CAS/lib -lmca_core_0_gx
#######################################################

default: del all

del:
	rm -rf out.elf

##############################################################################
-include  $(GXSRC_PATH)/scripts/inc.Makefile.conf.mak
##############################################################################
```

**4th step:** call MCA_Init(...) in solution/main.c

Include header file:

```c
#include "mca_basic_api.h"
```

Call MCA_Init(...) according to the CAS type you want when system startups:

```c
int GxCore_Startup(int argc, char **argv) {
    ......
    app_player_module_register();
#if NETAPPS_SUPPORT
    gx_netapps_event_init();
#endif
#if SW_FILTER_SUPPORT
    gx_search_softfilter_register(GX_PMT_SW_FILTER);
#endif
#if CA_SUPPORT
    app_extend_register();
#endif
    
//#######################Multi-CAS#######################
    if (1)
    {
        MCA_Param_t stParam;

        mca_memset(&stParam, 0x00, sizeof(stParam));
        stParam.m_enLogLevel= MCA_TRACE_BASIC;
        stParam.m_enCASType = MCA_CAS_CONAX;
        stParam.m_enPairType= MCA_PAIR_OFF;
    
        MCA_Init(&stParam);
        mca_app_common_init();
    }
//#######################################################

    GDI_RegisterJPEG_SOFT();
    GDI_RegisterJPEG_SOFT_Special();
    GDI_RegisterGIF();
    GDI_RegisterPNG();
    ......
}
```

**5th step:** send CAT to Multi-CAS

Call MCA_PostCAT(...) in solution/app/module/psi-si/app_cat.c

```c
static private_parse_status _cat_private_function(uint8_t *p_section_data, uint32_t len)
{
    ......
//#######################Multi-CAS#######################
    MCA_PostCAT(0, p_section_data,len);
//#######################################################
	
    return PRIVATE_SUBTABLE_OK;
}
```

**6th step:** send PMT to Multi-CAS

Call MCA_PostPMT(...) in solution/app/module/psi-si/app_pmt.c

```c

static private_parse_status pmt_private_function(uint8_t *p_section_data, uint32_t len)
{
    ......
//#######################Multi-CAS#######################
    MCA_PostPMT(0, p_section_data,len);
//#######################################################
	
    return PRIVATE_SUBTABLE_OK;
}
```

