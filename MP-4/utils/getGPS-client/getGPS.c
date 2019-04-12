////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// File Name: getGPS.c                                                        //
// Description: Request GPS data from server and send to Zedboard.            //
//              and Display debug messages from Zedboard.                     //
//                                                                            //
// compile: compile from codeBlocks (make sure to add ws2_32 lib for winsock  //
//                                                                            //
// Usage: getGPS <Server IP> <GPS UDP Port> <Zedboard Com Port>               //
//                                                                            //
// Example: getGPS 129.186.158.240 4560 1                                     //
//                                                                            //
// Note: Spring 2014 Server is on co2041-16.ece.iastate.edu                   //
//                                                                            //
/* Comments from base soruce code (Simple UDP client example                  */
/* CHANGES FROM UNIX VERSION                                                  */
/*                                                                            */
/* 1.  Changed header files.                                                  */
/* 2.  Added WSAStartUP() and WSACleanUp().                                   */
/* 3.  Used closesocket() instead of close().                                 */

// phjones: Make sure to add ws2_32 to build option linker settings


#include <stdio.h>      /* for printf(), fprintf() */
#include <winsock.h>    /* for socket(),... */
#include <stdlib.h>     /* for exit() */

#include <conio.h>
#include "rs232.h"

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in GPS_ServAddr; /* Echo server address */
    struct sockaddr_in fromAddr;     /* Source address of echo */
    unsigned short GPS_ServPort;     /* Echo server port */
    int fromSize;                    /* In-out of address size for recvfrom() */
    char *servIP;                    /* IP address of server */

    unsigned char *GPS_request_str = "S";  /* String to send to GPS server */
    int GPS_req_StringLen  = 1;      /* Length of request command string */

    unsigned char GPS_resp_Buffer[100];  /* Buffer to GPS data response */
    int GPS_resp_StringLen;              /* size of response received */
    int GPS_RESP_SIZE = 16;              /* Expected size of GPS response */
    float *GPS_float = (float *) GPS_resp_Buffer; // For float based array addressing

    // COM port interface
    int bdrate       = 115200;        // 115200 baud
    int cport_nr_ZED = 0;             // /dev/ttyS0 (COM1 on windows)
    unsigned char get_ZED_buf[4096];  // Data received for Zedboard COM port

    int Debug_payload_count = 0;   // Keep track of Zedboad Debug payload size

    char ch = 0;                   // store user key press
    int n;                         // General variable

    WSADATA wsaData;                 /* Structure for WinSock setup communication */



     /* Test for correct number of arguments */
    if (argc != 4)
    {
        fprintf(stderr,"Usage: %s <Server IP> <GPS UDP Port> <Zedboard Com Port>\n", argv[0]);
        exit(1);
    }

    // Get server IP (dotted quad)
    servIP = argv[1];

    // Get UPD port to send/recieve data
    GPS_ServPort = atoi(argv[2]);

    // Get COM port to interace to Zedboard
    cport_nr_ZED = atoi(argv[3]) - 1;

    // Open Zedboad COM port
    if(RS232_OpenComport(cport_nr_ZED, bdrate))
    {
      printf("Can not open QUAD comport\n");
      return(0);
    }
    printf("QUAD Connected\n");

    /* Load Winsock 2.0 DLL */
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
    {
        fprintf(stderr, "WSAStartup() failed");
        exit(1);
    }

    /* Create a best-effort datagram socket using UDP */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
     printf("Socket failed");
     exit(1);
    }

    /* Construct the server address structure */
    memset(&GPS_ServAddr, 0, sizeof(GPS_ServAddr));    /* Zero out structure */
    GPS_ServAddr.sin_family = AF_INET;                 /* Internet address family */
    GPS_ServAddr.sin_addr.s_addr = inet_addr(servIP);  /* Server IP address */
    GPS_ServAddr.sin_port   = htons(GPS_ServPort);     /* Server port */

    printf("Press q to exit\n");

    // Main loop
    while(ch != 'q')
    {

      // Get user keypress
      if(kbhit())
      {
        ch = getch();
      }

      /* Send an 'S' to the server to request a GPS update */
      if (sendto(sock, GPS_request_str, GPS_req_StringLen, 0, (struct sockaddr *)
                 &GPS_ServAddr, sizeof(GPS_ServAddr)) != GPS_req_StringLen)
      {
        printf("Wrong number of bytes");
        exit(1);
      }

      /* Wait for the response */
      fromSize = sizeof(fromAddr);
      if ((GPS_resp_StringLen = recvfrom(sock, GPS_resp_Buffer, GPS_RESP_SIZE, 0, (struct sockaddr *) &fromAddr,
                   &fromSize)) != GPS_RESP_SIZE)
      {
        printf("recvfrom failed");
        exit(1);
      }

      // Check that response came from the GPS server
      if (GPS_ServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
      {
        fprintf(stderr,"Error: received a packet from unknown source.\n");
        exit(1);
      }

      // Print GPS information
      printf("X =%4.2f, Y=%4.2f, Z=%4.2f, Yaw=%4.2f\r",
             GPS_float[0], GPS_float[1], GPS_float[2], GPS_float[3]);

      // Check for a Zedboard GPS update request 'S' or 'D'
      get_ZED_buf[0] = 0;
      n = RS232_PollComport(cport_nr_ZED, get_ZED_buf, 1);

      if(get_ZED_buf[0] == 'S' || get_ZED_buf[0] == 'D')
      {

        // Display Debug information if 'D' request was received
        if(get_ZED_buf[0] == 'D')
        {
          Debug_payload_count = 0;
          while(Debug_payload_count < 1000)
          {
            n = RS232_PollComport(cport_nr_ZED, &get_ZED_buf[Debug_payload_count], 1000);
            Debug_payload_count = Debug_payload_count + n;
          }
          get_ZED_buf[1001] = 0;  // Make sure buffer is NULL terminated
          printf("\n%s\n", get_ZED_buf);
        }

        // Send GPS information to Zedboard
        RS232_SendBuf(cport_nr_ZED, GPS_resp_Buffer, 16);

      }
    }

    printf("Exiting\n");

    /* Cleanup Winsock */
    closesocket(sock);
    WSACleanup();

    exit(0);
}
