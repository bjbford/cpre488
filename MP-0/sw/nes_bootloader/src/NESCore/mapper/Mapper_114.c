/*===================================================================*/
/*                                                                   */
/*                       Mapper 114 (PC-SuperGames)                  */
/*                                                                   */
/*===================================================================*/

byte  Map114_Regs[ 8 ];
dword Map114_Prg0, Map114_Prg1;
dword Map114_Chr01, Map114_Chr23;
dword Map114_Chr4, Map114_Chr5, Map114_Chr6, Map114_Chr7;

byte Map114_IRQ_Enable;
byte Map114_IRQ_Cnt;
byte Map114_IRQ_Latch;

#define Map114_Chr_Swap()    ( Map114_Regs[ 0 ] & 0x80 )
#define Map114_Prg_Swap()    ( Map114_Regs[ 0 ] & 0x40 )

/*-------------------------------------------------------------------*/
/*  Initialize Mapper 114                                            */
/*-------------------------------------------------------------------*/
void Map114_Init()
{
  /* Initialize Mapper */
  MapperInit = Map114_Init;

  /* Write to Mapper */
  MapperWrite = Map114_Write;

  /* Write to SRAM */
  MapperSram = Map114_Sram;

  /* Write to APU */
  MapperApu = Map0_Apu;

  /* Read from APU */
  MapperReadApu = Map0_ReadApu;

  /* Callback at VSync */
  MapperVSync = Map0_VSync;

  /* Callback at HSync */
  MapperHSync = Map114_HSync;

  /* Callback at PPU */
  MapperPPU = Map0_PPU;

  /* Callback at Rendering Screen ( 1:BG, 0:Sprite ) */
  MapperRenderScreen = Map0_RenderScreen;

  /* Set SRAM Banks */
  W.SRAMBANK = S.SRAM;

  /* Initialize State Registers */
  int nPage ;
for (nPage = 0; nPage < 8; nPage++)
  {
    Map114_Regs[ nPage ] = 0x00;
  }

  /* Set ROM Banks */
  Map114_Prg0 = 0;
  Map114_Prg1 = 1;
  Map114_Set_CPU_Banks();

  /* Set PPU Banks */
  if ( S.NesHeader.VROMSize > 0 )
  {
    Map114_Chr01 = 0;
    Map114_Chr23 = 2;
    Map114_Chr4  = 4;
    Map114_Chr5  = 5;
    Map114_Chr6  = 6;
    Map114_Chr7  = 7;
    Map114_Set_PPU_Banks();
  } else {
    Map114_Chr01 = Map114_Chr23 = 0;
    Map114_Chr4 = Map114_Chr5 = Map114_Chr6 = Map114_Chr7 = 0;
  }

  /* Initialize IRQ Registers */
  Map114_IRQ_Enable = 0;
  Map114_IRQ_Cnt = 0;
  Map114_IRQ_Latch = 0;


 
}

