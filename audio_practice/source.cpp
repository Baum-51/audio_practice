#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "wave_class.h"

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

	MONO_PCM Sawtooth;
	MONO_PCM Square;
	double f0, gain;

	Sawtooth.fs = 44100; /* 標本化周波数 */
	Sawtooth.bits = 16; /* 量子化精度 */
	Sawtooth.length = Sawtooth.fs * 1; /* 音データの長さ */
	Sawtooth.s.resize(Sawtooth.length); /* 音データ */

	Square.fs = 44100; /* 標本化周波数 */
	Square.bits = 16; /* 量子化精度 */
	Square.length = Square.fs * 1; /* 音データの長さ */
	Square.s.resize(Square.length); /* 音データ */

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

	f0 = 500.0; // 基本周波数

	// ノコギリ波
	for (int i = 1; i <= 44; i++)
	{
		for (int n = 0; n < Sawtooth.length; n++)
		{
			Sawtooth.s[n] += 1.0 / i * sin(2.0 * M_PI * i * f0 * n / Sawtooth.fs);
		}
	}
	// 矩形波
	for (int i = 1; i <= 44; i += 2)
	{
		for (int n = 0; n < Square.length; ++n)
		{
			Square.s[n] += 1.0 / i * sin(2.0 * M_PI * i * f0 * n / Square.fs);
		}
	}

	gain = 0.1; // ゲイン

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
