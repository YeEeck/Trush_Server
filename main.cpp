// Server.cpp : Defines the entry point for the console application.
//

#include "winsock2.h"

#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <fstream>
#include <codecvt>
#include <locale>
//#include "main.h"

using namespace std;

string GbkToUtf8(const char *src_str) {
    int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, nullptr, 0);
    auto *wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    char *str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, nullptr, nullptr);
    string strTemp = str;
    delete[] wstr;
    delete[] str;
    return strTemp;
}

int GetLocalIP(std::string &local_ip) {
    WSADATA wsaData = {0};
    if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
        return -1;
    char szHostName[MAX_PATH] = {0};
    int nRetCode;
    nRetCode = gethostname(szHostName, sizeof(szHostName));
    PHOSTENT hostinfo;
    if (nRetCode != 0)
        return WSAGetLastError();
    hostinfo = gethostbyname(szHostName);
    local_ip = inet_ntoa(*(struct in_addr *) *hostinfo->h_addr_list);
    WSACleanup();
    return 1;
}


int main(int argc, char *argv[]) {
    const int BUF_SIZE = 64;
    WSADATA wsd;            //WSADATA����
    SOCKET sServer;        //�������׽���
    SOCKET sClient;        //�ͻ����׽���
    SOCKADDR_IN addrServ;;      //��������ַ
    char buf[BUF_SIZE];  //�������ݻ�����
    char sendBuf[BUF_SIZE];//���ظ��ͻ��˵�����
    int retVal;         //����ֵ
    //��ʼ���׽��ֶ�̬��
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0) {
        cout << "WSAStartup failed!" << endl;
        return 1;
    }

    //�����׽���
    sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == sServer) {
        cout << "socket failed!" << endl;
        WSACleanup();//�ͷ��׽�����Դ;
        return -1;
    }
    int p = 1;
    int *t = &p;
    ioctlsocket(sServer, FIONBIO, nullptr);
    //�������׽��ֵ�ַ
    addrServ.sin_family = AF_INET;
    addrServ.sin_port = htons(4999);
    addrServ.sin_addr.s_addr = INADDR_ANY;
    //���׽���
    retVal = bind(sServer, (LPSOCKADDR) &addrServ, sizeof(SOCKADDR_IN));
    if (SOCKET_ERROR == retVal) {
        cout << "bind failed!" << endl;
        closesocket(sServer);   //�ر��׽���
        WSACleanup();           //�ͷ��׽�����Դ
        return -1;
    }
    string ip_s;
    GetLocalIP(ip_s);
    cout << "��������IP " << ip_s << " �˿� " << 4999 << " �Ͻ��м���" << endl;
    while (true) {
        //��ʼ����
        retVal = listen(sServer, 1);
        if (SOCKET_ERROR == retVal) {
            cout << "listen failed!" << endl;
            closesocket(sServer);   //�ر��׽���
            WSACleanup();           //�ͷ��׽�����Դ;
            return -1;
        }

        //���ܿͻ�������
        sockaddr_in addrClient;
        int addrClientlen = sizeof(addrClient);
        sClient = accept(sServer, (sockaddr FAR *) &addrClient, &addrClientlen);
        if (INVALID_SOCKET == sClient) {
            cout << "accept failed!" << endl;
            closesocket(sServer);   //�ر��׽���
            WSACleanup();           //�ͷ��׽�����Դ;
            return -1;
        }
        cout << "�ͻ���: " << inet_ntoa(addrClient.sin_addr) << " ����" << endl;

        //���տͻ�������
        ZeroMemory(buf, BUF_SIZE);
        char password[100];
        retVal = recv(sClient, buf, BUF_SIZE, 0);
        if (SOCKET_ERROR == retVal) {
            cout << "recv failed!" << endl;
            closesocket(sServer);   //�ر��׽���
            closesocket(sClient);   //�ر��׽���
            WSACleanup();           //�ͷ��׽�����Դ;
            return -1;
        }
//        if (retVal == 0){
//            continue;
//        }
        cout << buf << endl;
        if (strcmp(buf, "Close") == 0) {
            break;
        }
        ofstream output;
        if (strcmp(buf, "0") == 0) {
            output.open("index.htm", ios::out);
            output << GbkToUtf8("״̬��δ��");
        } else if (strcmp(buf, "1") == 0) {
            output.open("index.htm", ios::out);
            output << GbkToUtf8("״̬������");
        } else if (strcmp(buf, "2") == 0) {
            output.open("index.htm", ios::out);
            output << GbkToUtf8("״̬���д����");
        } else if (strcmp(buf, "100") == 0) {
            output.open("fire.htm", ios::out);
            output << "1\n";
        } else if (strcmp(buf, "101") == 0) {
            output.open("fire.htm", ios::out);
            output << "0\n";
        }
        output.close();
    }

    //�˳�
    closesocket(sServer);   //�ر��׽���
    closesocket(sClient);   //�ر��׽���
    WSACleanup();           //�ͷ��׽�����Դ;

    return 0;
}