/*-------------------------------------------------------------------*/
/*  Mapper 114 Write to Sram Function                                */
/*-------------------------------------------------------------------*/
void Map114_Sram( word wAddr, byte byData )
{
  if ( ( wAddr == 0x6000 ) && ( byData == 0x00 ) )
  {
    /* Initialize State Registers */
    int nPage ;
for (nPage = 0; nPage < 8; nPage++)
    {
      Map114_Regs[ nPage ] = 0x00;
    }

    /* Set ROM Banks */
    Map114_Prg0 = 0;
    Map114_Prg1 = 1;
    Map114_Set_CPU_Banks();

    /* Set PPU Banks */
    if ( S.NesHeader.VROMSize > 0 )
    { 
      Map114_Chr01 = 0;
      Map114_Chr23 = 2;
      Map114_Chr4  = 4;
      Map114_Chr5  = 5;
      Map114_Chr6  = 6;
      Map114_Chr7  = 7;
      Map114_Set_PPU_Banks();
    } else {
      Map114_Chr01 = Map114_Chr23 = 0;
      Map114_Chr4 = Map114_Chr5 = Map114_Chr6 = Map114_Chr7 = 0;
    }

    /* Initialize IRQ Registers */
    Map114_IRQ_Enable = 0;
    Map114_IRQ_Cnt = 0;
    Map114_IRQ_Latch = 0;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 114 Write Function                                        */
/*-------------------------------------------------------------------*/
void Map114_Write( word wAddr, byte byData )
{
  dword dwBankNum;

  switch ( wAddr & 0xe001 )
  {
    case 0x8000:
      Map114_Regs[ 0 ] = byData;
      Map114_Set_PPU_Banks();
      Map114_Set_CPU_Banks();
      break;

    case 0x8001:
      Map114_Regs[ 1 ] = byData;
      dwBankNum = Map114_Regs[ 1 ];

      switch ( Map114_Regs[ 0 ] & 0x07 )
      {
        /* Set PPU Banks */
        case 0x00:
          if ( S.NesHeader.VROMSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map114_Chr01 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        case 0x01:
          if ( S.NesHeader.VROMSize > 0 )
          {
            dwBankNum &= 0xfe;
            Map114_Chr23 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        case 0x02:
          if ( S.NesHeader.VROMSize > 0 )
          {
            Map114_Chr4 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        case 0x03:
          if ( S.NesHeader.VROMSize > 0 )
          {
            Map114_Chr5 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        case 0x04:
          if ( S.NesHeader.VROMSize > 0 )
          {
            Map114_Chr6 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        case 0x05:
          if ( S.NesHeader.VROMSize > 0 )
          {
            Map114_Chr7 = dwBankNum;
            Map114_Set_PPU_Banks();
          }
          break;

        /* Set ROM Banks */
        case 0x06:
          Map114_Prg0 = dwBankNum;
          Map114_Set_CPU_Banks();
          break;

        case 0x07:
          Map114_Prg1 = dwBankNum;
          Map114_Set_CPU_Banks();
          break;
      }
      break;

    case 0xa000:
      Map114_Regs[ 2 ] = byData;

      if ( !S.ROM_FourScr )
      {
        if ( byData & 0x01 )
        {
          NESCore_Mirroring( 0 );
        } else {
          NESCore_Mirroring( 1 );
        }
      }
      break;

    case 0xa001:
      Map114_Regs[ 3 ] = byData;

      if ( byData & 0x80 )
      {
        /* Enable Save RAM $6000-$7fff */
      } else {
        /* Disable Save RAM $6000-$7fff */
      }
      break;

    case 0xc000:
      Map114_Regs[ 4 ] = byData;
      Map114_IRQ_Cnt = Map114_Regs[ 4 ];
      break;

    case 0xc001:
      Map114_Regs[ 5 ] = byData;
      Map114_IRQ_Latch = Map114_Regs[ 5 ];
      break;

    case 0xe000:
      Map114_Regs[ 6 ] = byData;
      Map114_IRQ_Enable = 0;
      break;

    case 0xe001:
      Map114_Regs[ 7 ] = byData;
      Map114_IRQ_Enable = 1;
      break;
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 114 H-Sync Function                                       */
/*-------------------------------------------------------------------*/
void Map114_HSync()
{
/*
 *  Callback at HSync
 *
 */
  if ( Map114_IRQ_Enable )
  {
    if ( /* 0 <= S.PPU_Scanline && */ S.PPU_Scanline <= 239 )
    {
      if ( S.PPU_R1 & R1_SHOW_SCR || S.PPU_R1 & R1_SHOW_SP )
      {
        if ( !( Map114_IRQ_Cnt-- ) )
        {
          Map114_IRQ_Cnt = Map114_IRQ_Latch;
          IRQ_REQ;
        }
      }
    }
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 114 Set CPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map114_Set_CPU_Banks()
{
  if ( Map114_Prg_Swap() )
  {
    W.ROMBANK0 = ROMLASTPAGE( 1 );
    W.ROMBANK1 = ROMPAGE( Map114_Prg1 % ( S.NesHeader.ROMSize << 1 ) );
    W.ROMBANK2 = ROMPAGE( Map114_Prg0 % ( S.NesHeader.ROMSize << 1 ) );
    W.ROMBANK3 = ROMLASTPAGE( 0 );
  } else {
    W.ROMBANK0 = ROMPAGE( Map114_Prg0 % ( S.NesHeader.ROMSize << 1 ) );
    W.ROMBANK1 = ROMPAGE( Map114_Prg1 % ( S.NesHeader.ROMSize << 1 ) );
    W.ROMBANK2 = ROMLASTPAGE( 1 );
    W.ROMBANK3 = ROMLASTPAGE( 0 );
  }
}

/*-------------------------------------------------------------------*/
/*  Mapper 114 Set PPU Banks Function                                */
/*-------------------------------------------------------------------*/
void Map114_Set_PPU_Banks()
{
  if ( S.NesHeader.VROMSize > 0 )
  {
    if ( Map114_Chr_Swap() )
    { 
      W.PPUBANK[ 0 ] = VROMPAGE( Map114_Chr4 % ( S.NesHeader.VROMSize << 3 ) );
      W.PPUBANK[ 1 ] = VROMPAGE( Map114_Chr5 % ( S.NesHeader.VROMSize << 3 ) );
      W.PPUBANK[ 2 ] = VROMPAGE( Map114_Chr6 % ( S.NesHeader.VROMSize << 3 ) );
      W.PPUBANK[ 3 ] = VROMPAGE( Map114_Chr7 % ( S.NesHeader.VROMSize << 3 ) );
      W.PPUBANK[ 4 ] = VROMPAGE( ( Map114_Chr01 + 0 ) % ( S.NesHeader.VROMSize << 3 ) );
      W.PPUBANK[ 5 ] = VROMPAGE( ( Map114_Chr01 + 1 ) % ( S.NesHeader.VROMSize << 3 ) );
      W.PPUBANK[ 6 ] = VROMPAGE( ( Map114_Chr23 + 0 ) % ( S.NesHeader.VROMSize << 3 ) );
      W.PPUBANK[ 7 ] = VROMPAGE( ( Map114_Chr23 + 1 ) % ( S.NesHeader.VROMSize << 3 ) );
      NESCore_Develop_Character_Data();
    } else {
      W.PPUBANK[ 0 ] = VROMPAGE( ( Map114_Chr01 + 0 ) % ( S.NesHeader.VROMSize << 3 ) );
      W.PPUBANK[ 1 ] = VROMPAGE( ( Map114_Chr01 + 1 ) % ( S.NesHeader.VROMSize << 3 ) );
      W.PPUBANK[ 2 ] = VROMPAGE( ( Map114_Chr23 + 0 ) % ( S.NesHeader.VROMSize << 3 ) );
      W.PPUBANK[ 3 ] = VROMPAGE( ( Map114_Chr23 + 1 ) % ( S.NesHeader.VROMSize << 3 ) );
      W.PPUBANK[ 4 ] = VROMPAGE( Map114_Chr4 % ( S.NesHeader.VROMSize << 3 ) );
      W.PPUBANK[ 5 ] = VROMPAGE( Map114_Chr5 % ( S.NesHeader.VROMSize << 3 ) );
      W.PPUBANK[ 6 ] = VROMPAGE( Map114_Chr6 % ( S.NesHeader.VROMSize << 3 ) );
      W.PPUBANK[ 7 ] = VROMPAGE( Map114_Chr7 % ( S.NesHeader.VROMSize << 3 ) );
      NESCore_Develop_Character_Data();
    }
  }
}
