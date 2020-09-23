#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>
#include "wave_class.h"
using namespace std;

void sine_wave(MONO_PCM& pcm, double f0, double a, int offset, const int duration)
{
	//offset:    値の初期位置の指定
	//duration:　１つのsin波の長さ(割合)
	int n;
	std::vector<double> s(duration);

	/* サイン波 */
	for (n = 0; n < duration; n++)
	{
		s[n] = a * sin(2.0 * M_PI * f0 * n / pcm.fs);
	}

 
	//フェード処理
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
	//のこぎり波
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
	// 矩形波
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
	// 三角波
	for (int i = 1; i <= 44; i = i + 2)
	{
		long i2 = i * i;
		for (int n = 0; n < pcm.length; n++)
		{
			pcm.s[n] += 1.0 / i2 * sin(M_PI * i / 2.0) * sin(2.0 * M_PI * i * f0 * n / pcm.fs);
		}
	}

	gain = 0.1; // ゲイン

	for (int n = 0; n < pcm.length; n++)
	{
		pcm.s[n] *= gain;
	}
}

void cosSawtooth(MONO_PCM& pcm, double f0, double gain)
{
	// コサイン波の重ね合わせによるノコギリ波
	for (int i = 1; i <= 44; i++)
	{
		for (int n = 0; n < pcm.length; n++)
		{
			pcm.s[n] += 1.0 / i * cos(2.0 * M_PI * i * f0 * n / pcm.fs);
		}
	}

	gain = 0.1; // ゲイン

	for (int n = 0; n < pcm.length; n++)
	{
		pcm.s[n] *= gain;
	}
}

void whitenoise(MONO_PCM& pcm, double f0)
{
	
	// 白色雑音
	for (int i = 1; i <= 2205; i++)
	{
		double theta = (double)rand() / RAND_MAX * 2.0 * M_PI;
		for (int n = 0; n < pcm.length; n++)
		{
			pcm.s[n] += sin(2.0 * M_PI * i * f0 * n / pcm.fs + theta);
		}
	}

	double gain = 0.1; // ゲイン

	for (int n = 0; n < pcm.length; n++)
	{
		pcm.s[n] *= gain;
	}
}

void DFT(std::string file_name)
{
	int N = 64; //DFTのサイズ
	std::vector<double> x_real(N), x_imag(N), X_real(N), X_imag(N);
	double W_real, W_imag;
	MONO_PCM pcm;

	pcm.wave_read_16bit_mono(file_name);


	// 波形
	for (int n = 0; n < N; n++)
	{
		x_real[n] = pcm.s[n]; // x(n)の実数部
		x_imag[n] = 0.0; // x(n)の虚数部
	}

	// DFT 
	for (int k = 0; k < N; k++)
	{
		for (int n = 0; n < N; n++)
		{
			W_real = cos(2.0 * M_PI * k * n / N);
			W_imag = -sin(2.0 * M_PI * k * n / N);
			X_real[k] += W_real * x_real[n] - W_imag * x_imag[n]; // X(k)の実数部
			X_imag[k] += W_real * x_imag[n] + W_imag * x_real[n]; // X(k)の虚数部
		}
	}

	// 周波数特性
	for (int k = 0; k < N; k++)
	{
		printf("X(%d) = %f+j%f\n", k, X_real[k], X_imag[k]);
	}
}

int main(void) {

	/*
	MONO_PCM pcm0, pcm1;
	int n;

	pcm0.wave_read_16bit_mono("a.wav"); // 音データの入力

	pcm1.fs = pcm0.fs; // 標本化周波数
	pcm1.bits = pcm0.bits; // 量子化精度
	pcm1.length = pcm0.length; // 音データの長さ
	pcm1.s .resize(pcm1.length); // 音データ

	for (n = 0; n < pcm1.length; n++)
	{
		pcm1.s[n] = pcm0.s[n]; // 音データのコピー
	}

	pcm1.wave_write_16bit_mono("b.wav"); // 音データの出力


	return 0;
	*/
	/*
	MONO_PCM pcm;
	int n;
	double a, f0;

	pcm.fs = 44100; //サンプリング周波数
	pcm.bits = 16;//量子化精度
	pcm.length = pcm.fs * 1;//データ長
	pcm.s.resize(pcm.length); // メモリの確保//音データ

	a = 0.5; //振幅
	f0 = 440.0;//周波数

	for (n = 0; n < pcm.length; ++n) {
		pcm.s[n] = a * sin(2.0 * M_PI * f0 * n / pcm.fs);
	}

	pcm.wave_write_16bit_mono("scale_A.wav");
	return 0;
	*/

	MONO_PCM pcm;
	/*
	double f0, gain;

	pcm.fs = 44100; // 標本化周波数
	pcm.bits = 16; // 量子化精度
	pcm.length = pcm.fs * 1; // 音データの長さ
	pcm.s.resize(pcm.length); // 音データ
	*/

	/*//ドレミ
	sine_wave(pcm, 261.63, 0.1, pcm.fs * 0.00, pcm.fs * 0.25); // C4
	sine_wave(pcm, 293.66, 0.1, pcm.fs * 0.25, pcm.fs * 0.25); // D4
	sine_wave(pcm, 329.63, 0.1, pcm.fs * 0.50, pcm.fs * 0.25); // E4
	sine_wave(pcm, 349.23, 0.1, pcm.fs * 0.75, pcm.fs * 0.25); // F4
	sine_wave(pcm, 392.00, 0.1, pcm.fs * 1.00, pcm.fs * 0.25); // G4
	sine_wave(pcm, 440.00, 0.1, pcm.fs * 1.25, pcm.fs * 0.25); // A4
	sine_wave(pcm, 493.88, 0.1, pcm.fs * 1.50, pcm.fs * 0.25); // B4
	sine_wave(pcm, 523.25, 0.1, pcm.fs * 1.75, pcm.fs * 0.25); // C5
	*/


	double f0 = 440.0; // 基本周波数
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
