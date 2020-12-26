# Имя проекта
#-------------------------------------------------------------------------------
TARGET  = template

# Используемые модули библиотеки периферии
#-------------------------------------------------------------------------------
# PERIPHDRIVERS += stm32f10x_adc
# PERIPHDRIVERS += stm32f10x_bkp
# PERIPHDRIVERS += stm32f10x_can
# PERIPHDRIVERS += stm32f10x_cec
# PERIPHDRIVERS += stm32f10x_crc
# PERIPHDRIVERS += stm32f10x_dbgmcu
# PERIPHDRIVERS += stm32f10x_exti
# PERIPHDRIVERS += stm32f10x_flash
# PERIPHDRIVERS += stm32f10x_fsmc
# PERIPHDRIVERS += stm32f10x_gpio
# PERIPHDRIVERS += stm32f10x_i2c
# PERIPHDRIVERS += stm32f10x_iwdg
# PERIPHDRIVERS += stm32f10x_pwr
# PERIPHDRIVERS += stm32f10x_rcc
# PERIPHDRIVERS += stm32f10x_rtc
# PERIPHDRIVERS += stm32f10x_sdio
# PERIPHDRIVERS += stm32f10x_spi
# PERIPHDRIVERS += stm32f10x_tim
# PERIPHDRIVERS += stm32f10x_usart
# PERIPHDRIVERS += stm32f10x_wwdg
# PERIPHDRIVERS += misc.c
PERIPHDRIVERS +=

# LIB_RCC_SRC		= X:/GDrive/AtollicTS_projects/Libraries_v.1.0/RCC/source/
# LIB_RCC_INC		= X:/GDrive/AtollicTS_projects/Libraries_v.1.0/RCC/include/

# LIB_GPIO_SRC 	= X:/GDrive/AtollicTS_projects/Libraries_v.1.0/GPIO/source/
# LIB_GPIO_INC 	= X:/GDrive/AtollicTS_projects/Libraries_v.1.0/GPIO/include/

# LIB_DELAY_SRC 	= X:/GDrive/AtollicTS_projects/Libraries_v.1.0/Delay/source
# LIB_DELAY_INC 	= X:/GDrive/AtollicTS_projects/Libraries_v.1.0/Delay/include

# LIB_SYS_DEFINES_INC = X:/GDrive/AtollicTS_projects/Libraries_v.1.0/System_defines

# Дефайны
#-------------------------------------------------------------------------------
#DEFINES += USE_STDPERIPH_DRIVER
DEFINES += STM32L431xx

#DEFINES += GCC_ARMCM3
#DEFINES += VECT_TAB_FLASH

# Инструменты
#-------------------------------------------------------------------------------
AS = X:/AtollicTS/TrueSTUDIO_for_STM32_9_3_0/ARMTools/bin/arm-atollic-eabi-gcc -c
CC = X:/AtollicTS/TrueSTUDIO_for_STM32_9_3_0/ARMTools/bin/arm-atollic-eabi-gcc -c
LD = X:/AtollicTS/TrueSTUDIO_for_STM32_9_3_0/ARMTools/bin/arm-atollic-eabi-gcc
CP = X:/AtollicTS/TrueSTUDIO_for_STM32_9_3_0/ARMTools/bin/arm-atollic-eabi-objcopy
SZ = X:/AtollicTS/TrueSTUDIO_for_STM32_9_3_0/ARMTools/bin/arm-atollic-eabi-size
RM = rm

PROJECT_DIR =  X:\GDrive\VSCode_projects\STM32_LoRa
DEBUG_PATH := $(PROJECT_DIR)\Debug
# Пути к CMSIS, StdPeriph Lib
#-------------------------------------------------------------------------------
CMSIS_INC_PATH     = $(PROJECT_DIR)\CMSIS_L431\include
CMSIS_SRC_PATH     = $(PROJECT_DIR)\CMSIS_L431\src

PERIPH_SRC_PATH	   = $(PROJECT_DIR)\Periph\source
PERIPH_INC_PATH    = $(PROJECT_DIR)\Periph\inc

CORTEX_SRC_PATH    = $(PROJECT_DIR)\Cortex\source
CORTEX_INC_PATH    = $(PROJECT_DIR)\Cortex\inc

MIDDLEWARE_SRC_PATH=$(PROJECT_DIR)\Middleware\source
MIDDLEWARE_INC_PATH=$(PROJECT_DIR)\Middleware\inc

