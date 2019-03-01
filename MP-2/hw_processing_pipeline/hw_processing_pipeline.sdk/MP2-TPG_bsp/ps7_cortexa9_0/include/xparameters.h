#ifndef XPARAMETERS_H   /* prevent circular inclusions */
#define XPARAMETERS_H   /* by using protection macros */

/* Definition for CPU ID */
#define XPAR_CPU_ID 0U

/* Definitions for peripheral PS7_CORTEXA9_0 */
#define XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ 666666687


/******************************************************************/

/* Canonical definitions for peripheral PS7_CORTEXA9_0 */
#define XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ 666666687


/******************************************************************/

#include "xparameters_ps.h"

#define STDIN_BASEADDRESS 0xE0001000
#define STDOUT_BASEADDRESS 0xE0001000

/******************************************************************/

/* Platform specific definitions */
#define PLATFORM_ZYNQ
 
/* Definitions for sleep timer configuration */
#define XSLEEP_TIMER_IS_DEFAULT_TIMER
 
 
/******************************************************************/
/* Definitions for driver AXIVDMA */
#define XPAR_XAXIVDMA_NUM_INSTANCES 1U

/* Definitions for peripheral AXI_VDMA_0 */
#define XPAR_AXI_VDMA_0_DEVICE_ID 0U
#define XPAR_AXI_VDMA_0_BASEADDR 0x43000000U
#define XPAR_AXI_VDMA_0_HIGHADDR 0x4300FFFFU
#define XPAR_AXI_VDMA_0_NUM_FSTORES 3U
#define XPAR_AXI_VDMA_0_INCLUDE_MM2S 1U
#define XPAR_AXI_VDMA_0_INCLUDE_MM2S_DRE 0U
#define XPAR_AXI_VDMA_0_M_AXI_MM2S_DATA_WIDTH 64U
#define XPAR_AXI_VDMA_0_INCLUDE_S2MM 1U
#define XPAR_AXI_VDMA_0_INCLUDE_S2MM_DRE 0U
#define XPAR_AXI_VDMA_0_M_AXI_S2MM_DATA_WIDTH 64U
#define XPAR_AXI_VDMA_0_AXI_MM2S_ACLK_FREQ_HZ 0U
#define XPAR_AXI_VDMA_0_AXI_S2MM_ACLK_FREQ_HZ 0U
#define XPAR_AXI_VDMA_0_MM2S_GENLOCK_MODE 3U
#define XPAR_AXI_VDMA_0_MM2S_GENLOCK_NUM_MASTERS 1U
#define XPAR_AXI_VDMA_0_S2MM_GENLOCK_MODE 2U
#define XPAR_AXI_VDMA_0_S2MM_GENLOCK_NUM_MASTERS 1U
#define XPAR_AXI_VDMA_0_INCLUDE_SG 0U
#define XPAR_AXI_VDMA_0_ENABLE_VIDPRMTR_READS 1U
#define XPAR_AXI_VDMA_0_USE_FSYNC 1U
#define XPAR_AXI_VDMA_0_FLUSH_ON_FSYNC 1U
#define XPAR_AXI_VDMA_0_MM2S_LINEBUFFER_DEPTH 4096U
#define XPAR_AXI_VDMA_0_S2MM_LINEBUFFER_DEPTH 4096U
#define XPAR_AXI_VDMA_0_INCLUDE_INTERNAL_GENLOCK 1U
#define XPAR_AXI_VDMA_0_S2MM_SOF_ENABLE 1U
#define XPAR_AXI_VDMA_0_M_AXIS_MM2S_TDATA_WIDTH 16U
#define XPAR_AXI_VDMA_0_S_AXIS_S2MM_TDATA_WIDTH 16U
#define XPAR_AXI_VDMA_0_ENABLE_DEBUG_INFO_1 0U
#define XPAR_AXI_VDMA_0_ENABLE_DEBUG_INFO_5 0U
#define XPAR_AXI_VDMA_0_ENABLE_DEBUG_INFO_6 1U
#define XPAR_AXI_VDMA_0_ENABLE_DEBUG_INFO_7 1U
#define XPAR_AXI_VDMA_0_ENABLE_DEBUG_INFO_9 0U
#define XPAR_AXI_VDMA_0_ENABLE_DEBUG_INFO_13 0U
#define XPAR_AXI_VDMA_0_ENABLE_DEBUG_INFO_14 1U
#define XPAR_AXI_VDMA_0_ENABLE_DEBUG_INFO_15 1U
#define XPAR_AXI_VDMA_0_ENABLE_DEBUG_ALL 0U
#define XPAR_AXI_VDMA_0_ADDR_WIDTH 32U
#define XPAR_AXI_VDMA_0_ENABLE_VERT_FLIP 0U


/******************************************************************/

