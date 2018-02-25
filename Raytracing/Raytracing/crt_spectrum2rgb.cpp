//#include "stdafx.h"

//#define _USE_MATH_DEFINES
#include "crt.h"
#include <stdio.h>
#include <math.h>
//#define SPECTRUM_SIZE 81 // задаём количество самплов (что бы было проще я взял количесвто самплов 81 как в спектрах выше)

namespace
{
	// внешние данные
	// заполняем массивы данными приведёнными ниже
	double x_spec[SPECTRUM_SIZE], y_spec[SPECTRUM_SIZE], z_spec[SPECTRUM_SIZE];

	// m_fRation --> <ratio>0.03786</ratio>
	double ratio = 0.03786;
	// m_fGamma = <gamma>0.455</gamma>, i.e. 1/gamma = 2.197
	double display_gamma = 1 / 0.455;
}
static int file_loaded = 0;

/*
<x_spec>
<ambientSpectrum s_wave="380" e_wave="780" count="81">
<s v="0.0014"/><s v="0.0022"/><s v="0.0042"/><s v="0.0076"/><s v="0.0143"/><s v="0.0232"/><s v="0.0435"/><s v="0.0776"/>
<s v="0.1344"/><s v="0.2148"/><s v="0.2839"/><s v="0.3285"/><s v="0.3483"/><s v="0.3481"/><s v="0.3362"/><s v="0.3187"/>
<s v="0.2908"/><s v="0.2511"/><s v="0.1954"/><s v="0.1421"/><s v="0.0956"/><s v="0.0580"/><s v="0.0320"/><s v="0.0147"/>
<s v="0.0049"/><s v="0.0024"/><s v="0.0093"/><s v="0.0291"/><s v="0.0633"/><s v="0.1096"/><s v="0.1655"/><s v="0.2257"/>
<s v="0.2904"/><s v="0.3597"/><s v="0.4334"/><s v="0.5121"/><s v="0.5945"/><s v="0.6784"/><s v="0.7621"/><s v="0.8425"/>
<s v="0.9163"/><s v="0.9786"/><s v="1.0263"/><s v="1.0567"/><s v="1.0622"/><s v="1.0456"/><s v="1.0026"/><s v="0.9384"/>
<s v="0.8544"/><s v="0.7514"/><s v="0.6424"/><s v="0.5419"/><s v="0.4479"/><s v="0.3608"/><s v="0.2835"/><s v="0.2187"/>
<s v="0.1649"/><s v="0.1212"/><s v="0.0874"/><s v="0.0636"/><s v="0.0468"/><s v="0.0329"/><s v="0.0227"/><s v="0.0158"/>
<s v="0.0114"/><s v="0.0081"/><s v="0.0058"/><s v="0.0041"/><s v="0.0029"/><s v="0.0020"/><s v="0.0014"/><s v="0.0010"/>
<s v="0.0007"/><s v="0.0005"/><s v="0.0003"/><s v="0.0002"/><s v="0.0002"/><s v="0.0001"/><s v="0.0001"/><s v="0.0001"/>
<s v="0.0000"/>
</ambientSpectrum>
</x_spec>

<y_spec>
<ambientSpectrum s_wave="380" e_wave="780" count="81">
<s v="0.0000"/><s v="0.0001"/><s v="0.0001"/><s v="0.0002"/><s v="0.0004"/><s v="0.0006"/><s v="0.0012"/><s v="0.0022"/>
<s v="0.0040"/><s v="0.0073"/><s v="0.0116"/><s v="0.0168"/><s v="0.0230"/><s v="0.0298"/><s v="0.0380"/><s v="0.0480"/>
<s v="0.0600"/><s v="0.0739"/><s v="0.0910"/><s v="0.1126"/><s v="0.1390"/><s v="0.1693"/><s v="0.2080"/><s v="0.2586"/>
<s v="0.3230"/><s v="0.4073"/><s v="0.5030"/><s v="0.6082"/><s v="0.7100"/><s v="0.7932"/><s v="0.8620"/><s v="0.9149"/>
<s v="0.9540"/><s v="0.9803"/><s v="0.9950"/><s v="1.0000"/><s v="0.9950"/><s v="0.9786"/><s v="0.9520"/><s v="0.9154"/>
<s v="0.8700"/><s v="0.8163"/><s v="0.7570"/><s v="0.6949"/><s v="0.6310"/><s v="0.5668"/><s v="0.5030"/><s v="0.4412"/>
<s v="0.3810"/><s v="0.3210"/><s v="0.2650"/><s v="0.2170"/><s v="0.1750"/><s v="0.1382"/><s v="0.1070"/><s v="0.0816"/>
<s v="0.0610"/><s v="0.0446"/><s v="0.0320"/><s v="0.0232"/><s v="0.0170"/><s v="0.0119"/><s v="0.0082"/><s v="0.0057"/>
<s v="0.0041"/><s v="0.0029"/><s v="0.0021"/><s v="0.0015"/><s v="0.0010"/><s v="0.0007"/><s v="0.0005"/><s v="0.0004"/>
<s v="0.0002"/><s v="0.0002"/><s v="0.0001"/><s v="0.0001"/><s v="0.0001"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/>
<s v="0.0000"/>
</ambientSpectrum>
</y_spec>

<z_spec>
<ambientSpectrum s_wave="380" e_wave="780" count="81">
<s v="0.0065"/><s v="0.0105"/><s v="0.0201"/><s v="0.0362"/><s v="0.0679"/><s v="0.1102"/><s v="0.2074"/><s v="0.3713"/>
<s v="0.6456"/><s v="1.0391"/><s v="1.3856"/><s v="1.6230"/><s v="1.7471"/><s v="1.7826"/><s v="1.7721"/><s v="1.7441"/>
<s v="1.6692"/><s v="1.5281"/><s v="1.2876"/><s v="1.0419"/><s v="0.8130"/><s v="0.6162"/><s v="0.4652"/><s v="0.3533"/>
<s v="0.2720"/><s v="0.2123"/><s v="0.1582"/><s v="0.1117"/><s v="0.0782"/><s v="0.0573"/><s v="0.0422"/><s v="0.0298"/>
<s v="0.0203"/><s v="0.0134"/><s v="0.0087"/><s v="0.0057"/><s v="0.0039"/><s v="0.0027"/><s v="0.0021"/><s v="0.0018"/>
<s v="0.0017"/><s v="0.0014"/><s v="0.0011"/><s v="0.0010"/><s v="0.0008"/><s v="0.0006"/><s v="0.0003"/><s v="0.0002"/>
<s v="0.0002"/><s v="0.0001"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/>
<s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/>
<s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/>
<s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/><s v="0.0000"/>
<s v="0.0000"/>
</ambientSpectrum>
</z_spec>

*/
/*
namespace
{
double ambientSpectrum[SPECTRUM_SIZE]; // count = SPECTRUM_SIZE
}
*/
// входное параметры:
// 1) указатель на массив компоментов спектра (кол-во компонентов, соответствует параметру 2)
// 2) количество компонентов в спектре
// выходные параметры:
// 1) компоненты цвета, заданные на интервале от 0 до 1
void convert_spectrum_to_rgb(const char *fileName, double * spectrum, size_t count, double *ored, double *ogreen, double *oblue)
{
	if (file_loaded == 0)
	{
		FILE *fi; // для чтения теста
		char str[200];

		if ((fi = fopen(fileName, "r")) == NULL)
		{
			printf("file %s does not exist\n", fileName);
			return;
		}
		int i;
		double q;
		for (i = 0; i < SPECTRUM_SIZE; i++)
		{
			fgets(str, 200, fi);
			sscanf(str, "%lf", &q);
			x_spec[i] = q;
		}
		for (i = 0; i < SPECTRUM_SIZE; i++)
		{
			fgets(str, 200, fi);
			sscanf(str, "%lf", &q);
			y_spec[i] = q;
		}
		for (i = 0; i < SPECTRUM_SIZE; i++)
		{
			fgets(str, 200, fi);
			sscanf(str, "%lf", &q);
			z_spec[i] = q;
		}
		file_loaded = 1;
		fclose(fi);
	}

	double xyz_x, xyz_y, xyz_z, red, green, blue;

	// вычисляем XYZ цвет для спектра
	double sum = 0.0, step = 1.0 / (count - 1);

	for (size_t k = 0; k < count - 1; k++)
		sum += (x_spec[k] * spectrum[k] + x_spec[k + 1] * spectrum[k + 1]) * 0.5;

	xyz_x = ratio * sum * step;

	sum = 0;

	for (size_t k = 0; k < count - 1; k++)
		sum += (y_spec[k] * spectrum[k] + y_spec[k + 1] * spectrum[k + 1]) * 0.5;

	xyz_y = ratio * sum * step;

	sum = 0;

	for (size_t k = 0; k < count - 1; k++)
		sum += (z_spec[k] * spectrum[k] + z_spec[k + 1] * spectrum[k + 1]) * 0.5;

	xyz_z = ratio * sum * step;

	const double CIEObservNTSC[3][3] =
	{
		{ 1.967, -0.955, 0.064 },
		{ -0.548, 1.938, -0.130 },
		{ -0.297, -0.027, 0.982 }
	};

	// преобразовываем XYZ в RGB
	red = CIEObservNTSC[0][0] * xyz_x + CIEObservNTSC[1][0] * xyz_y + CIEObservNTSC[2][0] * xyz_z;
	green = CIEObservNTSC[0][1] * xyz_x + CIEObservNTSC[1][1] * xyz_y + CIEObservNTSC[2][1] * xyz_z;
	blue = CIEObservNTSC[0][2] * xyz_x + CIEObservNTSC[1][2] * xyz_y + CIEObservNTSC[2][2] * xyz_z;

	// обрезаем значения меньшие нуля
	if (red < 0) red = 0;
	if (green < 0) green = 0;
	if (blue < 0) blue = 0;

	// обрезаем значения больше единицы
	if (red > 1) red = 1;
	if (green > 1) green = 1;
	if (blue > 1) blue = 1;

	// гамма коррекция
	red = pow(red, 1 / display_gamma);
	green = pow(green, 1 / display_gamma);
	blue = pow(blue, 1 / display_gamma);
	*ored = red;
	*ogreen = green;
	*oblue = blue;
}
