#pragma once
#define SPECTRUM_SIZE 81
#define sourceSpectrFileName "data2spectraTVCG.txt"

void convert_spectrum_to_rgb(const char *fileName, double * spectrum, size_t count, double *ored, double *ogreen, double *oblue);
void crt_get_d65(const char *fileName, float *d);

// ������� ���������:
// 1) ���������� �����, �������� �� ��������� �� 0 �� 1
// 2) ���������� ����������� � �������
// �������/�������� ���������:
// 1) ��������� �� ������ ����������� ������� (���-�� �����������, ������������� ��������� 2)
// void convert_rgb_to_spectrum(double red, double green, double color1, double * ambientSpectrum, size_t count) ������ �������� �� float
void convert_rgb_to_spectrum(const char *fileName, double red, double green, double blue, float *spectrum, size_t count);