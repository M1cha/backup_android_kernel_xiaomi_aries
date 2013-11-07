/* Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/bootmem.h>
#include <linux/gpio.h>
#include <asm/mach-types.h>
#include <asm/mach/mmc.h>
#include <mach/msm_bus_board.h>
#include <mach/board.h>
#include <mach/gpiomux.h>
#include <mach/socinfo.h>
#include "devices.h"
#include "board-aries.h"

#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
static struct gpiomux_setting gpio_eth_config = {
	.pull = GPIOMUX_PULL_NONE,
	.drv = GPIOMUX_DRV_8MA,
	.func = GPIOMUX_FUNC_GPIO,
};

/* The SPI configurations apply to GSBI 5*/
static struct gpiomux_setting gpio_spi_config = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};

/* The SPI configurations apply to GSBI 5 chip select 2*/
static struct gpiomux_setting gpio_spi_cs2_config = {
	.func = GPIOMUX_FUNC_3,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};


struct msm_gpiomux_config apq8064_ethernet_configs[] = {
	{
		.gpio = 43,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_eth_config,
			[GPIOMUX_ACTIVE] = &gpio_eth_config,
		}
	},
};
#endif

#ifdef CONFIG_AUDIENCE_ES310
static struct gpiomux_setting anc_mclk_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_NONE,
};
#endif

static struct gpiomux_setting hs_aux_mic_sw_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting hs_aux_mic_sw_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting us_eu_hs_sw_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting us_eu_hs_sw_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting hs_uart_sw_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting hs_uart_sw_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
#ifdef CONFIG_MSM_UART_HS_USE_HS
	.pull = GPIOMUX_PULL_DOWN,
#else
	.pull = GPIOMUX_PULL_UP,
#endif
};

static struct gpiomux_setting mhl_int_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting mhl_int_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct msm_gpiomux_config mhl_gpio_config[] __initdata = {
	/* MHL interrupt */
	{
		.gpio = 23,
		.settings = {
			[GPIOMUX_ACTIVE] = &mhl_int_act_cfg,
			[GPIOMUX_SUSPENDED] = &mhl_int_sus_cfg,
		},
	},
};

static struct gpiomux_setting tp_int_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting tp_int_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct msm_gpiomux_config ts_gpio_config[] __initdata = {
	/* TS INTERRUPT */
	{
		.gpio = 6,
	 	.settings = {
			[GPIOMUX_ACTIVE] = &tp_int_act_cfg,
			[GPIOMUX_SUSPENDED] =  &tp_int_sus_cfg,
		},
	},
};

static struct gpiomux_setting fte_suspended_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting fte_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct msm_gpiomux_config lcd_gpio_config[] __initdata = {
	/* LCD FTE */
	{
		.gpio = 0,
		.settings = {
			[GPIOMUX_ACTIVE] = &fte_active_cfg,
			[GPIOMUX_SUSPENDED] = &fte_suspended_cfg,
		},
	},
};

static struct gpiomux_setting cdc_mclk = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting wcnss_5wire_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting wcnss_5wire_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv  = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};


static struct gpiomux_setting slimbus = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_KEEPER,
};

#ifdef CONFIG_MSM_UART_HS_USE_HS
static struct gpiomux_setting gsbi7_gpio_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#endif

static struct gpiomux_setting gsbi5_func2_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi5_suspended_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi5_active_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi3_suspended_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_KEEPER,
};

static struct gpiomux_setting gsbi3_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gpio_i2c_config = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gpio_i2c_config_sus = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_KEEPER,
};

static struct gpiomux_setting audience_wakeup_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting gsbi7_func1_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi7_func2_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting hdmi_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting hdmi_active_1_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting hdmi_active_2_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_DOWN,
};

#ifdef CONFIG_USB_EHCI_MSM_HSIC
static struct gpiomux_setting hsic_act_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting hsic_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting hsic_wakeup_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting hsic_wakeup_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_IN,
};