/* Canonical definitions for peripheral AXI_VDMA_0 */
#define XPAR_AXIVDMA_0_DEVICE_ID XPAR_AXI_VDMA_0_DEVICE_ID
#define XPAR_AXIVDMA_0_BASEADDR 0x43000000U
#define XPAR_AXIVDMA_0_HIGHADDR 0x4300FFFFU
#define XPAR_AXIVDMA_0_NUM_FSTORES 3U
#define XPAR_AXIVDMA_0_INCLUDE_MM2S 1U
#define XPAR_AXIVDMA_0_INCLUDE_MM2S_DRE 0U
#define XPAR_AXIVDMA_0_M_AXI_MM2S_DATA_WIDTH 64U
#define XPAR_AXIVDMA_0_INCLUDE_S2MM 1U
#define XPAR_AXIVDMA_0_INCLUDE_S2MM_DRE 0U
#define XPAR_AXIVDMA_0_M_AXI_S2MM_DATA_WIDTH 64U
#define XPAR_AXIVDMA_0_AXI_MM2S_ACLK_FREQ_HZ 0U
#define XPAR_AXIVDMA_0_AXI_S2MM_ACLK_FREQ_HZ 0U
#define XPAR_AXIVDMA_0_MM2S_GENLOCK_MODE 3U
#define XPAR_AXIVDMA_0_MM2S_GENLOCK_NUM_MASTERS 1U
#define XPAR_AXIVDMA_0_S2MM_GENLOCK_MODE 2U
#define XPAR_AXIVDMA_0_S2MM_GENLOCK_NUM_MASTERS 1U
#define XPAR_AXIVDMA_0_INCLUDE_SG 0U
#define XPAR_AXIVDMA_0_ENABLE_VIDPRMTR_READS 1U
#define XPAR_AXIVDMA_0_USE_FSYNC 1U
#define XPAR_AXIVDMA_0_FLUSH_ON_FSYNC 1U
#define XPAR_AXIVDMA_0_MM2S_LINEBUFFER_DEPTH 4096U
#define XPAR_AXIVDMA_0_S2MM_LINEBUFFER_DEPTH 4096U
#define XPAR_AXIVDMA_0_INCLUDE_INTERNAL_GENLOCK 1U
#define XPAR_AXIVDMA_0_S2MM_SOF_ENABLE 1U
#define XPAR_AXIVDMA_0_M_AXIS_MM2S_TDATA_WIDTH 16U
#define XPAR_AXIVDMA_0_S_AXIS_S2MM_TDATA_WIDTH 16U
#define XPAR_AXIVDMA_0_ENABLE_DEBUG_INFO_1 0U
#define XPAR_AXIVDMA_0_ENABLE_DEBUG_INFO_5 0U
#define XPAR_AXIVDMA_0_ENABLE_DEBUG_INFO_6 1U
#define XPAR_AXIVDMA_0_ENABLE_DEBUG_INFO_7 1U
#define XPAR_AXIVDMA_0_ENABLE_DEBUG_INFO_9 0U
#define XPAR_AXIVDMA_0_ENABLE_DEBUG_INFO_13 0U
#define XPAR_AXIVDMA_0_ENABLE_DEBUG_INFO_14 1U
#define XPAR_AXIVDMA_0_ENABLE_DEBUG_INFO_15 1U
#define XPAR_AXIVDMA_0_ENABLE_DEBUG_ALL 0U
#define XPAR_AXIVDMA_0_c_addr_width 32U
#define XPAR_AXIVDMA_0_c_enable_vert_flip 0U


/******************************************************************/


/* Definitions for peripheral PS7_DDR_0 */
#define XPAR_PS7_DDR_0_S_AXI_BASEADDR 0x00100000
#define XPAR_PS7_DDR_0_S_AXI_HIGHADDR 0x1FFFFFFF


/******************************************************************/

/* Definitions for driver DEVCFG */
#define XPAR_XDCFG_NUM_INSTANCES 1U

/* Definitions for peripheral PS7_DEV_CFG_0 */
#define XPAR_PS7_DEV_CFG_0_DEVICE_ID 0U
#define XPAR_PS7_DEV_CFG_0_BASEADDR 0xF8007000U
#define XPAR_PS7_DEV_CFG_0_HIGHADDR 0xF80070FFU


/******************************************************************/

/* Canonical definitions for peripheral PS7_DEV_CFG_0 */
#define XPAR_XDCFG_0_DEVICE_ID XPAR_PS7_DEV_CFG_0_DEVICE_ID
#define XPAR_XDCFG_0_BASEADDR 0xF8007000U
#define XPAR_XDCFG_0_HIGHADDR 0xF80070FFU


/******************************************************************/

/* Definitions for driver DMAPS */
#define XPAR_XDMAPS_NUM_INSTANCES 2

/* Definitions for peripheral PS7_DMA_NS */
#define XPAR_PS7_DMA_NS_DEVICE_ID 0
#define XPAR_PS7_DMA_NS_BASEADDR 0xF8004000
#define XPAR_PS7_DMA_NS_HIGHADDR 0xF8004FFF


/* Definitions for peripheral PS7_DMA_S */
#define XPAR_PS7_DMA_S_DEVICE_ID 1
#define XPAR_PS7_DMA_S_BASEADDR 0xF8003000
#define XPAR_PS7_DMA_S_HIGHADDR 0xF8003FFF


/******************************************************************/

/* Canonical definitions for peripheral PS7_DMA_NS */
#define XPAR_XDMAPS_0_DEVICE_ID XPAR_PS7_DMA_NS_DEVICE_ID
#define XPAR_XDMAPS_0_BASEADDR 0xF8004000
#define XPAR_XDMAPS_0_HIGHADDR 0xF8004FFF

/* Canonical definitions for peripheral PS7_DMA_S */
#define XPAR_XDMAPS_1_DEVICE_ID XPAR_PS7_DMA_S_DEVICE_ID
#define XPAR_XDMAPS_1_BASEADDR 0xF8003000
#define XPAR_XDMAPS_1_HIGHADDR 0xF8003FFF


/******************************************************************/

/* Definitions for driver EMACPS */
#define XPAR_XEMACPS_NUM_INSTANCES 1

/* Definitions for peripheral PS7_ETHERNET_0 */
#define XPAR_PS7_ETHERNET_0_DEVICE_ID 0
#define XPAR_PS7_ETHERNET_0_BASEADDR 0xE000B000
#define XPAR_PS7_ETHERNET_0_HIGHADDR 0xE000BFFF
#define XPAR_PS7_ETHERNET_0_ENET_CLK_FREQ_HZ 125000000
#define XPAR_PS7_ETHERNET_0_ENET_SLCR_1000MBPS_DIV0 8
#define XPAR_PS7_ETHERNET_0_ENET_SLCR_1000MBPS_DIV1 1
#define XPAR_PS7_ETHERNET_0_ENET_SLCR_100MBPS_DIV0 8
#define XPAR_PS7_ETHERNET_0_ENET_SLCR_100MBPS_DIV1 5
#define XPAR_PS7_ETHERNET_0_ENET_SLCR_10MBPS_DIV0 8
#define XPAR_PS7_ETHERNET_0_ENET_SLCR_10MBPS_DIV1 50
#define XPAR_PS7_ETHERNET_0_ENET_TSU_CLK_FREQ_HZ 0


/******************************************************************/

