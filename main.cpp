#include<iostream>
#include<vector>
#include<fstream>
#include<iomanip>
#include "Statement.h"
#include<Windows.h>
using namespace std;

// FAT32 ʵ��
class FAT32 {
public:
	uint64_t dbr, fat1, fat2, root, first, Capacity;
	uint16_t Cu;
	uint32_t fat;
};

void Template_FAT32() {
	cout <<
		"-----------------FAT32 DBR ģ��------------------" << endl <<
		"               �̶�ֵ                  ��  ����" << endl <<
		"EB 58 90 4D 53 44 4F 53 35 2E 30 00 02 10 3A 10" << endl <<
		"  FAT����           �̶�ֵ           �������   " << endl <<
		"02 00 00 00 00 F8 00 00 3F 00 FF 00 00 80 00 00" << endl <<
		"����������  FAT����                  root��ʼ�� " << endl <<
		"00 18 3F 01 E3 27 00 00 00 00 00 00 02 00 00 00" << endl <<
		"01 00 06 00 00 00 00 00 00 00 00 00 00 00 00 00" << endl <<
		"80 00 09 00 00 00 00 4E 4F 20 4E 41 4D 45 20 20" << endl <<
		"20 20 46 41 54 33 32 20 20 20" << endl <<
		"-------------------------------------------------" << endl;
}

void Traverse_FAT32(FAT32& fat32) {
	cout << "������DBR����������";
	cin >> fat32.dbr;
	cout << "������FAT1�������� (F8 FF FF 0F)��";
	cin >> fat32.fat1;
	cout << "����������Ϊ��" << fat32.fat1 - fat32.dbr << endl; // FAT1λ�� - DBRλ�� = ��������
	cout << "������FAT2�������� (F8 FF FF 0F)��";
	cin >> fat32.fat2;
	fat32.fat = (uint32_t)(fat32.fat2 - fat32.fat1); // FAT2λ�� - FAT1λ�� = FAT������
	cout << "FAT�������Ϊ��" << fat32.fat << endl;
	fat32.root = fat32.fat2 + fat32.fat; // FAT2 + FAT���� = ��Ŀ¼λ��
	cout << "��Ŀ¼��root����������Ϊ��" << fat32.root << endl;
	cout << "������root֮���һ���ļ��л��ļ�����������";
	cin >> fat32.first;
	fat32.Cu = (uint16_t)(fat32.first - fat32.root); // ��һ���ļ�����λ�� - ��Ŀ¼λ�� = ��
	cout << "ÿ��������Ϊ��" << fat32.Cu << endl; // �����
	fat32.Capacity = (uint64_t)(fat32.fat) * 512 / 4 * fat32.Cu; // FAT������ * 512 / 4 * �� = ������
	cout << "������������(������)ԼΪ��" << fat32.Capacity << endl;
	cout << "------------------------------------------------------" << endl;
} // ����FAT32

void Correct_FAT32(uint16_t Cu) {
	if (Cu == 0) {
		cout << "���������������";
		cin >> Cu;
	}
	cout << "�����ļ�λ�ã�";
	uint32_t swap;
	cin >> hex >> swap;
	cout.fill('0');
	swap = swapInt32(swap); // ������ֵ
	printf("ת�����ֵΪ��%08X\n", swap);
	cout << "�ļ���ʼ��Ϊ��" << swap << endl;
	cout << "�Ӹ�Ŀ¼��ת��������" << (swap - 2) * Cu << endl;
	cout << "------------------------------------------------------" << endl;
}

uint32_t swapInt32(uint32_t val) { // 00 00 29 0d
	return (val & 0x00FF0000) << 8 | (val & 0xFF000000) >> 8 |
		(val & 0x00FF) << 8 | (val & 0xFF00) >> 8;
} // ���ؽ����ļ�λ����ֵ
// FAT32 ʵ�����

// NTFS ʵ��
class NTFS {
public:
	uint64_t dbr, mftMir, mft, mftStartCu, mftCu, bitmapByte, temp;
	uint16_t Cu;
	uint32_t Capacity;
};

void Template_NTFS() {
	cout <<
		"-----------------NTFS DBR ģ��-------------------" << endl <<
		"               �̶�ֵ                  �� -----" << endl <<
		"EB 52 90 4E 54 46 53 20 20 20 20 00 02 08 00 00" << endl <<
		"                    �̶�ֵ            �������  " << endl <<
		"00 00 00 00 00 F8 00 00 3F 00 FF 00 00 80 00 00" << endl <<
		"         �̶�ֵ         ����������  -----------" << endl <<
		"00 00 00 00 80 00 80 00 FF E7 3F 01 00 00 00 00" << endl <<
		" MFT��ʼ��  ------------ MFT������ʼ��---------  " << endl <<
		"00 00 0C 00 00 00 00 00 02 00 00 00 00 00 00 00" << endl <<
		"F6 00 00 00 01 00 00 00" << endl
		<< "-------------------------------------------------" << endl;
}

void Traverse_NTFS(NTFS& ntfs) {
	char ch;
	cout << "������DBR�������� (EB 52 90)��";
	cin >> ntfs.dbr;
	cout << "�Ƿ����MFTMir (Y/N)��";
	cin >> ch;
	if (ch == 'y' || ch == 'Y') {
		cout << "������MFTMir�������� (46 49 4C 45)��";
		cin >> ntfs.mftMir;
		ntfs.Cu = (uint16_t)(ntfs.mftMir - ntfs.dbr) / 2; // ����һ��λ��DBR��������, MFT���� - DBRλ�� / 2 = ��
		cout << "ÿ��������Ϊ��" << ntfs.Cu << endl;
		cout << "������MFT��ʼ�أ�";
		cin >> ntfs.mftStartCu;
		ntfs.mft = ntfs.mftStartCu * ntfs.Cu + ntfs.dbr; // MFT��ʼ�� * �� + DBRλ�� = MFT��������
		cout << "MTF��������Ϊ��" << ntfs.mft << endl;

		cout << "�Ƿ��ٴ������������ (Y/N)��";
		cin >> ch;
		if (ch == 'y' || ch == 'Y') {
			cout << "������MFT��ռ�ֽ�����";
			cin >> ntfs.temp;
			cout << "������MFT��ռ������";
			cin >> ntfs.mftCu;
			ntfs.Cu = (uint16_t)(ntfs.temp / 512 / ntfs.mftCu); // MFT��ռ�ֽ��� / 512 / MFT��ռ���� = ��
			cout << "(����ȷ��)ÿ��������Ϊ��" << ntfs.Cu << endl;
		}
	}
	else {
		cout << "������MFT�������� (46 49 4C 45)��";
		cin >> ntfs.mft;
		cout << "������MFT��ռ�ֽ�����";
		cin >> ntfs.temp;
		cout << "������MFT��ռ������";
		cin >> ntfs.mftCu;
		ntfs.Cu = (uint16_t)(ntfs.temp / 512 / ntfs.mftCu); // MFT��ռ�ֽ��� / 512 / MFT��ռ���� = ��
		cout << "ÿ��������Ϊ��" << ntfs.Cu << endl;
	}

	cout << "������bitmap��ռ�ֽ��� (һ��λ��MFT����12����)��";
	cin >> ntfs.bitmapByte;
	ntfs.Capacity = (uint32_t)(ntfs.bitmapByte * 8 * ntfs.Cu); // λͼ��ռ�ֽ��� * 8 * �� = ����������
	cout << "������������Ϊ (������)��" << ntfs.Capacity << endl;
	cout << "------------------------------------------------------" << endl;
}
// NTFS ʵ�����

