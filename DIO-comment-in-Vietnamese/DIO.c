/******************************************************************************
 *  @file    Dio.c
 *  @brief   Định nghĩa các hàm điều khiển Đầu vào/Đầu ra số (DIO).
 *
 *  @details File này chứa phần triển khai các hàm điều khiển DIO, chịu trách 
 *           nhiệm cấu hình và điều khiển các chân số của vi điều khiển. 
 *           Module DIO cho phép đọc và ghi các tín hiệu số, tuân thủ tiêu chuẩn AUTOSAR.
 *  
 *  @version 1.0
 *  @date    2024-11-01
 *
 *  @section Tác giả
 *           Tống Xuân Hoàng
 *
 ******************************************************************************/

#include "Dio.h"

// #if (DIO_DEV_ERROR_DETECT == STD_ON)
// #include "Det.h"
// #endif

#include "stm32f10x_gpio.h"

/**
  * @brief  Đọc mức tín hiệu (CAO hoặc THẤP) của chân GPIO được chỉ định.
  * 
  * @param  ChannelId: ID duy nhất của chân GPIO, mã hóa cả thông tin về cổng (port) và chân (pin).
  *                    Mỗi nhóm 16 ChannelId tương ứng với một cổng:
  *                    - ChannelId từ 0-15: tương ứng các chân 0-15 của GPIOA
  *                    - ChannelId từ 16-31: tương ứng các chân 0-15 của GPIOB
  *                    - ChannelId từ 32-47: tương ứng các chân 0-15 của GPIOC
  *
  * @retval Dio_LevelType: Trả về mức tín hiệu hiện tại của chân GPIO chỉ định.
  *                    - STD_HIGH: Chân ở mức tín hiệu cao.
  *                    - STD_LOW: Chân ở mức tín hiệu thấp.
  *
  * @note   Nếu số cổng không hợp lệ, hàm sẽ trả về STD_LOW theo mặc định.
  * 
  */
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId) 
{
    GPIO_TypeDef* GPIOx;
    uint16 GPIO_Pin;
    Dio_LevelType level = STD_LOW;

    /* Extract port and pin from ChannelId */
    uint8 port = ChannelId / 16;     // Determine port number
    uint8 pin = ChannelId % 16;      // Determine pin within port

    /* Determine GPIOx based on port number */
    switch (port) 
	{
        case 0:
            GPIOx = GPIOA;
            break;
        case 1:
            GPIOx = GPIOB;
            break;
        case 2:
            GPIOx = GPIOC;
            break;
        default:
            return STD_LOW;  // Return LOW if port is invalid
    }

    /* Convert pin to SPL format */
    GPIO_Pin = (1 << pin);  // Shift to match SPL's GPIO_Pin_x format

    /* Read the pin level */
    if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == Bit_SET) 
	{
        level = STD_HIGH;
    } 
	else 
	{
        level = STD_LOW;
    }

    return level;
}

/**
  * @brief  Đặt mức tín hiệu (CAO hoặc THẤP) cho chân GPIO được chỉ định.
  * 
  * @param  ChannelId: ID duy nhất của chân GPIO, bao gồm thông tin về cổng và số chân:
  *                    - ChannelId từ 0-15: Tương ứng với các chân 0-15 của GPIOA.
  *                    - ChannelId từ 16-31: Tương ứng với các chân 0-15 của GPIOB.
  *                    - ChannelId từ 32-47: Tương ứng với các chân 0-15 của GPIOC.
  *
  * @param  Level: Mức tín hiệu cần đặt cho chân GPIO.
  *                - STD_HIGH: Đặt chân ở mức tín hiệu cao.
  *                - STD_LOW: Đặt chân ở mức tín hiệu thấp.
  *
  * @retval Không có.
  *
  * @note   Nếu `ChannelId` chứa cổng không hợp lệ, hàm sẽ thoát mà không thực hiện thay đổi nào.
  */
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;

    /* Convert ChannelId into port and pin */
    uint8 port = ChannelId / 16; // Calculate GPIO port from ChannelId
    uint8 pin = ChannelId % 16;  // Calculate pin within the port

    /* Determine GPIOx and GPIO_Pin based on port and pin */
    switch (port)
    {
    case 0:
        GPIOx = GPIOA;
        break;
    case 1:
        GPIOx = GPIOB;
        break;
    case 2:
        GPIOx = GPIOC;
        break;
    default:
        return; // Exit if port is invalid
    }

    /* Convert pin to SPL format */
    GPIO_Pin = (1 << pin);

    /* Write the signal level to the pin */
    if (Level == STD_HIGH)
    {
        GPIO_SetBits(GPIOx, GPIO_Pin);   // Set pin to high level
    }
    else
    {
        GPIO_ResetBits(GPIOx, GPIO_Pin); // Set pin to low level
    }
}