#define XPAR_PS7_ETHERNET_0_IS_CACHE_COHERENT 0
#define XPAR_XEMACPS_0_IS_CACHE_COHERENT 0
/* Canonical definitions for peripheral PS7_ETHERNET_0 */
#define XPAR_XEMACPS_0_DEVICE_ID XPAR_PS7_ETHERNET_0_DEVICE_ID
#define XPAR_XEMACPS_0_BASEADDR 0xE000B000
#define XPAR_XEMACPS_0_HIGHADDR 0xE000BFFF
#define XPAR_XEMACPS_0_ENET_CLK_FREQ_HZ 125000000
#define XPAR_XEMACPS_0_ENET_SLCR_1000Mbps_DIV0 8
#define XPAR_XEMACPS_0_ENET_SLCR_1000Mbps_DIV1 1
#define XPAR_XEMACPS_0_ENET_SLCR_100Mbps_DIV0 8
#define XPAR_XEMACPS_0_ENET_SLCR_100Mbps_DIV1 5
#define XPAR_XEMACPS_0_ENET_SLCR_10Mbps_DIV0 8
#define XPAR_XEMACPS_0_ENET_SLCR_10Mbps_DIV1 50
#define XPAR_XEMACPS_0_ENET_TSU_CLK_FREQ_HZ 0


/******************************************************************/


/* Definitions for peripheral PS7_AFI_0 */
#define XPAR_PS7_AFI_0_S_AXI_BASEADDR 0xF8008000
#define XPAR_PS7_AFI_0_S_AXI_HIGHADDR 0xF8008FFF


/* Definitions for peripheral PS7_AFI_1 */
#define XPAR_PS7_AFI_1_S_AXI_BASEADDR 0xF8009000
#define XPAR_PS7_AFI_1_S_AXI_HIGHADDR 0xF8009FFF


/* Definitions for peripheral PS7_AFI_2 */
#define XPAR_PS7_AFI_2_S_AXI_BASEADDR 0xF800A000
#define XPAR_PS7_AFI_2_S_AXI_HIGHADDR 0xF800AFFF


/* Definitions for peripheral PS7_AFI_3 */
#define XPAR_PS7_AFI_3_S_AXI_BASEADDR 0xF800B000
#define XPAR_PS7_AFI_3_S_AXI_HIGHADDR 0xF800BFFF


/* Definitions for peripheral PS7_DDRC_0 */
#define XPAR_PS7_DDRC_0_S_AXI_BASEADDR 0xF8006000
#define XPAR_PS7_DDRC_0_S_AXI_HIGHADDR 0xF8006FFF


/* Definitions for peripheral PS7_GLOBALTIMER_0 */
#define XPAR_PS7_GLOBALTIMER_0_S_AXI_BASEADDR 0xF8F00200
#define XPAR_PS7_GLOBALTIMER_0_S_AXI_HIGHADDR 0xF8F002FF


/* Definitions for peripheral PS7_GPV_0 */
#define XPAR_PS7_GPV_0_S_AXI_BASEADDR 0xF8900000
#define XPAR_PS7_GPV_0_S_AXI_HIGHADDR 0xF89FFFFF


/* Definitions for peripheral PS7_INTC_DIST_0 */
#define XPAR_PS7_INTC_DIST_0_S_AXI_BASEADDR 0xF8F01000
#define XPAR_PS7_INTC_DIST_0_S_AXI_HIGHADDR 0xF8F01FFF


/* Definitions for peripheral PS7_IOP_BUS_CONFIG_0 */
#define XPAR_PS7_IOP_BUS_CONFIG_0_S_AXI_BASEADDR 0xE0200000
#define XPAR_PS7_IOP_BUS_CONFIG_0_S_AXI_HIGHADDR 0xE0200FFF


/* Definitions for peripheral PS7_L2CACHEC_0 */
#define XPAR_PS7_L2CACHEC_0_S_AXI_BASEADDR 0xF8F02000
#define XPAR_PS7_L2CACHEC_0_S_AXI_HIGHADDR 0xF8F02FFF


/* Definitions for peripheral PS7_OCMC_0 */
#define XPAR_PS7_OCMC_0_S_AXI_BASEADDR 0xF800C000
#define XPAR_PS7_OCMC_0_S_AXI_HIGHADDR 0xF800CFFF


/* Definitions for peripheral PS7_PL310_0 */
#define XPAR_PS7_PL310_0_S_AXI_BASEADDR 0xF8F02000
#define XPAR_PS7_PL310_0_S_AXI_HIGHADDR 0xF8F02FFF


/* Definitions for peripheral PS7_PMU_0 */
#define XPAR_PS7_PMU_0_S_AXI_BASEADDR 0xF8891000
#define XPAR_PS7_PMU_0_S_AXI_HIGHADDR 0xF8891FFF
#define XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR 0xF8893000
#define XPAR_PS7_PMU_0_PMU1_S_AXI_HIGHADDR 0xF8893FFF


/* Definitions for peripheral PS7_QSPI_LINEAR_0 */
#define XPAR_PS7_QSPI_LINEAR_0_S_AXI_BASEADDR 0xFC000000
#define XPAR_PS7_QSPI_LINEAR_0_S_AXI_HIGHADDR 0xFCFFFFFF


/* Definitions for peripheral PS7_RAM_0 */
#define XPAR_PS7_RAM_0_S_AXI_BASEADDR 0x00000000
#define XPAR_PS7_RAM_0_S_AXI_HIGHADDR 0x0003FFFF


/* Definitions for peripheral PS7_RAM_1 */
#define XPAR_PS7_RAM_1_S_AXI_BASEADDR 0xFFFC0000
#define XPAR_PS7_RAM_1_S_AXI_HIGHADDR 0xFFFFFFFF


/* Definitions for peripheral PS7_SCUC_0 */
#define XPAR_PS7_SCUC_0_S_AXI_BASEADDR 0xF8F00000
#define XPAR_PS7_SCUC_0_S_AXI_HIGHADDR 0xF8F000FC


/* Definitions for peripheral PS7_SLCR_0 */
#define XPAR_PS7_SLCR_0_S_AXI_BASEADDR 0xF8000000
#define XPAR_PS7_SLCR_0_S_AXI_HIGHADDR 0xF8000FFF