// exFAT ʵ��
class exFAT {
public:
	uint64_t dbr, fat, bitmap, upcase, root;
	uint32_t temp, bmbyte, Cu, Capacity, sumCu, sumFat, rootCu;
}; // ����exFAT�������

void Template_exFAT() {
	cout <<
		"-----------------exFAT DBR ģ��------------------" << endl <<
		"                     �̶�ֵ                    " << endl <<
		"EB 76 90 45 58 46 41 54 20 20 20 00 00 00 00 00" << endl <<
		"00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00" << endl <<
		"00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00" << endl <<
		"00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00" << endl <<
		"  DBRλ��   --------------������������-----------" << endl <<
		"00 80 00 00 00 00 00 00 00 70 3F 01 00 00 00 00" << endl <<
		"FAT��λ��    FAT����   DBR��λͼ���� �����ܴ���" << endl <<
		"00 08 00 00 40 95 00 00 00 A0 00 00 C0 83 4A 00" << endl <<
		"��Ŀ¼����     ���ֵ        �̶�ֵ    �� �̶�ֵ" << endl <<
		"17 00 00 00 93 A2 A8 B6 00 01 00 00 09 04 01 80" << endl <<
		"-------------------------------------------------" << endl;
}

void Traverse_exFAT(exFAT& exfat) {
	cout << "������DBR�������� (EB 76 90)��";
	cin >> exfat.dbr;
	cout << "������FAT���������� (F8 FF FF FF)��";
	cin >> exfat.fat;
	cout << "��ȷ��bitamp��ռ������";
	cin >> exfat.temp;
	cout << "����������Ϊ��" << exfat.fat - exfat.dbr << endl; // fat - dbr = ��������
	cout << "������Bitmap�������� (FF FF FF FF)��";
	cin >> exfat.bitmap;
	cout << "�������д�ַ��������� (00 00 01 00)��";
	cin >> exfat.upcase;
	exfat.Cu = (exfat.upcase - exfat.bitmap) / exfat.temp; // (��Сд - λͼ�� / λͼ��ռ���� = ÿ��������
	cout << "ÿ��������Ϊ��" << exfat.Cu << endl;
	exfat.root = exfat.upcase + exfat.Cu; // ��Сд + ÿ�������� = ��Ŀ¼λ��
	cout << "��Ŀ¼��������Ϊ��" << exfat.root << endl;
	if (exfat.root - exfat.upcase < 16)
		exfat.rootCu = (exfat.temp + 2 + 2); // (λͼ��ռ���� + ��ʼ�غţ�2�� + λͼ��ռ������2��) = ��Ŀ¼��ʼ��
	else
		exfat.rootCu = (exfat.temp + 2 + 1); // (λͼ��ռ���� + ��ʼ�غţ�2�� + λͼ��ռ������1��) = ��Ŀ¼��ʼ��
	cout << "��Ŀ¼���ڴغ�Ϊ��" << exfat.rootCu << endl;
	cout << "��ȷ��bitmap��ռ�ֽ�����";
	cin >> exfat.bmbyte;
	exfat.Capacity = (exfat.bmbyte * 8 * exfat.Cu) + (exfat.bitmap - exfat.dbr); // (λͼ�ֽ��� * 8 * ��) + (λͼ - dbr) = ������
	cout << "������������ (������)��" << exfat.Capacity << endl;
	exfat.sumCu = (exfat.Capacity - (exfat.bitmap - exfat.dbr)) / exfat.Cu; // (������ - (λͼ - dbr)) / �� = �ܴ�
	cout << "�ܴ�Ϊ��" << exfat.sumCu << endl;
	exfat.sumFat = ((uint32_t)((exfat.sumCu + 2) / 128 / exfat.Cu) + 1) * exfat.Cu; // ((�ܴ� + 2) / 128 / �� + 1) * �� = FAT������
	cout << "FAT����������(������)Ϊ��" << exfat.sumFat << endl;
	cout << "------------------------------------------------------" << endl;
}
// exFAT ʵ�����

// EXT ʵ��
class EXT {
public:
	uint64_t Superblock, Capacity, temp;
	uint32_t BlockGroup, TotalBlocks, NodeTable, iNodes, AlliNodes;
	uint16_t num, Cu;

};

void Template_EXT() {
	cout <<
		"-----------------EXT ������ ģ��-----------------" << endl <<
		"  i�ڵ�����     ������  -----------------------" << endl <<
		"00 05 FA 00 40 00 FA 00 03 80 0C 00 12 90 F5 00" << endl <<
		"----------------------- �� --------------------" << endl <<
		"F5 04 FA 00 00 00 00 00 04 00 00 00 04 00 00 00" << endl <<
		"  �������  -----------��i�ڵ���-----------------" << endl <<
		"F8 FF 00 00 F8 FF 00 00 00 FF 00 00 00 00 00 00" << endl <<
		"------------------    �̶�ֵ    ---------------" << endl <<
		"00 00 00 00 00 00 FF FF 53 EF 01 00 01 00 00 00" << endl <<
		"00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00" << endl <<
		"-----------------------------���--------------" << endl <<
		"00 00 00 00 0B 00 00 00 00 01 00 00 3C 00 00 00" << endl <<
		"-------------------------------------------------" << endl;
}

