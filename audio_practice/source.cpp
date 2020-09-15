#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "wave_class.h"

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
		s[duration - n - 1] *= (double)n / (pcm.fs * 0.01);
	}

	for (n = 0; n < duration; n++)
	{
		pcm.s[offset + n] += s[n];
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

	MONO_PCM Sawtooth;
	MONO_PCM Square;
	double f0, gain;

	Sawtooth.fs = 44100; /* �W�{�����g�� */
	Sawtooth.bits = 16; /* �ʎq�����x */
	Sawtooth.length = Sawtooth.fs * 1; /* ���f�[�^�̒��� */
	Sawtooth.s.resize(Sawtooth.length); /* ���f�[�^ */

	Square.fs = 44100; /* �W�{�����g�� */
	Square.bits = 16; /* �ʎq�����x */
	Square.length = Square.fs * 1; /* ���f�[�^�̒��� */
	Square.s.resize(Square.length); /* ���f�[�^ */

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

	f0 = 500.0; // ��{���g��

	// �m�R�M���g
	for (int i = 1; i <= 44; i++)
	{
		for (int n = 0; n < Sawtooth.length; n++)
		{
			Sawtooth.s[n] += 1.0 / i * sin(2.0 * M_PI * i * f0 * n / Sawtooth.fs);
		}
	}
	// ��`�g
	for (int i = 1; i <= 44; i += 2)
	{
		for (int n = 0; n < Square.length; ++n)
		{
			Square.s[n] += 1.0 / i * sin(2.0 * M_PI * i * f0 * n / Square.fs);
		}
	}

	gain = 0.1; // �Q�C��

	for (int n = 0; n < Sawtooth.length; n++)
	{
		Sawtooth.s[n] *= gain;
	}

	for (int n = 0; n < Square.length; n++)
	{
		Square.s[n] *= gain;
	}

	Sawtooth.wave_write_16bit_mono("ex3_1.wav");
	Square.wave_write_16bit_mono("ex3_2.wav");

	return 0;
}