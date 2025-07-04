//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <windows.h>
#include <stdio.h>
#include <System.hpp> // ��� ShowMessage
#include <math.h> // ��� sqrtf
#include <algorithm> // ��� std::max, std::min
#include <Registry.hpp>
#include <stdlib.h>
#include "IniFiles.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
String mode,hue,bright;
TIniFile *ini = new TIniFile (ExtractFilePath(ParamStr(0))+"EWEsetting.ini");
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------


 // ��������� ��� �������� HSV
struct HSVColor {
    uint8_t h;
    uint8_t s;
    uint8_t v;
};

// ������� ��� �������������� RGB � HSV
HSVColor rgbToHsv(uint8_t r, uint8_t g, uint8_t b) {
    float rf = r / 255.0f;
    float gf = g / 255.0f;
    float bf = b / 255.0f;

    float max_val = std::max({rf, gf, bf});
    float min_val = std::min({rf, gf, bf});
    float hue = 0.0f, saturation = 0.0f, value = max_val;

    if (max_val != min_val) {
        float delta = max_val - min_val;
        if( max_val == rf ) {
            hue = 60.0f * (fmod((gf - bf) / delta, 6.0f));
        } else if( max_val == gf ) {
            hue = 60.0f * ((bf - rf) / delta + 2.0f);
        } else if( max_val == bf ) {
           hue = 60.0f * ((rf - gf) / delta + 4.0f);
        }
        if (hue < 0.0f)
            hue += 360.0f;

        if (max_val != 0.0f)
           saturation = delta / max_val;
        else
           saturation = 0.0f;
    }


    HSVColor hsv;
    hsv.h = static_cast<uint8_t>(hue / 360.0f * 255.0f);
    hsv.s = static_cast<uint8_t>(saturation * 255.0f);
    hsv.v = static_cast<uint8_t>(value * 255.0f);

    return hsv;
}



//---------------------------------------------------------------------------