void Traverse_EXT(EXT& ext) {
	cout << "--------------------------------------------------------------------------------------------------" << endl;
	char ch;
	int temp = 2;
	cout << "�������� 53 EF ƫ��Ϊ 512 = 56, ���������Ϊ����, ������Ѽ����Ա�֤ǰ��������û�б��޸�" << endl;
	cout << "�����뵱ǰ������򱸷��������������뺬��53 EF������, ���ǿհ���������";
	cin >> ext.Superblock; // Superblock = ������
	cout << "��ǰ���������Ƿ�Ϊ0�� (Y/N)��";
	cin >> ch;
	if (ch != 'Y' && ch != 'y') {
		cout << "������ÿ���������";
		cin >> ext.BlockGroup; // BlockGroup = ����
		cout << "�����뵱ǰ�������ţ�";
		cin >> ext.num;
		cout << "������ؿ�����";
		cin >> ext.Cu;
		ext.Superblock = ext.Superblock - (uint64_t)(ext.BlockGroup) * ext.num * ext.Cu; // ���� * ��� * �� = 0��ƫ�Ƶ���ǰ���������
		cout << "0�ų���������������Ϊ��" << ext.Superblock << endl;
		cout << "���Ʊ��ݺ�, ��0�ų���������������ճ����֮�����תλ�þ��ӿհ�������ת" << endl;
		cout << "�������ܿ�����";
		cin >> ext.TotalBlocks;
		ext.Capacity = (uint64_t)ext.TotalBlocks * ext.Cu; // �ܿ��� * �� = ������������
		cout << "����������������������Ϊ��" << ext.Capacity << endl;
		cout << "������ÿ��������� i�ڵ� ����";
		cin >> ext.iNodes; // iNodes = i�ڵ��� 
	}
	else {
		ext.Superblock -= 2;
		cout << "�������ܿ�����";
		cin >> ext.TotalBlocks;
		cout << "������ؿ�����";
		cin >> ext.Cu;
		ext.Capacity = (uint64_t)ext.TotalBlocks * ext.Cu; // �ܿ��� * �� = ������������
		cout << "����������������������Ϊ��" << ext.Capacity << endl;
		cout << "֮�����תλ�þ��ӿհ�������ת" << endl;
		cout << "������ÿ���������";
		cin >> ext.BlockGroup; // BlockGroup = ����
		cout << "������ÿ��������� i�ڵ� ����";
		cin >> ext.iNodes; // iNodes = i�ڵ��� 
	}

	cout << "������ת" << ext.Cu << "����ڵ�������" << endl;
	cout << "�������һ�� �ڵ�� ��ʼ�أ�";
	cin >> ext.NodeTable; // NodeTable = �ڵ����ʼ��
	cout << "��ת" << ext.NodeTable * ext.Cu << "���������1���ڵ��" << endl;
	cout << "�Ƿ�����ڵ�� (Y/N)��";
	cin >> ch;
	if (ch == 'Y' || ch == 'y') {
		ext.AlliNodes = ext.TotalBlocks / ext.BlockGroup + 1; // �ܿ��� / ������ + 1 = i�ڵ������
		cout << "�Ƿ�������нڵ�� (Y/N)��";
		cin >> ch;
		if (ch == 'y' || ch == 'Y') {
			ext.temp = (uint64_t)ext.NodeTable + ext.BlockGroup; // ���ڵ����ʼ�� + ��������* 32 = �ڶ����ڵ����������
			cout << "��2���ڵ����������Ϊ��" << ext.temp * 32 << endl;
			for (uint32_t i = 2; i != ext.AlliNodes; ++i)
				cout << "��" << i + 1 << "���ڵ����������Ϊ��" << ((uint64_t)(ext.BlockGroup) * i + 2) * (uint64_t)ext.Cu << endl;
			// ���� * i = i��ŵ�λͼ�ڵ�λ�ã�+2 = ƫ�����أ� * �� = �ڵ��λ�á�
		}
		else {
			cout << "�ڵ������Ϊ��" << ext.AlliNodes << endl;
			cout << "������������� (��Ҫ���ڽڵ������)��";
			cin >> temp;
			if (temp <= ext.AlliNodes) {
				ext.temp = (uint64_t)ext.NodeTable + ext.BlockGroup; // ���ڵ����ʼ�� + ��������* 32 = �ڶ����ڵ����������
				cout << "��2���ڵ����������Ϊ��" << ext.temp * 32 << endl;
				for (uint32_t i = 2; i != temp; ++i)
					cout << "��" << i + 1 << "���ڵ����������Ϊ��" << ((uint64_t)(ext.BlockGroup) * i + 2) * (uint64_t)ext.Cu << endl;
				// ���� * i = i��ŵ�λͼ�ڵ�λ�ã�+2 = ƫ�����أ� * �� = �ڵ��λ�á�
			}
			else
				cerr << "�������ֹͣ����" << endl;
		}
	}
	cout << "--------------------------------------------------------------------------------------------------" << endl;
} // ����EXT
// EXT ʵ�����

// ��������������
void setCipher(vector<uint16_t>& Cipher) {
	int temp;
	char ch;
	cout << "�뱣֤���ĺ������ֽ�������Ӧ" << endl;
	cout << "���������� (����ʮ�����Ƶ��ֽ�, �ո����)��";
	while ((cin >> hex >> temp).get(ch)) {
		Cipher.push_back(temp);
		if (ch == '\n') break;
	}
} // ��ȡ����
bool setPlaintext(vector<uint16_t>& Plaintext, vector<uint16_t> Cipher) {
	int temp;
	char ch;
	cout << "���������� (����ʮ�����Ƶ��ֽ�, �ո����)��";
	while ((cin >> hex >> temp).get(ch)) {
		Plaintext.push_back(temp);
		if (ch == '\n') break;
	}
	if (Cipher.size() != Plaintext.size()) {
		cout << endl << "������������������Ӧ" << endl << endl;
		system("pause");
		return false;
	}
	return true;
} // ��ȡ����

// ������Ӽ����
void xor_and_dec(vector<uint16_t> Plaintext, vector<uint16_t> Cipher) {
	vector<uint16_t> resXor, result, copyPlain;
	int temp, j = 0;
	for (int n = 1; n != 256; ++n) {
		copyPlain = Plaintext;

		for (auto s : Cipher)
			resXor.push_back(s ^ n); // a1 b1 c1 d1

		temp = ((resXor.back() + 256) - Plaintext.back()) % 256; // d1 + 256 - m4 % 256

		for (auto s : resXor)
			result.push_back(((s + 256) - temp) % 256); // a1 + 256 - d1 % 256
		result.pop_back();
		copyPlain.pop_back(); // ce cf 0d
		if (result == copyPlain) {
			j++;
			if (j == 1) cout << endl << "----------���Ӽ�----------" << endl << endl;
			cout << "�����" << setfill('0') << setw(2) << hex << uppercase << n << " �ټ�ȥ��" << temp << endl;
		}
		result.clear();
		resXor.clear();
		if (n == 255 && j != 0) cout << endl << "--------���Ӽ�����--------" << endl;
	}
} // �������Ӽ�(xor and dec)
void add_and_xor(vector<uint16_t> Plaintext, vector<uint16_t> Cipher) {
	vector<uint16_t> resAdd, result, copyPlain;
	int temp, j = 0;
	for (int n = 1; n != 256; ++n) {
		copyPlain = Plaintext;
		for (auto s : Cipher)
			resAdd.push_back((s + n) % 256); // (a + n) % 256

		temp = (resAdd.back() ^ Plaintext.back()); // (d1 ^ m4)

		for (auto s : resAdd)
			result.push_back(s ^ temp); // (a1 ^ d1)
		result.pop_back(); // remove (d1 ^ d1)
		copyPlain.pop_back(); // remove (m4)

		if (result == copyPlain) {
			j++;
			if (j == 1) cout << endl << "----------�Ӽ����----------" << endl << endl;
			cout << "�ȼӣ�" << setfill('0') << setw(2) << hex << uppercase << n << " �����" << temp << endl;

		}
		result.clear();
		resAdd.clear();
		if (n == 255 && j != 0) cout << endl << "--------�Ӽ�������--------" << endl;
	}
}

// ����
void Levo(vector<uint16_t> Plaintext, vector<uint16_t> Cipher) {
	int n = 0;
	vector<uint16_t> vtimes;
	cout << endl << "----------������ʼ----------" << endl << endl;
	for (const auto s : Cipher) {
		int result, Cp = s;
		for (int times = 1; times != 9; ++times) {
			result = (uint16_t)(((Cp * pow(2, times)) / 256) + ((Cp * (uint16_t)pow(2, times)) % 256));
			if (result == Plaintext[n]) {
				cout << setfill('0') << setw(2) << hex << uppercase << Cp << " ��������Ϊ��" << times << "��" << endl;
				n++;
				vtimes.push_back(times);
				break;
			}
			else if (result != Plaintext[n] && times == 8) {
				cout << setfill('0') << setw(2) << hex << uppercase << Cp << " ��������������" << endl;
				n++;
				vtimes.push_back(255);
				break;
			}
		}
	}
	cout << endl << "----------��������----------" << endl;
}