/**
  * @brief  Đọc mức tín hiệu của tất cả các chân trong cổng GPIO được chỉ định.
  * 
  * @param  PortId: ID duy nhất của cổng GPIO.
  *                 - 0: GPIOA
  *                 - 1: GPIOB
  *                 - 2: GPIOC
  *                 - Thêm 3,4,...: GPIOD...G, tùy thuộc vào cấu hình của MCU.
  *
  * @retval Dio_PortLevelType: Mức tín hiệu kết hợp của tất cả các chân trong cổng được chỉ định.
  *                 - Mỗi bit trong giá trị trả về tương ứng với mức tín hiệu của một chân trong cổng:
  *                   - Bit '1': CAO (chân ở mức tín hiệu cao).
  *                   - Bit '0': THẤP (chân ở mức tín hiệu thấp).
  *
  * @note   Nếu `PortId` không hợp lệ, hàm sẽ trả về 0.
  */
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId)
{
    GPIO_TypeDef* GPIOx;
    Dio_PortLevelType portLevel = 0;

    /* Determine GPIOx based on PortId */
    switch (PortId) 
    {
        case 0:
            GPIOx = GPIOA;
            break;
        case 1:
            GPIOx = GPIOB;
            break;
        case 2:
            GPIOx = GPIOC;
            break;
        default:
            return 0;  // Return 0 if PortId is invalid
    }

    /* Read the level of all pins in the port */
    portLevel = (Dio_PortLevelType)GPIO_ReadInputData(GPIOx);

    return portLevel;
}

/**
  * @brief  Ghi mức tín hiệu chỉ định lên tất cả các chân trong cổng GPIO được chỉ định.
  * 
  * @param  PortId: ID duy nhất của cổng GPIO.
  *                 - 0: GPIOA
  *                 - 1: GPIOB
  *                 - 2: GPIOC
  *                 - và tiếp tục tùy thuộc vào cấu hình của MCU.
  *
  * @param  Level: Mức tín hiệu cần ghi lên tất cả các chân trong cổng.
  *                - Mỗi bit trong `Level` tương ứng với mức tín hiệu mong muốn của một chân cụ thể trong cổng:
  *                  Bit '1' đặt chân ở mức CAO, và bit '0' đặt chân ở mức THẤP.
  *
  * @retval Không có giá trị trả về.
  *
  * @note   Nếu `PortId` không hợp lệ, hàm sẽ thoát mà không thay đổi trạng thái chân nào.
  */
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level)
{
    GPIO_TypeDef* GPIOx;

    /* Determine GPIOx based on PortId */
    switch (PortId) 
    {
        case 0:
            GPIOx = GPIOA;
            break;
        case 1:
            GPIOx = GPIOB;
            break;
        case 2:
            GPIOx = GPIOC;
            break;
        default:
            return; // Exit if PortId is invalid
    }

    /* Write the Level to the GPIO port */
    GPIO_Write(GPIOx, Level);
}

/**
  * @brief  Đọc mức tín hiệu của một nhóm chân cụ thể trong một cổng GPIO.
  * 
  * @param  ChannelGroupIdPtr: Con trỏ tới cấu trúc `Dio_ChannelGroupType` chứa thông tin
  *                             về cổng, mặt nạ, và độ lệch của nhóm chân.
  *                             - `port`: Số hiệu của cổng GPIO (ví dụ: 0 là GPIOA, 1 là GPIOB).
  *                             - `mask`: Mặt nạ bit xác định các chân cụ thể trong cổng.
  *                             - `offset`: Độ lệch bit chỉ vị trí của nhóm chân trong cổng.
  *
  * @retval Dio_PortLevelType: Mức tín hiệu của nhóm chân được chỉ định, sau khi áp dụng mặt nạ
  *                            và dịch theo độ lệch.
  *
  * @note   Nếu cổng được chỉ định trong `ChannelGroupIdPtr` không hợp lệ, hàm sẽ trả về 0.
  */
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr)
{
    GPIO_TypeDef* GPIOx;
    Dio_PortLevelType groupLevel = 0;

    /* Determine GPIOx based on ChannelGroupIdPtr->port */
    switch (ChannelGroupIdPtr->port)
    {
        case 0:
            GPIOx = GPIOA;
            break;
        case 1:
            GPIOx = GPIOB;
            break;
        case 2:
            GPIOx = GPIOC;
            break;
        // Add more cases if additional ports are needed
        default:
            return 0;  // Return 0 if the port is invalid
    }

    /* Read data from the GPIO port */
    uint16 portValue = GPIO_ReadInputData(GPIOx); // Read the full port value

    /* Apply mask and offset to retrieve the pin group value */
    groupLevel = (portValue & ChannelGroupIdPtr->mask) >> ChannelGroupIdPtr->offset;

    return groupLevel;
}

