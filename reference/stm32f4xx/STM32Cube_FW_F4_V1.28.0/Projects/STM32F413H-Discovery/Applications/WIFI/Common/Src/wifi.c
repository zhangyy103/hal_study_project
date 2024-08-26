/**
  ******************************************************************************
  * @file    wifi.c
  * @author  MCD Application Team
  * @brief   WIFI interface file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "wifi.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ES_WIFIObject_t    EsWifiObj;

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialize the LL part of the WIFI core
  * @param  None
  * @retval Operation status
  */
WIFI_Status_t WIFI_Init(void)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;
  
  if(ES_WIFI_RegisterBusIO(&EsWifiObj, 
                           SPI_WIFI_Init, 
                           SPI_WIFI_DeInit,
                           SPI_WIFI_Delay,
                           SPI_WIFI_SendData,
                           SPI_WIFI_ReceiveData) == ES_WIFI_STATUS_OK)
  {
    
    if(ES_WIFI_Init(&EsWifiObj) == ES_WIFI_STATUS_OK)
    {
      ret = WIFI_STATUS_OK;
    }
  }
  return ret;
}

/**
  * @brief  List a defined number of available access points
  * @param  APs : pointer to APs structure
  * @param  AP_MaxNbr : Max APs number to be listed
  * @retval Operation status
  */
WIFI_Status_t WIFI_ListAccessPoints(WIFI_APs_t *APs, uint8_t AP_MaxNbr)
{
  uint8_t APCount;
  WIFI_Status_t ret = WIFI_STATUS_ERROR;  
  ES_WIFI_APs_t esWifiAPs;
  
  if(ES_WIFI_ListAccessPoints(&EsWifiObj, &esWifiAPs) == ES_WIFI_STATUS_OK)
  {
    if(esWifiAPs.nbr > 0)
    {
      APs->count = MIN(esWifiAPs.nbr, AP_MaxNbr);  
      for(APCount = 0; APCount < APs->count; APCount++)
      {
        APs->ap[APCount].Ecn = (WIFI_Ecn_t)esWifiAPs.AP[APCount].Security;
        strncpy( (char *)APs->ap[APCount].SSID, (char *)esWifiAPs.AP[APCount].SSID, MIN (WIFI_MAX_SSID_NAME, WIFI_MAX_SSID_NAME));    
        APs->ap[APCount].RSSI = esWifiAPs.AP[APCount].RSSI;
        memcpy(APs->ap[APCount].MAC, esWifiAPs.AP[APCount].MAC, 6);
      }
    }
    ret = WIFI_STATUS_OK;  
  }
  return ret;
}

/**
  * @brief  Join an Access Point
  * @param  SSID : SSID string
  * @param  Password : Password string
  * @param  ecn : Encryption type
  * @param  IP_Addr : Got IP Address
  * @param  IP_Mask : Network IP mask
  * @param  Gateway_Addr : Gateway IP address
  * @param  MAC : pointer to MAC Address
  * @retval Operation status
  */
WIFI_Status_t WIFI_Connect(
                             const char* SSID, 
                             const char* Password,
                             WIFI_Ecn_t ecn)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;  
 
  if(ES_WIFI_Connect(&EsWifiObj, SSID, Password, (ES_WIFI_SecurityType_t) ecn) == ES_WIFI_STATUS_OK)
  {
    if(ES_WIFI_GetNetworkSettings(&EsWifiObj) == ES_WIFI_STATUS_OK)
    {
       ret = WIFI_STATUS_OK;
    }
    
  }
  return ret;
}

/**
  * @brief  This function retrieves the WiFi interface's MAC address.
  * @retval Operation Status.
  */
WIFI_Status_t WIFI_GetMAC_Address(uint8_t  *mac)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR; 
  
  if(ES_WIFI_GetMACAddress(&EsWifiObj, mac) == ES_WIFI_STATUS_OK)
  {
    ret = WIFI_STATUS_OK;
  }
  return ret;
}

/**
  * @brief  This function retrieves the WiFi interface's IP address.
  * @retval Operation Status.
  */
WIFI_Status_t WIFI_GetIP_Address (uint8_t  *ipaddr)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR; 
  
  if(EsWifiObj.NetSettings.IsConnected)
  {
    memcpy(ipaddr, EsWifiObj.NetSettings.IP_Addr, 4);
    ret = WIFI_STATUS_OK;
  }
  return ret;
}

/**
  * @brief  Disconnect from a network
  * @param  None
  * @retval Operation status
  */
WIFI_Status_t WIFI_Disconnect(void)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;    
  if( ES_WIFI_Disconnect(&EsWifiObj)== ES_WIFI_STATUS_OK)
  {
      ret = WIFI_STATUS_OK; 
  }
  
  return ret;
}

/**
  * @brief  Ping an IP address in the network
  * @param  ipaddr : array of the IP address
  * @retval Operation status
  */
WIFI_Status_t WIFI_Ping(uint8_t* ipaddr, uint16_t count, uint16_t interval_ms)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;  

  if(ES_WIFI_Ping(&EsWifiObj, ipaddr, count, interval_ms) == ES_WIFI_STATUS_OK)
  {
    ret = WIFI_STATUS_OK;
  }
  return ret;
}

/**
  * @brief  Configure and start a client connection
  * @param  type : Connection type TCP/UDP
  * @param  name : name of the connection
  * @param  location : Client address
  * @param  port : Remote port
  * @param  local_port : Local port
  * @retval Operation status
  */
