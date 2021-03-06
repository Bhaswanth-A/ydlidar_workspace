/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2018, EAIBOT, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/** @page YDlidarDriver
 * YDlidarDriver API
    <table>
        <tr><th>Library     <td>YDlidarDriver
        <tr><th>File        <td>ydlidar_driver.h
        <tr><th>Author      <td>Tony [code at ydlidar com]
        <tr><th>Source      <td>https://github.com/ydlidar/YDLidar-SDK
        <tr><th>Version     <td>1.0.0
    </table>
    This YDlidarDriver support [TYPE_TRIANGLE](\ref LidarTypeID::TYPE_TRIANGLE) and [TYPE_TOF](\ref LidarTypeID::TYPE_TOF) LiDAR

* @copyright    Copyright (c) 2018-2020  EAIBOT
     Jump to the @link ::ydlidar::YDlidarDriver @endlink interface documentation.
*/
#ifndef YDLIDAR_DRIVER_H
#define YDLIDAR_DRIVER_H
#include <stdlib.h>
#include <atomic>
#include <map>
#include "serial.h"
#include "locker.h"
#include "thread.h"
#include "ydlidar_protocol.h"
#include "help_info.h"

#if !defined(__cplusplus)
#ifndef __cplusplus
#error "The YDLIDAR SDK requires a C++ compiler to be built"
#endif
#endif


using namespace std;
using namespace serial;

namespace ydlidar {

/*!
* Class that provides a lidar interface.
*/
class YDlidarDriver {
 public:
  /**
    * @brief Set and Get LiDAR single channel.
    * Whether LiDAR communication channel is a single-channel
    * @note For a single-channel LiDAR, if the settings are reversed.\n
    * an error will occur in obtaining device information and the LiDAR will Faied to Start.\n
    * For dual-channel LiDAR, if th setttings are reversed.\n
    * the device information cannot be obtained.\n
    * Set the single channel to match the LiDAR.
    * @remarks
    <table>
         <tr><th>G1/G2/G2A/G2C                          <td>false
         <tr><th>G4/G4B/G4PRO/G6/F4/F4PRO               <td>false
         <tr><th>S4/S4B/X4/R2/G4C                       <td>false
         <tr><th>S2/X2/X2L                              <td>true
         <tr><th>TG15/TG30/TG50                         <td>false
         <tr><th>TX8/TX20                               <td>true
         <tr><th>T5/T15                                 <td>false
         <tr><th>                                       <td>true
     </table>
    * @see DriverInterface::setSingleChannel and DriverInterface::getSingleChannel
    */
  PropertyBuilderByName(bool, SingleChannel, private);
  /**
  * @brief Set and Get LiDAR Type.
  * @note Refer to the table below for the LiDAR Type.\n
  * Set the LiDAR Type to match the LiDAR.
  * @remarks
  <table>
       <tr><th>G1/G2A/G2/G2C                    <td>[TYPE_TRIANGLE](\ref LidarTypeID::TYPE_TRIANGLE)
       <tr><th>G4/G4B/G4C/G4PRO                 <td>[TYPE_TRIANGLE](\ref LidarTypeID::TYPE_TRIANGLE)
       <tr><th>G6/F4/F4PRO                      <td>[TYPE_TRIANGLE](\ref LidarTypeID::TYPE_TRIANGLE)
       <tr><th>S4/S4B/X4/R2/S2/X2/X2L           <td>[TYPE_TRIANGLE](\ref LidarTypeID::TYPE_TRIANGLE)
       <tr><th>TG15/TG30/TG50/TX8/TX20          <td>[TYPE_TOF](\ref LidarTypeID::TYPE_TOF)
       <tr><th>T5/T15                           <td>[TYPE_TOF_NET](\ref LidarTypeID::TYPE_TOF_NET)
   </table>
  * @see [LidarTypeID](\ref LidarTypeID)
  * @see DriverInterface::setLidarType and DriverInterface::getLidarType
  */
  PropertyBuilderByName(int, LidarType, private);
  /**
  * @brief Set and Get Sampling interval.
  * @note Negative correlation between sampling interval and lidar sampling rate.\n
  * sampling interval = 1e9 / sampling rate(/s)\n
  * Set the LiDAR sampling interval to match the LiDAR.
  * @see DriverInterface::setPointTime and DriverInterface::getPointTime
  */
  PropertyBuilderByName(uint32_t, PointTime,private);
  /*!
  * A constructor.
  * A more elaborate description of the constructor.
  */
  YDlidarDriver();