EXTDEV_SRC_PATH    =$(PROJECT_DIR)\ExternalDevices\source
EXTDEV_INC_PATH    =$(PROJECT_DIR)\ExternalDevices\inc

LIBC_PATH 		   = X:\arm_none_eabi\10_2020-q4-major\arm-none-eabi\lib
#STDPERIPH_INC_PATH = stdperiph/inc
#STDPERIPH_SRC_PATH = stdperiph/src

# startup файл
#-------------------------------------------------------------------------------
STARTUP = $(PROJECT_DIR)\CMSIS_L431\Startup\startup_stm32l431rbtx.s

# Пути поиска исходных файлов
#-------------------------------------------------------------------------------
HEADERS := $(PROJECT_DIR)\Main\inc
HEADERS += $(CMSIS_INC_PATH)
HEADERS += $(PERIPH_INC_PATH)
HEADERS += $(CORTEX_INC_PATH)
HEADERS += $(MIDDLEWARE_INC_PATH)
HEADERS += $(EXTDEV_INC_PATH)

SOURCEDIRS := $(PROJECT_DIR)\Main\code
SOURCEDIRS += $(CMSIS_SRC_PATH)
SOURCEDIRS += $(PERIPH_SRC_PATH)
SOURCEDIRS += $(CORTEX_SRC_PATH)
SOURCEDIRS += $(MIDDLEWARE_SRC_PATH)
SOURCEDIRS += $(EXTDEV_SRC_PATH)
# SOURCEDIRS += $(LIB_GPIO_SRC)
# SOURCEDIRS += $(LIB_DELAY_SRC)
# SOURCEDIRS += $(LIB_RCC_SRC)


# Пути поиска хидеров
#-------------------------------------------------------------------------------
INCLUDES += .
INCLUDES += $(HEADERS) 
INCLUDES += $(PROJECT_DIR)\CMSIS_L431\Startup
INCLUDES += $(SOURCEDIRS) 
INCLUDES += $(CMSIS_INC_PATH)
INCLUDES += $(PERIPH_SRC_PATH)
INCLUDES += $(CORTEX_SRC_PATH)
INCLUDES += $(MIDDLEWARE_SRC_PATH)
INCLUDES += $(LIBC_PATH)
#INCLUDES += $(STDPERIPH_INC_PATH)
# INCLUDES += $(LIB_GPIO_INC) 
# INCLUDES += $(LIB_DELAY_INC) 
# INCLUDES += $(LIB_RCC_INC) 
# INCLUDES += $(LIB_SYS_DEFINES_INC) 

# Библиотеки
#-------------------------------------------------------------------------------
LIBPATH += $(LIBC_PATH)
LIBS    += 

# Настройки компилятора
#-------------------------------------------------------------------------------
CFLAGS += -mthumb -mcpu=cortex-m4 -std=gnu11 -O1 -g -fstack-usage -Wall -specs=nano.specs
#CFLAGS += -I"X:\GDrive\AtollicTS_projects\Libraries_v.1.0\Delay\include" -I"X:\GDrive\AtollicTS_projects\Libraries_v.1.0\GPIO\include" -I"X:\GDrive\AtollicTS_projects\Libraries_v.1.0\System_defines" -I"X:\GDrive\AtollicTS_projects\Libraries_v.1.0\ST7735\include" -I"X:\GDrive\AtollicTS_projects\Libraries_v.1.0\RCC\include" -I"X:\GDrive\AtollicTS_projects\Libraries_v.1.0\SPI\include" -I"X:\GDrive\AtollicTS_projects\TFT_test\CMSIS\include"
CFLAGS += $(addprefix -I, $(INCLUDES))
CFLAGS += $(addprefix -D, $(DEFINES))

# Скрипт линкера
#-------------------------------------------------------------------------------
LDSCR_PATH = $(PROJECT_DIR)\CMSIS_L431\Startup
LDSCRIPT   = Debug_STM32L431RB_FLASH.ld