/* Definitions for peripheral ONSEMI_VITA_CAM_0 */
#define XPAR_ONSEMI_VITA_CAM_0_S00_AXI_BASEADDR 0x43C30000
#define XPAR_ONSEMI_VITA_CAM_0_S00_AXI_HIGHADDR 0x43C3FFFF


/* Definitions for peripheral ONSEMI_VITA_SPI_0 */
#define XPAR_ONSEMI_VITA_SPI_0_S00_AXI_BASEADDR 0x43C20000
#define XPAR_ONSEMI_VITA_SPI_0_S00_AXI_HIGHADDR 0x43C2FFFF


/******************************************************************/

/* Definitions for driver GPIO */
#define XPAR_XGPIO_NUM_INSTANCES 2

/* Definitions for peripheral BTNS_5BITS */
#define XPAR_BTNS_5BITS_BASEADDR 0x41200000
#define XPAR_BTNS_5BITS_HIGHADDR 0x4120FFFF
#define XPAR_BTNS_5BITS_DEVICE_ID 0
#define XPAR_BTNS_5BITS_INTERRUPT_PRESENT 0
#define XPAR_BTNS_5BITS_IS_DUAL 0


/* Definitions for peripheral SWS_8BITS */
#define XPAR_SWS_8BITS_BASEADDR 0x41210000
#define XPAR_SWS_8BITS_HIGHADDR 0x4121FFFF
#define XPAR_SWS_8BITS_DEVICE_ID 1
#define XPAR_SWS_8BITS_INTERRUPT_PRESENT 0
#define XPAR_SWS_8BITS_IS_DUAL 0


/******************************************************************/

/* Canonical definitions for peripheral BTNS_5BITS */
#define XPAR_GPIO_0_BASEADDR 0x41200000
#define XPAR_GPIO_0_HIGHADDR 0x4120FFFF
#define XPAR_GPIO_0_DEVICE_ID XPAR_BTNS_5BITS_DEVICE_ID
#define XPAR_GPIO_0_INTERRUPT_PRESENT 0
#define XPAR_GPIO_0_IS_DUAL 0

/* Canonical definitions for peripheral SWS_8BITS */
#define XPAR_GPIO_1_BASEADDR 0x41210000
#define XPAR_GPIO_1_HIGHADDR 0x4121FFFF
#define XPAR_GPIO_1_DEVICE_ID XPAR_SWS_8BITS_DEVICE_ID
#define XPAR_GPIO_1_INTERRUPT_PRESENT 0
#define XPAR_GPIO_1_IS_DUAL 0


/******************************************************************/

/* Definitions for driver GPIOPS */
#define XPAR_XGPIOPS_NUM_INSTANCES 1

/* Definitions for peripheral PS7_GPIO_0 */
#define XPAR_PS7_GPIO_0_DEVICE_ID 0
#define XPAR_PS7_GPIO_0_BASEADDR 0xE000A000
#define XPAR_PS7_GPIO_0_HIGHADDR 0xE000AFFF


/******************************************************************/

/* Canonical definitions for peripheral PS7_GPIO_0 */
#define XPAR_XGPIOPS_0_DEVICE_ID XPAR_PS7_GPIO_0_DEVICE_ID
#define XPAR_XGPIOPS_0_BASEADDR 0xE000A000
#define XPAR_XGPIOPS_0_HIGHADDR 0xE000AFFF


/******************************************************************/

/* Definitions for driver IIC */
#define XPAR_XIIC_NUM_INSTANCES 2

/* Definitions for peripheral FMC_IMAGEON_IIC_0 */
#define XPAR_FMC_IMAGEON_IIC_0_DEVICE_ID 0
#define XPAR_FMC_IMAGEON_IIC_0_BASEADDR 0x41600000
#define XPAR_FMC_IMAGEON_IIC_0_HIGHADDR 0x4160FFFF
#define XPAR_FMC_IMAGEON_IIC_0_TEN_BIT_ADR 0
#define XPAR_FMC_IMAGEON_IIC_0_GPO_WIDTH 1


/* Definitions for peripheral FMC_IPMI_ID_EEPROM_0 */
#define XPAR_FMC_IPMI_ID_EEPROM_0_DEVICE_ID 1
#define XPAR_FMC_IPMI_ID_EEPROM_0_BASEADDR 0x41610000
#define XPAR_FMC_IPMI_ID_EEPROM_0_HIGHADDR 0x4161FFFF
#define XPAR_FMC_IPMI_ID_EEPROM_0_TEN_BIT_ADR 0
#define XPAR_FMC_IPMI_ID_EEPROM_0_GPO_WIDTH 8


/******************************************************************/

/* Canonical definitions for peripheral FMC_IMAGEON_IIC_0 */
#define XPAR_IIC_0_DEVICE_ID XPAR_FMC_IMAGEON_IIC_0_DEVICE_ID
#define XPAR_IIC_0_BASEADDR 0x41600000
#define XPAR_IIC_0_HIGHADDR 0x4160FFFF
#define XPAR_IIC_0_TEN_BIT_ADR 0
#define XPAR_IIC_0_GPO_WIDTH 1

/* Canonical definitions for peripheral FMC_IPMI_ID_EEPROM_0 */
#define XPAR_IIC_1_DEVICE_ID XPAR_FMC_IPMI_ID_EEPROM_0_DEVICE_ID
#define XPAR_IIC_1_BASEADDR 0x41610000
#define XPAR_IIC_1_HIGHADDR 0x4161FFFF
#define XPAR_IIC_1_TEN_BIT_ADR 0
#define XPAR_IIC_1_GPO_WIDTH 8


/******************************************************************/

/* Definitions for driver QSPIPS */
#define XPAR_XQSPIPS_NUM_INSTANCES 1