static struct msm_gpiomux_config apq8064_hsic_configs[] = {
	{
		.gpio = 88,               /*HSIC_STROBE */
		.settings = {
			[GPIOMUX_ACTIVE] = &hsic_act_cfg,
			[GPIOMUX_SUSPENDED] = &hsic_sus_cfg,
		},
	},
	{
		.gpio = 89,               /* HSIC_DATA */
		.settings = {
			[GPIOMUX_ACTIVE] = &hsic_act_cfg,
			[GPIOMUX_SUSPENDED] = &hsic_sus_cfg,
		},
	},
	{
		.gpio = 47,              /* wake up */
		.settings = {
			[GPIOMUX_ACTIVE] = &hsic_wakeup_act_cfg,
			[GPIOMUX_SUSPENDED] = &hsic_wakeup_sus_cfg,
		},
	},
};
#endif

static struct msm_gpiomux_config apq8064_hdmi_configs[] __initdata = {
	{
		.gpio = 69,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
	{
		.gpio = 70,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
	{
		.gpio = 71,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
	{
		.gpio = 72,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_2_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
};

static struct msm_gpiomux_config apq8064_gsbi_configs[] __initdata = {
	/* GSBI3 I2C QUP SDA */
	{
		.gpio = 8,
	 	.settings = {
			[GPIOMUX_ACTIVE] = &gsbi3_active_cfg,
			[GPIOMUX_SUSPENDED] = &gsbi3_suspended_cfg,
		},
	},
	/* GSBI3 I2C QUP SCL */
	{
		.gpio = 9,
	 	.settings = {
			[GPIOMUX_ACTIVE] = &gsbi3_active_cfg,
			[GPIOMUX_SUSPENDED] = &gsbi3_suspended_cfg,
		},
	},
	/* GSBI1 QUP I2C_DATA */
	{
		.gpio = 20,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_i2c_config,
			[GPIOMUX_SUSPENDED] = &gpio_i2c_config_sus,
		},
	},
	/* GSBI1 QUP I2C_CLK */
	{
		.gpio = 21,
		.settings = {
			[GPIOMUX_ACTIVE] = &gpio_i2c_config,
			[GPIOMUX_SUSPENDED] =  &gpio_i2c_config_sus,
		},
	},
	/* GSBI5 UART2 TX */
	{
		.gpio = 51,
		.settings = {
			[GPIOMUX_ACTIVE] = &audience_wakeup_active_cfg,
			[GPIOMUX_SUSPENDED] = &gsbi5_func2_cfg,
		},
	},
	/* GSBI5 UART2 RX */
	{
		.gpio = 52,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi5_func2_cfg,
		},
	},
	/* GSBI5 I2C QUP SDA */
	{
		.gpio = 53,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi5_suspended_cfg,
			[GPIOMUX_ACTIVE] = &gsbi5_active_cfg,
		},
	},
	/* GSBI5 I2C QUP SCL */
	{
		.gpio = 54,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi5_suspended_cfg,
			[GPIOMUX_ACTIVE] = &gsbi5_active_cfg,
		},
	},
	/* Switch between HS and UART: HIGH->UART, LOW->HS */
	{
		.gpio = 62,
		.settings = {
			[GPIOMUX_ACTIVE] = &hs_uart_sw_active_cfg,
			[GPIOMUX_SUSPENDED] = &hs_uart_sw_suspend_cfg,
		},
	},
#ifdef CONFIG_MSM_UART_HS_USE_HS
	{
		.gpio = 82,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi7_gpio_cfg,
		},
	},
	{
		.gpio = 83,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi7_gpio_cfg,
		},
	},
#else
	/* GSBI7 UART2 TX */
	{
		.gpio = 82,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi7_func2_cfg,
		},
	},
	/* GSBI7 UART2 RX */
	{
		.gpio = 83,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi7_func1_cfg,
		},
	},
#endif
};

static struct msm_gpiomux_config apq8064_slimbus_config[] __initdata = {
	{
		.gpio   = 40,           /* slimbus clk */
		.settings = {
			[GPIOMUX_SUSPENDED] = &slimbus,
		},
	},
	{
		.gpio   = 41,           /* slimbus data */
		.settings = {
			[GPIOMUX_SUSPENDED] = &slimbus,
		},
	},
};