// ���
void Xor(vector<uint16_t> Plaintext, vector<uint16_t> Cipher) {
	cout << endl << "----------���ʼ----------" << endl << endl;
	int temp = 0;
	cout << "���� Xor ���� = ���" << endl << endl;
	cout << "���ģ�";
	for (auto s : Cipher) cout << s << " "; cout << endl;
	cout << "���ģ�";
	for (auto s : Plaintext) cout << s << " "; cout << endl;

	cout << "�����";
	for (auto i = 0; i != Cipher.size(); ++i) {
		temp = Cipher[i] ^ Plaintext[i];
		cout << setfill('0') << setw(2) << hex << uppercase << temp << " ";
	} cout << endl;
	cout << endl << "----------������----------" << endl;
}

// ��ȡ�ļ� �����������
void LeftIndex(int num, char* hexNumber) {
	for (int i = 0; i < 8; i++) {
		hexNumber[i] = '0';
	}
	int index = 7;
	while (num != 0 && index >= 0) {
		hexNumber[index--] = HEX[num % 16];
		num = num / 16;
	}
}

// ��ȡ�ļ� ���庯��
vector<uint16_t> ReadFile(string& filename) {
	vector<uint16_t> cfile;
	uint8_t temp;
	char ch;

	cout << "�����ļ���";
	cin >> filename;
	ifstream infile(filename, ios::binary);

	cout << "�Ƿ�����ļ����� (Y/N)��";
	cin >> ch;

	if (ch == 'y' || ch == 'Y') {
		// ��ߵ�һ������
		int num = 0;
		char hexNumber[9] = "00000000";
		cout << "\t     ";
		for (int i = 0; i < 16; i++) cout << HEX[i] << "  ";
		cout << endl;
		while (!infile.eof()) {
			for (int i = 0; i != 16; ++i) {
				if (infile.eof()) break;
				if (num % 16 == 0) { // ��һ��֮�������
					LeftIndex(num, hexNumber);
					cout << hexNumber << ":    ";
				}
				num++;
				//ʮ������������ʾ
				infile.read((char*)&temp, 1);
				cfile.push_back(temp);
				cout << setfill('0') << setw(2) << hex << uppercase << cfile.back() << " ";
			} cout << endl;
		}
		cfile.pop_back();
		cout << endl << "�벻Ҫ����Ϊʲô���ӡ��һ�������޸����ˣ����ǽ���ֻ�Ǵ�ӡ�������⣬�����о����ˣ������㿴����ռ�ֽ�����" << dec << cfile.size() << endl;
	}
	else {
		while (!infile.eof()) {
			infile.read((char*)&temp, 1);
			cfile.push_back(temp);
		}
		cfile.pop_back();
		cout << "------------------------------" << endl;
		cout << "�ļ���ռ�ֽ���Ϊ��" << dec << cfile.size() << endl;
	}

	filename = filename.substr(filename.size() - 4, filename.back()); // �����ļ����� �磺.jpg
	return cfile;
}

// �ļ��ļ򵥽���
void jpgDecryption(vector<uint16_t> jpg) { // Decryption = ����
	vector<uint16_t> jpgCipher = { jpg[0], jpg[1], jpg[2], jpg[3], jpg[jpg.size() - 1], jpg[jpg.size() - 2] };
	cout << "jpg����Ϊ��";
	for (auto s : jpgCipher) cout << setfill('0') << setw(2) << hex << uppercase << s << " ";
	cout << endl;
	cout << "jpg����Ϊ��";
	for (auto s : jpgPlaintext) cout << setfill('0') << setw(2) << hex << uppercase << s << " ";
	cout << endl << "���������ľ�Ϊһһ��Ӧ��ϵ" << endl;
	cout << "------------------------------" << endl;
	if (jpgPlaintext == jpgCipher) cout << "�������������, ������ ��� �� ����" << endl;
	else {
		xor_and_dec(jpgPlaintext, jpgCipher); // jpgPlaintext{ 0xFF, 0xD8, 0xFF, 0xE0, 0xD9, 0xFF };
		add_and_xor(jpgPlaintext, jpgCipher);
		cout << endl;
		Levo(jpgPlaintext, jpgCipher);
	}
}
void pngDecryption(vector<uint16_t> png) {
	vector<uint16_t> pngCipher{ png[0], png[1], png[2], png[3], png[4], png[5], png[6], png[7] };
	cout << "����Ϊ��";
	for (auto s : pngCipher) cout << setfill('0') << setw(2) << hex << uppercase << s << " ";
	cout << endl;
	cout << "����Ϊ��";
	for (auto s : pngPlaintext) cout << setfill('0') << setw(2) << hex << uppercase << s << " ";
	cout << endl;
	if (pngCipher == pngPlaintext) cout << "�������������, ������ ��� �� ����" << endl;
	else {
		xor_and_dec(pngPlaintext, pngCipher);
		add_and_xor(pngPlaintext, pngCipher);
		cout << endl;
		Levo(pngPlaintext, pngCipher);
	}
}
void docDecryption(vector<uint16_t> doc) {
	vector<uint16_t> docCipher{ doc[0], doc[1], doc[2], doc[3], doc[4], doc[5], doc[6], doc[7] };
	cout << "����Ϊ��";
	for (auto s : docCipher) cout << setfill('0') << setw(2) << hex << uppercase << s << " ";
	cout << endl;
	cout << "����Ϊ��";
	for (auto s : docPlaintext) cout << setfill('0') << setw(2) << hex << uppercase << s << " ";
	cout << endl;
	if (docCipher == docPlaintext) cout << "�������������, ������ ��� �� ����" << endl;
	else {
		xor_and_dec(docPlaintext, docCipher); // docPlaintext{ 0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1 };
		add_and_xor(docPlaintext, docCipher);
		cout << endl;
		Levo(docPlaintext, docCipher);
	}
}
void docxDecryption(vector<uint16_t> docx) {
	vector<uint16_t> docxCipher{ docx[0], docx[1], docx[2], docx[3] };
	cout << "����Ϊ��";
	for (auto s : docxCipher) cout << setfill('0') << setw(2) << hex << uppercase << s << " ";
	cout << endl;
	cout << "����Ϊ��";
	for (auto s : docxPlaintext) cout << setfill('0') << setw(2) << hex << uppercase << s << " ";
	cout << endl;
	if (docxCipher == docxPlaintext) cout << "�������������, ������ ��� �� ����" << endl;
	else {
		xor_and_dec(docxPlaintext, docxCipher);
		add_and_xor(docxPlaintext, docxCipher);
		cout << endl;
		Levo(docxPlaintext, docxCipher);
	}
}
void bmpDecryption(vector<uint16_t> bmp) {
	vector<uint16_t> bmpCipher{ bmp[0], bmp[1], bmp[10], bmp[14], bmp[15] };
	cout << "����Ϊ��";
	for (auto s : bmpCipher) cout << setfill('0') << setw(2) << hex << uppercase << s << " ";
	cout << endl;
	cout << "����Ϊ��";
	for (auto s : bmpPlaintext) cout << setfill('0') << setw(2) << hex << uppercase << s << " ";
	cout << endl;
	if (bmpCipher == bmpPlaintext) cout << "�������������, ������ ��� �� ����" << endl;
	else {
		xor_and_dec(bmpPlaintext, bmpCipher);
		add_and_xor(bmpPlaintext, bmpCipher);
		cout << endl;
		Levo(bmpPlaintext, bmpCipher);
	}
}
void rtfDecryption(vector<uint16_t> rtf) {
	vector<uint16_t> rtfCipher{ rtf[0], rtf[1], rtf[2], rtf[3], rtf[4] };
	cout << "����Ϊ��";
	for (auto s : rtfCipher) cout << setfill('0') << setw(2) << hex << uppercase << s << " ";
	cout << endl;
	cout << "����Ϊ��";
	for (auto s : rtfPlaintext) cout << setfill('0') << setw(2) << hex << uppercase << s << " ";
	cout << endl;
	if (rtfCipher == rtfPlaintext)	cout << "�������������, ��������� �� ����" << endl;
	else {
		xor_and_dec(rtfPlaintext, rtfCipher);
		add_and_xor(rtfPlaintext, rtfCipher);
		cout << endl;
		Levo(rtfPlaintext, rtfCipher);
	}
}
void gifDecryption(vector<uint16_t> gif) {
	vector<uint16_t> gifCipher{ gif[0], gif[1], gif[2], gif[3], gif[4], gif[5] };
	cout << "����Ϊ��";
	for (auto s : gifCipher) cout << setfill('0') << setw(2) << hex << uppercase << s << " ";
	cout << endl;
	cout << "����Ϊ��";
	for (auto s : gifPlaintext) cout << setfill('0') << setw(2) << hex << uppercase << s << " ";
	cout << endl;
	if (gifCipher == gifPlaintext) cout << "�������������, ��������� �� ����" << endl;
	else {
		xor_and_dec(gifPlaintext, gifCipher);
		add_and_xor(gifPlaintext, gifCipher);
		cout << endl;
		Levo(gifPlaintext, gifCipher);
	}
}

