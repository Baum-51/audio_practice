#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>
#include "wave_class.h"
using namespace std;

void sine_wave(MONO_PCM& pcm, double f0, double a, int offset, const int duration)
{
	//offset:    �l�̏����ʒu�̎w��
	//duration:�@�P��sin�g�̒���(����)
	int n;
	std::vector<double> s(duration);

	/* �T�C���g */
	for (n = 0; n < duration; n++)
	{
		s[n] = a * sin(2.0 * M_PI * f0 * n / pcm.fs);
	}

 
	//�t�F�[�h����
	for (n = 0; n < pcm.fs * 0.01; n++)
	{
		s[n] *= (double)n / (pcm.fs * 0.01);
		int i = duration - n - 1;
		s[i] *= (double)n / (pcm.fs * 0.01);
	}

	for (n = 0; n < duration; n++)
	{
		int i = offset + n;
		pcm.s[i] += s[n];
	}
}

void Sawtooth(MONO_PCM& pcm, double f0, double gain)
{	
	//�̂�����g
	for (int i = 1; i <= 44; i++)
	{
		for (int n = 0; n < pcm.length; n++)
		{
			pcm.s[n] += 1.0 / i * sin(2.0 * M_PI * i * f0 * n / pcm.fs);
		}
	}

	for (int n = 0; n < pcm.length; n++)
	{
		pcm.s[n] *= gain;
	}
	pcm.wave_write_16bit_mono("Sawtooth.wav");
}

void Square(MONO_PCM& pcm, double f0, double gain)
{
	// ��`�g
	for (int i = 1; i <= 44; i += 2)
	{
		for (int n = 0; n < pcm.length; ++n)
		{
			pcm.s[n] += 1.0 / i * sin(2.0 * M_PI * i * f0 * n / pcm.fs);
		}
	}

	for (int n = 0; n < pcm.length; n++)
	{
		pcm.s[n] *= gain;
	}
	pcm.wave_write_16bit_mono("Square.wav");
}

void Triangle(MONO_PCM& pcm, double f0, double gain)
{
	// �O�p�g
	for (int i = 1; i <= 44; i = i + 2)
	{
		long i2 = i * i;
		for (int n = 0; n < pcm.length; n++)
		{
			pcm.s[n] += 1.0 / i2 * sin(M_PI * i / 2.0) * sin(2.0 * M_PI * i * f0 * n / pcm.fs);
		}
	}

	gain = 0.1; // �Q�C��

	for (int n = 0; n < pcm.length; n++)
	{
		pcm.s[n] *= gain;
	}
}

void cosSawtooth(MONO_PCM& pcm, double f0, double gain)
{
	// �R�T�C���g�̏d�ˍ��킹�ɂ��m�R�M���g
	for (int i = 1; i <= 44; i++)
	{
		for (int n = 0; n < pcm.length; n++)
		{
			pcm.s[n] += 1.0 / i * cos(2.0 * M_PI * i * f0 * n / pcm.fs);
		}
	}

	gain = 0.1; // �Q�C��

	for (int n = 0; n < pcm.length; n++)
	{
		pcm.s[n] *= gain;
	}
}

void whitenoise(MONO_PCM& pcm, double f0)
{
	
	// ���F�G��
	for (int i = 1; i <= 2205; i++)
	{
		double theta = (double)rand() / RAND_MAX * 2.0 * M_PI;
		for (int n = 0; n < pcm.length; n++)
		{
			pcm.s[n] += sin(2.0 * M_PI * i * f0 * n / pcm.fs + theta);
		}
	}

	double gain = 0.1; // �Q�C��

	for (int n = 0; n < pcm.length; n++)
	{
		pcm.s[n] *= gain;
	}
}