/* Definitions for peripheral PS7_QSPI_0 */
#define XPAR_PS7_QSPI_0_DEVICE_ID 0
#define XPAR_PS7_QSPI_0_BASEADDR 0xE000D000
#define XPAR_PS7_QSPI_0_HIGHADDR 0xE000DFFF
#define XPAR_PS7_QSPI_0_QSPI_CLK_FREQ_HZ 200000000
#define XPAR_PS7_QSPI_0_QSPI_MODE 0
#define XPAR_PS7_QSPI_0_QSPI_BUS_WIDTH 2


/******************************************************************/

/* Canonical definitions for peripheral PS7_QSPI_0 */
#define XPAR_XQSPIPS_0_DEVICE_ID XPAR_PS7_QSPI_0_DEVICE_ID
#define XPAR_XQSPIPS_0_BASEADDR 0xE000D000
#define XPAR_XQSPIPS_0_HIGHADDR 0xE000DFFF
#define XPAR_XQSPIPS_0_QSPI_CLK_FREQ_HZ 200000000
#define XPAR_XQSPIPS_0_QSPI_MODE 0
#define XPAR_XQSPIPS_0_QSPI_BUS_WIDTH 2


/******************************************************************/

/* Definitions for driver RGB2YCRCB */
#define XPAR_XRGB2YCRCB_NUM_INSTANCES 1

/* Definitions for peripheral V_RGB2YCRCB_0 */
#define XPAR_V_RGB2YCRCB_0_DEVICE_ID 0
#define XPAR_V_RGB2YCRCB_0_BASEADDR 0x43C40000
#define XPAR_V_RGB2YCRCB_0_HIGHADDR 0x43C4FFFF
#define XPAR_V_RGB2YCRCB_0_S_AXIS_VIDEO_FORMAT 2
#define XPAR_V_RGB2YCRCB_0_M_AXIS_VIDEO_FORMAT 1
#define XPAR_V_RGB2YCRCB_0_HAS_DEBUG 0
#define XPAR_V_RGB2YCRCB_0_HAS_INTC_IF 0
#define XPAR_V_RGB2YCRCB_0_MAX_COLS 1920
#define XPAR_V_RGB2YCRCB_0_ACTIVE_COLS 1920
#define XPAR_V_RGB2YCRCB_0_ACTIVE_ROWS 1080
#define XPAR_V_RGB2YCRCB_0_HAS_CLIP 1
#define XPAR_V_RGB2YCRCB_0_HAS_CLAMP 1
#define XPAR_V_RGB2YCRCB_0_ACOEF 16829
#define XPAR_V_RGB2YCRCB_0_BCOEF 6415
#define XPAR_V_RGB2YCRCB_0_CCOEF 38731
#define XPAR_V_RGB2YCRCB_0_DCOEF 37827
#define XPAR_V_RGB2YCRCB_0_YOFFSET 16
#define XPAR_V_RGB2YCRCB_0_CBOFFSET 128
#define XPAR_V_RGB2YCRCB_0_CROFFSET 128
#define XPAR_V_RGB2YCRCB_0_YMAX 255
#define XPAR_V_RGB2YCRCB_0_YMIN 0
#define XPAR_V_RGB2YCRCB_0_CBMAX 255
#define XPAR_V_RGB2YCRCB_0_CBMIN 0
#define XPAR_V_RGB2YCRCB_0_CRMAX 255
#define XPAR_V_RGB2YCRCB_0_CRMIN 0
#define XPAR_V_RGB2YCRCB_0_S_AXI_CLK_FREQ_HZ 100000000
#define XPAR_V_RGB2YCRCB_0_STANDARD_SEL 0
#define XPAR_V_RGB2YCRCB_0_OUTPUT_RANGE 0


/******************************************************************/

/* Canonical definitions for peripheral V_RGB2YCRCB_0 */
#define XPAR_RGB2YCRCB_0_DEVICE_ID XPAR_V_RGB2YCRCB_0_DEVICE_ID
#define XPAR_RGB2YCRCB_0_BASEADDR 0x43C40000
#define XPAR_RGB2YCRCB_0_HIGHADDR 0x43C4FFFF
#define XPAR_RGB2YCRCB_0_S_AXIS_VIDEO_FORMAT 2
#define XPAR_RGB2YCRCB_0_M_AXIS_VIDEO_FORMAT 1
#define XPAR_RGB2YCRCB_0_HAS_DEBUG 0
#define XPAR_RGB2YCRCB_0_HAS_INTC_IF 0
#define XPAR_RGB2YCRCB_0_MAX_COLS 1920
#define XPAR_RGB2YCRCB_0_ACTIVE_COLS 1920
#define XPAR_RGB2YCRCB_0_ACTIVE_ROWS 1080
#define XPAR_RGB2YCRCB_0_HAS_CLIP 1
#define XPAR_RGB2YCRCB_0_HAS_CLAMP 1
#define XPAR_RGB2YCRCB_0_ACOEF 16829
#define XPAR_RGB2YCRCB_0_BCOEF 6415
#define XPAR_RGB2YCRCB_0_CCOEF 38731
#define XPAR_RGB2YCRCB_0_DCOEF 37827
#define XPAR_RGB2YCRCB_0_YOFFSET 16
#define XPAR_RGB2YCRCB_0_CBOFFSET 128
#define XPAR_RGB2YCRCB_0_CROFFSET 128
#define XPAR_RGB2YCRCB_0_YMAX 255
#define XPAR_RGB2YCRCB_0_YMIN 0
#define XPAR_RGB2YCRCB_0_CBMAX 255
#define XPAR_RGB2YCRCB_0_CBMIN 0
#define XPAR_RGB2YCRCB_0_CRMAX 255
#define XPAR_RGB2YCRCB_0_CRMIN 0
#define XPAR_RGB2YCRCB_0_S_AXI_CLK_FREQ_HZ 100000000
#define XPAR_RGB2YCRCB_0_STANDARD_SEL 0
#define XPAR_RGB2YCRCB_0_OUTPUT_RANGE 0


/******************************************************************/

/* Definitions for driver SCUGIC */
#define XPAR_XSCUGIC_NUM_INSTANCES 1U