  /*!
  * A destructor.
  * A more elaborate description of the destructor.
  */
  virtual ~YDlidarDriver();

  /*!
  * @brief ???????????? \n
  * ??????????????????????????????::disconnect????????????
  * @param[in] port_path    ?????????
  * @param[in] baudrate    ????????????YDLIDAR-SS??????????????????
  *     230400 G2-SS-1
  * @return ??????????????????
  * @retval 0     ??????
  * @retval < 0   ??????
  * @note??????????????????????????????::disconnect????????????
  * @see ??????::YDlidarDriver::disconnect (???::???????????????????????????,?????????????????????disconnect?????????,????????????????????????disconnect.)
  */
  result_t connect(const char *port_path, uint32_t baudrate);

  /*!
  * @brief ??????????????????
  */
  void disconnect();

  /*!
  * @brief ????????????SDK????????? \n
  * ????????????
  * @return ????????????SKD ?????????
  */
  static std::string getSDKVersion();

  /*!
  * @brief lidarPortList ??????????????????
  * @return ??????????????????
  */
  static std::map<std::string, std::string> lidarPortList();


  /*!
  * @brief ???????????? \n
  * @return ??????????????????????????????
  * @retval true     ????????????
  * @retval false    ????????????
  */
  bool isscanning() const;

  /*!
  * @brief ?????????????????? \n
  * @return ??????????????????
  * @retval true     ??????
  * @retval false    ??????
  */
  bool isconnected() const;

  /*!
  * @brief ????????????????????????????????? \n
  * ??????????????????????????????::disconnect????????????
  * @param[in] isintensities    ?????????????????????:
  *     true	???????????????
  *	  false ???????????????
  * @note??????S4B(????????????153600)???????????????????????????, ??????????????????????????????
  */
  void setIntensities(const bool &isintensities);

  /*!
  * @brief ???????????????????????????????????? \n
  * @param[in] enable    ????????????????????????:
  *     true	??????
  *	  false ??????
  */
  void setAutoReconnect(const bool &enable);

  /*!
  * @brief ?????????????????????????????? \n
  * @return ??????????????????
  * @retval RESULT_OK       ????????????
  * @retval RESULT_FAILE or RESULT_TIMEOUT   ????????????
  */
  result_t getHealth(device_health &health, uint32_t timeout = DEFAULT_TIMEOUT);

  /*!
  * @brief ???????????????????????? \n
  * @param[in] info     ????????????
  * @param[in] timeout  ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ????????????
  * @retval RESULT_FAILE or RESULT_TIMEOUT   ????????????
  */
  result_t getDeviceInfo(device_info &info, uint32_t timeout = DEFAULT_TIMEOUT);

  /*!
  * @brief ???????????? \n
  * @param[in] force    ????????????
  * @param[in] timeout  ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ????????????
  * @retval RESULT_FAILE    ????????????
  * @note ??????????????????????????????
  */
  result_t startScan(bool force = false, uint32_t timeout = DEFAULT_TIMEOUT) ;

  /*!
  * @brief ???????????? \n
  * @return ??????????????????
  * @retval RESULT_OK       ????????????
  * @retval RESULT_FAILE    ????????????
  */
  result_t stop();