static struct msm_gpiomux_config apq8064_audio_codec_configs[] __initdata = {
#ifdef CONFIG_AUDIENCE_ES310
	/* anc_mclk */
	{
		.gpio = 34,
		.settings = {
			[GPIOMUX_ACTIVE] = &anc_mclk_cfg,
			[GPIOMUX_SUSPENDED] = &anc_mclk_cfg,
		},
	},
#endif
	/* MIC_SWITCH_EN: NO HS -> SET TO LOW, HS - >SET TO HIGH */
	{
		.gpio = 36,
		.settings = {
			[GPIOMUX_ACTIVE] = &us_eu_hs_sw_active_cfg,
			[GPIOMUX_SUSPENDED] = &us_eu_hs_sw_suspend_cfg,
		},
	},
	/* HS_MIC_SWITCH_EN: HIGH - AUX MIC, LOW - HS MIC */
	{
		.gpio = 38,
		.settings = {
			[GPIOMUX_ACTIVE] = &hs_aux_mic_sw_active_cfg,
			[GPIOMUX_SUSPENDED] = &hs_aux_mic_sw_suspend_cfg,
		},
	},
	{
		.gpio = 39,
		.settings = {
			[GPIOMUX_SUSPENDED] = &cdc_mclk,
		},
	},
};

static struct gpiomux_setting ap2mdm_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting mdm2ap_status_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting mdm2ap_errfatal_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting mdm2ap_pblrdy = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};


static struct gpiomux_setting ap2mdm_soft_reset_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting ap2mdm_wakeup = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct msm_gpiomux_config mdm_configs[] __initdata = {
	/* AP2MDM_STATUS */
	{
		.gpio = 48,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_cfg,
		}
	},
	/* MDM2AP_STATUS */
	{
		.gpio = 49,
		.settings = {
			[GPIOMUX_ACTIVE] = &mdm2ap_status_cfg,
			[GPIOMUX_SUSPENDED] = &mdm2ap_status_cfg,
		}
	},
	/* MDM2AP_ERRFATAL */
	{
		.gpio = 19,
		.settings = {
			[GPIOMUX_SUSPENDED] = &mdm2ap_errfatal_cfg,
		}
	},
	/* AP2MDM_SOFT_RESET, aka AP2MDM_PON_RESET_N */
	{
		.gpio = 27,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_soft_reset_cfg,
		}
	},
	/* AP2MDM_WAKEUP */
	{
		.gpio = 35,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_wakeup,
		}
	},
	/* MDM2AP_PBL_READY*/
	{
		.gpio = 46,
		.settings = {
			[GPIOMUX_SUSPENDED] = &mdm2ap_pblrdy,
		}
	},
};