/* Definitions for peripheral PS7_SCUGIC_0 */
#define XPAR_PS7_SCUGIC_0_DEVICE_ID 0U
#define XPAR_PS7_SCUGIC_0_BASEADDR 0xF8F00100U
#define XPAR_PS7_SCUGIC_0_HIGHADDR 0xF8F001FFU
#define XPAR_PS7_SCUGIC_0_DIST_BASEADDR 0xF8F01000U


/******************************************************************/

/* Canonical definitions for peripheral PS7_SCUGIC_0 */
#define XPAR_SCUGIC_0_DEVICE_ID 0U
#define XPAR_SCUGIC_0_CPU_BASEADDR 0xF8F00100U
#define XPAR_SCUGIC_0_CPU_HIGHADDR 0xF8F001FFU
#define XPAR_SCUGIC_0_DIST_BASEADDR 0xF8F01000U


/******************************************************************/

/* Definitions for driver SCUTIMER */
#define XPAR_XSCUTIMER_NUM_INSTANCES 1

/* Definitions for peripheral PS7_SCUTIMER_0 */
#define XPAR_PS7_SCUTIMER_0_DEVICE_ID 0
#define XPAR_PS7_SCUTIMER_0_BASEADDR 0xF8F00600
#define XPAR_PS7_SCUTIMER_0_HIGHADDR 0xF8F0061F


/******************************************************************/

/* Canonical definitions for peripheral PS7_SCUTIMER_0 */
#define XPAR_XSCUTIMER_0_DEVICE_ID XPAR_PS7_SCUTIMER_0_DEVICE_ID
#define XPAR_XSCUTIMER_0_BASEADDR 0xF8F00600
#define XPAR_XSCUTIMER_0_HIGHADDR 0xF8F0061F


/******************************************************************/

/* Definitions for driver SCUWDT */
#define XPAR_XSCUWDT_NUM_INSTANCES 1

/* Definitions for peripheral PS7_SCUWDT_0 */
#define XPAR_PS7_SCUWDT_0_DEVICE_ID 0
#define XPAR_PS7_SCUWDT_0_BASEADDR 0xF8F00620
#define XPAR_PS7_SCUWDT_0_HIGHADDR 0xF8F006FF


/******************************************************************/

/* Canonical definitions for peripheral PS7_SCUWDT_0 */
#define XPAR_SCUWDT_0_DEVICE_ID XPAR_PS7_SCUWDT_0_DEVICE_ID
#define XPAR_SCUWDT_0_BASEADDR 0xF8F00620
#define XPAR_SCUWDT_0_HIGHADDR 0xF8F006FF


/******************************************************************/

/* Definitions for driver SDPS */
#define XPAR_XSDPS_NUM_INSTANCES 1

/* Definitions for peripheral PS7_SD_0 */
#define XPAR_PS7_SD_0_DEVICE_ID 0
#define XPAR_PS7_SD_0_BASEADDR 0xE0100000
#define XPAR_PS7_SD_0_HIGHADDR 0xE0100FFF
#define XPAR_PS7_SD_0_SDIO_CLK_FREQ_HZ 50000000
#define XPAR_PS7_SD_0_HAS_CD 1
#define XPAR_PS7_SD_0_HAS_WP 1
#define XPAR_PS7_SD_0_BUS_WIDTH 0
#define XPAR_PS7_SD_0_MIO_BANK 0
#define XPAR_PS7_SD_0_HAS_EMIO 0


/******************************************************************/

#define XPAR_PS7_SD_0_IS_CACHE_COHERENT 0
/* Canonical definitions for peripheral PS7_SD_0 */
#define XPAR_XSDPS_0_DEVICE_ID XPAR_PS7_SD_0_DEVICE_ID
#define XPAR_XSDPS_0_BASEADDR 0xE0100000
#define XPAR_XSDPS_0_HIGHADDR 0xE0100FFF
#define XPAR_XSDPS_0_SDIO_CLK_FREQ_HZ 50000000
#define XPAR_XSDPS_0_HAS_CD 1
#define XPAR_XSDPS_0_HAS_WP 1
#define XPAR_XSDPS_0_BUS_WIDTH 0
#define XPAR_XSDPS_0_MIO_BANK 0
#define XPAR_XSDPS_0_HAS_EMIO 0
#define XPAR_XSDPS_0_IS_CACHE_COHERENT 0


/******************************************************************/

/* Definitions for driver TTCPS */
#define XPAR_XTTCPS_NUM_INSTANCES 3U

/* Definitions for peripheral PS7_TTC_0 */
#define XPAR_PS7_TTC_0_DEVICE_ID 0U
#define XPAR_PS7_TTC_0_BASEADDR 0XF8001000U
#define XPAR_PS7_TTC_0_TTC_CLK_FREQ_HZ 111111115U
#define XPAR_PS7_TTC_0_TTC_CLK_CLKSRC 0U
#define XPAR_PS7_TTC_1_DEVICE_ID 1U
#define XPAR_PS7_TTC_1_BASEADDR 0XF8001004U
#define XPAR_PS7_TTC_1_TTC_CLK_FREQ_HZ 111111115U
#define XPAR_PS7_TTC_1_TTC_CLK_CLKSRC 0U
#define XPAR_PS7_TTC_2_DEVICE_ID 2U
#define XPAR_PS7_TTC_2_BASEADDR 0XF8001008U
#define XPAR_PS7_TTC_2_TTC_CLK_FREQ_HZ 111111115U
#define XPAR_PS7_TTC_2_TTC_CLK_CLKSRC 0U


/******************************************************************/

/* Canonical definitions for peripheral PS7_TTC_0 */
#define XPAR_XTTCPS_0_DEVICE_ID XPAR_PS7_TTC_0_DEVICE_ID
#define XPAR_XTTCPS_0_BASEADDR 0xF8001000U
#define XPAR_XTTCPS_0_TTC_CLK_FREQ_HZ 111111115U
#define XPAR_XTTCPS_0_TTC_CLK_CLKSRC 0U

#define XPAR_XTTCPS_1_DEVICE_ID XPAR_PS7_TTC_1_DEVICE_ID
#define XPAR_XTTCPS_1_BASEADDR 0xF8001004U
#define XPAR_XTTCPS_1_TTC_CLK_FREQ_HZ 111111115U
#define XPAR_XTTCPS_1_TTC_CLK_CLKSRC 0U

