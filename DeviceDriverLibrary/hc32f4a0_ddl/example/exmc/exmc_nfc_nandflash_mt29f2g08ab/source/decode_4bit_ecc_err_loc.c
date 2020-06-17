/**
 *******************************************************************************
 * @file  decode_4bit_ecc_err_loc.c
 * @brief This file contains all the functions prototypes of the decoding error 
 *        ECC 4bits location.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2020-06-12       Hongjh          First version
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co., Ltd. ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 *******************************************************************************
 */

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#include "a_to_i.h"
#include "i_to_a.h"

/**
 * @addtogroup HC32F4A0_DDL_Examples
 * @{
 */

/**
 * @addtogroup EXMC_NFC_Nandflash_MT29F2G08AB
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @addtogroup EXMC_NFC_Nandflash_MT29F2G08AB_Global_Functions
 * @{
 */

/**
 * @brief  Software decode ECC 4bit error location for 512-bytes
 * @param  [in] ecc_syndrome            The syndrome value
 * @param  [out] ecc_err_byte_number    The ECC error byte number
 * @param  [out] ecc_err_byte_bit       The ECC error byte bit
 * @param  [in] size                    The array buffer size
 * @retval ECC error bytes count
 * @note the ecc_syndrome array size value must be 8.
 */
int16_t NFC_SwDecodeEcc4BitsErrLocation(const int16_t ecc_syndrome[],
                                        int16_t ecc_err_byte_number[],
                                        int16_t ecc_err_byte_bit[],
                                        int16_t size)
{
    int16_t i, j, elp_sum;
    int16_t Matrix_a[11] = {0};
    int16_t Matrix_b[11] = {0};
    int16_t Matrix_c[12] = {0};
    int16_t Element[7] = {0};
    int16_t alpha = 0;
    int16_t temp_index = 0;

    int16_t block_length = 8191;
    int16_t data_length = 4096;
    int16_t syn_val[20] = {0};
    int16_t syn_err = 0;
    int16_t err_count = 0;
    int16_t d_flg = 0;
    int16_t data_location = 0, rev_location = 0;
    int16_t result_byte = 0, result_bit = 0;

    static int16_t err_location[20];
    static int16_t err_loc_ply[100][100];

    for (i = 1; i <= 8; i++)    /* initialize the whole array */
    {
        syn_val[i] = ecc_syndrome[i -  1];
    }

    for (i=1; i <= 8; i++)
    {
        if (syn_val[i] != 0)
        {
            syn_err = 1;
        }
    }

    if (syn_err)
    {
        /* initialise table entries */
        for (i = 1; i <= 8; i++)
        {
            syn_val[i] = i_to_a[syn_val[i]];
        }

        Matrix_c[0] = 0;
        Matrix_c[1] = syn_val[1];
        err_loc_ply[0][0] = 1;
        err_loc_ply[1][0] = 1;
        for (i = 1; i < 8; i++)
        {
            err_loc_ply[0][i] = 0;
            err_loc_ply[1][i] = 0;
        }
        Matrix_a[0] = 0;
        Matrix_a[1] = 0;
        Matrix_b[0] = -1;
        Matrix_b[1] = 0;
        alpha = -1; 

        do
        {
            /* skip even loops */
#if (DDL_PRINT_ENABLE == DDL_ON)
            printf("skip even loops\n");
            printf("alpha = %8x\n",alpha);
#endif
            alpha += 2;
            if (Matrix_c[alpha] != -1)
            {
                temp_index = alpha - 2;

                if (temp_index < 0)
                {
                    temp_index = 0;
                }

                while ((Matrix_c[temp_index] == -1) && (temp_index > 0))
                {
                    temp_index = temp_index - 2;
                }

                if (temp_index < 0)
                {
                     temp_index = 0;
                }

                if (temp_index > 0)
                {
                    j = temp_index;
                    do
                    {
                        j = j-2;

                        if (j < 0)
                        {
                            j = 0;
                        }

                        if ((Matrix_c[j] != -1) && (Matrix_b[temp_index] < Matrix_b[j]))
                        {
                            temp_index = j;
                        }
                    } while (j > 0);
                }

                if (Matrix_a[alpha] > Matrix_a[temp_index] + alpha - temp_index)
                {
                    Matrix_a[alpha + 2] = Matrix_a[alpha];
                }
                else
                {
                    Matrix_a[alpha + 2] = Matrix_a[temp_index] + alpha - temp_index;
                }

                for (i = 0; i < 8; i++)
                {
                    err_loc_ply[alpha + 2][i] = 0;
                }

                for (i = 0; i <= Matrix_a[temp_index]; i++)
                {
                    if (err_loc_ply[temp_index][i] != 0)
                    {
                        err_loc_ply[alpha + 2][i + alpha - temp_index] = a_to_i[(Matrix_c[alpha] + block_length - Matrix_c[temp_index] + i_to_a[err_loc_ply[temp_index][i]]) % block_length];
                    }
                }

                for (i = 0; i <= Matrix_a[alpha]; i++)
                {
                    err_loc_ply[alpha + 2][i] ^= err_loc_ply[alpha][i];
                }
            }
            else
            {
                Matrix_a[alpha + 2] = Matrix_a[alpha];
                for (i = 0; i <= Matrix_a[alpha]; i++)
                {
                    err_loc_ply[alpha + 2][i] = err_loc_ply[alpha][i];
                }
            }

            Matrix_b[alpha + 2] = alpha + 1 - Matrix_a[alpha + 2];

            /* Form (alpha+2)th discrepancy. */
#if (DDL_PRINT_ENABLE == DDL_ON)
            printf("Form (alpha+2)th discrepancy\n");
#endif
            if (alpha < 8)
            {
                if (syn_val[alpha + 2] != -1)
                {
                    Matrix_c[alpha + 2] = a_to_i[syn_val[alpha + 2]];
                }
                else
                {
                    Matrix_c[alpha + 2] = 0;
                }

                for (i = 1; i <= Matrix_a[alpha + 2]; i++)
                {
                    if ((syn_val[alpha + 2 - i] != -1) && (err_loc_ply[alpha + 2][i] != 0))
                    {
                       Matrix_c[alpha + 2] ^= a_to_i[(syn_val[alpha + 2 - i] + i_to_a[err_loc_ply[alpha + 2][i]]) % block_length];
                    }
                }
                Matrix_c[alpha + 2] = i_to_a[Matrix_c[alpha + 2]];
            }
        } while ((alpha < 7) && (Matrix_a[alpha + 2] <= 4));

        alpha=alpha+2;
        Matrix_a[7] = Matrix_a[alpha];
        if (Matrix_a[7] <= 4)
        {
            for (i = 1; i <= Matrix_a[7]; i++)
            {
                Element[i] = i_to_a[err_loc_ply[alpha][i]];
                #if (DDL_PRINT_ENABLE == DDL_ON)
                printf("iElement[%8x] = %8x\n", i, Element[i]);
                #endif
            }

            err_count = 0 ;
            for (i = 1; i <= block_length; i++)
            {
                elp_sum = 1 ;
                for (j = 1; j <= Matrix_a[7]; j++)
                {
                   if (Element[j] != -1)
                   {
                        Element[j] = (Element[j] + j) % block_length ;
                        elp_sum ^= a_to_i[Element[j]] ;
                        #if (DDL_PRINT_ENABLE == DDL_ON)
                        printf("jElement[%8x] = %8x\n", j, Element[j]);
                        printf("elp_sum = %8x\n", elp_sum);
                        #endif
                   }
                }

                if (!elp_sum)
                {
                    err_location[err_count] = block_length - i;
                    err_count++ ;
                    #if (DDL_PRINT_ENABLE == DDL_ON)
                    printf("err_count = %8x\n", err_count);
                    #endif
                }
            }

            if (err_count == Matrix_a[7])
            {
                d_flg = 1;
            }
        }
    }
    else
    {
        d_flg = 1;
        err_count = 0;
    }

    if (d_flg == 0)
    {
        #if (DDL_PRINT_ENABLE == DDL_ON)
        printf("Unable to decode !\n");
        #endif
    }
    else
    {
        for (i = 0; i < err_count; i++)
        {
            #if (DDL_PRINT_ENABLE == DDL_ON)
            printf("error bit : %4d\n", i);
            #endif
            if (err_location[i] >= 52)
            {
                data_location = err_location[i] - 52;
            }
            else
            {
                data_location = err_location[i] + data_length;
            }

            #if (DDL_PRINT_ENABLE == DDL_ON)
            printf("error location = %4d, ", data_location);
            #endif

            if (data_location <= 4095)
            {
                rev_location = 4095 - data_location;
                result_bit = rev_location%8;
                result_byte = (rev_location - result_bit)/8;
                #if (DDL_PRINT_ENABLE == DDL_ON)
                printf("byte = %4d, bit = %4d\n", result_byte, result_bit);
                #endif
            }
            else
            {
                rev_location = 4147 - data_location;
                result_bit = rev_location%8;
                result_byte = (rev_location - result_bit)/8;
                #if (DDL_PRINT_ENABLE == DDL_ON)
                printf("BCH byte = %4d, bit = %4d\n", result_byte, result_bit);
                #endif
            }

            if (i < size)
            {
                ecc_err_byte_number[i] = result_byte;
                ecc_err_byte_bit[i] = result_bit;
            }
        }
    }

    return err_count;
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