/**
  * @brief  Ghi mức tín hiệu chỉ định vào một nhóm các chân liên tiếp trong một cổng GPIO.
  * 
  * @param  ChannelGroupIdPtr: Con trỏ tới cấu trúc `Dio_ChannelGroupType`, chứa:
  *                            - `port`: ID của cổng GPIO (ví dụ: 0 cho GPIOA, 1 cho GPIOB).
  *                            - `mask`: Mặt nạ bit xác định các chân thuộc nhóm trong cổng.
  *                            - `offset`: Vị trí bit bắt đầu của nhóm chân trong cổng.
  *
  * @param  Level: Mức tín hiệu cần ghi vào nhóm chân được chỉ định. Mỗi bit trong `Level` 
  *                tương ứng với một chân trong nhóm, được căn chỉnh theo `offset`.
  *
  * @retval Không có giá trị trả về.
  *
  * @note   Hàm này chỉ cập nhật mức tín hiệu cho nhóm các chân liên tiếp trong cổng, 
  *         không ảnh hưởng đến các chân khác ngoài nhóm.
  */
void Dio_WriteChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr, Dio_PortLevelType Level)
{
    GPIO_TypeDef* GPIOx;

    /* Determine GPIOx based on ChannelGroupIdPtr->port */
    switch (ChannelGroupIdPtr->port)
    {
        case 0:
            GPIOx = GPIOA;
            break;
        case 1:
            GPIOx = GPIOB;
            break;
        case 2:
            GPIOx = GPIOC;
            break;
        // Add additional cases if there are more ports
        default:
            return;  // Exit if port is invalid
    }

    /* Read the current value of the GPIO port */
    uint32_t portValue = GPIO_ReadOutputData(GPIOx);

    /* Clear the bits of the specified group in the port */
    portValue &= ~(ChannelGroupIdPtr->mask); // Clear bits in the group using the mask

    /* Set the new level for the group of pins */
    portValue |= ((Level << ChannelGroupIdPtr->offset) & ChannelGroupIdPtr->mask); // Shift Level by offset and apply mask

    /* Write the updated value back to the GPIO port */
    GPIO_Write(GPIOx, (uint16_t)portValue);
}

/**
  * @brief  Lấy thông tin phiên bản của module DIO.
  * 
  * @param  versioninfo: Con trỏ tới cấu trúc `Std_VersionInfoType` nơi thông tin phiên bản
  *                      sẽ được lưu trữ.
  *                      - `vendorID`: ID nhà cung cấp của module DIO.
  *                      - `moduleID`: ID của module DIO.
  *                      - `sw_major_version`: Số phiên bản chính của phần mềm.
  *                      - `sw_minor_version`: Số phiên bản phụ của phần mềm.
  *                      - `sw_patch_version`: Số phiên bản vá lỗi của phần mềm.
  *
  * @retval Không có giá trị trả về.
  *
  * @note   Nếu `versioninfo` là một con trỏ null, hàm sẽ không thực hiện hành động nào.
  *         Đảm bảo rằng `versioninfo` hợp lệ trước khi gọi hàm này.
  */
#define NULL_PTR          ((void *)0)
void Dio_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
	if (versioninfo != NULL_PTR)
    {
		/* Copy the vendor Id */
		versioninfo->vendorID = (uint16)DIO_VENDOR_ID;
		/* Copy the module Id */
		versioninfo->moduleID = (uint16)DIO_MODULE_ID;
		/* Copy Software Major Version */
		versioninfo->sw_major_version = (uint8)DIO_SW_MAJOR_VERSION;
		/* Copy Software Minor Version */
		versioninfo->sw_minor_version = (uint8)DIO_SW_MINOR_VERSION;
		/* Copy Software Patch Version */
		versioninfo->sw_patch_version = (uint8)DIO_SW_PATCH_VERSION;
	}
}

/**
  * @brief  Đảo trạng thái hiện tại của kênh DIO được chỉ định.
  *         Nếu kênh hiện tại đang ở mức CAO, nó sẽ được đặt về mức THẤP và ngược lại.
  * 
  * @param  ChannelId: Định danh duy nhất của kênh DIO cần đảo trạng thái.
  *
  * @retval Dio_LevelType: Trạng thái mới của kênh sau khi được đảo.
  *                        - `STD_HIGH`: Kênh hiện được đặt ở mức CAO.
  *                        - `STD_LOW`: Kênh hiện được đặt ở mức THẤP.
  *
  * @note   Hàm này đọc trạng thái hiện tại của kênh DIO được chỉ định, đảo trạng thái,
  *         ghi lại trạng thái mới vào kênh và trả về trạng thái mới để sử dụng sau này. 
  *         Thao tác này không ảnh hưởng đến các kênh khác.
  */
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId)
{
    Dio_LevelType currentLevel;

    /* Read the current level of the channel */
    currentLevel = Dio_ReadChannel(ChannelId);

    /* Determine the new level by flipping the current level */
    Dio_LevelType newLevel = (currentLevel == STD_HIGH) ? STD_LOW : STD_HIGH;

    /* Write the new level to the channel */
    Dio_WriteChannel(ChannelId, newLevel);

    /* Return the new level */
    return newLevel;
}