#define XPAR_XTTCPS_2_DEVICE_ID XPAR_PS7_TTC_2_DEVICE_ID
#define XPAR_XTTCPS_2_BASEADDR 0xF8001008U
#define XPAR_XTTCPS_2_TTC_CLK_FREQ_HZ 111111115U
#define XPAR_XTTCPS_2_TTC_CLK_CLKSRC 0U


/******************************************************************/

/* Definitions for driver UARTPS */
#define XPAR_XUARTPS_NUM_INSTANCES 1

/* Definitions for peripheral PS7_UART_1 */
#define XPAR_PS7_UART_1_DEVICE_ID 0
#define XPAR_PS7_UART_1_BASEADDR 0xE0001000
#define XPAR_PS7_UART_1_HIGHADDR 0xE0001FFF
#define XPAR_PS7_UART_1_UART_CLK_FREQ_HZ 50000000
#define XPAR_PS7_UART_1_HAS_MODEM 0


/******************************************************************/

/* Canonical definitions for peripheral PS7_UART_1 */
#define XPAR_XUARTPS_0_DEVICE_ID XPAR_PS7_UART_1_DEVICE_ID
#define XPAR_XUARTPS_0_BASEADDR 0xE0001000
#define XPAR_XUARTPS_0_HIGHADDR 0xE0001FFF
#define XPAR_XUARTPS_0_UART_CLK_FREQ_HZ 50000000
#define XPAR_XUARTPS_0_HAS_MODEM 0


/******************************************************************/

/* Definitions for driver USBPS */
#define XPAR_XUSBPS_NUM_INSTANCES 1

/* Definitions for peripheral PS7_USB_0 */
#define XPAR_PS7_USB_0_DEVICE_ID 0
#define XPAR_PS7_USB_0_BASEADDR 0xE0002000
#define XPAR_PS7_USB_0_HIGHADDR 0xE0002FFF


/******************************************************************/

/* Canonical definitions for peripheral PS7_USB_0 */
#define XPAR_XUSBPS_0_DEVICE_ID XPAR_PS7_USB_0_DEVICE_ID
#define XPAR_XUSBPS_0_BASEADDR 0xE0002000
#define XPAR_XUSBPS_0_HIGHADDR 0xE0002FFF


/******************************************************************/

/* Definitions for driver V_DEMOSAIC */
#define XPAR_XV_DEMOSAIC_NUM_INSTANCES 1

/* Definitions for peripheral V_DEMOSAIC_0 */
#define XPAR_V_DEMOSAIC_0_DEVICE_ID 0
#define XPAR_V_DEMOSAIC_0_S_AXI_CTRL_BASEADDR 0x43C00000
#define XPAR_V_DEMOSAIC_0_S_AXI_CTRL_HIGHADDR 0x43C0FFFF
#define XPAR_V_DEMOSAIC_0_SAMPLES_PER_CLOCK 1
#define XPAR_V_DEMOSAIC_0_MAX_COLS 1920
#define XPAR_V_DEMOSAIC_0_MAX_ROWS 1080
#define XPAR_V_DEMOSAIC_0_MAX_DATA_WIDTH 8
#define XPAR_V_DEMOSAIC_0_ALGORITHM 1


/******************************************************************/

/* Canonical definitions for peripheral V_DEMOSAIC_0 */
#define XPAR_XV_DEMOSAIC_0_DEVICE_ID XPAR_V_DEMOSAIC_0_DEVICE_ID
#define XPAR_XV_DEMOSAIC_0_S_AXI_CTRL_BASEADDR 0x43C00000
#define XPAR_XV_DEMOSAIC_0_S_AXI_CTRL_HIGHADDR 0x43C0FFFF
#define XPAR_XV_DEMOSAIC_0_SAMPLES_PER_CLOCK 1
#define XPAR_XV_DEMOSAIC_0_MAX_COLS 1920
#define XPAR_XV_DEMOSAIC_0_MAX_ROWS 1080
#define XPAR_XV_DEMOSAIC_0_MAX_DATA_WIDTH 8
#define XPAR_XV_DEMOSAIC_0_ALGORITHM 1


/******************************************************************/

/* Definitions for driver V_HCRESAMPLER */
#define XPAR_XV_HCRESAMPLER_NUM_INSTANCES 1

/* Definitions for peripheral V_PROC_SS_0_HCR */
#define XPAR_V_PROC_SS_0_HCR_DEVICE_ID 0
#define XPAR_V_PROC_SS_0_HCR_S_AXI_CTRL_BASEADDR 0x00000000
#define XPAR_V_PROC_SS_0_HCR_S_AXI_CTRL_HIGHADDR 0x0000FFFF
#define XPAR_V_PROC_SS_0_HCR_SAMPLES_PER_CLOCK 1
#define XPAR_V_PROC_SS_0_HCR_MAX_COLS 1920
#define XPAR_V_PROC_SS_0_HCR_MAX_ROWS 1080
#define XPAR_V_PROC_SS_0_HCR_MAX_DATA_WIDTH 8
#define XPAR_V_PROC_SS_0_HCR_CONVERT_TYPE 1
#define XPAR_V_PROC_SS_0_HCR_NUM_H_TAPS 4


/******************************************************************/

/* Canonical definitions for peripheral V_PROC_SS_0_HCR */
#define XPAR_XV_HCRESAMPLER_0_DEVICE_ID XPAR_V_PROC_SS_0_HCR_DEVICE_ID
#define XPAR_XV_HCRESAMPLER_0_S_AXI_CTRL_BASEADDR 0x00000000
#define XPAR_XV_HCRESAMPLER_0_S_AXI_CTRL_HIGHADDR 0x0000FFFF
#define XPAR_XV_HCRESAMPLER_0_SAMPLES_PER_CLOCK 1
#define XPAR_XV_HCRESAMPLER_0_MAX_COLS 1920
#define XPAR_XV_HCRESAMPLER_0_MAX_ROWS 1080
#define XPAR_XV_HCRESAMPLER_0_MAX_DATA_WIDTH 8
#define XPAR_XV_HCRESAMPLER_0_CONVERT_TYPE 1
#define XPAR_XV_HCRESAMPLER_0_NUM_H_TAPS 4


