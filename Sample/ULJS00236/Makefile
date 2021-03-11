#############################
#    ここからが設定項目     #
#############################

# ゲームIDを指定 ( 例 : NARUTO 疾風伝 [ULJS-00236] )
# XXXX-YYYYY => XXXXYYYYY (ULJS00236)

TARGET = ULJS00236

#############################
#    ここまでが設定項目     #
#############################



#############################
# ここから下は変更しないこと#
#############################

#※ある程度理解してる方は必要に応じて該当箇所を変更して下さい
# OBJS には exports.o 、$(MSDIR)/Lib_MagicSave.o の追加必須

MSDIR = ./include

OBJS =	exports.o $(MSDIR)/Lib_MagicSave.o main.o


INCDIR		= $(MSDIR)
CFLAGS		= -Os -G 0 -Wall
CXXFLAGS	= $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS		= $(CFLAGS)


PRX_EXPORTS = exports.exp

USE_KERNEL_LIBC	= 1
USE_KERNEL_LIBS	= 1

LIBDIR		=
LDFLAGS		= -mno-crt0 -nostartfiles
LIBS		=


PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak

