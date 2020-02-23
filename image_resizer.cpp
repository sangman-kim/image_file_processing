//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "1280_944_rgb_header.h"


typedef unsigned char   INT8U;
typedef signed char     INT8S;
typedef signed short    INT16S;
typedef unsigned short  INT16U;
typedef signed int		INT32S;
typedef unsigned int    INT32U;
typedef float	FP32;


#define IMG_WIDTH  1280
#define IMG_HEIGHT 944

#define RESIZE_WIDTH   640		
#define RESIZE_HEIGHT  472

//#define CLAMP(t) t = (t<0 ? 0 : t<255 ? t:255)

#pragma warning(disable:4996)

INT32U sa_resize_width_coeff[RESIZE_WIDTH];
INT32U sa_resize_height_coeff[RESIZE_HEIGHT];

INT8U sa_out_resize_u8[RESIZE_WIDTH * RESIZE_HEIGHT * 3];
//INT8U sa_out_rgb_u8[IMG_WIDTH * IMG_HEIGHT * 3];
//INT8U sa_inp_rgb_u8[IMG_WIDTH * IMG_HEIGHT * 3];

INT8U* pa_inp_rgb_u8 = (INT8U*)&input_image_u8[0];
//INT8U * pa_inp_rgb_u8 = (INT8U*)&sa_inp_rgb_u8[0];
INT8U* pa_out_resize_u8 = (INT8U*)&sa_out_resize_u8[0];
//INT8U* pa_out_rgb_u8 = (INT8U*)&sa_out_rgb_u8[0];
INT32U* pa_resize_width_coeff = (INT32U*)&sa_resize_width_coeff[0];
INT32U* pa_resize_height_coeff = (INT32U*)&sa_resize_height_coeff[0];


INT32S clamp(INT32S x_value)
{
	if (x_value < 0)
	{
		x_value = 0;
	}
	else
	{
		if (x_value > 255)
		{
			x_value = 255;
		}
		else
		{
			x_value = x_value;
		}
	}
	return x_value;
}

void resize_ref_pos(const INT32U ori_length, const INT32U resize_length, INT32U* pa_resize_length_coeff)
{
	INT32U length_coeff = 0;
	INT32U temp = 0;
	
	length_coeff = ori_length / resize_length;

	
	for (INT32U idx_x_32u = 0; idx_x_32u < resize_length; idx_x_32u++)
	{
		*pa_resize_length_coeff++ = idx_x_32u * length_coeff;
		temp = idx_x_32u * length_coeff;
		printf("%d\n", temp);
	}
	
}


int main()
{

	INT32U idx_x_32u = 0;
	INT32U idx_y_32u = 0;

	//Make raw file from generated header file
	printf("Make RGB Rawfile from generated header file...\n\n\n");

	FILE* raw_rgb_from_header = fopen("gen_1280_944.rgb", "wb");

	fwrite(pa_inp_rgb_u8, sizeof(input_image_u8[0]), IMG_WIDTH * IMG_HEIGHT * 3, raw_rgb_from_header);

	fclose(raw_rgb_from_header);

	pa_resize_width_coeff = (INT32U*)&sa_resize_width_coeff[0];
	pa_resize_height_coeff = (INT32U*)&sa_resize_height_coeff[0];

	resize_ref_pos(IMG_WIDTH, RESIZE_WIDTH, pa_resize_width_coeff);

	resize_ref_pos(IMG_HEIGHT, RESIZE_HEIGHT, pa_resize_height_coeff);

	INT32U ref_pos_x_u32 = 0;
	INT32U ref_pos_y_u32 = 0;

	INT32U ori_pos_u32 = 0;

	pa_resize_width_coeff = (INT32U*)&sa_resize_width_coeff[0];
	pa_resize_height_coeff = (INT32U*)&sa_resize_height_coeff[0];
	
	INT32U temp_cnt = 0;

	for (idx_y_32u = 0; idx_y_32u < RESIZE_HEIGHT; idx_y_32u++)
	{
		ref_pos_y_u32 = *pa_resize_height_coeff++;
		//pa_resize_width_coeff = (INT8U*)&sa_resize_width_coeff[0];

		for (idx_x_32u = 0; idx_x_32u < RESIZE_WIDTH; idx_x_32u++)
		{
			//ref_pos_x_u32 = *pa_resize_width_coeff++;
			ref_pos_x_u32 = sa_resize_width_coeff[idx_x_32u];

			ori_pos_u32 = ref_pos_y_u32 * IMG_WIDTH * 3 + ref_pos_x_u32 * 3;
			
			*pa_out_resize_u8++ = input_image_u8[ori_pos_u32++];
			*pa_out_resize_u8++ = input_image_u8[ori_pos_u32++];
			*pa_out_resize_u8++ = input_image_u8[ori_pos_u32];
			
			/*
			*pa_out_resize_u8++ = 255;
			*pa_out_resize_u8++ = 0;
			*pa_out_resize_u8++ = 0;
			*/
			
		}
	}

	temp_cnt = temp_cnt;

	pa_out_resize_u8 = (INT8U*)&sa_out_resize_u8[0];

	//Make uyvy raw file from generated header file
	printf("Make Resize Rawfile from generated header file...\n\n\n");

	FILE* resize_raw_file_from_header = fopen("resize_640_472.rgb", "wb");

	fwrite(pa_out_resize_u8, sizeof(sa_out_resize_u8[0]), RESIZE_WIDTH * RESIZE_HEIGHT * 3, resize_raw_file_from_header);

	fclose(resize_raw_file_from_header);


	return 0;

}