static struct msm_gpiomux_config wcnss_5wire_interface[] = {
	{
		.gpio = 64,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 65,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 66,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 67,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 68,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
};

#ifdef CONFIG_MMC_MSM_SDC2_SUPPORT
static struct gpiomux_setting sdc2_clk_active_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting sdc2_cmd_data_0_3_active_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting sdc2_suspended_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting sdc2_data_1_suspended_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct msm_gpiomux_config apq8064_sdc2_configs[] __initdata = {
	{
		.gpio      = 59,
		.settings = {
			[GPIOMUX_ACTIVE] = &sdc2_clk_active_cfg,
			[GPIOMUX_SUSPENDED] = &sdc2_suspended_cfg,
		},
	},
	{
		.gpio      = 57,
		.settings = {
			[GPIOMUX_ACTIVE] = &sdc2_cmd_data_0_3_active_cfg,
			[GPIOMUX_SUSPENDED] = &sdc2_suspended_cfg,
		},

	},
	{
		.gpio      = 62,
		.settings = {
			[GPIOMUX_ACTIVE] = &sdc2_cmd_data_0_3_active_cfg,
			[GPIOMUX_SUSPENDED] = &sdc2_suspended_cfg,
		},
	},
	{
		.gpio      = 61,
		.settings = {
			[GPIOMUX_ACTIVE] = &sdc2_cmd_data_0_3_active_cfg,
			[GPIOMUX_SUSPENDED] = &sdc2_data_1_suspended_cfg,
		},
	},
	{
		.gpio      = 60,
		.settings = {
			[GPIOMUX_ACTIVE] = &sdc2_cmd_data_0_3_active_cfg,
			[GPIOMUX_SUSPENDED] = &sdc2_suspended_cfg,
		},
	},
	{
		.gpio      = 58,
		.settings = {
			[GPIOMUX_ACTIVE] = &sdc2_cmd_data_0_3_active_cfg,
			[GPIOMUX_SUSPENDED] = &sdc2_suspended_cfg,
		},
	},
};
#endif


#ifdef CONFIG_MMC_MSM_SDC4_SUPPORT
static struct gpiomux_setting sdc4_clk_active_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting sdc4_cmd_data_0_3_active_cfg = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting sdc4_suspended_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting sdc4_data_1_suspended_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct msm_gpiomux_config apq8064_sdc4_configs[] __initdata = {
	{
		.gpio      = 68,
		.settings = {
			[GPIOMUX_ACTIVE] = &sdc4_clk_active_cfg,
			[GPIOMUX_SUSPENDED] = &sdc4_suspended_cfg,
		},
	},
	{
		.gpio      = 67,
		.settings = {
			[GPIOMUX_ACTIVE] = &sdc4_cmd_data_0_3_active_cfg,
			[GPIOMUX_SUSPENDED] = &sdc4_suspended_cfg,
		},

	},
	{
		.gpio      = 66,
		.settings = {
			[GPIOMUX_ACTIVE] = &sdc4_cmd_data_0_3_active_cfg,
			[GPIOMUX_SUSPENDED] = &sdc4_suspended_cfg,
		},
	},
	{
		.gpio      = 65,
		.settings = {
			[GPIOMUX_ACTIVE] = &sdc4_cmd_data_0_3_active_cfg,
			[GPIOMUX_SUSPENDED] = &sdc4_data_1_suspended_cfg,
		},
	},
	{
		.gpio      = 64,
		.settings = {
			[GPIOMUX_ACTIVE] = &sdc4_cmd_data_0_3_active_cfg,
			[GPIOMUX_SUSPENDED] = &sdc4_suspended_cfg,
		},
	},
	{
		.gpio      = 63,
		.settings = {
			[GPIOMUX_ACTIVE] = &sdc4_cmd_data_0_3_active_cfg,
			[GPIOMUX_SUSPENDED] = &sdc4_suspended_cfg,
		},
	},
};
#endif

void __init apq8064_init_gpiomux(void)
{
	int rc;

	rc = msm_gpiomux_init(NR_GPIO_IRQS);
	if (rc) {
		pr_err(KERN_ERR "msm_gpiomux_init failed %d\n", rc);
		return;
	}

	msm_gpiomux_install(wcnss_5wire_interface,
			ARRAY_SIZE(wcnss_5wire_interface));

	#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
		msm_gpiomux_install(apq8064_ethernet_configs,
				ARRAY_SIZE(apq8064_ethernet_configs));
	#endif

	msm_gpiomux_install(apq8064_gsbi_configs,
			ARRAY_SIZE(apq8064_gsbi_configs));

	msm_gpiomux_install(apq8064_slimbus_config,
			ARRAY_SIZE(apq8064_slimbus_config));

	msm_gpiomux_install(apq8064_audio_codec_configs,
			ARRAY_SIZE(apq8064_audio_codec_configs));

	msm_gpiomux_install(mdm_configs,
			ARRAY_SIZE(mdm_configs));

#ifdef CONFIG_USB_EHCI_MSM_HSIC
	msm_gpiomux_install(apq8064_hsic_configs,
			ARRAY_SIZE(apq8064_hsic_configs));
#endif

	msm_gpiomux_install(apq8064_hdmi_configs,
			ARRAY_SIZE(apq8064_hdmi_configs));

#ifdef CONFIG_MMC_MSM_SDC2_SUPPORT
	 msm_gpiomux_install(apq8064_sdc2_configs,
			     ARRAY_SIZE(apq8064_sdc2_configs));
#endif

#ifdef CONFIG_MMC_MSM_SDC4_SUPPORT
	 msm_gpiomux_install(apq8064_sdc4_configs,
			     ARRAY_SIZE(apq8064_sdc4_configs));
#endif

	msm_gpiomux_install(mhl_gpio_config,
			ARRAY_SIZE(mhl_gpio_config));

	msm_gpiomux_install(ts_gpio_config,
			ARRAY_SIZE(ts_gpio_config));

	msm_gpiomux_install(lcd_gpio_config,
			ARRAY_SIZE(lcd_gpio_config));
}