  /*!
  * @brief ?????????????????? \n
  * @param[in] nodebuffer ???????????????
  * @param[in] count      ??????????????????
  * @param[in] timeout    ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ????????????
  * @retval RESULT_FAILE    ????????????
  * @note ???????????????????????????::startScan??????????????????
  */
  result_t grabScanData(node_info *nodebuffer, size_t &count,
                        uint32_t timeout = DEFAULT_TIMEOUT) ;


  /*!
  * @brief ?????????????????? \n
  * ??????????????????0???360?????????
  * @param[in] nodebuffer ???????????????
  * @param[in] count      ??????????????????
  * @return ??????????????????
  * @retval RESULT_OK       ??????
  * @retval RESULT_FAILE    ??????
  * @note ???????????????????????????::grabScanData??????????????????????????????
  */
  result_t ascendScanData(node_info *nodebuffer, size_t count);

  /*!
  * @brief ?????????????????? \n
  * @param[in] timeout      ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ??????
  * @retval RESULT_FAILE    ??????
  * @note ????????????????????????????????????, ????????????????????????::stop??????????????????
  */
  result_t reset(uint32_t timeout = DEFAULT_TIMEOUT);

  /*!
  * @brief ???????????? \n
    * @return ??????????????????
    * @retval RESULT_OK       ??????
    * @retval RESULT_FAILE    ??????
    */
  result_t startMotor();

  /*!
  * @brief ???????????? \n
    * @return ??????????????????
    * @retval RESULT_OK       ??????
    * @retval RESULT_FAILE    ??????
    */
  result_t stopMotor();

  /*!
  * @brief ???????????????????????????????????? \n
  * @param[in] frequency    ????????????
  * @param[in] timeout      ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ??????
  * @retval RESULT_FAILE    ??????
  * @note ????????????????????????????????????
  */
  result_t getScanFrequency(scan_frequency &frequency,
                            uint32_t timeout = DEFAULT_TIMEOUT);

  /*!
  * @brief ????????????????????????1HZ \n
  * @param[in] frequency    ????????????
  * @param[in] timeout      ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ??????
  * @retval RESULT_FAILE    ??????
  * @note ????????????????????????????????????
  */
  result_t setScanFrequencyAdd(scan_frequency &frequency,
                               uint32_t timeout = DEFAULT_TIMEOUT);

  /*!
  * @brief ????????????????????????1HZ \n
  * @param[in] frequency    ????????????
  * @param[in] timeout      ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ??????
  * @retval RESULT_FAILE    ??????
  * @note ????????????????????????????????????
  */
  result_t setScanFrequencyDis(scan_frequency &frequency,
                               uint32_t timeout = DEFAULT_TIMEOUT);

  /*!
  * @brief ????????????????????????0.1HZ \n
  * @param[in] frequency    ????????????
  * @param[in] timeout      ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ??????
  * @retval RESULT_FAILE    ??????
  * @note ????????????????????????????????????
  */
  result_t setScanFrequencyAddMic(scan_frequency &frequency,
                                  uint32_t timeout = DEFAULT_TIMEOUT);

  /*!
  * @brief ????????????????????????0.1HZ \n
  * @param[in] frequency    ????????????
  * @param[in] timeout      ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ??????
  * @retval RESULT_FAILE    ??????
  * @note ????????????????????????????????????
  */
  result_t setScanFrequencyDisMic(scan_frequency &frequency,
                                  uint32_t timeout = DEFAULT_TIMEOUT);

  /*!
  * @brief ???????????????????????????????????? \n
  * @param[in] frequency    ????????????
  * @param[in] timeout      ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ??????
  * @retval RESULT_FAILE    ??????
  * @note ????????????????????????????????????
  */
  result_t getSamplingRate(sampling_rate &rate,
                           uint32_t timeout = DEFAULT_TIMEOUT);

