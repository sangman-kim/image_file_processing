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
//#define CLAMP(t) t = (t<0 ? 0 : t<255 ? t:255)

#pragma warning(disable:4996)


INT8U sa_out_uyvy_422_u8[IMG_WIDTH * IMG_HEIGHT * 2];
INT8U sa_out_rgb_u8[IMG_WIDTH * IMG_HEIGHT * 3];
//INT8U sa_inp_rgb_u8[IMG_WIDTH * IMG_HEIGHT * 3];

INT8U * pa_inp_rgb_u8 = (INT8U *)&input_image_u8[0];
//INT8U * pa_inp_rgb_u8 = (INT8U*)&sa_inp_rgb_u8[0];
INT8U * pa_out_uyvy_422_u8 = (INT8U *)&sa_out_uyvy_422_u8[0];
INT8U * pa_out_rgb_u8 = (INT8U *)&sa_out_rgb_u8[0];

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


int main()
{

	INT32U idx_x_32u = 0;

	INT8U r0_u8 = 0;
	INT8U g0_u8 = 0;
	INT8U b0_u8 = 0;

	INT8U r1_u8 = 0;
	INT8U g1_u8 = 0;
	INT8U b1_u8 = 0;

	INT16U y0_u16 = 0;
	INT16S u_s16 = 0;
	INT16U y1_u16 = 0;
	INT16S v_s16 = 0;

	//Make raw file from generated header file
	printf("Make RGB Rawfile from generated header file...\n\n\n");

	FILE* raw_rgb_from_header = fopen("gen_1280_944.rgb", "wb");

	fwrite(pa_inp_rgb_u8, sizeof(sa_out_uyvy_422_u8[0]), IMG_WIDTH * IMG_HEIGHT * 3, raw_rgb_from_header);

	fclose(raw_rgb_from_header);


	pa_inp_rgb_u8 = (INT8U *)&input_image_u8[0];

	for(idx_x_32u = 0; idx_x_32u < (IMG_WIDTH * IMG_HEIGHT) / 2; idx_x_32u++)
	{
		r0_u8 = *pa_inp_rgb_u8++;
		g0_u8 = *pa_inp_rgb_u8++;
		b0_u8 = *pa_inp_rgb_u8++;

		r1_u8 = *pa_inp_rgb_u8++;
		g1_u8 = *pa_inp_rgb_u8++;
		b1_u8 = *pa_inp_rgb_u8++;

		/* RGB to yuv reference formual
		Y = 0.229R + 0.587G + 0.114B
		U = -0.147R - 0.289G + 0.436B
		V = 0.615R - 0.515G - 0.100B
		*/
		y0_u16 = (( 77 * r0_u8 + 150 * g0_u8 +  29 * b0_u8 + 128) >> 8);
		u_s16  = ((-43 * r0_u8 -  84 * g0_u8 + 127 * b0_u8 + 128) >> 8) + 128;
		y1_u16 = (( 77 * r1_u8 + 150 * g1_u8 +  29 * b1_u8 + 128) >> 8);
		v_s16  = ((127 * r0_u8 - 106 * g0_u8 -  21 * b0_u8 + 128) >> 8) + 128;

		*pa_out_uyvy_422_u8++ = (INT8U)u_s16;
		*pa_out_uyvy_422_u8++ = (INT8U)y0_u16;
		*pa_out_uyvy_422_u8++ = (INT8U)v_s16;
		*pa_out_uyvy_422_u8++ = (INT8U)y1_u16;
	}
	
	pa_out_uyvy_422_u8 = (INT8U*)&sa_out_uyvy_422_u8[0];
	//Make uyvy raw file from generated header file
	printf("Make UYVY Rawfile from generated header file...\n\n\n");

	FILE* raw_file_from_header = fopen("gen_1280_944.uyvy", "wb");

	fwrite(sa_out_uyvy_422_u8, sizeof(sa_out_uyvy_422_u8[0]), IMG_WIDTH * IMG_HEIGHT * 2, raw_file_from_header);

	fclose(raw_file_from_header);

	INT16S c0_s16 = 0;
	INT16S c1_s16 = 0;
	INT16S d_s16 = 0;
	INT16S e_s16 = 0;

	pa_out_uyvy_422_u8 = (INT8U*)&sa_out_uyvy_422_u8[0];
	for (idx_x_32u = 0; idx_x_32u < (IMG_WIDTH * IMG_HEIGHT / 2); idx_x_32u++)
	{		
		
		u_s16 = *pa_out_uyvy_422_u8++;
		y0_u16 = *pa_out_uyvy_422_u8++;
		v_s16 = *pa_out_uyvy_422_u8++;
		y1_u16 = *pa_out_uyvy_422_u8++;

		c0_s16 = y0_u16 - 16;
		c0_s16 = y1_u16 - 16;
		d_s16 = u_s16 - 128;
		e_s16 = v_s16 - 128;

		r0_u8 = (INT8U)clamp((INT32S)(298 * c0_s16 + 409 * e_s16 + 128) >> 8);
		g0_u8 = (INT8U)clamp((INT32S)(298 * c0_s16 - 100 * d_s16 - 208 * e_s16 + 128) >> 8);
		b0_u8 = (INT8U)clamp((INT32S)(298 * c0_s16 + 516 * d_s16 + 128) >> 8);

		r1_u8 = (INT8U)clamp((INT32S)(298 * c1_s16 + 409 * e_s16 + 128) >> 8);
		g1_u8 = (INT8U)clamp((INT32S)(298 * c1_s16 - 100 * d_s16 - 208 * e_s16 + 128) >> 8);
		b1_u8 = (INT8U)clamp((INT32S)(298 * c1_s16 + 516 * d_s16 + 128) >> 8);

		*pa_out_rgb_u8++ = r0_u8;
		*pa_out_rgb_u8++ = g0_u8;
		*pa_out_rgb_u8++ = b0_u8;

		*pa_out_rgb_u8++ = r1_u8;
		*pa_out_rgb_u8++ = g1_u8;
		*pa_out_rgb_u8++ = b1_u8;
	}

	pa_out_rgb_u8 = (INT8U*)&sa_out_rgb_u8[0];

	//Make raw file from generated header file
	printf("Make rgb Rawfile from generated uyvy...\n\n\n");

	FILE* raw_file_from_uyvy = fopen("gen_rgb_from_uyvy.rgb", "wb");

	fwrite(pa_out_rgb_u8, sizeof(sa_out_rgb_u8[0]), IMG_WIDTH * IMG_HEIGHT * 3, raw_file_from_uyvy);

	fclose(raw_file_from_uyvy);


	return 0;

}