// �ж��ļ����ͣ������ø��ļ��Ľ��ܺ���
void Choicefile(const string filename, vector<uint16_t> fileContent) {
	if (filename == ".jpg")
		jpgDecryption(fileContent);
	if (filename == ".png")
		pngDecryption(fileContent);
	if (filename == ".doc" || filename == ".xls" || filename == "ppt")
		docDecryption(fileContent);
	if (filename == "docx" || filename == "pptx" || filename == "xlsx")
		docxDecryption(fileContent);
	if (filename == ".bmp")
		bmpDecryption(fileContent);
	if (filename == ".rtf")
		rtfDecryption(fileContent);
	if (filename == ".gif")
		gifDecryption(fileContent);
}

// jpg �ļ��ṹ�޸�
class JPG {
public:
	int E0 = 0, C0 = 0, DA = 0, DAend = 0;
	vector<int> DB{ 0, 0 }, C4{ 0, 0, 0, 0 };
};

void jpgStructure() {
	cout << "------------------------------------------------------------" << endl;
	cout << "ֻ֧��windows�Դ��Ļ�ͼ���������jpg�ļ�" << endl;
	vector<uint16_t> jpgfile, jpgCopy;
	string filename;
	jpgfile = ReadFile(filename);
	jpgCopy = jpgfile;
	JPG* jpg = new JPG(); // int E0, DB, C0, C4, DA;

	int index = 1;
	for (auto s = jpgfile.begin(); s != jpgfile.end(); ++s, ++index) {
		if (*s == 0xFF) {
			switch (*(s + 1)) {
			case 0xE0:
				jpg->E0 = index;
				break;
			case 0xDB:
				if (jpg->DB[0] == 0)
					jpg->DB[0] = index;
				else if (jpg->DB[1] == 0)
					jpg->DB[1] = index;
				break;
			case 0xC0:
				jpg->C0 = index;
				break;
			case 0xC4:
				if (jpg->C4[0] == 0)
					jpg->C4[0] = index;
				else if (jpg->C4[1] == 0)
					jpg->C4[1] = index;
				else if (jpg->C4[2] == 0)
					jpg->C4[2] = index;
				else if (jpg->C4[3] == 0)
					jpg->C4[3] = index;
				break;
			case 0xDA:
				jpg->DA = index;
				break;
			}
		}
		if (*(s + 1) == 0x3F) {
			jpg->DAend = index + 1;
			break;
		}
	}

	cout << "------------------------------------------------------------" << endl;
	uint16_t temp;
	temp = (jpgfile[jpg->E0 + 1] * 256) + jpgfile[jpg->E0 + 2];
	//cout << "FF E0ƫ����Ϊ��" << setfill('0') << setw(2) << hex << temp << endl;
	if (jpgfile[jpg->E0 + 1 + temp + 1] != 0xDB) {
		cout << "FF E0ƫ��������, ӦΪ��" << setfill('0') << setw(2) << hex << uppercase << jpg->DB[0] - jpg->E0 - 2 << endl;
		jpgCopy[jpg->E0 + 1] = (jpg->DB[0] - jpg->E0 - 2) / 256;
		jpgCopy[jpg->E0 + 2] = (jpg->DB[0] - jpg->E0 - 2) % 256;
	}

	temp = (jpgfile[jpg->DB[0] + 1] * 256) + jpgfile[jpg->DB[0] + 2];
	//cout << "��һ��FF DBƫ����Ϊ��" << setfill('0') << setw(2) << hex << uppercase << temp << '\t';
	if (jpgfile[jpg->DB[0] + 1 + temp + 1] != 0xDB) {
		cout << "��һ��FF DBƫ��������, ӦΪ��" << setfill('0') << setw(2) << hex << uppercase << jpg->DB[1] - jpg->DB[0] - 2 << '\t';
		jpgCopy[jpg->DB[0] + 1] = (jpg->DB[1] - jpg->DB[0] - 2) / 256;
		jpgCopy[jpg->DB[0] + 2] = (jpg->DB[1] - jpg->DB[0] - 2) % 256;
	}
	if (jpgfile[jpg->DB[0] + 3] != 0x00) {
		cout << "������Ϊ��" << setfill('0') << setw(2) << hex << uppercase << jpgfile[jpg->DB[0] + 3] << " ����" << endl;
		jpgCopy[jpg->DB[0] + 3] = 0x00;
	}

	temp = (jpgfile[jpg->DB[1] + 1] * 256) + jpgfile[jpg->DB[1] + 2];
	//cout << "�ڶ���FF DBƫ����Ϊ��" << setfill('0') << setw(2) << hex << temp << '\t';
	if (jpgfile[jpg->DB[1] + 1 + temp + 1] != 0xC0) {
		cout << "�ڶ���FF DBƫ��������, ӦΪ��" << setfill('0') << setw(2) << hex << uppercase << jpg->C0 - jpg->DB[1] - 2 << '\t';
		jpgCopy[jpg->DB[1] + 1] = (jpg->C0 - jpg->DB[1] - 2) / 256;
		jpgCopy[jpg->DB[1] + 2] = (jpg->C0 - jpg->DB[1] - 2) % 256;
	}
	if (jpgfile[jpg->DB[1] + 3] != 0x01) {
		cout << "������Ϊ��" << setfill('0') << setw(2) << hex << uppercase << jpgfile[jpg->DB[1] + 3] << " ����" << endl;
		jpgCopy[jpg->DB[1] + 3] = 0x01;
	}

	temp = (jpgfile[jpg->C0 + 1] * 256) + jpgfile[jpg->C0 + 2];
	//cout << "FF C0ƫ����Ϊ��" << setfill('0') << setw(2) << hex << temp << '\t';
	if (jpgfile[jpg->C0 + 1 + temp + 1] != 0xC4) {
		cout << "FF C0ƫ��������, ӦΪ��" << setfill('0') << setw(2) << hex << uppercase << jpg->C4[0] - jpg->C0 << '\t';
		jpgCopy[jpg->C0 + 1] = (jpg->C4[0] - jpg->C0 - 2) / 256;
		jpgCopy[jpg->C0 + 2] = (jpg->C4[0] - jpg->C0 - 2) % 256;
	}
	if (jpgfile[jpg->C0 + 3] != 0x08) {
		cout << "�������ȴ���, ӦΪ��08" << endl;
		jpgCopy[jpg->C0 + 3] = 0x08;
	}
	if (jpgfile[jpg->C0 + 8] != 0x03) {
		cout << "�����������, ��ǰֵΪ��" << setfill('0') << setw(2) << hex << uppercase << jpgfile[jpg->C0 + 8] << " ӦΪ��03" << endl;
		jpgCopy[jpg->C0 + 8] = 0x03;
	}
	if (jpgfile[jpg->C0 + 9] != 0x01 || jpgfile[jpg->C0 + 10] != 0x22 || jpgfile[jpg->C0 + 11] != 0x00) {
		cout << "��ֱ�����ʴ���, ��ǰֵΪ��" << setfill('0') << setw(2) << hex << uppercase <<
			jpgfile[jpg->C0 + 9] << " " << jpgfile[jpg->C0 + 10] << " " << setfill('0') << setw(2) << uppercase << jpgfile[jpg->C0 + 11] <<
			" ӦΪ��01 22 00" << endl;
		jpgCopy[jpg->C0 + 9] = 0x01, jpgCopy[jpg->C0 + 10] = 0x22, jpgCopy[jpg->C0 + 11] = 0x00;
	}
	if (jpgfile[jpg->C0 + 12] != 0x02 || jpgfile[jpg->C0 + 13] != 0x11 || jpgfile[jpg->C0 + 14] != 0x01) {
		cout << "ˮƽ�����ʴ���, ��ǰֵΪ��" << setfill('0') << setw(2) << hex << uppercase <<
			jpgfile[jpg->C0 + 12] << " " << jpgfile[jpg->C0 + 13] << " " << setfill('0') << setw(2) << uppercase << jpgfile[jpg->C0 + 14] <<
			" ӦΪ��02 11 01" << endl;
		jpgCopy[jpg->C0 + 12] = 0x02, jpgCopy[jpg->C0 + 13] = 0x11, jpgCopy[jpg->C0 + 14] = 0x01;
	}
	if (jpgfile[jpg->C0 + 15] != 0x03 || jpgfile[jpg->C0 + 16] != 0x11 || jpgfile[jpg->C0 + 17] != 0x01) {
		cout << "ˮƽ�����ʴ���, ��ǰֵΪ��" << setfill('0') << setw(2) << hex << uppercase <<
			jpgfile[jpg->C0 + 15] << " " << jpgfile[jpg->C0 + 16] << " " << setfill('0') << setw(2) << uppercase << jpgfile[jpg->C0 + 17] <<
			"ӦΪ��03 11 01" << endl;
		jpgCopy[jpg->C0 + 15] = 0x03, jpgCopy[jpg->C0 + 16] = 0x11, jpgCopy[jpg->C0 + 17] = 0x01;
	}

	for (int i = 0; i != 4; i++) {
		temp = (jpgfile[jpg->C4[i] + 1] * 256) + jpgfile[jpg->C4[i] + 2];
		//cout << "�� " << i + 1 <<" ��FF C4ƫ����Ϊ��" << setfill('0') << setw(2) << hex << uppercase << temp << '\t';
		if (i != 3) {
			if (jpgfile[jpg->C4[i] + 1 + temp + 1] != 0xC4) {
				cout << "�� " << i + 1 << " ��FF C4ƫ��������, ӦΪ��" << setfill('0') << setw(2) << hex << uppercase << jpg->C4[i + 1] - jpg->C4[i] - 2 << endl;
				jpgCopy[jpg->C4[i] + 1] = (jpg->C4[i + 1] - jpg->C4[i] - 2) / 256;
				jpgCopy[jpg->C4[i] + 2] = (jpg->C4[i + 1] - jpg->C4[i] - 2) % 256;
			}
		}
		else if (i == 3) {
			temp = (jpgfile[jpg->C4[3] + 1] * 256) + jpgfile[jpg->C4[3] + 2];
			if (jpgfile[jpg->C4[3] + 1 + temp + 1] != 0xDA) {
				cout << "�� " << 4 << " ��FF C4ƫ��������, ӦΪ��" << setfill('0') << setw(2) << hex << uppercase << jpg->DA - jpg->C4[3] - 2 << endl;
				jpgCopy[jpg->C4[3] + 1] = (jpg->DA - jpg->C4[3] - 2) / 256;
				jpgCopy[jpg->C4[3] + 2] = (jpg->DA - jpg->C4[3] - 2) % 256;
			}
		}
		switch (i) {
		case 0:
			if (jpgfile[jpg->C4[i] + 3] != 0x00) {
				cout << "�����������, ӦΪ 00" << endl;
				jpgCopy[jpg->C4[i] + 3] = 0x00;
			}break;
		case 1:
			if (jpgfile[jpg->C4[i] + 3] != 0x10) {
				cout << "�����������, ӦΪ 10" << endl;
				jpgCopy[jpg->C4[i] + 3] = 0x10;
			}break;
		case 2:
			if (jpgfile[jpg->C4[i] + 3] != 0x01) {
				cout << "�����������, ӦΪ 01" << endl;
				jpgCopy[jpg->C4[i] + 3] = 0x01;
			}break;
		case 3:
			if (jpgfile[jpg->C4[3] + 3] != 0x11) {
				cout << "�����������, ӦΪ 01" << endl;
				jpgCopy[jpg->C4[3] + 3] = 0x11;
			}break;
		}
	}

	temp = (jpgfile[jpg->DA + 1] * 256) + jpgfile[jpg->DA + 2];
	//cout << "FF DAƫ����Ϊ��" << setfill('0') << setw(2) << hex << uppercase << temp << '\t';
	if (jpg->DA + 1 + temp != 0x3F) {
		cout << "FF DAƫ��������, ӦΪ��" << setfill('0') << setw(2) << hex << uppercase << jpg->DAend - jpg->DA << endl;
		jpgCopy[jpg->DA + 1] = (jpg->DAend - jpg->DA) / 256;
		jpgCopy[jpg->DA + 2] = (jpg->DAend - jpg->DA) % 256;
	}
	if (jpgfile[jpg->DA + 3] != 0x03) {
		cout << "�����������, ӦΪ��03" << endl;
		jpgCopy[jpg->DA + 3] = 0x03;
	}
	if (jpgfile[jpg->DA + 4] != 0x01 || jpgfile[jpg->DA + 5] != 0x00) {
		cout << "1���������, ӦΪ��01 00" << endl;
		jpgCopy[jpg->DA + 4] = 0x01;
		jpgCopy[jpg->DA + 5] = 0x00;
	}
	if (jpgfile[jpg->DA + 6] != 0x02 || jpgfile[jpg->DA + 7] != 0x11) {
		cout << "2���������, ӦΪ��02 11" << endl;
		jpgCopy[jpg->DA + 6] = 0x02;
		jpgCopy[jpg->DA + 7] = 0x11;
	}
	if (jpgfile[jpg->DA + 8] != 0x03 || jpgfile[jpg->DA + 9] != 0x11) {
		cout << "3���������, ӦΪ��03 11" << endl;
		jpgCopy[jpg->DA + 8] = 0x03;
		jpgCopy[jpg->DA + 9] = 0x11;
	}
	cout << "------------------------------------------------------------" << endl;

	// ����ļ�
	ofstream outfile;
	auto j = 0;
	for (auto i = 0; i != jpgCopy.size(); i += 10000) {
		outfile.open("D:\\demo.jpg", ios::app | ios::binary);
		for (j = i; j != i + 10000; j++) {
			outfile << (char)jpgCopy[j];
			if (j == jpgCopy.size() - 1) break;
		}
		outfile.close();
		if (j == jpgCopy.size() - 1) break;
	}
	cout << "�����ļ�����λ��Ϊ��D:\\demo.jpg" << endl;
	cout << "------------------------------------------------------------" << endl;
}