/******************************************************************/

/* Definitions for driver VPROCSS */
#define XPAR_XVPROCSS_NUM_INSTANCES 1

/* Definitions for peripheral V_PROC_SS_0 */
#define XPAR_V_PROC_SS_0_BASEADDR 0x43C50000
#define XPAR_V_PROC_SS_0_HIGHADDR 0x43C5FFFF
#define XPAR_V_PROC_SS_0_DEVICE_ID 0
#define XPAR_V_PROC_SS_0_SCALER_ALGORITHM 2
#define XPAR_V_PROC_SS_0_TOPOLOGY 5
#define XPAR_V_PROC_SS_0_SAMPLES_PER_CLK 1
#define XPAR_V_PROC_SS_0_MAX_DATA_WIDTH 8
#define XPAR_V_PROC_SS_0_NUM_VIDEO_COMPONENTS 3
#define XPAR_V_PROC_SS_0_MAX_COLS 1920
#define XPAR_V_PROC_SS_0_MAX_ROWS 1080
#define XPAR_V_PROC_SS_0_H_SCALER_TAPS 6
#define XPAR_V_PROC_SS_0_V_SCALER_TAPS 6
#define XPAR_V_PROC_SS_0_H_SCALER_PHASES 64
#define XPAR_V_PROC_SS_0_V_SCALER_PHASES 64
#define XPAR_V_PROC_SS_0_CHROMA_ALGORITHM 2
#define XPAR_V_PROC_SS_0_H_CHROMA_TAPS 4
#define XPAR_V_PROC_SS_0_V_CHROMA_TAPS 4
#define XPAR_V_PROC_SS_0_DEINT_MOTION_ADAPTIVE 1


/******************************************************************/

/* Canonical definitions for peripheral V_PROC_SS_0 */
#define XPAR_XVPROCSS_0_BASEADDR 0x43C50000
#define XPAR_XVPROCSS_0_HIGHADDR 0x43C5FFFF
#define XPAR_XVPROCSS_0_DEVICE_ID XPAR_V_PROC_SS_0_DEVICE_ID
#define XPAR_XVPROCSS_0_SCALER_ALGORITHM 2
#define XPAR_XVPROCSS_0_TOPOLOGY 5
#define XPAR_XVPROCSS_0_SAMPLES_PER_CLK 1
#define XPAR_XVPROCSS_0_MAX_DATA_WIDTH 8
#define XPAR_XVPROCSS_0_NUM_VIDEO_COMPONENTS 3
#define XPAR_XVPROCSS_0_MAX_COLS 1920
#define XPAR_XVPROCSS_0_MAX_ROWS 1080
#define XPAR_XVPROCSS_0_H_SCALER_TAPS 6
#define XPAR_XVPROCSS_0_V_SCALER_TAPS 6
#define XPAR_XVPROCSS_0_H_SCALER_PHASES 64
#define XPAR_XVPROCSS_0_V_SCALER_PHASES 64
#define XPAR_XVPROCSS_0_CHROMA_ALGORITHM 2
#define XPAR_XVPROCSS_0_H_CHROMA_TAPS 4
#define XPAR_XVPROCSS_0_V_CHROMA_TAPS 4
#define XPAR_XVPROCSS_0_DEINT_MOTION_ADAPTIVE 1


/******************************************************************/

/* Definitions for driver VTC */
#define XPAR_XVTC_NUM_INSTANCES 1

/* Definitions for peripheral V_TC_0 */
#define XPAR_V_TC_0_DEVICE_ID 0
#define XPAR_V_TC_0_BASEADDR 0x43C10000
#define XPAR_V_TC_0_HIGHADDR 0x43C1FFFF
#define XPAR_V_TC_0_GENERATE_EN 1
#define XPAR_V_TC_0_DETECT_EN 0
#define XPAR_V_TC_0_DET_HSYNC_EN 1
#define XPAR_V_TC_0_DET_VSYNC_EN 1
#define XPAR_V_TC_0_DET_HBLANK_EN 1
#define XPAR_V_TC_0_DET_VBLANK_EN 1
#define XPAR_V_TC_0_DET_AVIDEO_EN 1
#define XPAR_V_TC_0_DET_ACHROMA_EN 0


/******************************************************************/

/* Canonical definitions for peripheral V_TC_0 */
#define XPAR_VTC_0_DEVICE_ID XPAR_V_TC_0_DEVICE_ID
#define XPAR_VTC_0_BASEADDR 0x43C10000
#define XPAR_VTC_0_HIGHADDR 0x43C1FFFF
#define XPAR_VTC_0_GENERATE_EN 1
#define XPAR_VTC_0_DETECT_EN 0
#define XPAR_VTC_0_DET_HSYNC_EN 1
#define XPAR_VTC_0_DET_VSYNC_EN 1
#define XPAR_VTC_0_DET_HBLANK_EN 1
#define XPAR_VTC_0_DET_VBLANK_EN 1
#define XPAR_VTC_0_DET_AVIDEO_EN 1
#define XPAR_VTC_0_DET_ACHROMA_EN 0


/******************************************************************/

/* Definitions for driver XADCPS */
#define XPAR_XADCPS_NUM_INSTANCES 1

/* Definitions for peripheral PS7_XADC_0 */
#define XPAR_PS7_XADC_0_DEVICE_ID 0
#define XPAR_PS7_XADC_0_BASEADDR 0xF8007100
#define XPAR_PS7_XADC_0_HIGHADDR 0xF8007120


/******************************************************************/

/* Canonical definitions for peripheral PS7_XADC_0 */
#define XPAR_XADCPS_0_DEVICE_ID XPAR_PS7_XADC_0_DEVICE_ID
#define XPAR_XADCPS_0_BASEADDR 0xF8007100
#define XPAR_XADCPS_0_HIGHADDR 0xF8007120


/******************************************************************/

#endif  /* end of protection macro */