void DFT(std::string file_name)
{
	int N = 64; //DFT�̃T�C�Y
	std::vector<double> x_real(N), x_imag(N), X_real(N), X_imag(N);
	double W_real, W_imag;
	MONO_PCM pcm;

	pcm.wave_read_16bit_mono(file_name);


	// �g�`
	for (int n = 0; n < N; n++)
	{
		x_real[n] = pcm.s[n]; // x(n)�̎�����
		x_imag[n] = 0.0; // x(n)�̋�����
	}

	// DFT 
	for (int k = 0; k < N; k++)
	{
		for (int n = 0; n < N; n++)
		{
			W_real = cos(2.0 * M_PI * k * n / N);
			W_imag = -sin(2.0 * M_PI * k * n / N);
			X_real[k] += W_real * x_real[n] - W_imag * x_imag[n]; // X(k)�̎�����
			X_imag[k] += W_real * x_imag[n] + W_imag * x_real[n]; // X(k)�̋�����
		}
	}

	// ���g������
	for (int k = 0; k < N; k++)
	{
		printf("X(%d) = %f+j%f\n", k, X_real[k], X_imag[k]);
	}
}

int main(void) {

	/*
	MONO_PCM pcm0, pcm1;
	int n;

	pcm0.wave_read_16bit_mono("a.wav"); // ���f�[�^�̓���

	pcm1.fs = pcm0.fs; // �W�{�����g��
	pcm1.bits = pcm0.bits; // �ʎq�����x
	pcm1.length = pcm0.length; // ���f�[�^�̒���
	pcm1.s .resize(pcm1.length); // ���f�[�^

	for (n = 0; n < pcm1.length; n++)
	{
		pcm1.s[n] = pcm0.s[n]; // ���f�[�^�̃R�s�[
	}

	pcm1.wave_write_16bit_mono("b.wav"); // ���f�[�^�̏o��


	return 0;
	*/
	/*
	MONO_PCM pcm;
	int n;
	double a, f0;

	pcm.fs = 44100; //�T���v�����O���g��
	pcm.bits = 16;//�ʎq�����x
	pcm.length = pcm.fs * 1;//�f�[�^��
	pcm.s.resize(pcm.length); // �������̊m��//���f�[�^

	a = 0.5; //�U��
	f0 = 440.0;//���g��

	for (n = 0; n < pcm.length; ++n) {
		pcm.s[n] = a * sin(2.0 * M_PI * f0 * n / pcm.fs);
	}

	pcm.wave_write_16bit_mono("scale_A.wav");
	return 0;
	*/

	MONO_PCM pcm;
	/*
	double f0, gain;

	pcm.fs = 44100; // �W�{�����g��
	pcm.bits = 16; // �ʎq�����x
	pcm.length = pcm.fs * 1; // ���f�[�^�̒���
	pcm.s.resize(pcm.length); // ���f�[�^
	*/

	/*//�h���~
	sine_wave(pcm, 261.63, 0.1, pcm.fs * 0.00, pcm.fs * 0.25); // C4
	sine_wave(pcm, 293.66, 0.1, pcm.fs * 0.25, pcm.fs * 0.25); // D4
	sine_wave(pcm, 329.63, 0.1, pcm.fs * 0.50, pcm.fs * 0.25); // E4
	sine_wave(pcm, 349.23, 0.1, pcm.fs * 0.75, pcm.fs * 0.25); // F4
	sine_wave(pcm, 392.00, 0.1, pcm.fs * 1.00, pcm.fs * 0.25); // G4
	sine_wave(pcm, 440.00, 0.1, pcm.fs * 1.25, pcm.fs * 0.25); // A4
	sine_wave(pcm, 493.88, 0.1, pcm.fs * 1.50, pcm.fs * 0.25); // B4
	sine_wave(pcm, 523.25, 0.1, pcm.fs * 1.75, pcm.fs * 0.25); // C5
	*/


	double f0 = 440.0; // ��{���g��
	double gain = 0.1;

	pcm.s.resize(pcm.length);

	for (int n = 0; n < pcm.length; n++)
	{
		pcm.s[n] = 0.1 * sin(2.0 * M_PI * f0 * n / pcm.fs);
	}

	DFT("sine_440hz.wav");
	//pcm.wave_write_16bit_mono("sine_500hz.wav");

	return 0;
}
