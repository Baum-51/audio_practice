#ifndef WAVE_CLASS_H_20200913
#define WAVE_CLASS_H_20200913

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>

class MONO_PCM
{
public:
	int fs = 44100;
	int bits = 16;
	int length = 44100 * 1;
	std::vector<double> s;

private:
    std::string file_name;

    std::fstream file;
    char riff_chunk_ID[4];
    long riff_chunk_size;
    char file_format_type[4];
    char fmt_chunk_ID[4];
    long fmt_chunk_size;
    short wave_format_type;
    short channel;
    long samples_per_sec;
    long bytes_per_sec;
    short block_size;
    short bits_per_sample;
    char data_chunk_ID[4];
    long data_chunk_size;
    short data;
    int n;

public:
    int wave_read_16bit_mono(std::string file_name);
    int wave_write_16bit_mono(std::string file_name);
};

struct STEREO_PCM
{
	int fs;
	int bits;
	int length;
	double* sL;
	double* sR;
};

int MONO_PCM:: wave_read_16bit_mono(std::string ifile_name)
{
    file_name = ifile_name;
    //fopen_s(&fp, file_name, "r");
    file.open(file_name.c_str(), std::ios::in | std::ios::binary);
    if (!file) {
        return EXIT_FAILURE;
    }
    
    file.read(riff_chunk_ID, sizeof(char) * 4);
    file.read((char*)&riff_chunk_size, sizeof(long));
    file.read(file_format_type, sizeof(char) * 4);
    file.read(fmt_chunk_ID, sizeof(char) * 4);
    file.read((char*)&fmt_chunk_size, sizeof(long));
    file.read((char*)&wave_format_type, sizeof(short));
    file.read((char*)&channel, sizeof(short));
    file.read((char*)&samples_per_sec, sizeof(long));
    file.read((char*)&bytes_per_sec, sizeof(long));
    file.read((char*)&block_size, sizeof(short));
    file.read((char*)&bits_per_sample, sizeof(short));
    file.read(data_chunk_ID, sizeof(char) * 4);
    file.read((char*)&data_chunk_size, sizeof(long));


    fs = samples_per_sec; // �W�{�����g�� 
    bits = bits_per_sample; // �ʎq�����x 
    length = data_chunk_size / 2; // ���f�[�^�̒��� 
    s.resize(length); // �������̊m�� 

    for (n = 0; n < length; n++)
    {
        //file.read((char*)&data, 2); // ���f�[�^�̓ǂݎ�� 
        file.read((char*)&data, sizeof(short));
        s[n] = (double)data / 32768.0; // ���f�[�^��-1�ȏ�1�����͈̔͂ɐ��K������ 
    }

    file.close();
    return 0;

}

int MONO_PCM::wave_write_16bit_mono(std::string file_name)
{
    riff_chunk_ID[0] = 'R';
    riff_chunk_ID[1] = 'I';
    riff_chunk_ID[2] = 'F';
    riff_chunk_ID[3] = 'F';
    riff_chunk_size = 36 + length * 2;
    file_format_type[0] = 'W';
    file_format_type[1] = 'A';
    file_format_type[2] = 'V';
    file_format_type[3] = 'E';

    fmt_chunk_ID[0] = 'f';
    fmt_chunk_ID[1] = 'm';
    fmt_chunk_ID[2] = 't';
    fmt_chunk_ID[3] = ' ';
    fmt_chunk_size = 16;
    wave_format_type = 1;
    channel = 1;
    samples_per_sec = fs; // �W�{�����g�� 
    bytes_per_sec = fs * bits / 8;
    block_size = bits / 8;
    bits_per_sample = bits; // �ʎq�����x 

    data_chunk_ID[0] = 'd';
    data_chunk_ID[1] = 'a';
    data_chunk_ID[2] = 't';
    data_chunk_ID[3] = 'a';
    data_chunk_size = length * 2;

    file.open(file_name.c_str(), std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        return EXIT_FAILURE;
    }

    file.write(riff_chunk_ID, sizeof(char) * 4);
    file.write((char*)&riff_chunk_size, sizeof(long));
    file.write(file_format_type, sizeof(char) * 4);
    file.write(fmt_chunk_ID, sizeof(char) * 4);
    file.write((char*)&fmt_chunk_size, sizeof(long));
    file.write((char*)&wave_format_type, sizeof(short));
    file.write((char*)&channel, sizeof(short));
    file.write((char*)&samples_per_sec, sizeof(long));
    file.write((char*)&bytes_per_sec, sizeof(long));
    file.write((char*)&block_size, sizeof(short));
    file.write((char*)&bits_per_sample, sizeof(short));
    file.write(data_chunk_ID, sizeof(char) * 4);
    file.write((char*)&data_chunk_size, sizeof(long));

    for (n = 0; n < length; n++)
    {
        double s_data = ((s[n] + 1.0) / 2.0 * 65536.0);

        if (s_data > 65535.0)
        {
            s_data = 65535.0; // �N���b�s���O 
        }
        else if (s_data < 0.0)
        {
            s_data = 0.0; // �N���b�s���O 
        }

        data = (short)((s_data + 0.5) - 32768); // �l�̌ܓ��ƃI�t�Z�b�g�̒��� 
        file.write((char*)&data, sizeof(short)); // ���f�[�^�̏����o�� 
    }

    file.close();
    return 0;
}


#endif //ifndef WAVE_CLASS_H_20200913