  /*!
  * @brief ???????????????????????????????????? \n
  * @param[in] rate    ?????????????????????
  * @param[in] timeout      ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ??????
  * @retval RESULT_FAILE    ??????
  * @note ????????????????????????????????????
  */
  result_t setSamplingRate(sampling_rate &rate,
                           uint32_t timeout = DEFAULT_TIMEOUT);

  /*!
  * @brief ????????????????????????????????? \n
  * @param[in] angle?????????   ???????????????
  * @param[in] timeout      ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ??????
  * @retval RESULT_FAILE    ??????
  * @note ????????????????????????????????????
  */
  result_t getZeroOffsetAngle(offset_angle &angle,
                              uint32_t timeout = DEFAULT_TIMEOUT);

 protected:

  /*!
  * @brief ?????????????????????????????? \n
  * @note ???????????????????????????????????????????????????::startScan????????????????????????
  */
  result_t createThread();


  /*!
  * @brief ???????????????????????? \n
  * @param[in] force    ????????????
  * @param[in] timeout  ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ????????????
  * @retval RESULT_FAILE    ????????????
  * @note sdk ????????????????????????
  */
  result_t startAutoScan(bool force = false, uint32_t timeout = DEFAULT_TIMEOUT) ;

  /*!
  * @brief stopScan
  * @param timeout
  * @return
  */
  result_t stopScan(uint32_t timeout = DEFAULT_TIMEOUT);

  /*!
     * @brief checkDeviceStatus
     * @param byte
     * @return
     */
  result_t checkDeviceInfo(uint8_t *recvBuffer, uint8_t byte, int recvPos,
                           int recvSize, int pos);

  /*!
   * @brief waitDevicePackage
   * @param timeout
   * @return
   */
  result_t waitDevicePackage(uint32_t timeout = DEFAULT_TIMEOUT);
  /*!
  * @brief ?????????????????? \n
  * @param[in] node ????????????????????????
  * @param[in] timeout     ????????????
  */
  result_t waitPackage(node_info *node, uint32_t timeout = DEFAULT_TIMEOUT);

  /*!
  * @brief ????????????????????? \n
  * @param[in] nodebuffer ??????????????????
  * @param[in] count      ??????????????????
  * @param[in] timeout      ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ??????
  * @retval RESULT_TIMEOUT  ????????????
  * @retval RESULT_FAILE    ??????
  */
  result_t waitScanData(node_info *nodebuffer, size_t &count,
                        uint32_t timeout = DEFAULT_TIMEOUT);

  /*!
  * @brief ???????????????????????? \n
  */
  int cacheScanData();

  /*!
  * @brief ????????????????????? \n
  * @param[in] cmd 	 ?????????
  * @param[in] payload      payload
  * @param[in] payloadsize      payloadsize
  * @return ??????????????????
  * @retval RESULT_OK       ??????
  * @retval RESULT_FAILE    ??????
  */
  result_t sendCommand(uint8_t cmd, const void *payload = NULL,
                       size_t payloadsize = 0);

  /*!
  * @brief ???????????????????????? \n
  * @param[in] header 	 ??????
  * @param[in] timeout      ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ????????????
  * @retval RESULT_TIMEOUT  ????????????
  * @retval RESULT_FAILE    ????????????
  * @note ???timeout = -1 ???, ???????????????
  */
  result_t waitResponseHeader(lidar_ans_header *header,
                              uint32_t timeout = DEFAULT_TIMEOUT);

  /*!
  * @brief ?????????????????????????????? \n
  * @param[in] data_count 	 ??????????????????
  * @param[in] timeout    	 ????????????
  * @param[in] returned_size   ??????????????????
  * @return ??????????????????
  * @retval RESULT_OK       ????????????
  * @retval RESULT_TIMEOUT  ????????????
  * @retval RESULT_FAILE    ????????????
  * @note ???timeout = -1 ???, ???????????????
  */
  result_t waitForData(size_t data_count, uint32_t timeout = DEFAULT_TIMEOUT,
                       size_t *returned_size = NULL);