WIFI_Status_t WIFI_OpenClientConnection(uint32_t socket, WIFI_Protocol_t type, const char* name, char* location, uint16_t port, uint16_t local_port)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;
  ES_WIFI_Conn_t conn;
  
  conn.Number = socket;
  conn.RemotePort = port;
  conn.LocalPort = local_port;
  conn.Type = (type == WIFI_TCP_PROTOCOL)? ES_WIFI_TCP_CONNECTION : ES_WIFI_UDP_CONNECTION;
  conn.RemoteIP[0] = location[0];
  conn.RemoteIP[1] = location[1];
  conn.RemoteIP[2] = location[2];
  conn.RemoteIP[3] = location[3];
  if(ES_WIFI_StartClientConnection(&EsWifiObj, &conn)== ES_WIFI_STATUS_OK)
  {
    ret = WIFI_STATUS_OK;
  }
  return ret;
}

/**
  * @brief  Close client connection
  * @param  type : Connection type TCP/UDP
  * @param  name : name of the connection
  * @param  location : Client address
  * @param  port : Remote port
  * @param  local_port : Local port
  * @retval Operation status
  */
WIFI_Status_t WIFI_CloseClientConnection(void)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;  
  if(ES_WIFI_StopClientConnection(&EsWifiObj, 0)== ES_WIFI_STATUS_OK)
  {
    ret = WIFI_STATUS_OK;
  }
  return ret; 
}

/**
  * @brief  Configure and start a Server
  * @param  type : Connection type TCP/UDP
  * @param  name : name of the connection
  * @param  port : Remote port
  * @retval Operation status
  */
WIFI_Status_t WIFI_StartServer(uint32_t socket, WIFI_Protocol_t protocol, const char* name, uint16_t port)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;
  ES_WIFI_Conn_t conn;
  conn.Number = socket;
  conn.LocalPort = port;
  conn.Type = (protocol == WIFI_TCP_PROTOCOL)? ES_WIFI_TCP_CONNECTION : ES_WIFI_UDP_CONNECTION;
  if(ES_WIFI_StartServerSingleConn(&EsWifiObj, &conn)== ES_WIFI_STATUS_OK)
  {
    ret = WIFI_STATUS_OK;
  }
  return ret;
}

/**
  * @brief  Stop a server
  * @retval Operation status
  */
WIFI_Status_t WIFI_StopServer(uint32_t socket)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;
  
  if(ES_WIFI_StopServerSingleConn(&EsWifiObj)== ES_WIFI_STATUS_OK)
  {
    ret = WIFI_STATUS_OK;
  }
  return ret;
}
/**
  * @brief  Send Data on a socket
  * @param  pdata : pointer to data to be sent
  * @param  len : length of data to be sent
  * @retval Operation status
  */
WIFI_Status_t WIFI_SendData(uint8_t socket, uint8_t *pdata, uint16_t Reqlen, uint16_t *SentDatalen)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR;

    if(ES_WIFI_SendData(&EsWifiObj, socket, pdata, Reqlen, SentDatalen, 10000) == ES_WIFI_STATUS_OK)
    {
      ret = WIFI_STATUS_OK;
    }

  return ret;
}

/**
  * @brief  Receive Data from a socket
  * @param  pdata : pointer to Rx buffer
  * @param  *len :  pointer to length of data
  * @retval Operation status
  */
WIFI_Status_t WIFI_ReceiveData(uint8_t socket, uint8_t *pdata, uint16_t Reqlen, uint16_t *RcvDatalen)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR; 

  if(ES_WIFI_ReceiveData(&EsWifiObj, socket, pdata, Reqlen, RcvDatalen, 10000) == ES_WIFI_STATUS_OK)
  {
    ret = WIFI_STATUS_OK; 
  }
  return ret;
}

/**
  * @brief  Customize module data
  * @param  name : MFC name
  * @param  Mac :  Mac Address
  * @retval Operation status
  */
WIFI_Status_t WIFI_SetOEMProperties(const char *name, uint8_t *Mac)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR; 
  
  if(ES_WIFI_SetProductName(&EsWifiObj, (uint8_t *)name) == ES_WIFI_STATUS_OK)
  {
    if(ES_WIFI_SetMACAddress(&EsWifiObj, Mac) == ES_WIFI_STATUS_OK)
    {
      ret = WIFI_STATUS_OK;
    }
  }
  return ret;
}

/**
  * @brief  Reset the WIFI module
  * @retval Operation status
  */
WIFI_Status_t WIFI_ResetModule(void)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR; 
  
  if(ES_WIFI_ResetModule(&EsWifiObj) == ES_WIFI_STATUS_OK)
  {
      ret = WIFI_STATUS_OK;
  }
  return ret;
}

/**
  * @brief  Restore module default configuration
  * @retval Operation status
  */
WIFI_Status_t WIFI_SetModuleDefault(void)
{
  WIFI_Status_t ret = WIFI_STATUS_ERROR; 
  
  if(ES_WIFI_ResetToFactoryDefault(&EsWifiObj) == ES_WIFI_STATUS_OK)
  {
      ret = WIFI_STATUS_OK;
  }
  return ret;
}


/**
  * @brief  Update module firmware
  * @param  location : Binary Location IP address
  * @retval Operation status
  */
WIFI_Status_t WIFI_ModuleFirmwareUpdate(const char *location)
{
  return WIFI_STATUS_NOT_SUPPORTED;
}