# Настройки линкера
#-------------------------------------------------------------------------------
# LDFLAGS += -nostartfiles
# LDFLAGS += -sysroot=X:\arm_none_eabi\10_2020-q4-major\arm-none-eabi\lib
#LDFLAGS += -nostartfiles -specs=nosys.specs -specs=nano.specs -nostdlib -gc-sections -mthumb -mcpu=cortex-m3
#LDFLAGS += -Map $(TARGET).map
# -static -Wl,-cref,-u,Reset_Handler "-Wl,-Map=${TARGET}.map" -Wl,--defsym=malloc_getpagesize_P=0x80 -Wl,--start-group -lc -lm -Wl,--end-group -specs=nosys.specs 
LDFLAGS += -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -march=armv7e-m -lc -Wl,--print-memory-usage 
LDFLAGS += -specs=nano.specs
LDFLAGS += -specs=nosys.specs
# LDFLAGS += -L$(LDSCR_PATH)
LDFLAGS += -T$(LDSCR_PATH)/$(LDSCRIPT)
LDFLAGS += $(addprefix -L, $(LIBPATH))
LDFLAGS += $(LIBS)

# Настройки ассемблера
#-------------------------------------------------------------------------------
AFLAGS += -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -g -Wa,--warn -x assembler-with-cpp -specs=nano.specs
#-ahls -mapcs-32

# Список объектных файлов
#-------------------------------------------------------------------------------
OBJS += $(patsubst %.c, %.o, $(wildcard  $(addsuffix /*.c, $(SOURCEDIRS))))
OBJS += $(patsubst %.s, %.o, $(STARTUP))  # abs/path/file_name.ext
OBJ_FILES := $(notdir $(OBJS))  # file_name.ext
OBJS := $(addprefix $(DEBUG_PATH)/src/, $(OBJ_FILES))



# Define dependencies files for all objects
DEPS = $(OBJS:.o=.d)
#-------------------------------------------------

# Пути поиска make
#-------------------------------------------------------------------------------
VPATH += $(PROJECT_DIR)\CMSIS_L431\Startup
VPATH += $(SOURCEDIRS)



# Список файлов к удалению командой "make clean"
#-------------------------------------------------------------------------------
# TOREMOVE += $(addprefix $(DEBUG_PATH)/, $(TARGET).elf)
# TOREMOVE += $(addprefix $(DEBUG_PATH)/, $(TARGET).hex)
# TOREMOVE += $(addprefix $(DEBUG_PATH)/src/, $(OBJ_FILES))
# TOREMOVE += $(patsubst %.o, %.su, $(addprefix $(DEBUG_PATH)/src/, $(OBJ_FILES)))
# TOREMOVE += $(patsubst %.o, %.d, $(addprefix $(DEBUG_PATH)/src/, $(OBJ_FILES)))


# Собрать все
#-------------------------------------------------------------------------------
all: debug_dir $(DEBUG_PATH)/$(TARGET).hex size

debug_dir:
		if not exist "$(DEBUG_PATH)" mkdir $(DEBUG_PATH)
		if not exist "$(DEBUG_PATH)\src" mkdir $(DEBUG_PATH)\src

# Очистка
#-------------------------------------------------------------------------------
clean:
		@echo "---------------------------------------------------" 
		@echo $(OBJS)
		@echo "---------------------------------------------------" 
		@rmdir /Q /S $(DEBUG_PATH)
# @$(RM) -f $(TOREMOVE))  

# Создание .hex файла
#-------------------------------------------------------------------------------
$(DEBUG_PATH)/$(TARGET).hex: $(DEBUG_PATH)/$(TARGET).elf
		@$(CP) -Oihex $(DEBUG_PATH)/$(TARGET).elf $(DEBUG_PATH)/$(TARGET).hex
        



# Показываем размер
#-------------------------------------------------------------------------------
size:
		@echo "---------------------------------------------------"
		@$(SZ) $(DEBUG_PATH)/$(TARGET).elf

# Линковка
#------------------------------------------------------------------------------- 
$(DEBUG_PATH)/$(TARGET).elf: $(OBJS)
		@echo "---------------------------------------------------"
		@echo "Linking: $(LDFLAGS)"
		@$(LD) $(LDFLAGS) $^ -o $@
		@echo "==================================================="

# Include dependencies
# -include $(DEPS)



# Компиляция
#------------------------------------------------------------------------------- 
$(DEBUG_PATH)/src/%.o: %.c
		@echo Compiling: $<
		@$(CC) $(CFLAGS) -Wall -MD -c $< -o $@ 

$(DEBUG_PATH)/src/%.o: %.s
		@$(AS) $(AFLAGS) -c $< -o $@


        

# Сгенерированные gcc зависимости
#-------------------------------------------------------------------------------
# include $(wildcart *.d)