  /*!
  * @brief ?????????????????? \n
  * @param[in] data 	 ????????????
  * @param[in] size    ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ????????????
  * @retval RESULT_FAILE    ????????????
  */
  result_t getData(uint8_t *data, size_t size);

  /*!
  * @brief ?????????????????? \n
  * @param[in] data 	 ??????????????????
  * @param[in] size    ????????????
  * @return ??????????????????
  * @retval RESULT_OK       ????????????
  * @retval RESULT_FAILE    ????????????
  */
  result_t sendData(const uint8_t *data, size_t size);


  /*!
  * @brief checkTransDelay
  */
  void checkTransDelay();

  /*!
  * @brief ???????????????????????? \n
  */
  void disableDataGrabbing();

  /*!
  * @brief ????????????DTR \n
  */
  void setDTR();

  /*!
  * @brief ????????????DTR \n
  */
  void clearDTR();

  /*!
   * @brief flushSerial
   */
  void flushSerial();

  /*!
   * @brief checkAutoConnecting
   */
  result_t checkAutoConnecting();


 public:
  std::atomic<bool>     isConnected;  ///< ??????????????????
  std::atomic<bool>     isScanning;   ///< ????????????
  std::atomic<bool>     isAutoReconnect;  ///< ????????????????????????
  std::atomic<bool>     isAutoconnting;  ///< ???????????????????????????


  enum {
    DEFAULT_TIMEOUT = 2000,    /**< ??????????????????. */
    DEFAULT_HEART_BEAT = 1000, /**< ??????????????????????????????. */
    MAX_SCAN_NODES = 3600,	   /**< ??????????????????. */
    DEFAULT_TIMEOUT_COUNT = 1,
  };

  node_info      *scan_node_buf;    ///< ???????????????
  size_t         scan_node_count;   ///< ????????????
  Event          _dataEvent;        ///< ??????????????????
  Locker         _lock;				///< ?????????
  Locker         _serial_lock;		///< ?????????
  Thread 	     _thread;		   ///< ??????id

 private:
  int PackageSampleBytes;            ///< ??????????????????????????????
  serial::Serial *_serial;			///< ??????
  bool m_intensities;				///< ??????????????????
  uint32_t m_baudrate;				///< ?????????
  bool isSupportMotorDtrCtrl;	    ///< ????????????????????????
  uint32_t trans_delay;				///< ??????????????????byte??????
  int m_sampling_rate;              ///< ????????????
  int model;                        ///< ????????????
  int sample_rate;                  ///<

  node_package package;             ///< ????????????????????????
  node_packages packages;           ///< ???????????????????????????

  uint16_t package_Sample_Index;    ///< ??????????????????
  float IntervalSampleAngle;
  float IntervalSampleAngle_LastPackage;
  uint16_t FirstSampleAngle;        ///< ???????????????
  uint16_t LastSampleAngle;         ///< ???????????????
  uint16_t CheckSum;                ///< ?????????
  uint8_t scan_frequence;           ///< ?????????????????????

  uint16_t CheckSumCal;
  uint16_t SampleNumlAndCTCal;
  uint16_t LastSampleAngleCal;
  bool CheckSumResult;
  uint16_t Valu8Tou16;

  std::string serial_port;///< ????????????
  uint8_t *globalRecvBuffer;
  int retryCount;
  bool has_device_header;
  uint8_t last_device_byte;
  int         asyncRecvPos;
  uint16_t    async_size;

  //singleChannel
  device_info info_;
  device_health health_;
  lidar_ans_header header_;
  uint8_t  *headerBuffer;
  uint8_t  *infoBuffer;
  uint8_t  *healthBuffer;
  bool     get_device_info_success;
  bool     get_device_health_success;

  int package_index;
  bool has_package_error;

};

}// namespace ydlidar

#endif // YDLIDAR_DRIVER_H