// ������� ��� �������� ������ � COM-����
bool SendDataToComPort(const char *portName, const char *dataToSend) {
    HANDLE hComm;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    DWORD bytesWritten;

    // �������� COM-�����
    hComm = CreateFileA(portName,  // ���������� CreateFileA
                     GENERIC_READ | GENERIC_WRITE,
                     0,
                     NULL,
                     OPEN_EXISTING,
                     0,
                     NULL);

    if (hComm == INVALID_HANDLE_VALUE) {
        printf("������ ��� �������� �����: %d\n", GetLastError());
        return false;
    }

    // ��������� ���������� �����
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hComm, &dcbSerialParams)) {
        printf("������ ��� ��������� ���������� �����: %d\n", GetLastError());
        CloseHandle(hComm);
        return false;
    }

    dcbSerialParams.BaudRate = CBR_9600; // �������� ����� (9600)
    dcbSerialParams.ByteSize = 8;        // ���� ������
    dcbSerialParams.Parity   = NOPARITY; // ��������
    dcbSerialParams.StopBits = ONESTOPBIT; // �������� ����

    if (!SetCommState(hComm, &dcbSerialParams)) {
        printf("������ ��� ��������� ���������� �����: %d\n", GetLastError());
        CloseHandle(hComm);
        return false;
    }

    // ��������� ���������
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hComm, &timeouts)) {
       printf("������ ��� ��������� ���������: %d\n", GetLastError());
       CloseHandle(hComm);
       return false;
    }

    // �������� ������
    if (!WriteFile(hComm, dataToSend, strlen(dataToSend), &bytesWritten, NULL)) {
         printf("������ ��� �������� ������: %d\n", GetLastError());
        CloseHandle(hComm);
         return false;
    }


    // �������� �����
    CloseHandle(hComm);
    return true;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{

	Label1->Caption = "MODE:"+mode+",COLOR:"+TrackBar1->Position+",BRIGHT:"+abs(TrackBar2->Position);


	AnsiString ansiString = Label1->Caption;
    AnsiString portNameStr = "\\\\.\\" + ComboBox1->Text;
	const char* portName = portNameStr.c_str();

	//const char *portName = "\\\\.\\COM11"; // �������� �� ��� COM-����
	const char *dataToSend = ansiString.c_str(); // ������ ��� ��������

	if (SendDataToComPort(portName, dataToSend))
	{
		//ShowMessage("������ ���������� �������!");

	}
	else
	{
		ShowMessage("������ ��� �������� ������!");
	}



	//---------------��������� ��������� ������� �������------------------

    if(ComboBox2->Text == "��������� 0")
	{

		 ini->WriteString("1","Color","0");
		 ini->WriteString("1","Bright","0");
	}
	if(ComboBox2->Text == "���� ���� 1")
	{

		ini->WriteString("1","Color",TrackBar1->Position);
		ini->WriteString("1","Bright",TrackBar2->Position);
	}
	if(ComboBox2->Text == "������� ����� ������ ���� ����� 2")
	{

		ini->WriteString("2","Bright",TrackBar2->Position);
	}
	if(ComboBox2->Text == "���������� ������ 3")
	{

		ini->WriteString("3","Bright",TrackBar2->Position);
	}
	if(ComboBox2->Text == "��������� ����� ������ 4")
	{

		ini->WriteString("4","Bright",TrackBar2->Position);
	}
	if(ComboBox2->Text == "�������� ��������� 5")
	{
		ini->WriteString("5","Bright",TrackBar2->Position);
		ini->WriteString("5","Color",TrackBar1->Position);
	}
	if(ComboBox2->Text == "�������� ��������� ����������� 6")
	{
		ini->WriteString("6","Bright",TrackBar2->Position);
		ini->WriteString("6","Color",TrackBar1->Position);
	}
	if(ComboBox2->Text == "���������� 9")
	{
		ini->WriteString("7","Bright",TrackBar2->Position);
		ini->WriteString("7","Color",TrackBar1->Position);
	}
	if(ComboBox2->Text == "��������� 10")
	{

	}

	if(RadioButton1->Checked == true)
	{
		ini->WriteString("VERTICAL","Mode",ComboBox2->Text);
	}


}
//---------------------------------------------------------------------------
void __fastcall TForm1::ColorListBox1Click(TObject *Sender)
{
if (ColorListBox1->ItemIndex != -1){
       TColor selectedColor = ColorListBox1->Selected;

       // ��������� R, G, B �� TColor
       uint8_t red = GetRValue(selectedColor);
       uint8_t green = GetGValue(selectedColor);
       uint8_t blue = GetBValue(selectedColor);

        // ����������� RGB � HSV
        HSVColor hsv = rgbToHsv(red, green, blue);

       // ������� �������� HSV (����� ������������ ��� FastLED)
		//printf("RGB: (%d, %d, %d)\n", red, green, blue);
		//printf("HSV: (H: %u, S: %u, V: %u)\n", hsv.h, hsv.s, hsv.v);
        Label1->Caption = "HSV: H:" + IntToStr(hsv.h) + " S:" + IntToStr(hsv.s) + " V:" + IntToStr(hsv.v);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TrackBar1Change(TObject *Sender)
{
	hue = TrackBar1->Position;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::RadioButton1Click(TObject *Sender)
{
	ComboBox2->Items->LoadFromFile("vertical.txt");
	ini->WriteString("LEGS","Legs","vertical");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton2Click(TObject *Sender)
{
	ComboBox2->Items->LoadFromFile("horizontal.txt");
	ini->WriteString("LEGS","Legs","horizontal");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox2Change(TObject *Sender)
{


	if(ComboBox2->Text == "��������� 0")
	{
		mode="0";
		TrackBar1->Visible = false;
		Image1->Visible = false;
		TrackBar2->Position = 0;
	}
	if(ComboBox2->Text == "���� ���� 1")
	{
		mode="1";
		TrackBar1->Visible = true;
		Image1->Visible = true;
		TrackBar1->Position = StrToInt(ini->ReadString("1","Color","0"));
		TrackBar2->Position = StrToInt(ini->ReadString("1","Bright","0"));
	}
	if(ComboBox2->Text == "������� ����� ������ ���� ����� 2")
	{
		mode="2";
		TrackBar1->Visible = false;
		Image1->Visible = false;
		TrackBar2->Position = StrToInt(ini->ReadString("2","Bright","0"));
	}
	if(ComboBox2->Text == "���������� ������ 3")
	{
		mode="3";
		TrackBar1->Visible = false;
		Image1->Visible = false;
        TrackBar2->Position = StrToInt(ini->ReadString("3","Bright","0"));
	}
	if(ComboBox2->Text == "��������� ����� ������ 4")
	{
		mode="4";
		TrackBar1->Visible = false;
		Image1->Visible = false;
	}
	if(ComboBox2->Text == "�������� ��������� 5")
	{
		mode="5";
		TrackBar1->Visible = true;
		Image1->Visible = true;
		TrackBar1->Position = StrToInt(ini->ReadString("5","Color","0"));
		TrackBar2->Position = StrToInt(ini->ReadString("5","Bright","0"));
	}
	if(ComboBox2->Text == "�������� ��������� ����������� 6")
	{
		mode="6";
		TrackBar1->Visible = true;
		Image1->Visible = true;
		TrackBar1->Position = StrToInt(ini->ReadString("6","Color","0"));
		TrackBar2->Position = StrToInt(ini->ReadString("6","Bright","0"));
	}
	if(ComboBox2->Text == "���������� 9")
	{
		mode="7";
        TrackBar1->Visible = true;
		Image1->Visible = true;
		TrackBar1->Position = StrToInt(ini->ReadString("7","Color","0"));
		TrackBar2->Position = StrToInt(ini->ReadString("7","Bright","0"));
	}
	if(ComboBox2->Text == "��������� 10")
	{
		mode="10";
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormCreate(TObject *Sender)
{
		ComboBox1->Clear(); // ������� ComboBox ����� �����������

			TRegistry *registry = new TRegistry();
			try {
				registry->RootKey = HKEY_LOCAL_MACHINE;
				registry->Access = KEY_READ;

				if (registry->OpenKey("HARDWARE\\DEVICEMAP\\SERIALCOMM", false)) {
					TStrings *portNames = new TStringList();
					try {
						registry->GetValueNames(portNames);

						for (int i = 0; i < portNames->Count; i++) {
							String portName = registry->ReadString(portNames->Strings[i]);
							ComboBox1->Items->Add(portName);
						}

						if (ComboBox1->Items->Count > 0) {
							ComboBox1->ItemIndex = 0; // �������� ������ �������
						}
					}
					__finally {
						delete portNames;
					}
				}
			}
			__finally {
				registry->CloseKey();
				delete registry;
			}



			if(FileExists("ComPort.ini"))
			{
				TStringList *settings = new TStringList();
				try
				{
					// ��������� ���������
					settings->LoadFromFile("ComPort.ini");

					// ������������� ����������� �������� � ComboBox
					String savedPort = settings->Values["COMPort"];
					if(!savedPort.IsEmpty())
					{
						int index = ComboBox1->Items->IndexOf(savedPort);
						if(index != -1)
							ComboBox1->ItemIndex = index;
					}
				}
				__finally
				{
					delete settings;
				}
			}



			//----------------��������� ���������---------------------

			if(ini->ReadString("LEGS","Legs","vertical")== "vertical")
			{
				RadioButton1->Checked = true;
			}
			else
			{
				RadioButton2->Checked = true;
			}


			if(RadioButton1->Checked == true)
			{

				int index = ComboBox2->Items->IndexOf(ini->ReadString("VERTICAL","Mode","1"));
				mode = index;
						if(index != -1)
						{
							ComboBox2->ItemIndex = index;
							TrackBar1->Position = StrToInt(ini->ReadString(index,"Color","0"));
							TrackBar2->Position = StrToInt(ini->ReadString(index,"Bright","0"));
						}

				if(ComboBox2->ItemIndex == 2 || ComboBox2->ItemIndex == 3 || ComboBox2->ItemIndex == 4)
				{
                     TrackBar1->Visible = false;
					 Image1->Visible = false;

				}
				else
				{
					 TrackBar1->Visible = true;
					 Image1->Visible = true;
				}

			}



}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox1Change(TObject *Sender)
{
	   TStringList *settings = new TStringList();
		try
		{
			// ���������� ��������� ����
			settings->Values["COMPort"] = ComboBox1->Text;

			// ��������� � ����
			settings->SaveToFile("ComPort.ini");
		}
		__finally
		{
			delete settings;
		}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::TrackBar2Change(TObject *Sender)
{
	Label2->Caption = TrackBar2->Position;

}
//---------------------------------------------------------------------------