// doc �ļ��ṹ�޸�
class DOC {
public:
	const vector<uint16_t> docFix{
		0xD0,0xCF,0x11,0xE0,0xA1,0xB1,0x1A,0xE1,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x00,0x03,0x00,0xFE,0xFF,0x09,0x00,
		0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	uint32_t SAT = 0, SATnum = 0, root = 0, SSAT = 0, SSATnum = 0, MSATnum = 0, MSAT = 0, FstSAT = 0, ECA5 = 0;
	vector<uint32_t> SID;
};

void docStructure() {
	DOC* doc = new DOC();
	string filename;
	vector<uint16_t> docfile = ReadFile(filename);
	vector<uint16_t> docCopy = docfile;

	for (auto i = 0; i != 44; ++i) {
		docCopy[i] = doc->docFix[i];
		// cout << setfill('0') << setw(2) << hex << uppercase << docCopy[i] << " ";
	} // cout << endl;

	// ���� SAT ����� �� SID
	cout << "�㷨�д�ֻ֧��ĳЩdoc��" << endl;
	int SID = 0;
	for (auto i = 0; i != docfile.size(); ++i) {
		if (docfile[i] == 0xFD && docfile[i + 1] == 0xFF && docfile[i + 2] == 0xFF && docfile[i + 3] == 0xFF) {
			if (docfile[i + 4] == 0xFD && docfile[i + 5] == 0xFF && docfile[i + 6] == 0xFF && docfile[i + 7] == 0xFF) {
				doc->SATnum++;
				int j = i, jNum = 0;
				while (true) {
					if (i % 512 == 0) {
						doc->SID.push_back(j / 512 - 1);
						break;
					}
					else if (docfile[j - 1] != 0xFF && docfile[j - 2] != 0xFF && docfile[j - 3] != 0xFF && (docfile[j - 4] != 0xFF || docfile[j - 4] != 0xFE)) {
						uint32_t SATtemp;
						SATtemp = (docfile[j - 1] * 256 * 256 * 256) + (docfile[j - 2] * 256 * 256) + (docfile[j - 3] * 256) + (docfile[j - 4]);
						doc->SID.push_back(SATtemp + jNum);
						break;
					}
					j -= 4, jNum++;
				}
			}
			else if (docfile[i - 4] == 0xFD && docfile[i - 5] == 0xFF && docfile[i - 6] == 0xFF && docfile[i - 7] == 0xFF) {
				doc->SATnum++;
				int j = i, jNum = 0;
				while (true) {
					if (i % 512 == 0) {
						doc->SID.push_back(j / 512 - 1);
						break;
					}
					else if (docfile[j - 1] != 0xFF && docfile[j - 2] != 0xFF && docfile[j - 3] != 0xFF && (docfile[j - 4] != 0xFF || docfile[j - 4] != 0xFE)) {
						uint32_t SATtemp;
						SATtemp = (docfile[j - 1] * 256 * 256 * 256) + (docfile[j - 2] * 256 * 256) + (docfile[j - 3] * 256) + (docfile[j - 4]);
						doc->SID.push_back(SATtemp + jNum);
						break;
					}
					j -= 4, jNum++;
				}
			}
			else {
				doc->SATnum++;
				int j = i, jNum = 0;
				while (true) {
					if (i % 512 == 0) {
						doc->SID.push_back(j / 512 - 1);
						break;
					}
					else if (docfile[j - 1] != 0xFF && docfile[j - 2] != 0xFF && docfile[j - 3] != 0xFF && (docfile[j - 4] != 0xFF || docfile[j - 4] != 0xFE)) {
						uint32_t SATtemp;
						SATtemp = (docfile[j - 1] * 256 * 256 * 256) + (docfile[j - 2] * 256 * 256) + (docfile[j - 3] * 256) + (docfile[j - 4]);
						doc->SID.push_back(SATtemp - 1 + jNum);
						break;
					}
					j -= 4, jNum++;
				}
			}
		}
	}	cout << "SAT�����Ϊ��" << dec << doc->SATnum << endl;

	if (doc->SATnum > 256) {
		docCopy[44] = doc->SATnum % 256, docCopy[45] = doc->SATnum % 65536 / 256;
		docCopy[46] = doc->SATnum % 16777216 / 65536, docCopy[47] = doc->SATnum % 4294967296 / 16777216;
	}
	else
		docCopy[44] = doc->SATnum, docCopy[45] == 0, docCopy[46] == 0, docCopy[47] == 0;

	int num = 76;
	for (auto s : doc->SID) {
		docCopy[num++] = s % 256, docCopy[num++] = s % 65536 / 256;
		docCopy[num++] = s % 16777216 / 65536, docCopy[num++] = s % 4294967296 / 16777216;
	}

	if (doc->SATnum == 1) {
		cout << "��һ��SAT����������Ϊ��" << dec << ++doc->SID[0] + 1 << " ����MSAT��ӦΪ��" << hex << uppercase << doc->SID[0] << endl;
		docCopy[76] = doc->SID[0] % 256, docCopy[77] = doc->SID[0] % 65536 / 256;
		docCopy[78] = doc->SID[0] % 16777216 / 65536, docCopy[79] = doc->SID[0] % 4294967296 / 16777216;
	}
	else
		for (auto s : doc->SID) cout << dec << "SID��" << s << endl;
	// root
	for (auto i = 0; i != docfile.size(); ++i)
		if (docfile[i] == 0x52 && docfile[i + 2] == 0x6F && docfile[i + 4] == 0x6F && docfile[i + 6] == 0x74)
			doc->root = i / 512 - 1;
	docCopy[48] = doc->root;

	cout << "Root��" << dec << doc->root << endl;
	// ssat
	for (auto i = 0; i != docfile.size(); ++i)
		if (docfile[i] == 0x01 && docfile[i + 1] == 0x00 && docfile[i + 2] == 0x00 && docfile[i + 3] == 0x00)
			if (i % 512 == 0) {
				for (auto s : doc->SID) {
					if (i / 512 == s + 1) break;
					else doc->SSAT = i / 512 - 1;
				}
			}
	cout << "SSAT:" << doc->SSAT << endl;
	doc->SSATnum++;
	docCopy[60] = doc->SSAT;
	docCopy[64] = doc->SSATnum;

	for (auto i = 0; i != docfile.size(); ++i)
		if (docfile[i] == 0x57 && docfile[i + 2] == 0x6F && docfile[i + 4] == 0x72 && docfile[i + 6] == 0x64 && docfile[i + 8] == 0x44) {
			doc->ECA5 = (docfile[i + 116 + 3] * 256 * 256 * 256) + (docfile[i + 2 + 116] * 256 * 256) + (docfile[i + 1 + 116] * 256) + (docfile[i + 116]);
			break;
		}
	cout << "ECA5�����ܲ���ȷ��:" << dec << doc->ECA5 << endl;

	// ����ļ�
	ofstream outfile;
	auto j = 0;
	for (auto i = 0; i != docCopy.size(); i += 10000) {
		outfile.open("D:\\demo.doc", ios::app | ios::binary);
		for (j = i; j != i + 10000; j++) {
			outfile << (char)docCopy[j];
			if (j == docCopy.size() - 1) break;
		}
		outfile.close();
		if (j == docCopy.size() - 1) break;
	}
	cout << "�����ļ�����λ��Ϊ��D:\\demo.doc" << endl;
	cout << "------------------------------------------------------------" << endl;
}

int Choose(int& index) {
	cout << "-------------------------------------------------------" << endl;
	cout << "1. FAT32    2. NTFS    3. exFAT    4. EXT" << endl;
	cout << "5. FAT32�ļ�λ�ý���    6. ���Ӽ�&����&���" << endl;
	cout << "7. �ļ��򵥽���    8.jpg�ļ��ṹ�޸�    9.doc�ļ�ͷ�޸�" << endl;
	cout << "10. �˳�      ��������, bug�ܶ�, ���½�      " << endl;
	cout << "                                     ��ϵQQ: 1762202060" << endl;
	cout << "-------------------------------------------------------" << endl;
	cout << "��ѡ���� (��Ӧ���)��";
	cin >> dec >> index;
	cout << "-------------------------------------------------------" << endl;
	return index;
}

void exit() {
	system("cls");
	cout << "-------------------------------------------------" << endl;
	cout << "��  ������ ������ ����          ������  ����        " << endl;
	cout << "��  �����������ߩ������ߩ�������      �����ߩ����������ߩ���      " << endl;
	cout << "��  ���������������� ��      ���������� ��      " << endl;
	cout << "��  ��������������������      ��   ����   ��      " << endl;
	cout << "��  �����������ש������� ��      �����ש������� ��      " << endl;
	cout << "��  ���������������� ��      ���������� ��      " << endl;
	cout << "��  ����������������  ��      ���� �ߡ� ����      " << endl;
	cout << "��  ������������������������      ������������������      " << endl;
	cout << "��  ����������������           ����������       " << endl;
	cout << "            ��    ��           ��������         " << endl;
	cout << "��  ��  ����������������           ����  ��������      " << endl;
	cout << "��     ���ϡ���������  ���޻��� ������ ���ǩ�     " << endl;
	cout << "��     ������   �� ��  1.5 �汾 ������ ������     " << endl;
	cout << "��  ��  �������ש�����������           �����������ש�����      " << endl;
	cout << "��  ���� �ǩǩ����ǩǩ�             ���ϩ� ���ϩ�       " << endl;
	cout << "��  ���� ���ߩ������ߩ�             ���ߩ� ���ߩ�       " << endl;
	cout << "-------------------------------------------------" << endl;
	cout << "                                       ���ߣ�����" << endl;
	system("pause");
}
// ���޻���

int main(void) {
	FAT32* fat32 = new FAT32(); // ����FAT32����
	NTFS* ntfs = new NTFS(); // ����NTFS����
	exFAT* exfat = new exFAT(); // ����exFAT����
	EXT* ext = new EXT(); // ����EXT����

	vector<uint16_t> Cipher; // ���� ��������ͨ�õ� ��������
	vector<uint16_t> Plaintext; // ���� ��������ͨ�õ� ��������

	string filename; // �ļ���, �ж��ļ�����
	vector<uint16_t> fileContent; // ���� �ļ����� ���� ����������
	int index;

	while (index = Choose(index)) {
		if (index == 10) break;
		switch (index) {
		case 1:
			system("cls");
			Template_FAT32();
			Traverse_FAT32(*fat32);
			system("pause"), system("cls");
			break;
		case 2:
			system("cls");
			Template_NTFS();
			Traverse_NTFS(*ntfs);
			system("pause"), system("cls");
			break;
		case 3:
			system("cls");
			Template_exFAT();
			Traverse_exFAT(*exfat);
			system("pause"), system("cls");
			break;
		case 4:
			system("cls");
			Template_EXT();
			Traverse_EXT(*ext);
			system("pause"), system("cls");
			break;
		case 5:
			system("cls");
			Correct_FAT32(fat32->Cu);
			system("pause"), system("cls");
			break;
		case 6:
			system("cls");
			setCipher(Cipher);
			if (!setPlaintext(Plaintext, Cipher)) {
				Cipher.clear(), Plaintext.clear();
				exit(); system("cls"); break;
			}
			xor_and_dec(Plaintext, Cipher); // �������Ӽ�����(xor and dec)
			add_and_xor(Plaintext, Cipher); // ���мӼ�������(add and xor)
			Levo(Plaintext, Cipher);
			Xor(Plaintext, Cipher);
			Cipher.clear(), Plaintext.clear();
			system("pause"), system("cls");
			break;
		case 7:
			system("cls");
			cout << "���rtf�ĵ��ĺ�׺����Ϊdoc, ��Ļ�rtf" << endl;
			fileContent = ReadFile(filename);
			Choicefile(filename, fileContent);
			system("pause"), system("cls");
			break;
		case 8:
			system("cls");
			jpgStructure();
			system("pause"), system("cls");
			break;
		case 9:
			system("cls");
			docStructure();
			system("pause"), system("cls");
			break;
		default:
			cout << "�������" << endl;
			system("pause");
			exit();
			system("cls");
			break;
		}
	}

	delete fat32, ntfs, exfat, ext;
	exit();
	return 0;
}